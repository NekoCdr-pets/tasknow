//===--- main.cpp ---------------------------------------------------------===//
//
// Copyright (c) 2023 Yuri Istomin
//
// Part of the NekoCdr's "tasknow" project
// Licensed under the Apache License, Version 2.0 (the "License").
// See http://www.apache.org/licenses/LICENSE-2.0 for license information.
//
//===----------------------------------------------------------------------===//

#include "serverd.h"

#include <iostream>
#include <string_view>

// NOLINTNEXTLINE
int main()
{
    try {
        tasknow::serverd::serve();
    } catch (const std::string_view& error_msg) {
        std::cout << error_msg << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}
