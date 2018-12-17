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
void queue_declare(SEXP xptr, int channel, std::string queue,
                   bool passive, bool durable, bool exclusive,
                   bool auto_delete) {
    amqp_bytes_t queuename;
    queuename = amqp_cstring_bytes(queue.c_str());
    amqp_connection_state_t conn = get_connection_state(xptr);
    amqp_queue_declare_ok_t *r = amqp_queue_declare(
        conn, channel, queuename, passive,
        durable, exclusive, auto_delete, amqp_empty_table
    );
}

// [[Rcpp::export("amqp_queue_delete")]]
void queue_delete(SEXP xptr, int channel, std::string queue,
                  bool if_unused, bool if_empty) {
    amqp_bytes_t queuename;
    queuename = amqp_cstring_bytes(queue.c_str());
    amqp_connection_state_t conn = get_connection_state(xptr);
    amqp_queue_delete_ok_t *r = amqp_queue_delete(
        conn, channel, queuename, if_unused, if_empty
    );
}

// [[Rcpp::export("amqp_basic_get")]]
List basic_get(SEXP xptr, int channel, std::string queue) {
    amqp_rpc_reply_t reply;
    amqp_message_t message;
    amqp_bytes_t queuename = amqp_cstring_bytes(queue.c_str());
    amqp_connection_state_t conn = get_connection_state(xptr);

    reply = amqp_basic_get(conn, channel, queuename, 1);

    amqp_read_message(conn, channel, &message, 0);

    char *body_bytes = (char*)malloc(message.body.len);
    memcpy(body_bytes, message.body.bytes, message.body.len);
    std::string body(body_bytes);

    return Rcpp::List::create(Rcpp::Named("body") = body);
}

void basic_publish(SEXP xptr, int channel, std::string exchange,
                   std::string routing_key, bool mandatory,
                   bool immediate, std::string body) {

    amqp_basic_properties_t = props;
    props.content_type = amqp_cstring_bytes("text/plain");
    amqp_connection_state_t conn = get_connection_state(xptr);
    amqp_basic_publish(conn, channel,
                       amqp_cstring_bytes(exchange),
                       amqp_cstring_bytes(routing_key),
                       mandatory, immediate, &props,
                       amqp_cstring_bytes(body.c_str()))
}
