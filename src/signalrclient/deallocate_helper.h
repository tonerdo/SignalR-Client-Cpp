// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include <functional>

template <typename T>
struct deallocate_helper
{
public:
    deallocate_helper(std::function<void(void*)> free)
    {
        m_free = free;
    }

    void operator()(T* ptr)
    {
        ptr->~T();
        m_free(ptr);
    }

private:
    std::function<void(void*)> m_free;
};