//===--- actions.h - Tasknow CLI actions ----------------------------------===//
//
// Copyright (c) 2024 Yuri Istomin
//
// Part of the NekoCdr's "tasknow" project
// Licensed under the Apache License, Version 2.0 (the "License").
// See http://www.apache.org/licenses/LICENSE-2.0 for license information.
//
//===----------------------------------------------------------------------===//

#ifndef TASKNOW_ACTIONS_H
#define TASKNOW_ACTIONS_H

#include "task_list.h"

namespace tasknow::cli {

auto print_task_list(Task_list tasks) -> void;

} // namespace tasknow::cli

#endif // TASKNOW_ACTIONS_H
