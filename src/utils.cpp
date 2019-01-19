#include <Rcpp.h>
#include <string>
#include <amqp.h>
#include <amqp_tcp_socket.h>

amqp_bytes_t rstr_to_bytes(SEXP a) {
    return amqp_cstring_bytes(Rcpp::as<std::string>(a).c_str());
}

amqp_basic_properties_t convert_properties(Rcpp::List r_props) {
    amqp_basic_properties_t amqp_props;
    amqp_props.content_type = rstr_to_bytes(r_props["content_type"]);
    amqp_props.content_encoding = rstr_to_bytes(r_props["content_encoding"]);
    amqp_props.correlation_id = rstr_to_bytes(r_props["correlation_id"]);
    amqp_props.reply_to = rstr_to_bytes(r_props["reply_to"]);
    amqp_props.expiration = rstr_to_bytes(r_props["expiration"]);
    amqp_props.message_id = rstr_to_bytes(r_props["message_id"]);
    amqp_props._flags = AMQP_BASIC_DELIVERY_MODE_FLAG |
                        AMQP_BASIC_CONTENT_TYPE_FLAG |
                        AMQP_BASIC_CONTENT_ENCODING_FLAG |
                        AMQP_BASIC_REPLY_TO_FLAG;
    return amqp_props;
}

Rcpp::List rabbitr_message(amqp_message_t message) {
    amqp_basic_properties_t props = message.properties;

    std::string content_type((char *) props.content_type.bytes,
                             props.content_type.len);
    std::string content_encoding((char *) props.content_encoding.bytes,
                                 props.content_encoding.len);
    std::string correlation_id((char *) props.correlation_id.bytes,
                               props.correlation_id.len);
    std::string reply_to((char *) props.reply_to.bytes, props.reply_to.len);
    std::string expiration((char *) props.expiration.bytes, props.expiration.len);
    std::string message_id((char *) props.message_id.bytes, props.message_id.len);
    std::string body((char *) message.body.bytes, message.body.len);

    return Rcpp::List::create(
        Rcpp::Named("properties") = Rcpp::List::create(
            Rcpp::Named("content_type") = Rcpp::String(content_type),
            Rcpp::Named("content_encoding") = Rcpp::String(content_encoding),
            Rcpp::Named("delivery_mode") = (int) props.delivery_mode,
            Rcpp::Named("priority") = (int) props.priority,
            Rcpp::Named("correlation_id") = Rcpp::String(correlation_id),
            Rcpp::Named("reply_to") = Rcpp::String(reply_to),
            Rcpp::Named("expiration") = Rcpp::String(expiration),
            Rcpp::Named("message_id") = Rcpp::String(message_id),
            Rcpp::Named("timestamp") = (long int) props.timestamp
        ),
        Rcpp::Named("body") = Rcpp::String(body)
    );
}

Rcpp::List rabbitr_envelope(amqp_envelope_t envelope) {
    std::string exchange((char *) envelope.exchange.bytes,
                         envelope.exchange.len);
    std::string routing_key((char *)envelope.routing_key.bytes,
                            envelope.routing_key.len);

    return Rcpp::List::create(
        Rcpp::Named("delivery_tag") = envelope.delivery_tag,
        Rcpp::Named("routing_key") = Rcpp::String(routing_key),
        Rcpp::Named("exchange") = Rcpp::String(exchange),
        Rcpp::Named("message") = rabbitr_message(envelope.message)
    );
}

void amqp_finalize_connection(amqp_connection_state_t_* conn) {
    amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(conn);
}

amqp_connection_state_t_* get_connection_state(SEXP xptr) {
    if (TYPEOF(xptr) != EXTPTRSXP) {
        throw std::range_error("Expected an external pointer");
    }
    Rcpp::XPtr<amqp_connection_state_t_, Rcpp::PreserveStorage, amqp_finalize_connection, true> conn(xptr);
    return conn;
}
