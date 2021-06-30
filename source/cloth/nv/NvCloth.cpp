#include "uniphysics/cloth/nv/NvCloth.h"

#include <sstream>
#include <iostream>

#include <assert.h>

namespace up
{
namespace cloth
{
namespace nv
{

NvCloth* NvCloth::m_instance = NULL;
NvCloth* NvCloth::Instance()
{
	if (!m_instance) {
		m_instance = new NvCloth();
	}
	return m_instance;
}

NvCloth::NvCloth()
{
	m_allocator = new Allocator;
	m_allocator->StartTrackingLeaks();
	m_foundation_allocator = new Allocator;
	m_foundation_allocator->StartTrackingLeaks();
	m_error_callback = new ErrorCallback;
	m_assert_handler = new AssertHandler;
	::nv::cloth::InitializeNvCloth(m_allocator, m_error_callback, m_assert_handler, nullptr);
#if USE_CUDA
	cuInit(0);
#endif
}

NvCloth::~NvCloth()
{
	m_allocator->StopTrackingLeaksAndReport();
	m_foundation_allocator->StopTrackingLeaksAndReport();
	delete m_error_callback;
	delete m_foundation_allocator;
	delete m_allocator;
	delete m_assert_handler;
}

//////////////////////////////////////////////////////////////////////////
// class Allocator
//////////////////////////////////////////////////////////////////////////

Allocator::Allocator()
{
	m_enable_leak_detection = false;
}

void* Allocator::allocate(size_t size, const char* typeName, const char* filename, int line)
{
#ifdef _MSC_VER
	void* ptr = _aligned_malloc(size, 16);
#else
	void* ptr;
	if(posix_memalign(&ptr, 16, size)) ptr = 0;
#endif
	if (m_enable_leak_detection)
	{
		std::lock_guard<std::mutex> lock(m_allocations_map_lock);
		m_allocations[ptr] = Allocation(size, typeName, filename, line);
	}
	return ptr;
}

void Allocator::deallocate(void* ptr)
{
	if (m_enable_leak_detection && ptr)
	{
		std::lock_guard<std::mutex> lock(m_allocations_map_lock);
		auto i = m_allocations.find(ptr);
		if (i == m_allocations.end())
		{
			printf("Tried to deallocate %p which was not allocated with this allocator callback.",ptr);
		}
		else
		{
			m_allocations.erase(i);
		}
	}
#ifdef _MSC_VER
	_aligned_free(ptr);
#else
	free(ptr);
#endif
}

void Allocator::StartTrackingLeaks()
{
	std::lock_guard<std::mutex> lock(m_allocations_map_lock);
	m_allocations.clear();
	m_enable_leak_detection = true;
}

void Allocator::StopTrackingLeaksAndReport()
{
	std::lock_guard<std::mutex> lock(m_allocations_map_lock);
	m_enable_leak_detection = false;

	size_t totalBytes = 0;
	std::stringstream message;
	message << "Memory leaks detected:\n";
	for (auto it = m_allocations.begin(); it != m_allocations.end(); ++it)
	{
		const Allocation& alloc = it->second;
		message << "* Allocated ptr " << it->first << " of " << alloc.size << "bytes (type=" << alloc.type_name << ") at " << alloc.file_name << ":" << alloc.line<<"\n";
		totalBytes += alloc.size;
	}
	if (m_allocations.size()>0)
	{
		message << "=====Total of " << totalBytes << " bytes in " << m_allocations.size() << " allocations leaked=====";
		const std::string& tmp = message.str();
#ifdef _MSC_VER
//				OutputDebugString(tmp.c_str()); //Write to visual studio output so we can see it after the application closes
#endif
		printf("%s\n", tmp.c_str());
	}

	m_allocations.clear();
}

//////////////////////////////////////////////////////////////////////////
// class ErrorCallback
//////////////////////////////////////////////////////////////////////////

void ErrorCallback::reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line)
{
	const char* codeName = "???";
	switch(code)
	{
#define CASE(x) case physx::PxErrorCode::Enum::x : codeName = #x; break;
		CASE(eNO_ERROR)
		CASE(eDEBUG_INFO)
		CASE(eDEBUG_WARNING)
		CASE(eINVALID_PARAMETER)
		CASE(eINVALID_OPERATION)
		CASE(eOUT_OF_MEMORY)
		CASE(eINTERNAL_ERROR)
		CASE(eABORT)
		CASE(ePERF_WARNING)
		default:
			;
#undef CASE
	}

	std::cout << "Log " << codeName << " from file:" << file << ":" << line << "\n MSG:" << message << std::endl;
}

//////////////////////////////////////////////////////////////////////////
// class AssertHandler
//////////////////////////////////////////////////////////////////////////

void AssertHandler::operator()(const char* exp, const char* file, int line, bool& ignore)
{
    PX_UNUSED(ignore);
    printf("NV_CLOTH_ASSERT(%s) from file:%s:%d Failed\n", exp, file, line);
    assert(("Assertion failed, see log/console for more info.", 0));
}

}
}
}