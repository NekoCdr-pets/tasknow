//===--- serverd.h - Tasknow daemon socket server -------------------------===//
//
// Copyright (c) 2023 Yuri Istomin
//
// Part of the NekoCdr's "tasknow" project
// Licensed under the Apache License, Version 2.0 (the "License").
// See http://www.apache.org/licenses/LICENSE-2.0 for license information.
//
//===----------------------------------------------------------------------===//

#ifndef TASKNOW_SERVERD_H
#define TASKNOW_SERVERD_H

#include "defines.h"

#include <string_view>
#include <sys/socket.h>
#include <sys/un.h>

namespace tasknow::serverd {

auto init(
    sockaddr_un* server_addr,
    sockaddr_un* client_addr,
    socklen_t addr_len,
    std::string_view sock_path
) -> void;

auto create_socket() -> int;

auto bind_socket(
    int* server_sock,
    sockaddr_un* server_addr,
    socklen_t addr_len
) -> void;

auto listen_socket(int* server_sock, int backlog_size) -> void;

auto accept_client(
    int* server_sock,
    sockaddr_un* client_addr,
    socklen_t* addr_len
) -> int;

auto get_peer_name(
    int* client_sock,
    sockaddr_un* client_addr,
    socklen_t* addr_len
) -> std::string;

auto recieve_method(int* client_sock) -> Query_method;

auto handle_request(int* client_sock, Query_method query_method) -> void;

[[noreturn]]
auto serve(
    std::string_view sock_path = DaemonSockPath,
    int backlog_size = DaemonBacklogSize
) -> void;

} // namespace tasknow::serverd

#endif // TASKNOW_SERVERD_H
