//===--- request_handler.cpp - Tasknow request handler for server daemon --===//
//
// Copyright (c) 2024 Yuri Istomin
//
// Part of the NekoCdr's "tasknow" project
// Licensed under the Apache License, Version 2.0 (the "License").
// See http://www.apache.org/licenses/LICENSE-2.0 for license information.
//
//===----------------------------------------------------------------------===//

#include "request_handler.h"

#include "buffer.h"
#include "defines.h"
#include "errors.h"
#include "task.h"
#include "task_list.h"

#include <cerrno>
#include <cstddef>
#include <cstring>
#include <memory>
#include <sys/socket.h>
#include <utility>

namespace tasknow::request_handler {

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
                        throw errors::WarningLinuxError{
                            errno,
                            "recv(2)",
                            "The receive was interrupted by delivery of a signal before any data was available."
                        };
                    }
                    continue;

                case ECONNREFUSED:
                    throw errors::WarningLinuxError{
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

auto send_response(
    int* client_sock,
    unsigned char* buff,
    const std::size_t size
) -> void {
    for (int i=1; i<=3; i++) {
        if (send(*client_sock, buff, size, 0) == ErrorCode) {
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

auto get_task_list(int* client_sock, Task_list* task_list) -> void
{
    Buffer<Task_list> serialized_tasks{serialize(task_list)};

    auto buff{std::make_unique<unsigned char[]>(
        static_cast<std::size_t>(serialized_tasks.size + BytesForBufferSize)
    )};

    memcpy(buff.get(), &serialized_tasks.size, BytesForBufferSize);
    memcpy(
        buff.get() + BytesForBufferSize,
        serialized_tasks.data.get(),
        static_cast<std::size_t>(serialized_tasks.size)
    );

    send_response(
        client_sock,
        buff.get(),
        static_cast<std::size_t>(serialized_tasks.size) + BytesForBufferSize
    );
}

auto create_new_task(int* client_sock, Task_list* task_list) -> void
{
    Buffer_size_t buff_size{};
    Buffer_size_t request_length{};

    request_length += static_cast<Buffer_size_t>(
        receive_data(client_sock, &buff_size, BytesForBufferSize)
    );

    Buffer<Task> serialized_task{};
    auto buff{std::make_unique<unsigned char[]>(
        static_cast<std::size_t>(buff_size)
    )};
    serialized_task.size = buff_size;
    serialized_task.data = std::move(buff);

    request_length += static_cast<Buffer_size_t>(
        receive_data(
            client_sock,
            serialized_task.data.get(),
            static_cast<std::size_t>(buff_size)
        )
    );

    if (buff_size + BytesForBufferSize != request_length) {
        throw errors::WarningProtocolError{
            "Estimated buffer size does not match the received buffer size."
        };
    }

    task_list->container.push_back(deserialize(&serialized_task));
}

} // namespace tasknow::request_handler

