#include "uniphysics/cloth/JobManager.h"
#include "uniphysics/cloth/Solver.h"

#include <assert.h>

namespace up
{
namespace cloth
{

void Job::Initialize(JobManager* parent, std::function<void(Job*)> function, int refcount)
{
	mFunction = function;
	mParent = parent;
	Reset(refcount);
}

Job::Job(const Job& job)
{
	mFunction = job.mFunction;
	mParent = job.mParent;
	mRefCount.store(job.mRefCount);
	mFinished = job.mFinished;
}

void Job::Reset(int refcount)
{
	mRefCount = refcount;
	mFinished = false;
}

void Job::Execute()
{
	if (mFunction)
		mFunction(this);
	else
		ExecuteInternal();

	{
		std::lock_guard<std::mutex> lock(mFinishedLock);
		mFinished = true;
	}
	mFinishedEvent.notify_one();
}

void Job::AddReference()
{
	mRefCount++;
}
void Job::RemoveReference()
{
	int refCount = --mRefCount;
	if (0 == refCount)
	{
		mParent->Submit(this);
	}
	assert(refCount >= 0);
}

void Job::Wait()
{
	std::unique_lock<std::mutex> lock(mFinishedLock);
	mFinishedEvent.wait(lock, [this](){return mFinished;} );
	lock.unlock();
	return;
}

void JobManager::WorkerEntryPoint(JobManager* parrent)
{
	while (true)
	{
		Job* job;
		{
			std::unique_lock<std::mutex> lock(parrent->mJobQueueLock);
			while (parrent->mJobQueue.size() == 0 && !parrent->mQuit)
				parrent->mJobQueueEvent.wait(lock);

			if (parrent->mQuit)
				return;

			job = parrent->mJobQueue.front();
			parrent->mJobQueue.pop();
		}
		job->Execute();
	}
}

void JobManager::Submit(Job* job)
{
	mJobQueueLock.lock();
	mJobQueue.push(job);
	mJobQueueLock.unlock();
	mJobQueueEvent.notify_one();
}

void MultithreadedSolverHelper::Initialize(Solver* solver, JobManager* jobManager)
{
	mSolver = solver;
	mJobManager = jobManager;
	mEndSimulationJob.Initialize(mJobManager, [this](Job*) {
		mSolver->EndSimulation();
	});

	mStartSimulationJob.Initialize(mJobManager, [this](Job*) {
		mSolver->BeginSimulation(mDt);
		for(int j = 0; j < mSolver->GetSimulationChunkCount(); j++)
			mSimulationChunkJobs[j].RemoveReference();
	});
}

void MultithreadedSolverHelper::StartSimulation(float dt)
{
	mDt = dt;

	if (mSolver->GetSimulationChunkCount() != mSimulationChunkJobs.size())
	{
		mSimulationChunkJobs.resize(mSolver->GetSimulationChunkCount(), JobDependency());
		for (int j = 0; j < mSolver->GetSimulationChunkCount(); j++)
		{
			mSimulationChunkJobs[j].Initialize(mJobManager, [this, j](Job*) {mSolver->SimulateChunk(j); });
			mSimulationChunkJobs[j].SetDependentJob(&mEndSimulationJob);
		}
	}
	else
	{
		for (int j = 0; j < mSolver->GetSimulationChunkCount(); j++)
			mSimulationChunkJobs[j].Reset();
	}

	mStartSimulationJob.Reset();
	mEndSimulationJob.Reset(mSolver->GetSimulationChunkCount());
	mStartSimulationJob.RemoveReference();
}

void MultithreadedSolverHelper::WaitForSimulation()
{
	if (mSolver->GetSimulationChunkCount() == 0)
		return;

	mEndSimulationJob.Wait();
}

}
}