//===--- request_handler.cpp - Tasknow request hadler for server daemon ---===//
//
// Copyright (c) 2024 Yuri Istomin
//
// Part of the NekoCdr's "tasknow" project
// Licensed under the Apache License, Version 2.0 (the "License").
// See http://www.apache.org/licenses/LICENSE-2.0 for license information.
//
//===----------------------------------------------------------------------===//

#include "request_handler.h"

#include "defines.h"
#include "errors.h"

#include <cerrno>
#include <cstring>
#include <sys/socket.h>

namespace tasknow::request_handler {

auto send_response(int* client_sock, Query_method query_method) -> void
{
    for (int i=1; i<=3; i++) {
        if (send(
            *client_sock,
            &query_method,
            BytesForSize,
            0
        ) == ErrorCode) {
            switch (errno) {
                case EINTR:
                    if (i==3) {
                        throw errors::WarningLinuxError{
                            errno,
                            "send(2)",
                            "A signal occurred before any data was transmitted."
                        };
                    }
                    continue;

                case ECONNRESET:
                    throw errors::WarningLinuxError{
                        errno,
                        "send(2)",
                        "Connection reset by peer."
                    };

                case ENOBUFS:
                    throw errors::WarningLinuxError{
                        errno,
                        "send(2)",
                        "The output queue for a network interface was full. This generally indicates that the interface has stopped sending, but may be caused by transient congestion."
                    };

                case EPIPE:
                    throw errors::WarningLinuxError{
                        errno,
                        "send(2)",
                        "The local end has been shut down on a connection oriented socket. In this case, the process will also receive a SIGPIPE unless MSG_NOSIGNAL is set."
                    };

                default:
                    throw errors::UnrecoverableLinuxError{
                        errno,
                        "send(2)",
                        std::strerror(errno) // NOLINT(concurrency-mt-unsafe)
                    };
            }
        }
        return;
    }
}

} // namespace tasknow::request_handler

