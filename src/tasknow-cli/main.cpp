//===--- main.cpp - CLI entrypoint ----------------------------------------===//
//
// Copyright (c) 2023 Yuri Istomin
//
// Part of the NekoCdr's "tasknow" project
// Licensed under the Apache License, Version 2.0 (the "License").
// See http://www.apache.org/licenses/LICENSE-2.0 for license information.
//
//===----------------------------------------------------------------------===//

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include <iostream>

namespace po = boost::program_options;

// NOLINTNEXTLINE
int main(int argc, char** argv)
{
    po::options_description desc{"Tasknow CLI help"};
    desc.add_options()
        ("help", "Show this help")
    ;

    po::variables_map v_map;
    po::store(po::parse_command_line(argc, argv, desc), v_map);
    po::notify(v_map);

    if (v_map.count("help")) {
        std::cout << desc << '\n';
        return 0;
    }

    return 0;
}
