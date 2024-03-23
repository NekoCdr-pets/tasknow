//===--- response_handler.cpp - Tasknow response handler for client -------===//
//
// Copyright (c) 2024 Yuri Istomin
//
// Part of the NekoCdr's "tasknow" project
// Licensed under the Apache License, Version 2.0 (the "License").
// See http://www.apache.org/licenses/LICENSE-2.0 for license information.
//
//===----------------------------------------------------------------------===//

#include "response_handler.h"

#include "defines.h"
#include "errors.h"

#include <cerrno>
#include <cstddef>
#include <cstring>
#include <format>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>

namespace tasknow::response_handler {

auto receive_data(
    int* client_sock,
    void* buff,
    const std::size_t size
) -> ssize_t {
    for (int i=1; i<=3; i++) {
        ssize_t message_length{recv(*client_sock, buff, size, 0)};
        if (message_length == ErrorCode) {
            switch (errno) {
                case EINTR:
                    if (i==3) {
                        throw errors::UnrecoverableLinuxError{
                            errno,
                            "recv(2)",
                            "The receive was interrupted by delivery of a signal before any data was available."
                        };
                    }
                    continue;

                case ECONNREFUSED:
                    throw errors::UnrecoverableLinuxError{
                        errno,
                        "recv(2)",
                        "A remote host refused to allow the network connection (typically because it is not running the requested service)."
                    };

                default:
                    throw errors::UnrecoverableLinuxError{
                        errno,
                        "recv(2)",
                        std::strerror(errno) // NOLINT(concurrency-mt-unsafe)
                    };
            }
        }
        return message_length;
    }
}

auto get_task_list(int* client_sock) -> void
{
    Buffer_size_t buff_size{};
    Buffer_size_t response_length{0};

    response_length += static_cast<Buffer_size_t>(
        receive_data(client_sock, &buff_size, BytesForBufferSize)
    );

    std::cout
        << std::format("Received {} bytes", response_length)
        << std::endl;
}

} // namespace tasknow::response_handler
