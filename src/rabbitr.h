#include <Rcpp.h>
#include <amqp.h>

SEXP connect(std::string host, int port, std::string username,
             std::string password);

void channel_open(SEXP xptr, int channel);

void channel_close(SEXP xptr, int channel);

void exchange_declare(SEXP xptr, int channel, std::string exchange,
                      std::string type, bool passive,
                      bool durable, bool auto_delete, bool internal);

void exchange_delete(SEXP xptr, int channel, std::string exchange,
                     bool if_unused);

void exchange_bind(SEXP xptr, int channel, std::string destination,
                   std::string source, std::string routing_key);

void exchange_unbind(SEXP xptr, int channel, std::string destination,
                     std::string source, std::string routing_key);

void queue_declare(SEXP xptr, int channel, std::string queue,
                   bool passive, bool durable, bool exclusive,
                   bool auto_delete);

void queue_delete(SEXP xptr, int channel, std::string queue,
                  bool if_unused, bool if_empty);

void queue_purge(SEXP xptr, int channel, std::string queue);

void queue_bind(SEXP xptr, int channel, std::string queue,
                std::string exchange, std::string routing_key);

void queue_unbind(SEXP xptr, int channel, std::string queue,
                  std::string exchange, std::string routing_key);

Rcpp::List basic_get(SEXP xptr, int channel, std::string queue, 
                     bool no_ack);

void basic_cancel(SEXP xptr, int channel, std::string consumer_tag);

void basic_publish(SEXP xptr, int channel, std::string exchange,
                   std::string routing_key, std::string body
                   Rcpp::List properties, bool mandatory,
                   bool immediate);

void basic_consume(SEXP xptr, int channel, std::string queue,
                   std::string consumer_tag,
                   bool no_ack, bool exclusive);

void basic_ack(SEXP xptr, int channel, int delivery_tag,
               bool multiple);

void basic_qos(SEXP xptr, int channel, int prefetch_size,
               int prefetch_count, bool global);

void basic_reject(SEXP xptr, int channel, int delivery_tag,
                  bool requeue);

void basic_recover(SEXP xptr, int channel, bool requeue);

SEXP consume_message(SEXP xptr, Rcpp::Nullable<int> timeout = R_NilValue);
