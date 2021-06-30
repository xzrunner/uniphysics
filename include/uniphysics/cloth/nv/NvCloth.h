#pragma once

// pxshared
#include <foundation/PxAllocatorCallback.h>
#include <foundation/PxErrorCallback.h>

#include <NvCloth/Callbacks.h>

#include <string>
#include <map>
#include <mutex>

namespace up
{
namespace cloth
{
namespace nv
{

class Allocator : public physx::PxAllocatorCallback
{
public:
    Allocator();
    virtual void* allocate(size_t size, const char* typeName, const char* filename, int line);
    virtual void deallocate(void* ptr);

    void StartTrackingLeaks();
    void StopTrackingLeaksAndReport();

private:
    struct Allocation
    {
        Allocation() {}
        Allocation(size_t size, const char* typeName, const char* filename, int line)
            : size(size), type_name(typeName), file_name(filename), line(line) {}

        size_t size;
        std::string type_name;
        std::string file_name;
        int line;
    };

private:
	bool m_enable_leak_detection;
	std::map<void*, Allocation> m_allocations;
	std::mutex m_allocations_map_lock;

}; // Allocator

class ErrorCallback : public physx::PxErrorCallback
{
public:
    ErrorCallback() {}
    virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line);
}; // ErrorCallback


class AssertHandler : public ::nv::cloth::PxAssertHandler
{
public:
    virtual void operator()(const char* exp, const char* file, int line, bool& ignore);
}; // AssertHandler

class NvCloth
{
public:

    static NvCloth* Instance();

private:
    NvCloth();
    ~NvCloth();
private:
    static NvCloth* m_instance;

private:
	Allocator*     m_allocator;
	Allocator*     m_foundation_allocator;
	ErrorCallback* m_error_callback;
	AssertHandler* m_assert_handler;

}; // NvCloth

}
}
}