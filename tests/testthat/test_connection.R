context("AMQP Connection")

test_that("rabbitr succesfully stablishes connection with RabbitMQ", {
    skip_if_no_rabbitmq()
    conn <- rabbitr()
})
