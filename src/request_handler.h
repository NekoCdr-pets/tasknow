//===--- request_handler.h - Tasknow request hadler for server daemon -----===//
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

#include "defines.h"

namespace tasknow::request_handler {

auto send_response(int* client_sock, Query_method query_method) -> void;

} // namespace tasknow::request_handler

#endif // TASKNOW_REQUEST_HANDLER_H
