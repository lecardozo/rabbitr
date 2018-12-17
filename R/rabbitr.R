#' @useDynLib rabbitr
#' @importFrom Rcpp sourceCpp
NULL

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
            self$host = host
            self$port = port
            self$username = username
            self$password = password
            self$xptr = amqp_connect(host, port)
        },

        channel = function() {
            ch = Channel$new(self, private$current_channel)
            private$current_channel = private$current_channel + 1
            return(ch)
        }
    ),

    private = list(
         channels = list(),
         current_channel = 1 
    )
)

Channel <- R6::R6Class(
    'Channel',
    public = list(
        conn = NULL,
        channel_number = NULL,
        initialize = function(conn, channel_number) {
            self$conn = conn
            self$channel_number = channel_number
            amqp_channel_open(self$conn$xptr, channel_number)
        },

        queue_declare = function(queue, passive=FALSE, durable=FALSE,
                                 exclusive=FALSE, auto_delete=FALSE) {
            amqp_queue_declare(self$conn$xptr, self$channel_number,
                               queue, passive=passive, durable=durable,
                               exclusive=exclusive,
                               auto_delete=auto_delete)
        },

        queue_delete = function(queue, if_unused=FALSE, if_empty=FALSE) {
            amqp_queue_delete(self$conn$xptr, self$channel_number,
                              queue, if_unused=if_unused,
                              if_empty=if_empty)
        },
        
        basic_get = function(queue) {
            amqp_basic_get(self$conn$xptr, self$channel_number, queue)
        },

        basic_publish = function(queue, msg) {
        }
    )
)
