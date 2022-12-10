# OS Layer in C #

[![Build Status](https://travis-ci.org/bjtj/osl-c.svg?branch=master)](https://travis-ci.org/bjtj/osl-c)

## Target platforms ##

* Linux
* Windows
* Mac OSX

## TODO ##

* xml

## APIs ##

* mutex
* thread
* semaphore
* string buffer
* file
* date
* list
* network
* socket -- server socket, socket, select, datagram, multicast
* library
* process
* logger
* url
* argument parser
* looper
* environment
* heap
* properties file


## Build ##

```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
```


## Test ##

```bash
$ make test
```

test with valgrind (memcheck)

```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
$ ctest -D ExperimentalMemCheck
```

or specific test only

e.g.)

```bash
$ ctest -R testName -D ExperimentalMemCheck
```
