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
    struct sockaddr_un* server_addr,
    struct sockaddr_un* client_addr,
    socklen_t addr_len,
    std::string_view sock_path
) -> void;
auto create_socket(int* server_sock) -> void;
auto bind_socket(
    int* server_sock,
    struct sockaddr_un* server_addr,
    socklen_t addr_len
) -> void;
auto listen_socket(int* server_sock, const int backlog_size) -> void;
auto accept_client(
    int* server_sock,
    int* client_sock,
    struct sockaddr_un* client_addr,
    socklen_t* addr_len
) -> void;
auto log_peer_name(
    int* client_sock,
    struct sockaddr_un* client_addr,
    socklen_t* addr_len
) -> void;
auto recieve_method(int* client_sock) -> Query_method;
auto handle_request(int* client_sock, Query_method query_method) -> void;

[[noreturn]]
auto serve(
    std::string_view sock_path = DaemonSockPath,
    const int backlog_size = DaemonBacklogSize
) -> void;

} // namespace tasknow

#endif // TASKNOW_SERVERD_H
