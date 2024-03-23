//===--- main.cpp - CLI entrypoint ----------------------------------------===//
//
// Copyright (c) 2023 Yuri Istomin
//
// Part of the NekoCdr's "tasknow" project
// Licensed under the Apache License, Version 2.0 (the "License").
// See http://www.apache.org/licenses/LICENSE-2.0 for license information.
//
//===----------------------------------------------------------------------===//

#include "client.h"
#include "errors.h"
#include "options.h"

#include <boost/program_options/errors.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <boost/program_options/variables_map.hpp>

#include <cstdlib>
#include <iostream>

namespace po = boost::program_options;

using namespace tasknow::cli;

// NOLINTNEXTLINE
int main(int argc, char** argv)
{
    try {
        po::options_description desc{"Tasknow CLI help"};
        desc.add_options()
            ("help,h", "Show this help")
            ("method,m", po::value<O_method>(), "Allowed values are: list")
        ;

        po::positional_options_description pos;
        pos.add("method", 1);

        po::variables_map v_map;
        po::store(
            po::command_line_parser(argc, argv)
                .options(desc)
                .positional(pos)
                .run(),
            v_map
        );
        po::notify(v_map);

        if (v_map.count("help")) {
            std::cout << desc << '\n';
            return EXIT_SUCCESS;
        }

        if (!v_map.count("method")) {
            throw po::required_option("method");
        }
        O_method method{v_map["method"].as<O_method>()};

        tasknow::client::run(method());
    } catch (tasknow::errors::LinuxError& error) {
        tasknow::errors::log_error_to_stdout(error);
        return EXIT_FAILURE;
    } catch (tasknow::errors::ProtocolError& error) {
        tasknow::errors::log_error_to_stdout(error);
        return EXIT_FAILURE;
    } catch (po::error& error) {
        std::cout << error.what() << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}
