//===--- task.cpp - Task struct and [un]serializer ------------------------===//
//
// Copyright (c) 2023 Yuri Istomin
//
// Part of the NekoCdr's "tasknow" project
// Licensed under the Apache License, Version 2.0 (the "License").
// See http://www.apache.org/licenses/LICENSE-2.0 for license information.
//
//===----------------------------------------------------------------------===//

#include "task.h"

#include "buffer.h"
#include "defines.h"

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <utility>

namespace tasknow {

auto Task::operator==(const Task& right) const -> bool
{
    bool result{true};

    result &= this->title == right.title;

    return result;
}

auto Task::operator!=(const Task& right) const -> bool
{
    return !(*this == right);
}

auto serialize(Task* input) -> Buffer
{
    Buffer output{};
    std::ptrdiff_t title_size = std::ssize(input->title);

    std::ptrdiff_t raw_data_size{BytesForSize + title_size};
    std::ptrdiff_t offsets[2]{
        0,
        BytesForSize,
    };

    auto buff{std::make_unique<unsigned char[]>(
        static_cast<std::size_t>(raw_data_size)
    )};

    memcpy(buff.get() + offsets[0], &title_size, BytesForSize);
    memcpy(
        buff.get() + offsets[1],
        input->title.data(),
        static_cast<std::size_t>(title_size)
    );

    output.size = static_cast<D_size_t>(raw_data_size);
    output.data = std::move(buff);

    return output;
}

auto unserialize(Buffer* input) -> Task
{
    Task output{};
    std::ptrdiff_t title_size{};
    std::ptrdiff_t offset{0};

    memcpy(&title_size, input->data.get() + offset, BytesForSize);
    offset += BytesForSize;

    output.title.assign(
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        reinterpret_cast<const char*>(input->data.get()) + offset,
        static_cast<std::size_t>(title_size)
    );

    return output;
}

} // namespace tasknow
