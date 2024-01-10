//===--- main.cpp ---------------------------------------------------------===//
//
// Copyright (c) 2023 Yuri Istomin
//
// Part of the NekoCdr's "tasknow" project
// Licensed under the Apache License, Version 2.0 (the "License").
// See http://www.apache.org/licenses/LICENSE-2.0 for license information.
//
//===----------------------------------------------------------------------===//

#include "errors.h"
#include "serverd.h"

#include <iostream>
#include <string_view>

// NOLINTNEXTLINE
int main()
{
    try {
        tasknow::serverd::serve();
    } catch (tasknow::errors::LinuxError& error) {
        tasknow::errors::log_error_to_stdout(error);
        return EXIT_FAILURE;
    } catch (tasknow::errors::ProtocolError& error) {
        tasknow::errors::log_error_to_stdout(error);
        return EXIT_FAILURE;
    }

    return 0;
}
