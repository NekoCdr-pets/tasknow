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

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <string>
#include <string_view>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

using namespace std::literals;

namespace tasknow::serverd {

auto init(
    struct sockaddr_un* server_addr,
    struct sockaddr_un* client_addr,
    socklen_t addr_len,
    std::string_view sock_path
) -> void {
    memset(server_addr, 0, addr_len);
    server_addr->sun_family = AF_UNIX;
    // TODO: add check for sock_path length
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
    struct sockaddr_un* server_addr,
    socklen_t addr_len
) -> void {
    if (bind(
        *server_sock,
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        reinterpret_cast<struct sockaddr*>(server_addr),
        addr_len
    ) == ErrorCode) {
        throw errors::UnrecoverableLinuxError{
            errno,
            "bind(2)",
            std::strerror(errno) // NOLINT(concurrency-mt-unsafe)
        };
    }
}

auto listen_socket(int* server_sock, const int backlog_size) -> void
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
    struct sockaddr_un* client_addr,
    socklen_t* addr_len
) -> int {
    int client_sock{};
    while (true) {
        client_sock = accept(
            *server_sock,
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            reinterpret_cast<struct sockaddr*>(client_addr),
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
    struct sockaddr_un* client_addr,
    socklen_t* addr_len
) -> std::string {
    if (getpeername(
        *client_sock,
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        reinterpret_cast<struct sockaddr*>(client_addr),
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

auto recieve_method(int* client_sock) -> Query_method
{
    Query_method query_method{Query_method::EnumEnd};

    for (int i=1; i<=3; i++) {
        if (recv(*client_sock, &query_method, BytesForSize, 0) == ErrorCode) {
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
        return query_method;
    }
}

auto handle_request(int* client_sock, Query_method query_method) -> void
{
    if (
        query_method < Query_method::EnumStart
        || query_method >= Query_method::EnumEnd
    ) {
        throw errors::WarningApplicationError{
            "Unknown query method."
        };
    }
    // TODO: call matching method

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
                        "recv(2)",
                        "Connection reset by peer."
                    };

                case ENOBUFS:
                    throw errors::WarningLinuxError{
                        errno,
                        "recv(2)",
                        "The output queue for a network interface was full. This generally indicates that the interface has stopped sending, but may be caused by transient congestion."
                    };

                case EPIPE:
                    throw errors::WarningLinuxError{
                        errno,
                        "recv(2)",
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
    }
}

auto serve(std::string_view sock_path, int backlog_size) -> void
{
    struct sockaddr_un server_addr{};
    struct sockaddr_un client_addr{};

    socklen_t addr_len{sizeof(struct sockaddr_un)};

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

    while (true) {
        try {
            client_sock = accept_client(&server_sock, &client_addr, &addr_len);

            std::string peer_name{
                get_peer_name(&client_sock, &client_addr, &addr_len)
            };

            Query_method query_method{recieve_method(&client_sock)};

            handle_request(&client_sock, query_method);
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
        } catch (errors::WarningApplicationError& error) {
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
