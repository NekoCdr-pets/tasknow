//===--- request_handler.h - Tasknow request handler for server daemon ----===//
//
// Copyright (c) 2024 Yuri Istomin
//
// Part of the NekoCdr's "tasknow" project
// Licensed under the Apache License, Version 2.0 (the "License").
// See http://www.apache.org/licenses/LICENSE-2.0 for license information.
//
//===----------------------------------------------------------------------===//

#ifndef TASKNOW_REQUEST_HANDLER_H
#define TASKNOW_REQUEST_HANDLER_H

#include "task_list.h"

#include <cstddef>

namespace tasknow::request_handler {

auto send_response(
    int* client_sock,
    unsigned char* buff,
    const std::size_t size
) -> void;

auto get_task_list(int* client_sock, Task_list* task_list) -> void;

} // namespace tasknow::request_handler

#endif // TASKNOW_REQUEST_HANDLER_H
