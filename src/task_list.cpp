//===--- task_list.cpp - Task list struct and [de]serializer --------------===//
//
// Copyright (c) 2024 Yuri Istomin
//
// Part of the NekoCdr's "tasknow" project
// Licensed under the Apache License, Version 2.0 (the "License").
// See http://www.apache.org/licenses/LICENSE-2.0 for license information.
//
//===----------------------------------------------------------------------===//

#include "task_list.h"

#include "buffer.h"
#include "defines.h"
#include "task.h"

#include <cstddef>
#include <cstring>
#include <memory>
#include <utility>
#include <vector>

namespace tasknow {

auto serialize(Task_list* input) -> Buffer<Task_list>
{
    Buffer<Task_list> output{};

    Data_size_t task_count{static_cast<Data_size_t>(input->container.size())};

    std::vector<Buffer<Task>> serialized_tasks;
    serialized_tasks.reserve(static_cast<std::size_t>(task_count));

    std::ptrdiff_t raw_data_size{BytesForBufferSize * task_count};
    std::ptrdiff_t offset{0};

    for (Task& task: input->container) {
        Buffer<Task> serialized_task{serialize(&task)};
        raw_data_size += serialized_task.size;
        serialized_tasks.push_back(std::move(serialized_task));
    }

    auto buff{std::make_unique<unsigned char[]>(
        static_cast<std::size_t>(raw_data_size)
    )};

    for (Buffer<Task>& serialized_task: serialized_tasks) {
        memcpy(buff.get() + offset, &serialized_task.size, BytesForBufferSize);
        offset += BytesForBufferSize;

        memcpy(
            buff.get() + offset,
            serialized_task.data.get(),
            static_cast<std::size_t>(serialized_task.size)
        );
        offset += serialized_task.size;
    }

    output.size = static_cast<Buffer_size_t>(raw_data_size);
    output.data = std::move(buff);

    return output;
}

auto deserialize(Buffer<Task_list>* input) -> Task_list
{
    Task_list output{};

    std::ptrdiff_t offset{0};

    while (offset < input->size) {
        Buffer<Task> serialized_task{};

        memcpy(
            &serialized_task.size,
            input->data.get() + offset,
            BytesForBufferSize
        );
        offset += BytesForBufferSize;

        auto buff(std::make_unique<unsigned char[]>(
            static_cast<std::size_t>(serialized_task.size)
        ));
        serialized_task.data = std::move(buff);

        memcpy(
            serialized_task.data.get(),
            input->data.get() + offset,
            static_cast<std::size_t>(serialized_task.size)
        );
        offset += serialized_task.size;

        output.container.push_back(deserialize(&serialized_task));
    }

    return output;
}

} // namespace tasknow
