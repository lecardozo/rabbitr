// [[Rcpp::plugins(cpp11)]]
#include <Rcpp.h>
#include <string>
#include <amqp.h>
#include <amqp_tcp_socket.h>

using namespace Rcpp;

amqp_bytes_t rstr_to_bytes(SEXP a) {
    return amqp_cstring_bytes(Rcpp::as<std::string>(a).c_str());
}

amqp_basic_properties_t convert_properties(List r_props) {
    amqp_basic_properties_t amqp_props;
    amqp_props.content_type = rstr_to_bytes(r_props["content_type"]);
    amqp_props.content_encoding = rstr_to_bytes(r_props["content_encoding"]);
    amqp_props.correlation_id = rstr_to_bytes(r_props["correlation_id"]);
    amqp_props.reply_to = rstr_to_bytes(r_props["reply_to"]);
    amqp_props.expiration = rstr_to_bytes(r_props["expiration"]);
    amqp_props.message_id = rstr_to_bytes(r_props["message_id"]);
    return amqp_props;
}

List rabbitr_message(amqp_message_t message) {
    amqp_basic_properties_t props = message.properties;

    std::string content_type((char *) props.content_type.bytes,
                             props.content_type.len);
    std::string content_encoding((char *) props.content_encoding.bytes,
                                 props.content_encoding.len);
    std::string correlation_id((char *) props.correlation_id.bytes,
                               props.correlation_id.len);
    std::string reply_to((char *) props.reply_to.bytes, props.reply_to.len);
    std::string expiration((char *)props.expiration.bytes, props.expiration.len);
    std::string message_id((char *)props.message_id.bytes, props.message_id.len);
    std::string body((char *)message.body.bytes, message.body.len);

    return List::create(
        Named("properties") = List::create(
            Named("content_type") = String(content_type),
            Named("content_encoding") = String(content_encoding),
            Named("delivery_mode") = (int) props.delivery_mode,
            Named("priority") = (int) props.priority,
            Named("correlation_id") = String(correlation_id),
            Named("reply_to") = String(reply_to),
            Named("expiration") = String(expiration),
            Named("message_id") = String(message_id),
            Named("timestamp") = (long int) props.timestamp
        ),
        Named("body") = String(body)
    );
}

List rabbitr_envelope(amqp_envelope_t envelope) {
    std::string exchange((char *) envelope.exchange.bytes,
                         envelope.exchange.len);
    std::string routing_key((char *)envelope.routing_key.bytes,
                            envelope.routing_key.len);

    return Rcpp::List::create(
        Rcpp::Named("delivery_tag") = envelope.delivery_tag,
        Rcpp::Named("routing_key") = String(routing_key),
        Rcpp::Named("exchange") = String(exchange),
        Rcpp::Named("message") = rabbitr_message(envelope.message)
    );
}

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

// [[Rcpp::export("amqp_queue_purge")]]
void queue_purge(SEXP xptr, int channel, std::string queue) {
    amqp_connection_state_t conn = get_connection_state(xptr);
    amqp_queue_purge(conn, channel, amqp_cstring_bytes(queue.c_str()));
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

//[[Rcpp::export("amqp_listen")]]
void listen(SEXP xptr, Rcpp::Function callback,
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

    for (;;) {
        Rcpp::checkUserInterrupt();

        amqp_rpc_reply_t res;
        amqp_envelope_t envelope;
        amqp_maybe_release_buffers(conn);

        res = amqp_consume_message(conn, &envelope, tout, 0);

        if (AMQP_RESPONSE_NORMAL != res.reply_type) {
            if (AMQP_RESPONSE_LIBRARY_EXCEPTION == res.reply_type &&
                AMQP_STATUS_UNEXPECTED_STATE == res.library_error) {
                if (AMQP_STATUS_OK != amqp_simple_wait_frame(conn, &frame)) {
                    break;
                }
            }
        } else {
            callback(rabbitr_envelope(envelope));
        }
        amqp_destroy_envelope(&envelope);
    }
}

// [[Rcpp::export("amqp_basic_get")]]
List basic_get(SEXP xptr, int channel, std::string queue, 
               bool no_ack) {
    amqp_rpc_reply_t reply;
    amqp_message_t message;
    amqp_bytes_t queuename = amqp_cstring_bytes(queue.c_str());
    amqp_connection_state_t conn = get_connection_state(xptr);

    reply = amqp_basic_get(conn, channel, queuename, no_ack);

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
                   std::string routing_key, bool mandatory,
                   bool immediate, std::string body, List properties) {

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
