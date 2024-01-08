//===--- errors.h - Tasknow errors ----------------------------------------===//
//
// Copyright (c) 2023 Yuri Istomin
//
// Part of the NekoCdr's "tasknow" project
// Licensed under the Apache License, Version 2.0 (the "License").
// See http://www.apache.org/licenses/LICENSE-2.0 for license information.
//
//===----------------------------------------------------------------------===//

#ifndef TASKNOW_ERRORS_H
#define TASKNOW_ERRORS_H

#include <concepts>
#include <format>
#include <iostream>
#include <string>
#include <type_traits>

using namespace std::literals;

namespace tasknow::errors {

struct ProtocolError{
    std::string message{};
};

struct UnrecoverableProtocolError: ProtocolError {};
struct WarningProtocolError: ProtocolError {};

struct LinuxError{
    int code{0};
    std::string function_name{};
    std::string message{};
};

struct UnrecoverableLinuxError: LinuxError {};
struct WarningLinuxError: LinuxError {};

template <typename E> requires std::is_base_of_v<LinuxError, E>
auto log_error_to_stdout(E error) -> void
{
    std::string error_level{"OTHER"};

    if constexpr (std::same_as<E, UnrecoverableLinuxError>) {
        error_level = "ERROR";
    }
    if constexpr (std::same_as<E, WarningLinuxError>) {
        error_level = "WARNING";
    }

    std::cout << std::format(
        "[{}] [{}] [{}]: {}",
        error_level,
        error.function_name,
        error.code,
        error.message
    ) << std::endl;
}

template <typename E> requires std::is_base_of_v<ProtocolError, E>
auto log_error_to_stdout(E error) -> void
{
    std::string error_level{"OTHER"};

    if constexpr (std::same_as<E, UnrecoverableProtocolError>) {
        error_level = "ERROR";
    }
    if constexpr (std::same_as<E, WarningProtocolError>) {
        error_level = "WARNING";
    }

    std::cout << std::format(
        "[{}]: {}",
        error_level,
        error.message
    ) << std::endl;
}

} // namespace tasknow::errors

#endif // TASKNOW_ERRORS_H
