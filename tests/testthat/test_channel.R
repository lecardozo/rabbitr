context("AMQP Channel")

test_that("channel creation", {
    skip_if_no_rabbitmq()
    conn <- rabbitr()
    chan <- conn$channel()
})
