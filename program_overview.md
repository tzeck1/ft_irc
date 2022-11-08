socket() to create socket to receive incoming connections;
make socket descriptors reusable;
set socket nonblocking (incoming connections inherit this property);
bind the socket to a socket address;
set listen backlog, allow server to listen;

DO WHILE (!end_server):
	poll() to wait on connections;

	FOR (all current connections):
		find the one that has revents, 
		if no POLLIN, break (need bitwise AND for that or check for other states too);

		if listen fd sent stuff, DO WHILE:
			accept all incoming connections

		else DO WHILE (true):
			recv from this connection until it fails;
			break if:
			-recv < 0 (fail)
			-recv == 0 (connection closed by user)
			-send < 0 (fail)

		if connection was closed:
			close the fd;
			set compress flag;
	
	after FOR, if compress flag is set, compress vector (no empty spot)

ending, close all fds before quitting.