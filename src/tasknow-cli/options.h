//===--- options.h - Tasknow CLI options ----------------------------------===//
//
// Copyright (c) 2024 Yuri Istomin
//
// Part of the NekoCdr's "tasknow" project
// Licensed under the Apache License, Version 2.0 (the "License").
// See http://www.apache.org/licenses/LICENSE-2.0 for license information.
//
//===----------------------------------------------------------------------===//

#ifndef TASKNOW_CLI_OPTIONS_H
#define TASKNOW_CLI_OPTIONS_H

#include "defines.h"

#include <boost/any.hpp>
#include <string>
#include <vector>

namespace tasknow::cli {

struct O_method {
    O_method (std::string input);
    
    auto operator()() -> Query_method;

private:
    std::string method;
};

auto validate(
    boost::any& value_storage,
    const std::vector<std::string>& input_values,
    O_method*,
    int
) -> void;

} // namespace tasknow::cli

#endif // TASKNOW_CLI_OPTIONS_H
