#include <Rcpp.h>
#include <string>
#include <amqp.h>
#include <amqp_tcp_socket.h>
#include "utils.h"

using namespace Rcpp;

amqp_connection_state_t_* get_connection_state(SEXP xptr) {

    if (TYPEOF(xptr) != EXTPTRSXP) {
        throw std::range_error("Expected an external pointer");
    }
    Rcpp::XPtr<amqp_connection_state_t_> conn(xptr);

    return conn;
}

void amqp_finalize_connection(amqp_connection_state_t_* conn) {
    amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(conn);
}

// [[Rcpp::export("amqp_connect")]]
SEXP connect(std::string host, int port) {
    int status;
    amqp_socket_t *socket = NULL;
    amqp_connection_state_t_ *conn;

    conn = amqp_new_connection();

    socket = amqp_tcp_socket_new(conn);
    if (!socket) {
        throw std::range_error("Could not create socket.");
    }

    status = amqp_socket_open(socket, host.c_str(), port);
    if (status != AMQP_STATUS_OK) {
        Rcout << status << std::endl;
        throw std::range_error("Failed to connect.");
    }

    amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, "guest", "guest");

    Rcpp::XPtr<amqp_connection_state_t_, PreserveStorage, amqp_finalize_connection> xptr(conn, true);
    return xptr;
}

// [[Rcpp::export("amqp_channel_open")]]
void channel_open(SEXP xptr, int channel) {
    amqp_connection_state_t conn = get_connection_state(xptr);
    amqp_channel_open(conn, channel);
}

// [[Rcpp::export("amqp_queue_declare")]]
void queue_declare(SEXP xptr) {
    amqp_bytes_t queue;
    amqp_connection_state_t conn = get_connection_state(xptr);
    amqp_queue_declare_ok_t *r = amqp_queue_declare(
        conn, 1, amqp_empty_bytes, 0, 0, 0, 1, amqp_empty_table
    );

    queue = amqp_bytes_malloc_dup(r->queue);
    if (queue.bytes == NULL) {
        Rcout << "Out of memory while copying queue name";
    }
}

// [[Rcpp::export("amqp_consume")]]
void consume(SEXP xptr) {
    amqp_rpc_reply_t res;
    amqp_envelope_t envelope;
    amqp_connection_state_t conn = get_connection_state(xptr);
    amqp_maybe_release_buffers(conn);

    res = amqp_consume_message(conn, &envelope, NULL, 0);

    amqp_destroy_envelope(&envelope);
}

