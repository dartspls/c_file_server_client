# File server and client in C
Server and client can be compiled using `make all` or `make server` and `make client` for individual
compilation.

To run the server: `./server 5000` will run the server on port 5000. The server can be shutdown with
a SIGINT and will close the socket, allowing the port to be reused immediately. The server uses
threads (no pooling) to handle concurrent transmissions.

To run the client: `./client <ip> <port> <filename>`. Each client may only request 1 file per transmission, but multiple clients may request files at the same time and the server will handle these requests in parallel.


## Decisions, assumptions, limitations
I decided to use threads with no pooling as it works OK for a smallish project.
Although the server is able to handle multiple transmissions in parallel, there is still
a single thread responsible for accepting and dispatching worker threads, which
would be a bottleneck with high numbers of concurrent connections.

## Resources used
I have written socket servers in Java before, but not in C. I found [Jacob Sorber's](https://www.youtube.com/@JacobSorber) videos particularly helpful for learning sockets in C. Specifically his [sockets in C series](https://www.youtube.com/playlist?list=PL9IEJIKnBJjH_zM5LnovnoaKlXML5qh17).

I followed [this tutorial from Colby College](https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/) to create the makefile.
