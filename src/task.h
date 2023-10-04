//===--- task.h - Task struct and [un]serializer --------------------------===//
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
};

auto serialize(Task* input) -> Buffer;
auto unserialize(Buffer* input) -> Task;

} // end namespace tasknow

#endif //TASKNOW_TASK_H
