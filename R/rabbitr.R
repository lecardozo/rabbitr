#' @useDynLib rabbitr
#' @importFrom Rcpp sourceCpp
#' @importFrom R6 R6Class
NULL

#' Establishing connection with RabbitMQ
#'
#' This is the main function of the package.
#' It establishes the connection between R and
#' the RabbitMQ server.
#'
#' @param host Address of the RabbitMQ server.
#' @param port Port to connect to RabbitMQ server.
#' @param username Username for authentication.
#' @param password Password for authentication.
#'
#' @return Connection object
#'
#' @examples
#' \dontrun{
#' conn <- rabbitr(host='localhost', port=5672)
#' }
#'
#' @export
rabbitr <- function(host='localhost', port=5672,
                    username='guest', password='guest') {
    return(Connection$new(host=host, port=port,
                          username=username, password=password))
}


Connection <- R6::R6Class(
    'Connection',
    public = list(
        host = NULL,
        port = NULL,
        username = NULL,
        password = NULL,
        xptr = NULL,

        initialize = function(host='localhost', port=5672,
                              username='guest', password='guest') {
            self$host <- host
            self$port <- port
            self$username <- username
            self$password <- password
            self$xptr <- amqp_connect(host, port, username, password)
        },

        channel = function() {
            ch <- Channel$new(self, private$current_channel)
            private$current_channel <- private$current_channel + 1
            return(ch)
        }
    ),

    private = list(
         channels = list(),
         current_channel = 1 
    )
)

#' RabbitMQ Channel object
#'
#' This class defines all AMQP Channel methods and functionality.
#'
#' @section Usage:
#' For more details see \bold{Methods, Arguments and Examples} sections.
#' \preformatted{
#' conn <- rabbitr()
#' chan <- conn$channel()
#' chan$exchange_declare(exchange, type, passive=FALSE,
#'                    durable=FALSE, auto_delete=FALSE)
#' chan$exchange_bind(destination, source, routing_key)
#' chan$exchange_unbind(destination, source, routing_key)
#' chan$exchange_delete(exchange, if_unused=FALSE)
#'
#' chan$queue_declare(queue="", passive=FALSE,
#'                    durable=FALSE, auto_delete=FALSE)
#' chan$queue_bind(queue, exchange, routing_key)
#' chan$queue_unbind(queue, exchange, routing_key)
#' chan$queue_purge(queue)
#' chan$queue_delete(queue, if_unused=FALSE, if_empty=FALSE)
#'
#' chan$basic_consume(callback, queue="", consumer_tag="",
#'                    no_ack=FALSE, exclusive=FALSE)
#' chan$basic_get(queue, no_ack=FALSE)
#' chan$basic_ack(delivery_tag, multiple=FALSE)
#' chan$basic_qos(prefetch_size=0, prefetch_count=0, global=FALSE)
#' chan$basic_reject(delivery_tag, requeue=TRUE)
#' chan$basic_recover(requeue=TRUE)
#' chan$basic_publish(exchange, routing_key, body, 
#'                    properties=message_properties(),
#'                    mandatory=FALSE, immediate=FALSE)
#'
#' chan$start_consuming(timeout=NULL)
#' } 
#' 
#' @section Methods:
#' \describe{
#'    \item{\code{$new(conn, channel_number)}}{Create new Channel instance. This 
#'          should not be used directly. Instead, construct the channel by 
#'          calling the active connection `$channel()` method.}
#'    \item{\code{$exchange_declare()}}{Declare/create exchange.}
#'    \item{\code{$exchange_delete()}}{Delete exchange.}
#'    \item{\code{$exchange_bind()}}{Bind exchange to another exchange.}
#'    \item{\code{$exchange_unbind()}}{Unbind exchange from another exchange.}
#'    \item{\code{$queue_declare()}}{Declare/create queue.}
#'    \item{\code{$queue_delete()}}{Delete queue.}
#'    \item{\code{$queue_purge()}}{Purge all messages from specified queue.}
#'    \item{\code{$queue_bind()}}{Bind queue to exchange.}
#'    \item{\code{$queue_unbind()}}{Unbind queue from a exchange.}
#'    \item{\code{$basic_consume()}}{Sends the AMQP 0-9-1 Basic.Consume 
#'    command to broker.}
#'    \item{\code{$basic_get()}}{Get a single message from the AMQP broker.}
#'    \item{\code{$basic_ack()}}{Acknowledge one or more messages.}
#'    \item{\code{$basic_qos()}}{Specify quality of service.}
#'    \item{\code{$basic_reject()}}{Reject an incoming message.}
#'    \item{\code{$basic_recover()}}{Ask server to redeliver all unacknowledged 
#'     messages on Channel.}
#'    \item{\code{$basic_publish()}}{Publish to the channel with the given
#'    exchange, routing_key and message body.}
#' }
#'
#' @rdname Channel
#' @name Channel 
#'
NULL

#' @export
Channel <- R6::R6Class(
    'Channel',
    private = list(
        exchanges = list(),
        queues = list(),
        queue_callbacks = list()
    ),
    public = list(
        conn = NULL,
        channel_number = NULL,
        initialize = function(conn, channel_number) {
            self$conn <- conn
            self$channel_number <- channel_number
            amqp_channel_open(self$conn$xptr, channel_number)
        },

        exchange_declare = function(exchange, type, passive=FALSE,
                                    durable=FALSE, auto_delete=FALSE,
                                    internal=FALSE) {
            private$exchanges <- append(
                private$exchanges,
                list(list(name=exchange, type=type, passive=passive,
                     durable=durable, auto_delete=auto_delete))
            )

            amqp_exchange_declare(self$conn$xptr, self$channel_number,
                                  exchange, type, passive, durable,
                                  auto_delete)
        },

        exchange_delete = function(exchange, if_unused=FALSE) {
            to_keep <- lapply(private$exchanges, function(exchange){
                (exchange$name != exchange)
            })
            private$exchanges <- private$exchanges[to_keep]
            amqp_exchange_delete(self$conn$xptr, self$channel_number,
                                 exchange, if_unused=if_unused)
        },

        exchange_bind = function(destination, source, routing_key) {
            amqp_exchange_bind(self$conn$xptr, self$channel_number,
                               destination, source, routing_key)
        },

        exchange_unbind = function(destination, source, routing_key) {
            amqp_exchange_unbind(self$conn$xptr, self$channel_number,
                                 destination, source, routing_key)
        },

        queue_declare = function(queue="", passive=FALSE, durable=FALSE,
                                 exclusive=FALSE, auto_delete=FALSE) {
            private$queues <- append(
                private$queues,
                list(list(name=queue, passive=passive, durable=durable,
                          exclusive=exclusive, auto_delete=auto_delete))
            )

            amqp_queue_declare(self$conn$xptr, self$channel_number,
                               queue, passive=passive, durable=durable,
                               exclusive=exclusive,
                               auto_delete=auto_delete)
        },

        queue_purge = function(queue) {
            if (missing(queue)) {
                stop("Must provide queue name.")
            }
            amqp_queue_purge(self$conn$xptr, self$channel_number, queue)
        },

        queue_delete = function(queue="", if_unused=FALSE, if_empty=FALSE) {
            to_keep <- lapply(private$queues, function(queue){
                (queue$name != queue)
            })
            private$queues <- private$queues[to_keep]
            amqp_queue_delete(self$conn$xptr, self$channel_number,
                              queue, if_unused=if_unused,
                              if_empty=if_empty)
        },

        queue_bind = function(queue, exchange, routing_key) {
            if (missing(queue) | missing(exchange) | missing(routing_key)) {
                stop("All arguments are required")
            }
            amqp_queue_bind(self$conn$xptr, self$channel_number,
                            queue=queue, exchange=exchange,
                            routing_key=routing_key)
        },

        queue_unbind = function(queue, exchange, routing_key) {
            if (missing(queue) | missing(exchange) | missing(routing_key)) {
                stop("All arguments are required")
            }
            amqp_queue_bind(self$conn$xptr, self$channel_number,
                            queue=queue, exchange=exchange,
                            routing_key=routing_key)
        },

        basic_consume = function(callback, queue="", consumer_tag="", 
                                 no_ack=FALSE, exclusive=FALSE) {
            private$queue_callbacks[[queue]] <- callback
            amqp_basic_consume(self$conn$xptr, self$channel_number,
                               queue=queue, consumer_tag=consumer_tag,
                               no_ack=no_ack, exclusive=exclusive)
        },

        basic_get = function(queue="", no_ack=FALSE) {
            amqp_basic_get(self$conn$xptr, self$channel_number, queue, no_ack)
        },

        basic_ack = function(delivery_tag, multiple=FALSE) {
            amqp_basic_ack(self$conn$xptr, self$channel_number,
                           delivery_tag=delivery_tag, multiple=multiple)
        },

        basic_qos = function(prefetch_size=0, prefetch_count=0, global=FALSE) {
            amqp_basic_qos(self$conn$xptr, self$channel_number,
                           prefetch_size=prefetch_size,
                           prefetch_count=prefetch_count, global=global)
        },

        basic_reject = function(delivery_tag, requeue=TRUE) {
            amqp_basic_reject(self$conn$xptr, self$channel_number,
                              delivery_tag=delivery_tag, requeue=requeue)
        },

        basic_recover = function(requeue=TRUE) {
            amqp_basic_recover(self$conn$xptr, self$channel_number,
                               requeue=requeue)
        },

        basic_publish = function(exchange, routing_key, body,
                                 properties, mandatory=FALSE,
                                 immediate=FALSE) {
            if (missing(properties)) {
                properties <- basic_message_properties()
            }
            amqp_basic_publish(self$conn$xptr, self$channel_number,
                               exchange=exchange, routing_key=routing_key,
                               body=body, properties=properties,
                               mandatory=mandatory, immediate=immediate)
        },

        start_consuming = function(timeout=0.1) {
            # TODO: this is probably not the ideal solution.
            # Maybe passing the queue_callback map to cpp function
            # and letting it execute the while loop on its own
            # is better...
            while(TRUE) {
                envelope <- amqp_consume_message(self$conn$xptr, 
                                                 timeout=0)
                if (!is.null(envelope)) {
                    private$queue_callbacks[[envelope$routing_key]](envelope)
                }
                Sys.sleep(timeout)
            }
        }
    )
)


basic_message_properties <- function(content_type="text/plain", content_encoding="",
                       delivery_mode=1, priority=0, correlation_id="",
                       reply_to="", expiration="", message_id="",
                       timestamp="", message_type="", user_id="",
                       app_id="") {
    return(as.list(environment(), all=TRUE))
}
