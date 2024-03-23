//===--- request_creator.h - Tasknow CLI request creator ------------------===//
//
// Copyright (c) 2024 Yuri Istomin
//
// Part of the NekoCdr's "tasknow" project
// Licensed under the Apache License, Version 2.0 (the "License").
// See http://www.apache.org/licenses/LICENSE-2.0 for license information.
//
//===----------------------------------------------------------------------===//

#ifndef TASKNOW_REQUEST_CREATOR_H
#define TASKNOW_REQUEST_CREATOR_H

#include "buffer.h"

namespace tasknow::cli {

auto request_task_list(Query_method query_method) -> Buffer<void>;

auto prepare_request(Query_method query_method) -> Buffer<void>;

} // namespace tasknow::cli

#endif // TASKNOW_REQUEST_CREATOR_H
