//===--- response_handler.h - Tasknow response handler for client ---------===//
//
// Copyright (c) 2024 Yuri Istomin
//
// Part of the NekoCdr's "tasknow" project
// Licensed under the Apache License, Version 2.0 (the "License").
// See http://www.apache.org/licenses/LICENSE-2.0 for license information.
//
//===----------------------------------------------------------------------===//

#ifndef TASKNOW_RESPONSE_HANDLER_H
#define TASKNOW_RESPONSE_HANDLER_H

#include "task_list.h"

#include <cstddef>
#include <sys/types.h>

namespace tasknow::response_handler {

auto receive_data(
    int* client_sock,
    void* buff,
    const std::size_t size
) -> ssize_t;

auto get_task_list(int* client_sock) -> Task_list;

} // namespace tasknow::response_handler

#endif // TASKNOW_RESPONSE_HANDLER_H
