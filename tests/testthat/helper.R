skip_if_no_rabbitmq <- function() {
    testthat::skip_on_cran()
    if (identical(Sys.getenv("TRAVIS"), "true")) {
        return(NULL)
    }
    testthat::skip("RabbitMQ server is not available")
}
