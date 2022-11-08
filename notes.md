socket():
returns a socket descriptor representing an endpoint.

setsockopt():
reuse local address when server restart without required wait time.

fcntl():
set the socket to nonblocking; incoming connection sockets inherit the nonblocking state from the listening socket.

bind():
gets a unique name for the socket.

listen():
allows server to accept incoming client connections.

poll():
allows process to sleep until an event occurs:
 0  - timed out
-1  - process failed
 1  - one descriptor ready to be processed, is processed only if it is the listening socket
 1++ - multiple descriptors waiting to be processed, poll() allows simultaneous connection with all descriptors in the queue on the listening socket

accept() and recv():
complete when EWOULDBLOCK is returned.

recv():
< 0 - failed
= 0 - client closed connection
>=0 - len of received data

send():
sends to client.

close():
closes any open socket descriptors.