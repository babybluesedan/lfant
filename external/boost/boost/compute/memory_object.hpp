//---------------------------------------------------------------------------//
// Copyright (c) 2013 Kyle Lutz <kyle.r.lutz@gmail.com>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
// See http://kylelutz.github.com/compute for more information.
//---------------------------------------------------------------------------//

#ifndef BOOST_COMPUTE_MEMORY_OBJECT_HPP
#define BOOST_COMPUTE_MEMORY_OBJECT_HPP

#include <boost/move/move.hpp>

#include <boost/compute/cl.hpp>
#include <boost/compute/context.hpp>
#include <boost/compute/detail/get_object_info.hpp>
#include <boost/compute/detail/assert_cl_success.hpp>

namespace boost {
namespace compute {

/// \class memory_object
/// \brief Base-class for memory objects.
///
/// The memory_object class is the base-class for memory objects on
/// compute devices.
///
/// \see buffer, vector
class memory_object
{
public:
    /// Flags for the creation of memory objects.
    enum mem_flags {
        read_write = CL_MEM_READ_WRITE,
        read_only = CL_MEM_READ_ONLY,
        write_only = CL_MEM_WRITE_ONLY,
        use_host_ptr = CL_MEM_USE_HOST_PTR,
        alloc_host_ptr = CL_MEM_ALLOC_HOST_PTR,
        copy_host_ptr = CL_MEM_COPY_HOST_PTR
    };

    /// Returns the underlying OpenCL memory object.
    cl_mem& get() const
    {
        return const_cast<cl_mem &>(m_mem);
    }

    /// Returns the size of the memory object in bytes.
    size_t get_memory_size() const
    {
        return get_memory_info<size_t>(CL_MEM_SIZE);
    }

    /// Returns the type for the memory object.
    cl_mem_object_type get_memory_type() const
    {
        return get_memory_info<cl_mem_object_type>(CL_MEM_TYPE);
    }

    /// Returns the flags for the memory object.
    cl_mem_flags get_memory_flags() const
    {
        return get_memory_info<cl_mem_flags>(CL_MEM_FLAGS);
    }

    /// Returns the context for the memory object.
    context get_context() const
    {
        return context(get_memory_info<cl_context>(CL_MEM_CONTEXT));
    }

    /// Returns the host pointer associated with the memory object.
    void* get_host_ptr() const
    {
        return get_memory_info<void *>(CL_MEM_HOST_PTR);
    }

    /// Returns the reference count for the memory object.
    uint_ reference_count() const
    {
        return get_memory_info<uint_>(CL_MEM_REFERENCE_COUNT);
    }

    /// Returns information about the memory object.
    ///
    /// \see_opencl_ref{clGetMemObjectInfo}
    template<class T>
    T get_memory_info(cl_mem_info info) const
    {
        return detail::get_object_info<T>(clGetMemObjectInfo, m_mem, info);
    }

    /// Returns \c true if the memory object is the same as \p other.
    bool operator==(const memory_object &other) const
    {
        return m_mem == other.m_mem;
    }

    /// Returns \c true if the memory object is different from \p other.
    bool operator!=(const memory_object &other) const
    {
        return m_mem != other.m_mem;
    }

protected:
    /// \internal_
    memory_object()
        : m_mem(0)
    {
    }

    /// \internal_
    explicit memory_object(cl_mem mem, bool retain = true)
        : m_mem(mem)
    {
        if(m_mem && retain){
            clRetainMemObject(m_mem);
        }
    }

    /// \internal_
    memory_object(const memory_object &other)
        : m_mem(other.m_mem)
    {
        if(m_mem){
            clRetainMemObject(m_mem);
        }
    }

    /// \internal_
    memory_object(BOOST_RV_REF(memory_object) other)
        : m_mem(other.m_mem)
    {
        other.m_mem = 0;
    }

    /// \internal_
    memory_object& operator=(const memory_object &other)
    {
        if(this != &other){
            if(m_mem){
                clReleaseMemObject(m_mem);
            }

            m_mem = other.m_mem;

            if(m_mem){
                clRetainMemObject(m_mem);
            }
        }

        return *this;
    }

    /// \internal_
    memory_object& operator=(BOOST_RV_REF(memory_object) other)
    {
        if(this != &other){
            if(m_mem){
                clReleaseMemObject(m_mem);
            }

            m_mem = other.m_mem;
            other.m_mem = 0;
        }

        return *this;
    }

    /// \internal_
    ~memory_object()
    {
        if(m_mem){
            BOOST_COMPUTE_ASSERT_CL_SUCCESS(
                clReleaseMemObject(m_mem)
            );
        }
    }

private:
    BOOST_COPYABLE_AND_MOVABLE(memory_object)

protected:
    cl_mem m_mem;
};

} // end compute namespace
} // end boost namespace

#endif // BOOST_COMPUTE_MEMORY_OBJECT_HPP
