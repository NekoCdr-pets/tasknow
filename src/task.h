//===--- task.h - Task struct and [de]serializer --------------------------===//
//
// Copyright (c) 2023 Yuri Istomin
//
// Part of the NekoCdr's "tasknow" project
// Licensed under the Apache License, Version 2.0 (the "License").
// See http://www.apache.org/licenses/LICENSE-2.0 for license information.
//
//===----------------------------------------------------------------------===//

#ifndef TASKNOW_TASK_H
#define TASKNOW_TASK_H

#include "buffer.h"

#include <string>

namespace tasknow {

struct Task {
    std::string title;

    auto operator==(const Task& right) const -> bool;
    auto operator!=(const Task& right) const -> bool;
};

auto serialize(Task* input) -> Buffer<Task>;
auto deserialize(Buffer<Task>* input) -> Task;

} // namespace tasknow

#endif //TASKNOW_TASK_H
