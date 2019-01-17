# rabbitr [![Travis-CI Build Status](https://travis-ci.org/lecardozo/rabbitr.svg?branch=master)](https://travis-ci.org/lecardozo/rabbitr)

RabbitMQ client library for R

## Installation

``` r
devtools::install_github("lecardozo/rabbitr")
```

## Usage
#### Publisher 
``` r
# connect to RabbitMQ server
conn <- rabbitr('localhost')

# create a channel
chan <- conn$channel()

# declare queue
chan$queue_declare('testing')

# publish message
chan$basic_publish(exchange='', routing_key='testing', 
                   body='this is the message')
```

#### Consumer
```r
# connect to RabbitMQ server
conn <- rabbitr('localhost')

# create a channel
chan <- conn$channel()

# declare queue
chan$queue_declare('testing')

# start consumer
chan$basic_consume(queue='testing')

# listen for incoming messages
chan$listen(function(envelope) {
    print(envelope$message)
})
```
