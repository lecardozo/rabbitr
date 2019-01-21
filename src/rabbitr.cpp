// [[Rcpp::plugins(cpp11)]]
#include <Rcpp.h>
#include <string>
#include <amqp.h>
#include <amqp_tcp_socket.h>
#include "utils.h"

using namespace Rcpp;

// [[Rcpp::export("amqp_connect")]]
SEXP connect(std::string host, int port, std::string username,
             std::string password) {
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
        throw std::range_error("Failed to connect.");
    }

    amqp_login(conn, "/", 1, 131072, 0, AMQP_SASL_METHOD_PLAIN,
               username.c_str(), password.c_str());

    Rcpp::XPtr<amqp_connection_state_t_, PreserveStorage, amqp_finalize_connection> xptr(conn, true);
    return xptr;
}

// [[Rcpp::export("amqp_channel_open")]]
void channel_open(SEXP xptr, int channel) {
    amqp_connection_state_t conn = get_connection_state(xptr);
    amqp_channel_open(conn, channel);
}

// [Rcpp::export("amqp_channel_close")]]
void channel_close(SEXP xptr, int channel) {
    amqp_connection_state_t conn = get_connection_state(xptr);
    amqp_channel_close(conn, channel, AMQP_REPLY_SUCCESS);
}


//[[Rcpp::export("amqp_exchange_declare")]]
void exchange_declare(SEXP xptr, int channel, std::string exchange,
                      std::string type, bool passive, bool durable,
                      bool auto_delete, bool internal) {
    amqp_connection_state_t conn = get_connection_state(xptr);
    #if (AMQP_VERSION_MINOR >= 6) && (AMQP_VERSION_PATCH >= 0)
    amqp_exchange_declare(conn, channel,
            amqp_cstring_bytes(exchange.c_str()),
            amqp_cstring_bytes(type.c_str()),
            passive, durable, auto_delete, internal,
            amqp_empty_table
    );
    #else
    amqp_exchange_declare(conn, channel,
            amqp_cstring_bytes(exchange.c_str()),
            amqp_cstring_bytes(type.c_str()),
            passive, durable, amqp_empty_table
    );
    #endif
}

//[[Rcpp::export("amqp_exchange_delete")]]
void exchange_delete(SEXP xptr, int channel, std::string exchange,
                     bool if_unused) {
    amqp_connection_state_t conn = get_connection_state(xptr);
    amqp_exchange_delete(conn, channel,
            amqp_cstring_bytes(exchange.c_str()),
            if_unused
    );
}

//[[Rcpp::export("amqp_exchange_bind")]]
void exchange_bind(SEXP xptr, int channel, std::string destination,
                   std::string source, std::string routing_key) {
    amqp_connection_state_t conn = get_connection_state(xptr);
    amqp_exchange_bind(conn, channel,
            amqp_cstring_bytes(destination.c_str()),
            amqp_cstring_bytes(source.c_str()),
            amqp_cstring_bytes(routing_key.c_str()),
            amqp_empty_table
    );
}

//[[Rcpp::export("amqp_exchange_unbind")]]
void exchange_unbind(SEXP xptr, int channel, std::string destination,
                     std::string source, std::string routing_key) {
    amqp_connection_state_t conn = get_connection_state(xptr);
    amqp_exchange_unbind(conn, channel,
            amqp_cstring_bytes(destination.c_str()),
            amqp_cstring_bytes(source.c_str()),
            amqp_cstring_bytes(routing_key.c_str()),
            amqp_empty_table
    );
}

// [[Rcpp::export("amqp_queue_declare")]]
void queue_declare(SEXP xptr, int channel, std::string queue,
                   bool passive, bool durable, bool exclusive,
                   bool auto_delete) {
    amqp_bytes_t queuename;
    queuename = amqp_cstring_bytes(queue.c_str());
    amqp_connection_state_t conn = get_connection_state(xptr);
    amqp_queue_declare(
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
    amqp_queue_delete(
        conn, channel, queuename, if_unused, if_empty
    );
}

// [[Rcpp::export("amqp_queue_purge")]]
void queue_purge(SEXP xptr, int channel, std::string queue) {
    amqp_connection_state_t conn = get_connection_state(xptr);
    amqp_queue_purge(conn, channel, amqp_cstring_bytes(queue.c_str()));
}

// [[Rcpp::export("amqp_queue_bind")]]
void queue_bind(SEXP xptr, int channel, std::string queue,
                std::string exchange, std::string routing_key) {
    amqp_connection_state_t conn = get_connection_state(xptr);
    amqp_queue_bind(
        conn, channel,
        amqp_cstring_bytes(queue.c_str()),
        amqp_cstring_bytes(exchange.c_str()),
        amqp_cstring_bytes(routing_key.c_str()),
        amqp_empty_table
    );
}

// [[Rcpp::export("amqp_queue_unbind")]]
void queue_unbind(SEXP xptr, int channel, std::string queue,
                  std::string exchange, std::string routing_key) {
    amqp_connection_state_t conn = get_connection_state(xptr);
    amqp_queue_unbind(
        conn, channel,
        amqp_cstring_bytes(queue.c_str()),
        amqp_cstring_bytes(exchange.c_str()),
        amqp_cstring_bytes(routing_key.c_str()),
        amqp_empty_table
    );
}


// [[Rcpp::export("amqp_basic_get")]]
List basic_get(SEXP xptr, int channel, std::string queue, 
               bool no_ack) {
    amqp_rpc_reply_t reply;
    amqp_message_t message;
    amqp_bytes_t queuename = amqp_cstring_bytes(queue.c_str());
    amqp_connection_state_t conn = get_connection_state(xptr);

    reply = amqp_basic_get(conn, channel, queuename, no_ack);
    
    if (reply.reply_type != AMQP_RESPONSE_NORMAL) {
      if (reply.reply_type == AMQP_RESPONSE_SERVER_EXCEPTION) {
        switch (reply.reply.id) {
          case AMQP_CHANNEL_CLOSE_METHOD: {
            amqp_channel_close_t *m = (amqp_channel_close_t *) reply.reply.decoded;
            std::string error((char *) m->reply_text.bytes,
                              (int) m->reply_text.len);
            amqp_channel_close_ok_t resp;
            amqp_send_method(conn, channel, AMQP_CHANNEL_CLOSE_OK_METHOD, 
                             &resp);
            throw std::runtime_error("Server channel error " + error);
            break;
          }
          case AMQP_CONNECTION_CLOSE_METHOD: {
            amqp_connection_close_ok_t  m;
            amqp_send_method(conn, channel, AMQP_CONNECTION_CLOSE_OK_METHOD, &m);
            break;
          }
        }
      } else if (reply.reply_type == AMQP_RESPONSE_LIBRARY_EXCEPTION) {
          Rcout << "lbrary exception" << std::endl;
      }
    }

    amqp_read_message(conn, channel, &message, 0);

    char *body_bytes = (char*)malloc(message.body.len);
    memcpy(body_bytes, message.body.bytes, message.body.len);
    std::string body(body_bytes);

    return Rcpp::List::create(Rcpp::Named("body") = body);
}

// [[Rcpp::export("amqp_basic_cancel")]]
void basic_cancel(SEXP xptr, int channel, std::string consumer_tag) {
    amqp_connection_state_t conn = get_connection_state(xptr);
    amqp_basic_cancel(conn, channel, amqp_cstring_bytes(consumer_tag.c_str()));
}

// [[Rcpp::export("amqp_basic_publish")]]
void basic_publish(SEXP xptr, int channel, std::string exchange,
                   std::string routing_key, std::string body,
                   List properties, bool mandatory,
                   bool immediate) {

    amqp_basic_properties_t props= convert_properties(properties);
    amqp_connection_state_t conn = get_connection_state(xptr);
    amqp_basic_publish(conn, channel,
                       amqp_cstring_bytes(exchange.c_str()),
                       amqp_cstring_bytes(routing_key.c_str()),
                       mandatory, immediate, &props,
                       amqp_cstring_bytes(body.c_str()));
}

// [[Rcpp::export("amqp_basic_consume")]]
void basic_consume(SEXP xptr, int channel, std::string queue,
                   std::string consumer_tag,
                   bool no_ack, bool exclusive) {
    amqp_connection_state_t conn = get_connection_state(xptr);
    amqp_basic_consume(
            conn, channel,
            amqp_cstring_bytes(queue.c_str()),
            amqp_cstring_bytes(consumer_tag.c_str()),
            0, no_ack, exclusive, amqp_empty_table
    );
}

// [Rcpp::export("amqp_basic_ack")]]
void basic_ack(SEXP xptr, int channel, int delivery_tag,
               bool multiple) {
    amqp_connection_state_t conn = get_connection_state(xptr);
    amqp_basic_ack(conn, channel, delivery_tag, multiple);
}

// [Rcpp::export("amqp_basic_qos")]]
void basic_qos(SEXP xptr, int channel, int prefetch_size,
               int prefetch_count, bool global) {
    amqp_connection_state_t conn = get_connection_state(xptr);
    amqp_basic_qos(conn, channel, prefetch_size, prefetch_count, global);
}

// [Rcpp::export("amqp_basic_reject")]]
void basic_reject(SEXP xptr, int channel, int delivery_tag,
                  bool requeue) {
    amqp_connection_state_t conn = get_connection_state(xptr);
    amqp_basic_reject(conn, channel, delivery_tag, requeue);
}

// [Rcpp::export("amqp_basic_recover")]]
void basic_recover(SEXP xptr, int channel, bool requeue) {
    amqp_connection_state_t conn = get_connection_state(xptr);
    amqp_basic_recover(conn, channel, requeue);
}

//[[Rcpp::export("amqp_consume_message")]]
SEXP consume_message(SEXP xptr,
                     Rcpp::Nullable<int> timeout = R_NilValue) {
    struct timeval tval;
    struct timeval *tout;

    if (timeout.isNull()) {
        tout = NULL;
    } else {
        tout = &tval;
        int secs = Rcpp::as<int>(timeout);
        tout->tv_sec = secs;
        tout->tv_usec = 0;
    }

    amqp_connection_state_t conn = get_connection_state(xptr);
    amqp_frame_t frame;
    amqp_rpc_reply_t res;
    amqp_envelope_t envelope;
    amqp_maybe_release_buffers(conn);

    res = amqp_consume_message(conn, &envelope, tout, 0);

    if (AMQP_RESPONSE_NORMAL != res.reply_type) {
        if (AMQP_RESPONSE_LIBRARY_EXCEPTION == res.reply_type &&
            AMQP_STATUS_UNEXPECTED_STATE == res.library_error) {
            if (AMQP_STATUS_OK != amqp_simple_wait_frame(conn, &frame)) {
                amqp_destroy_envelope(&envelope);
                return R_NilValue;
            }
        }
    } else {
        return rabbitr_envelope(envelope);
    }

    return R_NilValue;
}
