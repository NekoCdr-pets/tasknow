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

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <format>
#include <iostream>
#include <string_view>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

namespace tasknow::serverd {

auto init(
    struct sockaddr_un* server_addr,
    struct sockaddr_un* client_addr,
    socklen_t addr_len,
    const char* sock_path
) -> void {
    memset(server_addr, 0, addr_len);
    server_addr->sun_family = AF_UNIX;\
    // TODO: add check for sock_path length
    strcpy(static_cast<char*>(server_addr->sun_path), sock_path);

    memset(client_addr, 0, addr_len);

    if (std::remove(sock_path) == ErrorCode && errno != ENOENT) {
        throw std::string_view{"Can't delete socket file"};
    }
}

auto create_socket(int* server_sock) -> void
{
    *server_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (*server_sock == ErrorCode) {
        throw std::string_view{std::format("Socket error: %d", errno)};
    }
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
        throw std::string_view{std::format("Bind socket error: %d", errno)};
    }
}

auto listen_socket(int* server_sock, const int backlog_size) -> void
{
    if (listen(*server_sock, backlog_size) == ErrorCode) {
        throw std::string_view{std::format("Listen socket error: %d", errno)};
    }
}

auto accept_client(
    int* server_sock,
    int* client_sock,
    struct sockaddr_un* client_addr,
    socklen_t* addr_len
) -> void {
    *client_sock = accept(
        *server_sock,
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        reinterpret_cast<struct sockaddr*>(client_addr),
        addr_len
    );
    if (*client_sock == ErrorCode) {
        throw std::string_view{"Can't accept client"};
    }
}

auto log_peer_name(
    int* client_sock,
    struct sockaddr_un* client_addr,
    socklen_t* addr_len
) -> void {
    if (getpeername(
        *client_sock,
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        reinterpret_cast<struct sockaddr*>(client_addr),
        addr_len
    ) == ErrorCode) {
        // TODO: handle error
    }
    // TODO: return "client_sockaddr.sun_path"
}

auto recieve_method(int* client_sock) -> Query_method
{
    Query_method query_method{Query_method::EnumEnd};
    if (recv(
        *client_sock,
        &query_method,
        BytesForSize,
        0
    ) == ErrorCode) {
        throw std::string_view{"Can't get request method"};
    }
    return query_method;
}

auto handle_request(int* client_sock, Query_method query_method) -> void
{
    if (
        query_method < Query_method::EnumStart
        || query_method >= Query_method::EnumEnd
    ) {
        throw std::string_view{"Unknown query method"};
    }
    // TODO: call matching method

    if (send(
        *client_sock,
        &query_method,
        BytesForSize,
        0
    ) == ErrorCode) {
        throw std::string_view{"Can't send answer"};
    }
}

auto serve(const char* sock_path, const int backlog_size) -> void
{
    int server_sock{ErrorCode};
    int client_sock{ErrorCode};

    struct sockaddr_un server_addr{};
    struct sockaddr_un client_addr{};

    socklen_t addr_len{sizeof(struct sockaddr_un)};

    init(&server_addr, &client_addr, addr_len, sock_path);
    create_socket(&server_sock);
    try {
        bind_socket(&server_sock, &server_addr, addr_len);
        listen_socket(&server_sock, backlog_size);
    } catch (const std::string_view& error_msg) {
        if (server_sock != ErrorCode) {
            close(server_sock);
        }
        throw error_msg;
    }

    while (true) {
        try {
            accept_client(&server_sock, &client_sock, &client_addr, &addr_len);
            log_peer_name(&client_sock, &client_addr, &addr_len);
            Query_method query_method{recieve_method(&client_sock)};
            handle_request(&client_sock, query_method);
        } catch (const std::string_view& error_msg) {
            // TODO: handle error
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

} // namespace tasknow
