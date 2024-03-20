//===--- task_list.h - Task list struct and [de]serializer ----------------===//
//
// Copyright (c) 2024 Yuri Istomin
//
// Part of the NekoCdr's "tasknow" project
// Licensed under the Apache License, Version 2.0 (the "License").
// See http://www.apache.org/licenses/LICENSE-2.0 for license information.
//
//===----------------------------------------------------------------------===//

#ifndef TASKNOW_TASK_LIST_H
#define TASKNOW_TASK_LIST_H

#include "buffer.h"
#include "task.h"

#include <vector>

namespace tasknow {

struct Task_list {
    std::vector<Task> container;
};

auto serialize(Task_list* input) -> Buffer<Task_list>;
auto deserialize(Buffer<Task_list>* input) -> Task_list;

} // namespace tasknow

#endif //TASKNOW_TASK_LIST_H
