//===--- buffer.h - Serialized data buffer --------------------------------===//
//
// Copyright (c) 2023 Yuri Istomin
//
// Part of the NekoCdr's "tasknow" project
// Licensed under the Apache License, Version 2.0 (the "License").
// See http://www.apache.org/licenses/LICENSE-2.0 for license information.
//
//===----------------------------------------------------------------------===//

#ifndef TASKNOW_BUFFER_H
#define TASKNOW_BUFFER_H

#include "defines.h"

#include <memory>

namespace tasknow {

template<typename T>
struct Buffer {
    Buffer_size_t size{};
    std::unique_ptr<unsigned char[]> data;
};

} // namespace tasknow

#endif // TASKNOW_BUFFER_H
