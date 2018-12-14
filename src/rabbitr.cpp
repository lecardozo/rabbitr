#include <Rcpp.h>
#include <amqp.h>
#include <amqp_tcp_socket.h>

using namespace Rcpp;

// [[Rcpp::export]]
SEXP connect(std::string host, int port) {
    int status;
    amqp_socket_t *socket = NULL;
    amqp_connection_state_t conn;

    amqp_bytes_t queue;

    conn = amqp_new_connection();

    socket = amqp_tcp_socket_new(conn);

    status = amqp_socket_open(socket, "localhost", port);

    amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, "guest", "guest");

    amqp_channel_open(conn, 1);

    amqp_get_rpc_reply(conn);

    Rcpp::XPtr<amqp_connection_state_t> p(&conn);

    return p;
}
