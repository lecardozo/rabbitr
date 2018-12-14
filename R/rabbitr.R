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
        initialize = function(conn, channel_number) {
            self$conn = conn
            amqp_channel_open(self$conn$xptr, channel_number)
        },

        queue_declare = function(queue) {
            amqp_declare_queue(self$conn$xptr)
        },
        
        basic_consume = function() {
            while (TRUE) {
                amqp_consume(self$conn$xptr)
                sys.sleep(10)
            }
        }
    )
)
