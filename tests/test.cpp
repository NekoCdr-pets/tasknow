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

#include <boost/ut.hpp>

#include <string>

using namespace boost::ut;

namespace tn = tasknow;

// NOLINTNEXTLINE
int main()
{
    "task_[un]serializer"_test = [] {
        std::string str{"abc"};
        tn::Task task{str};

        size_t buff_size{tn::SizeofST + str.size()};

        tn::Buffer serialized_task = tn::serialize(&task);
        tn::Task unserialized_task = tn::unserialize(&serialized_task);

        expect(serialized_task.size == buff_size);
        expect(task == unserialized_task);
    };
    
    return 0;
}
