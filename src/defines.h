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
#include <string_view>

namespace tasknow {

using D_size_t = int16_t;
inline constexpr const std::ptrdiff_t BytesForSize{sizeof(D_size_t)};

inline constexpr const int ErrorCode{-1};

inline constexpr const std::string_view DaemonSockPath{"/tmp/tasknowd.socket"};
inline constexpr const std::string_view CLISockPath{"/tmp/tasknow-cli.socket"};
inline constexpr const int DaemonBacklogSize{10};

enum class Query_method: D_size_t
{
    EnumStart,

    GetTaskList,
    GetTaskDetails,

    EnumEnd,
};

} // namespace tasknow

#endif // TASKNOW_DEFINES_H
