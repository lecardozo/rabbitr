#include <Rcpp.h>
#include <amqp.h>
#include <amqp_tcp_socket.h>

using namespace Rcpp;

// [[Rcpp::export]]
SEXP rabbitmq_connect(std::string host, int port) {
    int status;
    amqp_socket_t *socket = NULL;
    amqp_connection_state_t conn;

    conn = amqp_new_connection();

    socket = amqp_tcp_socket_new(conn);

    status = amqp_socket_open(socket, host.c_str(), port);

    amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, "guest", "guest");

    amqp_channel_open(conn, 1);

    amqp_get_rpc_reply(conn);

    Rcpp::XPtr<amqp_connection_state_t> p(&conn);

    return p;
}

void rabbitmq_declare_queue(SEXP xptr) {
    amqp_bytes_t queue;
    amqp_connection_state_t *conn = rabbitmq_get_state(xptr);
    amqp_queue_declare_ok_t *r = amqp_queue_declare(
        conn, 1, amqp_empty_bytes, 0, 0, 0, 1, amqp_empty_table
    );

    queue = amqp_bytes_malloc_dup(r->queue);
    if (queue.bytes == NULL) {
        Rcout << "Out of memory while copying queue name";
    }
}

// [[Rcpp::export]]
void rabbitmq_consume(SEXP xptr) {
}


amqp_connection_state_t rabbitmq_get_state(SEXP xptr) {
    void *state = NULL;
    if (TYPEOF(extPtr) != EXTPTRSXP) {
      error("Expected an external pointer");
    }
    state = (amqp_connection_state_t*) R_ExternalPtrAddr(xptr);
    if (!state) {
      error("Rabbitmq is not connected");
    }
    return state;
}
