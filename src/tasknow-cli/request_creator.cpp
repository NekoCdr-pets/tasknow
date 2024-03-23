//===--- request_creator.cpp - Tasknow CLI request creator ----------------===//
//
// Copyright (c) 2024 Yuri Istomin
//
// Part of the NekoCdr's "tasknow" project
// Licensed under the Apache License, Version 2.0 (the "License").
// See http://www.apache.org/licenses/LICENSE-2.0 for license information.
//
//===----------------------------------------------------------------------===//

#include "request_creator.h"

#include "actions.h"
#include "buffer.h"
#include "defines.h"
#include "errors.h"
#include "task.h"

#include <cstddef>
#include <cstring>
#include <memory>
#include <utility>

namespace tasknow::cli {

auto request_task_list(Query_method query_method) -> Buffer<void>
{
    Buffer_size_t buff_size{static_cast<Buffer_size_t>(BytesForSize)};

    Buffer<void> request{};
    auto buff{std::make_unique<unsigned char[]>(
        static_cast<std::size_t>(buff_size)
    )};
    request.size = buff_size;
    request.data = std::move(buff);

    memcpy(request.data.get(), &query_method, BytesForSize);

    return request;
}

auto request_new_task_creating(Query_method query_method) -> Buffer<void>
{
    Buffer_size_t buff_size{static_cast<Buffer_size_t>(
        BytesForSize + BytesForBufferSize
    )};

    Task task{create_new_task()};
    Buffer<Task> serialized_task{serialize(&task)};
    buff_size += serialized_task.size;

    Buffer<void> request{};
    auto buff{std::make_unique<unsigned char[]>(
        static_cast<std::size_t>(buff_size)
    )};
    request.size = buff_size;
    request.data = std::move(buff);

    int offset{};

    memcpy(request.data.get() + offset, &query_method, BytesForSize);
    offset += BytesForSize;

    memcpy(
        request.data.get() + offset,
        &serialized_task.size,
        BytesForBufferSize
    );
    offset += BytesForBufferSize;

    memcpy(
        request.data.get() + offset,
        serialized_task.data.get(),
        static_cast<std::size_t>(serialized_task.size)
    );

    return request;
}

auto prepare_request(Query_method query_method) -> Buffer<void>
{
    switch (query_method) {
        case Query_method::GetTaskList:
            return request_task_list(query_method);
        case Query_method::CreateNewTask:
            return request_new_task_creating(query_method);
        default:
            throw errors::UnrecoverableProtocolError{
                "Unavailable query method."
            };
    }
}

} // namespace tasknow::cli
