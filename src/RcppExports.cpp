// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

// connect
SEXP connect(std::string host, int port, std::string username, std::string password);
RcppExport SEXP _rabbitr_connect(SEXP hostSEXP, SEXP portSEXP, SEXP usernameSEXP, SEXP passwordSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::string >::type host(hostSEXP);
    Rcpp::traits::input_parameter< int >::type port(portSEXP);
    Rcpp::traits::input_parameter< std::string >::type username(usernameSEXP);
    Rcpp::traits::input_parameter< std::string >::type password(passwordSEXP);
    rcpp_result_gen = Rcpp::wrap(connect(host, port, username, password));
    return rcpp_result_gen;
END_RCPP
}
// channel_open
void channel_open(SEXP xptr, int channel);
RcppExport SEXP _rabbitr_channel_open(SEXP xptrSEXP, SEXP channelSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< SEXP >::type xptr(xptrSEXP);
    Rcpp::traits::input_parameter< int >::type channel(channelSEXP);
    channel_open(xptr, channel);
    return R_NilValue;
END_RCPP
}
// queue_declare
void queue_declare(SEXP xptr, int channel, std::string queue, bool passive, bool durable, bool exclusive, bool auto_delete);
RcppExport SEXP _rabbitr_queue_declare(SEXP xptrSEXP, SEXP channelSEXP, SEXP queueSEXP, SEXP passiveSEXP, SEXP durableSEXP, SEXP exclusiveSEXP, SEXP auto_deleteSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< SEXP >::type xptr(xptrSEXP);
    Rcpp::traits::input_parameter< int >::type channel(channelSEXP);
    Rcpp::traits::input_parameter< std::string >::type queue(queueSEXP);
    Rcpp::traits::input_parameter< bool >::type passive(passiveSEXP);
    Rcpp::traits::input_parameter< bool >::type durable(durableSEXP);
    Rcpp::traits::input_parameter< bool >::type exclusive(exclusiveSEXP);
    Rcpp::traits::input_parameter< bool >::type auto_delete(auto_deleteSEXP);
    queue_declare(xptr, channel, queue, passive, durable, exclusive, auto_delete);
    return R_NilValue;
END_RCPP
}
// queue_bind
void queue_bind(SEXP xptr, int channel, std::string queue, std::string exchange, std::string routing_key);
RcppExport SEXP _rabbitr_queue_bind(SEXP xptrSEXP, SEXP channelSEXP, SEXP queueSEXP, SEXP exchangeSEXP, SEXP routing_keySEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< SEXP >::type xptr(xptrSEXP);
    Rcpp::traits::input_parameter< int >::type channel(channelSEXP);
    Rcpp::traits::input_parameter< std::string >::type queue(queueSEXP);
    Rcpp::traits::input_parameter< std::string >::type exchange(exchangeSEXP);
    Rcpp::traits::input_parameter< std::string >::type routing_key(routing_keySEXP);
    queue_bind(xptr, channel, queue, exchange, routing_key);
    return R_NilValue;
END_RCPP
}
// queue_unbind
void queue_unbind(SEXP xptr, int channel, std::string queue, std::string exchange, std::string routing_key);
RcppExport SEXP _rabbitr_queue_unbind(SEXP xptrSEXP, SEXP channelSEXP, SEXP queueSEXP, SEXP exchangeSEXP, SEXP routing_keySEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< SEXP >::type xptr(xptrSEXP);
    Rcpp::traits::input_parameter< int >::type channel(channelSEXP);
    Rcpp::traits::input_parameter< std::string >::type queue(queueSEXP);
    Rcpp::traits::input_parameter< std::string >::type exchange(exchangeSEXP);
    Rcpp::traits::input_parameter< std::string >::type routing_key(routing_keySEXP);
    queue_unbind(xptr, channel, queue, exchange, routing_key);
    return R_NilValue;
END_RCPP
}
// queue_purge
void queue_purge(SEXP xptr, int channel, std::string queue);
RcppExport SEXP _rabbitr_queue_purge(SEXP xptrSEXP, SEXP channelSEXP, SEXP queueSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< SEXP >::type xptr(xptrSEXP);
    Rcpp::traits::input_parameter< int >::type channel(channelSEXP);
    Rcpp::traits::input_parameter< std::string >::type queue(queueSEXP);
    queue_purge(xptr, channel, queue);
    return R_NilValue;
END_RCPP
}
// queue_delete
void queue_delete(SEXP xptr, int channel, std::string queue, bool if_unused, bool if_empty);
RcppExport SEXP _rabbitr_queue_delete(SEXP xptrSEXP, SEXP channelSEXP, SEXP queueSEXP, SEXP if_unusedSEXP, SEXP if_emptySEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< SEXP >::type xptr(xptrSEXP);
    Rcpp::traits::input_parameter< int >::type channel(channelSEXP);
    Rcpp::traits::input_parameter< std::string >::type queue(queueSEXP);
    Rcpp::traits::input_parameter< bool >::type if_unused(if_unusedSEXP);
    Rcpp::traits::input_parameter< bool >::type if_empty(if_emptySEXP);
    queue_delete(xptr, channel, queue, if_unused, if_empty);
    return R_NilValue;
END_RCPP
}
// listen
void listen(SEXP xptr, Rcpp::Function callback, Rcpp::Nullable<long int> timeout);
RcppExport SEXP _rabbitr_listen(SEXP xptrSEXP, SEXP callbackSEXP, SEXP timeoutSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< SEXP >::type xptr(xptrSEXP);
    Rcpp::traits::input_parameter< Rcpp::Function >::type callback(callbackSEXP);
    Rcpp::traits::input_parameter< Rcpp::Nullable<long int> >::type timeout(timeoutSEXP);
    listen(xptr, callback, timeout);
    return R_NilValue;
END_RCPP
}
// basic_get
List basic_get(SEXP xptr, int channel, std::string queue, bool no_ack);
RcppExport SEXP _rabbitr_basic_get(SEXP xptrSEXP, SEXP channelSEXP, SEXP queueSEXP, SEXP no_ackSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< SEXP >::type xptr(xptrSEXP);
    Rcpp::traits::input_parameter< int >::type channel(channelSEXP);
    Rcpp::traits::input_parameter< std::string >::type queue(queueSEXP);
    Rcpp::traits::input_parameter< bool >::type no_ack(no_ackSEXP);
    rcpp_result_gen = Rcpp::wrap(basic_get(xptr, channel, queue, no_ack));
    return rcpp_result_gen;
END_RCPP
}
// basic_cancel
void basic_cancel(SEXP xptr, int channel, std::string consumer_tag);
RcppExport SEXP _rabbitr_basic_cancel(SEXP xptrSEXP, SEXP channelSEXP, SEXP consumer_tagSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< SEXP >::type xptr(xptrSEXP);
    Rcpp::traits::input_parameter< int >::type channel(channelSEXP);
    Rcpp::traits::input_parameter< std::string >::type consumer_tag(consumer_tagSEXP);
    basic_cancel(xptr, channel, consumer_tag);
    return R_NilValue;
END_RCPP
}
// basic_publish
void basic_publish(SEXP xptr, int channel, std::string exchange, std::string routing_key, bool mandatory, bool immediate, std::string body);
RcppExport SEXP _rabbitr_basic_publish(SEXP xptrSEXP, SEXP channelSEXP, SEXP exchangeSEXP, SEXP routing_keySEXP, SEXP mandatorySEXP, SEXP immediateSEXP, SEXP bodySEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< SEXP >::type xptr(xptrSEXP);
    Rcpp::traits::input_parameter< int >::type channel(channelSEXP);
    Rcpp::traits::input_parameter< std::string >::type exchange(exchangeSEXP);
    Rcpp::traits::input_parameter< std::string >::type routing_key(routing_keySEXP);
    Rcpp::traits::input_parameter< bool >::type mandatory(mandatorySEXP);
    Rcpp::traits::input_parameter< bool >::type immediate(immediateSEXP);
    Rcpp::traits::input_parameter< std::string >::type body(bodySEXP);
    basic_publish(xptr, channel, exchange, routing_key, mandatory, immediate, body);
    return R_NilValue;
END_RCPP
}
// basic_consume
void basic_consume(SEXP xptr, int channel, std::string queue, std::string consumer_tag, bool no_ack, bool exclusive);
RcppExport SEXP _rabbitr_basic_consume(SEXP xptrSEXP, SEXP channelSEXP, SEXP queueSEXP, SEXP consumer_tagSEXP, SEXP no_ackSEXP, SEXP exclusiveSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< SEXP >::type xptr(xptrSEXP);
    Rcpp::traits::input_parameter< int >::type channel(channelSEXP);
    Rcpp::traits::input_parameter< std::string >::type queue(queueSEXP);
    Rcpp::traits::input_parameter< std::string >::type consumer_tag(consumer_tagSEXP);
    Rcpp::traits::input_parameter< bool >::type no_ack(no_ackSEXP);
    Rcpp::traits::input_parameter< bool >::type exclusive(exclusiveSEXP);
    basic_consume(xptr, channel, queue, consumer_tag, no_ack, exclusive);
    return R_NilValue;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_rabbitr_connect", (DL_FUNC) &_rabbitr_connect, 4},
    {"_rabbitr_channel_open", (DL_FUNC) &_rabbitr_channel_open, 2},
    {"_rabbitr_queue_declare", (DL_FUNC) &_rabbitr_queue_declare, 7},
    {"_rabbitr_queue_bind", (DL_FUNC) &_rabbitr_queue_bind, 5},
    {"_rabbitr_queue_unbind", (DL_FUNC) &_rabbitr_queue_unbind, 5},
    {"_rabbitr_queue_purge", (DL_FUNC) &_rabbitr_queue_purge, 3},
    {"_rabbitr_queue_delete", (DL_FUNC) &_rabbitr_queue_delete, 5},
    {"_rabbitr_listen", (DL_FUNC) &_rabbitr_listen, 3},
    {"_rabbitr_basic_get", (DL_FUNC) &_rabbitr_basic_get, 4},
    {"_rabbitr_basic_cancel", (DL_FUNC) &_rabbitr_basic_cancel, 3},
    {"_rabbitr_basic_publish", (DL_FUNC) &_rabbitr_basic_publish, 7},
    {"_rabbitr_basic_consume", (DL_FUNC) &_rabbitr_basic_consume, 6},
    {NULL, NULL, 0}
};

RcppExport void R_init_rabbitr(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
