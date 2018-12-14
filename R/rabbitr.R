#' @useDynLib rabbitr
#' @importFrom Rcpp sourceCpp
NULL

Connection <- R6::R6Class(
    'Connection',
    public = list(
        host = NULL,
        port = NULL,
        xptr = NULL,

        initialize = function(host='localhost', port=5673) {
            self$host = host
            self$port = port
            self$xptr <- rabbitmq_connect(host, port)
        }
    ),
)

Channel <- R6::R6Class(
    'Channel',
    public = list(
        conn = NULL,
        basic_consume = function() {
            while (TRUE) {
                # call
            }
        }
    )
)

Queue <- R6::R6Class(
    'Queue',
    public = list(
        declare = function() {
           # declare queue
        },

        bind = function(conn) {}
    )
)

Exchange <- R6::R6Class(
    'Exchange',
    public = list(
        name = NULL
    )
)
