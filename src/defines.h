//===--- defines.h - Tasknow global defines & constants -------------------===//
//
// Copyright (c) 2023 Yuri Istomin
//
// Part of the NekoCdr's "tasknow" project
// Licensed under the Apache License, Version 2.0 (the "License").
// See http://www.apache.org/licenses/LICENSE-2.0 for license information.
//
//===----------------------------------------------------------------------===//

#ifndef TASKNOW_DEFINES_H
#define TASKNOW_DEFINES_H

#include <cstddef>
#include <cstdint>

namespace tasknow {

using Size_t = int16_t;
constexpr const std::size_t SizeofST{sizeof(Size_t)};

} // namespace tasknow

#endif // TASKNOW_DEFINES_H
