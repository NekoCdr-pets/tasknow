//===--- client.h - Tasknow socket client ---------------------------------===//
//
// Copyright (c) 2024 Yuri Istomin
//
// Part of the NekoCdr's "tasknow" project
// Licensed under the Apache License, Version 2.0 (the "License").
// See http://www.apache.org/licenses/LICENSE-2.0 for license information.
//
//===----------------------------------------------------------------------===//

#ifndef TASKNOW_CLIENT_H
#define TASKNOW_CLIENT_H

#include "defines.h"

#include <string_view>
#include <sys/socket.h>
#include <sys/un.h>

namespace tasknow::client {

inline constexpr const int MaxSockPathLength{
    sizeof(sockaddr_un::sun_path) / sizeof(char)
};

auto init(
    sockaddr_un* server_addr,
    sockaddr_un* client_addr,
    socklen_t addr_len,
    std::string_view server_sock_path,
    std::string_view client_sock_path
) -> void;

auto create_socket() -> int;

auto bind_socket(
    int* server_sock,
    sockaddr_un* server_addr,
    socklen_t addr_len
) -> void;

auto connect_server(
    int* client_sock,
    sockaddr_un* server_addr,
    socklen_t addr_len
) -> void;

auto send_request(int* client_sock, Query_method query_method) -> void;

auto handle_response(int* client_sock, Query_method query_method) -> void;

auto run(
    Query_method query_method,
    std::string_view server_sock_path = DaemonSockPath,
    std::string_view client_sock_path = CLISockPath
) -> void;

} // namespace tasknow::client

#endif // TASKNOW_CLIENT_H
