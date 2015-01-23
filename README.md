# RingBuffers
#### _C++ thread-safe fixed-size circular buffer/queue templates for many concurrency use cases_

A template library of circular buffers to solve a few variations of the [Producer-Consumer problem](http://en.wikipedia.org/wiki/Producer%E2%80%93consumer_problem). It attempts to enable the following behaviors:

* Single or multiple reads of each element
* Parallel reads and writes
* Prioritized reads and writes

The goal is also to make use of C++11's general language improvements and multi-threading facilities using a classic problem of multi-threaded synchronization.


## The problem
### Introduction

In computing, certain processes are sequential in nature and can be divided into several independent stages to be executed one after the other. A stage is independent from the other in that all it needs to produce an output is its input. For example, a software module could compute a sequence of values of a mathematical function and another one could display those values for a user. This simple example describes two stages of computation but some sequential processes are more complex and can be divided into many more stages.

In most cases, it is necessary that the order in which the elements of data are processed by a stage be that in which those elements were produced by the previous stage. In the previous example, if the computed sequence of values was "1, 2, 3" it would then be necessary that the display module obtain those values in that order from the computation module and that the display reflect that order.

Processes that have those two properties (namely divisibility into independent stages and sensitivity to the order of the flow of data) are usually referred to as _pipelines_.

A program running in a single thread will usually execute only one stage at a time. Maybe it will execute one stage after the other or maybe, in order to avoid cache misses, it will perform many executions of one stage and save their results in memory before moving onto the next stage which will use those results as input.

A program that uses many threads, at most one per independent stage, however, can take advantage of the independence of the various stages to speed up the whole process to a possibly great extent. Indeed, in a pipeline where stage B follows stage A, while a thread is busy executing stage A to produce the input of B, another thread can execute B using the already-produced output of stage A without waiting on stage A to complete its run. Similarly, if a thread is busy executing B, another one can execute A to produce inputs for stage B without waiting on stage B to complete its run.

The problem that arises then is that of the communication between the threads that run the stages. They should be able to

* check for availability of input somewhere in memory...
  * ...waiting until input is available if necessary
* read the input

and to

* check for availability of memory space to write their output...
  * ...waiting until memory space is available if necessary
* write their output

without unwanted behaviors such as

 * deadlocks (thread 1 waiting on a condition A that only thread 2 can satisfy and thread 2 waiting on a condition B that only thread 1 can satisfy, or any such circular pattern of dependency)
 * [race conditions](http://en.wikipedia.org/wiki/Race_condition) (two threads attempt to write to the same memory location at once in a non-atomic fashion so that data is corrupted).

The problem of satisfying those constraints is called the _Producer-Consumer problem_.


### Vocabulary

The memory space that is allocated for use by two subsequent stages of a pipeline is called the _shared memory_.

From the point of view of a given shared memory, the stage that writes to it is called the "_producer_" while the stage that reads from it is called the "_consumer_". Note that a stage that is a consumer with respect to a given shared memory may be a producer for the memory that it shares with the subsequent stage, and vice versa.


### Variations on the problem
#### One producer, one consumer

The simplest instance of the Producer-Consumer problem has one producer, one consumer and one shared memory. The problem previously used as an example, where a module computes mathematical values and another displays them, is an instance of that problem.

In this library, in order to satisfy the above-mentioned criteria for correctness of the solution, synchronization primitives are used, namely, C++11 [mutexes](http://en.cppreference.com/w/cpp/thread/mutex) and custom [semaphores](http://en.wikipedia.org/wiki/Semaphore_%28programming%29) (which are themselves implemented in terms of C++11 mutexes and C++11 [condition variables](http://en.cppreference.com/w/cpp/thread/condition_variable)).

__TODO__


### Multiple consumers

In this variation of the problem, multiple stages attempt to read from the shared memory that the producer writes to. However, there are two different ways for consumers to read from the shared memory:

__TODO__


### Multiple producers

__TODO__


### Multiple consumers and producers

Simply combining the solutions to the previous two problems suffices to allow for multiple consumers and producers.


### Producers writing in parallel

__TODO__


### Prioritizing certain stages of the pipeline

__TODO__


## TODOs

* complete the readme
* doxygen
* comment the demos
* make comprehensive demos
* make unit tests
* profile (compare with boost circular buffer lib?)