# WebServio

WebServio is a web server written in C that partially conforms to HTTP/1.1.
It can be set to use a multiplexed I/O architecture (all connections simultaneously watched for activity by a single thread) or a multithreaded architecture (each connection is on its own thread). 
It includes a command-line interface for changing configuration settings. 

## Learning Objectives

As with most of my past projects, the purpose of building WebServio was more than to simply build something interesting. 
It was also to learn new technologies and challenge myself to read published documents from the IETF.

There were five main learning objectives for building WebServio:
1. Reading RFC documents, specifically [RFC 7230](https://tools.ietf.org/html/rfc7230) and [RFC 7231](https://tools.ietf.org/html/rfc7231)
2. Working with Berkeley sockets API
3. Working with ```pthreads```
4. Practice writing programs in C
5. Working in a Linux environment
