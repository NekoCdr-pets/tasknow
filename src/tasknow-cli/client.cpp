//===--- client.cpp - Tasknow socket client -------------------------------===//
//
// Copyright (c) 2024 Yuri Istomin
//
// Part of the NekoCdr's "tasknow" project
// Licensed under the Apache License, Version 2.0 (the "License").
// See http://www.apache.org/licenses/LICENSE-2.0 for license information.
//
//===----------------------------------------------------------------------===//

#include "client.h"

#include "actions.h"
#include "buffer.h"
#include "defines.h"
#include "errors.h"
#include "request_creator.h"
#include "response_handler.h"

#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

using namespace tasknow::response_handler;

namespace tasknow::client {

auto init(
    sockaddr_un* server_addr,
    sockaddr_un* client_addr,
    socklen_t addr_len,
    std::string_view server_sock_path,
    std::string_view client_sock_path
) -> void {
    if (
        std::ssize(server_sock_path) > MaxSockPathLength
        || std::ssize(client_sock_path) > MaxSockPathLength
    ) {
        throw errors::UnrecoverableProtocolError{
            std::format("Max sock_path length is {}", MaxSockPathLength)
        };
    }

    if (std::remove(client_sock_path.data()) == ErrorCode && errno != ENOENT) {
        throw errors::UnrecoverableLinuxError{
            errno,
            "std::remove()",
            "Can't delete socket file"
        };
    }

    memset(server_addr, 0, addr_len);
    server_addr->sun_family = AF_UNIX;
    strcpy(static_cast<char*>(server_addr->sun_path), server_sock_path.data());

    memset(client_addr, 0, addr_len);
    client_addr->sun_family = AF_UNIX;
    strcpy(static_cast<char*>(client_addr->sun_path), client_sock_path.data());
}

auto create_socket() -> int
{
    int client_sock{socket(AF_UNIX, SOCK_STREAM, 0)};
    if (client_sock == ErrorCode) {
        throw errors::UnrecoverableLinuxError{
            errno,
            "socket(2)",
            std::strerror(errno) // NOLINT(concurrency-mt-unsafe)
        };
    }
    return client_sock;
}

auto bind_socket(
    int* client_sock,
    sockaddr_un* client_addr,
    socklen_t addr_len
) -> void {
    if (bind(
        *client_sock,
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        reinterpret_cast<sockaddr*>(client_addr),
        addr_len
    ) == ErrorCode) {
        throw errors::UnrecoverableLinuxError{
            errno,
            "bind(2)",
            std::strerror(errno) // NOLINT(concurrency-mt-unsafe)
        };
    }
}

auto connect_server(
    int* client_sock,
    sockaddr_un* server_addr,
    socklen_t addr_len
) -> void {
    for (int i=1; i<=3; i++) {
        if (connect(
            *client_sock,
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            reinterpret_cast<sockaddr*>(server_addr),
            addr_len
        ) == ErrorCode) {
            switch (errno) {
                case EACCES:
                case EPERM:
                    throw errors::UnrecoverableLinuxError{
                        errno,
                        "connect(2)",
                        "Access error during connection."
                    };

                case EADDRINUSE:
                    throw errors::UnrecoverableLinuxError{
                        errno,
                        "connect(2)",
                        "Local address is already in use."
                    };

                case EADDRNOTAVAIL:
                    throw errors::UnrecoverableLinuxError{
                        errno,
                        "connect(2)",
                        "Internet domain sockets: address unavailable."
                    };

                case EAFNOSUPPORT:
                    throw errors::UnrecoverableLinuxError{
                        errno,
                        "connect(2)",
                        "An invalid address family is specified."
                    };

                case EAGAIN:
                    if (i==3) {
                        throw errors::UnrecoverableLinuxError{
                            errno,
                            "connect(2)",
                            "The socket is nonblocking, and the connection cannot be completed immediately"
                        };
                    }
                    continue;

                case EALREADY:
                    throw errors::UnrecoverableLinuxError{
                        errno,
                        "connect(2)",
                        "The socket is nonblocking and a previous connection attempt has not yet been completed."
                    };

                case EBADF:
                    throw errors::UnrecoverableLinuxError{
                        errno,
                        "connect(2)",
                        "sockfd is not a valid open file descriptor."
                    };

                case ECONNREFUSED:
                    throw errors::UnrecoverableLinuxError{
                        errno,
                        "connect(2)",
                        "A connect() on a stream socket found no one listening on the remote address."
                    };

                case EFAULT:
                    throw errors::UnrecoverableLinuxError{
                        errno,
                        "connect(2)",
                        "The socket structure address is outside the user's address space."
                    };

                case EINPROGRESS:
                    throw errors::UnrecoverableLinuxError{
                        errno,
                        "connect(2)",
                        "The socket is nonblocking and the connection cannot be completed immediately."
                    };

                case EINTR:
                    throw errors::UnrecoverableLinuxError{
                        errno,
                        "connect(2)",
                        "The system call was interrupted by a signal that was caught."
                    };

                case ENETUNREACH:
                    throw errors::UnrecoverableLinuxError{
                        errno,
                        "connect(2)",
                        "Network is unreachable."
                    };

                case EPROTOTYPE:
                    throw errors::UnrecoverableLinuxError{
                        errno,
                        "connect(2)",
                        "The socket type does not support the requested communications protocol."
                    };

                case ETIMEDOUT:
                    if (i==3) {
                        throw errors::UnrecoverableLinuxError{
                            errno,
                            "connect(2)",
                            "Timeout while attempting connection. The server may be too busy to accept new connections."
                        };
                    }
                    continue;

                default:
                    throw errors::UnrecoverableLinuxError{
                        errno,
                        "connect(2)",
                        std::strerror(errno) // NOLINT(concurrency-mt-unsafe)
                    };
            }
        }
        return;
    }
}

auto send_request(int* client_sock, Buffer<void>* request) -> void
{
    for (int i=1; i<=3; i++) {
        if (send(
            *client_sock,
            request->data.get(),
            static_cast<std::size_t>(request->size),
            0
        ) == ErrorCode) {
            switch (errno) {
                case EINTR:
                    if (i==3) {
                        throw errors::UnrecoverableLinuxError{
                            errno,
                            "send(2)",
                            "A signal occurred before any data was transmitted."
                        };
                    }
                    continue;

                case ECONNRESET:
                    throw errors::UnrecoverableLinuxError{
                        errno,
                        "send(2)",
                        "Connection reset by peer."
                    };

                case ENOBUFS:
                    throw errors::UnrecoverableLinuxError{
                        errno,
                        "send(2)",
                        "The output queue for a network interface was full. This generally indicates that the interface has stopped sending, but may be caused by transient congestion."
                    };

                case EPIPE:
                    throw errors::UnrecoverableLinuxError{
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

auto handle_response(int* client_sock, Query_method query_method) -> void
{
    switch (query_method) {
        case Query_method::GetTaskList:
            cli::print_task_list(get_task_list(client_sock));
            break;
        case Query_method::CreateNewTask:
            return;
        default:
            throw errors::WarningProtocolError{
                "Unavailable query method."
            };
    }
}

auto run(
    Query_method query_method,
    std::string_view server_sock_path,
    std::string_view client_sock_path
) -> void {
    sockaddr_un server_addr{};
    sockaddr_un client_addr{};

    socklen_t addr_len{sizeof(sockaddr_un)};

    init(&server_addr, &client_addr, addr_len, server_sock_path, client_sock_path);

    int client_sock{create_socket()};

    try {
        bind_socket(&client_sock, &client_addr, addr_len);
        connect_server(&client_sock, &server_addr, addr_len);

        Buffer<void> request{cli::prepare_request(query_method)};
        send_request(&client_sock, &request);
        handle_response(&client_sock, query_method);

        close(client_sock);
    } catch (errors::LinuxError& error) {
        if (client_sock != ErrorCode) {
            close(client_sock);
        }
        throw error;
    }
}

} // namespace tasknow::client
