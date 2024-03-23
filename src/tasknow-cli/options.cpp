//===--- options.cpp - Tasknow CLI options --------------------------------===//
//
// Copyright (c) 2024 Yuri Istomin
//
// Part of the NekoCdr's "tasknow" project
// Licensed under the Apache License, Version 2.0 (the "License").
// See http://www.apache.org/licenses/LICENSE-2.0 for license information.
//
//===----------------------------------------------------------------------===//

#include "options.h"

#include "defines.h"

#include <boost/any.hpp>
#include <boost/program_options/errors.hpp>
#include <boost/program_options/value_semantic.hpp>

#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

namespace po = boost::program_options;

namespace tasknow::cli {

O_method::O_method (std::string input) : method(std::move(input)) {}

auto O_method::operator()() -> Query_method
{
    if (method.compare("list") == 0) {
        return Query_method::GetTaskList;
    }
    if (method.compare("add") == 0) {
        return Query_method::CreateNewTask;
    }
    return Query_method::EnumEnd;
}

auto validate(
    boost::any& value_storage,
    const std::vector<std::string>& input_values,
    O_method*,
    int
) -> void {
    std::unordered_set<std::string> available_values{
        "list",
        "add",
    };

    po::validators::check_first_occurrence(value_storage);
    const std::string& value = po::validators::get_single_string(input_values);

    if (available_values.contains(value)) {
        value_storage = boost::any(O_method(value));
    } else {
        throw po::validation_error(po::validation_error::invalid_option_value);
    }
}

} // namespace tasknow::cli
