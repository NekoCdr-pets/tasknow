//===--- test.cpp - tests entry point  ------------------------------------===//
//
// Copyright (c) 2023 Yuri Istomin
//
// Part of the NekoCdr's "tasknow" project
// Licensed under the Apache License, Version 2.0 (the "License").
// See http://www.apache.org/licenses/LICENSE-2.0 for license information.
//
//===----------------------------------------------------------------------===//

#include "buffer.h"
#include "defines.h"
#include "task.h"
#include "task_list.h"

#include <boost/ut.hpp>

#include <cstddef>
#include <string>

using namespace boost::ut;

namespace tn = tasknow;

// NOLINTNEXTLINE
int main()
{
    "task_(de)serializer"_test = [] {
        std::string str{"abc"};
        tn::Task task{str};

        std::ptrdiff_t buff_size{tn::BytesForSize + std::ssize(str)};

        tn::Buffer<tn::Task> serialized_task = tn::serialize(&task);
        tn::Task unserialized_task = tn::deserialize(&serialized_task);

        expect(serialized_task.size == buff_size);
        expect(task == unserialized_task);
    };

    "task_list_(de)serializer"_test = [] {
        tn::Task_list task_list{};
        task_list.container.push_back({"abc"});
        task_list.container.push_back({"xyz"});

        std::ptrdiff_t buff_size{
            tn::BytesForBufferSize * 2
            + tn::BytesForSize * 2
            + std::ssize(task_list.container[0].title)
            + std::ssize(task_list.container[1].title)
        };

        tn::Buffer<tn::Task_list> serialized_list{tn::serialize(&task_list)};
        tn::Task_list unserialized_list{tn::deserialize(&serialized_list)};

        expect(serialized_list.size == buff_size);
        expect(task_list.container.size() == unserialized_list.container.size());
        expect(task_list.container[0] == unserialized_list.container[0]);
        expect(task_list.container[1] == unserialized_list.container[1]);
    };
    
    return 0;
}
