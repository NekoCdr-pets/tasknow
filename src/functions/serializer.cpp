//===--- serializer.cpp - Tasknow data serializer -------------------------===//
//
// Part of the NekoCdr's "tasknow" project, under the Apache License v2.0
//
//===----------------------------------------------------------------------===//

#include "serializer.h"

#include "defines.h"

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <utility>

namespace tasknow::functions {

void serialize(tn_types::task* input, tn_types::sdto* output)
{
    size_t title_size = input->title.size();

    size_t raw_data_size{Sizeof16T + Sizeof16T + title_size};
    size_t offsets[3]{
        0,
        Sizeof16T,
        Sizeof16T * 2,
    };

    auto buff{std::make_unique<unsigned char[]>(raw_data_size)};

    // NOLINTBEGIN(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunsafe-buffer-usage"
    memcpy(buff.get() + offsets[0], &raw_data_size, Sizeof16T);
    memcpy(buff.get() + offsets[1], &title_size, Sizeof16T);
    memcpy(buff.get() + offsets[2], input->title.data(), title_size);
    #pragma GCC diagnostic pop
    // NOLINTEND(cppcoreguidelines-pro-bounds-pointer-arithmetic)

    output->size = static_cast<Size16_t>(raw_data_size);
    output->data = std::move(buff);
}

} // end namespace tasknow::functions
