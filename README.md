# rabbitr [![Travis-CI Build Status](https://travis-ci.org/lecardozo/rabbitr.svg?branch=master)](https://travis-ci.org/lecardozo/rabbitr)

RabbitMQ client library for R.


#### System Dependencies
This package relies on the [rabbitmq-c](https://github.com/alanxz/rabbitmq-c) client. To install it run:
##### Ubuntu/Debian installation
```bash
$ sudo apt-get install librabbitmq-dev
```
##### Fedora/CentOS/RHEL installation
```bash
$ sudo yum install librabbitmq-devel
```
or, if you prefer, you can [compile it yourself](https://github.com/alanxz/rabbitmq-c#building-and-installing) .
## Installation


If you already installed the system dependencies, you can proceed to install the rabbitr R package.
``` r
devtools::install_github("lecardozo/rabbitr")
```

## Usage

#### Publishing messages
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

#### Consuming messages
```r
# connect to RabbitMQ server
conn <- rabbitr('localhost')

# create a channel
chan <- conn$channel()

# declare queue
chan$queue_declare('testing')

# define a callback messages arriving
callback <- function(envelope) { print(envelope$message) }

# start consumer
chan$basic_consume(callback, queue='testing')

# listen for incoming messages
chan$start_consuming()
```
