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

auto serialize(Task* input) -> Buffer
{
    Buffer output{};
    std::size_t title_size = input->title.size();

    std::size_t raw_data_size{SizeofST + title_size};
    std::size_t offsets[2]{
        0,
        SizeofST,
    };

    auto buff{std::make_unique<unsigned char[]>(raw_data_size)};

    // NOLINTBEGIN(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"

    memcpy(buff.get() + offsets[0], &title_size, SizeofST);
    memcpy(buff.get() + offsets[1], input->title.data(), title_size);

    #pragma GCC diagnostic pop
    // NOLINTEND(cppcoreguidelines-pro-bounds-pointer-arithmetic)

    output.size = static_cast<Size_t>(raw_data_size);
    output.data = std::move(buff);

    return output;
}

} // end namespace tasknow