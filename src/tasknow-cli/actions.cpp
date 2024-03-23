//===--- actions.cpp - Tasknow CLI actions --------------------------------===//
//
// Copyright (c) 2024 Yuri Istomin
//
// Part of the NekoCdr's "tasknow" project
// Licensed under the Apache License, Version 2.0 (the "License").
// See http://www.apache.org/licenses/LICENSE-2.0 for license information.
//
//===----------------------------------------------------------------------===//

#include "actions.h"

#include "defines.h"
#include "task.h"
#include "task_list.h"

#include <format>
#include <iostream>
#include <string>

namespace tasknow::cli {

auto print_task_list(Task_list tasks) -> void
{
    for (Task& task: tasks.container) {
        std::cout
            << CLIOutputSeparator << "\n"
            << std::format("title: {}\n", task.title)
        ;
    }
    if (!tasks.container.empty()) {
        std::cout << "\n";
    }
    std::cout
        << std::format("{} tasks printed\n", tasks.container.size())
        << std::endl
    ;
}

auto create_new_task() -> Task
{
    Task task{};
    std::cout << "Enter the task title:" << std::endl;
    std::getline(std::cin, task.title);
    return task;
}

} // namespace tasknow::cli
