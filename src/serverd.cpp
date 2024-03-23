//===--- serverd.cpp - Tasknow daemon socket server -----------------------===//
//
// Copyright (c) 2023 Yuri Istomin
//
// Part of the NekoCdr's "tasknow" project
// Licensed under the Apache License, Version 2.0 (the "License").
// See http://www.apache.org/licenses/LICENSE-2.0 for license information.
//
//===----------------------------------------------------------------------===//

#include "serverd.h"

#include "defines.h"
#include "errors.h"
#include "request_handler.h"
#include "task_list.h"

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <format>
#include <iterator>
#include <string>
#include <string_view>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

using namespace std::literals;

namespace tasknow::serverd {

auto init(
    sockaddr_un* server_addr,
    sockaddr_un* client_addr,
    socklen_t addr_len,
    std::string_view sock_path
) -> void {
    memset(server_addr, 0, addr_len);
    server_addr->sun_family = AF_UNIX;

    int max_sock_path_length = sizeof(sockaddr_un::sun_path) / sizeof(char);
    if (std::ssize(sock_path) > max_sock_path_length) {
        throw errors::UnrecoverableProtocolError{
            std::format("Max sock_path length is {}", max_sock_path_length)
        };
    }
    strcpy(static_cast<char*>(server_addr->sun_path), sock_path.data());

    memset(client_addr, 0, addr_len);

    if (std::remove(sock_path.data()) == ErrorCode && errno != ENOENT) {
        throw errors::UnrecoverableLinuxError{
            errno,
            "std::remove()",
            "Can't delete socket file"
        };
    }
}

auto create_socket() -> int
{
    int server_sock{socket(AF_UNIX, SOCK_STREAM, 0)};
    if (server_sock == ErrorCode) {
        throw errors::UnrecoverableLinuxError{
            errno,
            "socket(2)",
            std::strerror(errno) // NOLINT(concurrency-mt-unsafe)
        };
    }
    return server_sock;
}

auto bind_socket(
    int* server_sock,
    sockaddr_un* server_addr,
    socklen_t addr_len
) -> void {
    if (bind(
        *server_sock,
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        reinterpret_cast<sockaddr*>(server_addr),
        addr_len
    ) == ErrorCode) {
        throw errors::UnrecoverableLinuxError{
            errno,
            "bind(2)",
            std::strerror(errno) // NOLINT(concurrency-mt-unsafe)
        };
    }
}

auto listen_socket(int* server_sock, int backlog_size) -> void
{
    if (listen(*server_sock, backlog_size) == ErrorCode) {
        throw errors::UnrecoverableLinuxError{
            errno,
            "listen(2)",
            std::strerror(errno) // NOLINT(concurrency-mt-unsafe)
        };
    }
}

auto accept_client(
    int* server_sock,
    sockaddr_un* client_addr,
    socklen_t* addr_len
) -> int {
    while (true) {
        int client_sock{};
        client_sock = accept(
            *server_sock,
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            reinterpret_cast<sockaddr*>(client_addr),
            addr_len
        );

        if (client_sock == ErrorCode) {
            // TODO: do refactoring of similar cases
            switch (errno) {
                case ECONNABORTED:
                    errors::log_error_to_stdout(
                        errors::WarningLinuxError{
                            errno,
                            "accept(2)",
                            "A connection has been aborted."
                        }
                    );
                    continue;

                case EINTR:
                    errors::log_error_to_stdout(
                        errors::WarningLinuxError{
                            errno,
                            "accept(2)",
                            "The system call was interrupted by a signal that was caught before a valid connection arrived."
                        }
                    );
                    continue;

                case EPERM:
                    errors::log_error_to_stdout(
                        errors::WarningLinuxError{
                            errno,
                            "accept(2)",
                            "Firewall rules forbid connection."
                        }
                    );
                    continue;

                default:
                    throw errors::UnrecoverableLinuxError{
                        errno,
                        "accept(2)",
                        std::strerror(errno) // NOLINT(concurrency-mt-unsafe)
                    };
            }
        } else {
            return client_sock;
        }
    }
}

auto get_peer_name(
    int* client_sock,
    sockaddr_un* client_addr,
    socklen_t* addr_len
) -> std::string {
    if (getpeername(
        *client_sock,
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        reinterpret_cast<sockaddr*>(client_addr),
        addr_len
    ) == ErrorCode) {
        switch (errno) {
            case ENOTCONN:
                errors::log_error_to_stdout(
                    errors::WarningLinuxError{
                        errno,
                        "getpeername(2)",
                        "The socket is not connected."
                    }
                );
                return "Unknown peername"s;

            default:
                throw errors::UnrecoverableLinuxError{
                    errno,
                    "getpeername(2)",
                    std::strerror(errno) // NOLINT(concurrency-mt-unsafe)
                };
        }
    }
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    return {client_addr->sun_path};
}

auto get_request_method(int* client_sock) -> Query_method
{
    Query_method query_method{Query_method::EnumEnd};

    request_handler::receive_data(client_sock, &query_method, BytesForSize);

    return query_method;
}

auto handle_request(
    int* client_sock,
    Query_method query_method,
    Task_list* task_list
) -> void {
    if (
        query_method < Query_method::EnumStart
        || query_method >= Query_method::EnumEnd
    ) {
        throw errors::WarningProtocolError{
            "Unknown query method."
        };
    }
    // TODO: call matching method
    switch (query_method) {
        case Query_method::GetTaskList:
            request_handler::get_task_list(client_sock, task_list);
            break;
        default:
            throw errors::WarningProtocolError{
                "Unavailable query method."
            };
    }
}

auto serve(std::string_view sock_path, int backlog_size) -> void
{
    sockaddr_un server_addr{};
    sockaddr_un client_addr{};

    socklen_t addr_len{sizeof(sockaddr_un)};

    init(&server_addr, &client_addr, addr_len, sock_path);

    int server_sock{create_socket()};
    int client_sock{ErrorCode};

    try {
        bind_socket(&server_sock, &server_addr, addr_len);
        listen_socket(&server_sock, backlog_size);
    } catch (errors::LinuxError& error) {
        if (server_sock != ErrorCode) {
            close(server_sock);
        }
        throw error;
    }

    Task_list task_list{};

    while (true) {
        try {
            client_sock = accept_client(&server_sock, &client_addr, &addr_len);

            std::string peer_name{
                get_peer_name(&client_sock, &client_addr, &addr_len)
            };

            Query_method query_method{recieve_method(&client_sock)};

            handle_request(&client_sock, query_method, &task_list);
        } catch (errors::WarningLinuxError& error) {
            errors::log_error_to_stdout(error);
            if (client_sock != ErrorCode) {
                close(client_sock);
            }
            continue;
        } catch (errors::UnrecoverableLinuxError& error) {
            if (client_sock != ErrorCode) {
                close(client_sock);
            }
            throw error;
        } catch (errors::WarningProtocolError& error) {
            errors::log_error_to_stdout(error);
            if (client_sock != ErrorCode) {
                close(client_sock);
            }
            continue;
        }

        if (client_sock != ErrorCode) {
            close(client_sock);
        }
    }
}

} // namespace tasknow::serverd
