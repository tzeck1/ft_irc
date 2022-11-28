/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_server.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: btenzlin <btenzlin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 13:39:36 by tzeck             #+#    #+#             */
/*   Updated: 2022/11/28 11:35:50 by btenzlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prototypes.hpp"

int	init_server(int port)
{
	/*-----	create socket	-----*/
	int	socket_d = socket(AF_INET, SOCK_STREAM, 0); //socket descriptor
	if (socket_d < 0)
		irc_log(CRITICAL, "socket() failed!");

	/*-----	make socket reuseable	-----*/
	int on = 1; //string containing the option value (useless but will break with NULL)
	int	err = setsockopt(socket_d, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)); // enable socket options
	if (err == -1)
		irc_log(CRITICAL, "setsockopt() failed!");
	
	/*-----	set socket to non-blocking	-----*/
	err = fcntl(socket_d, F_SETFL, O_NONBLOCK); // set nonblocking flag to socket_d
	if (err < 0)
		irc_log(CRITICAL, "fcntl() failed!");

	/*-----	assign address to socket	-----*/
	struct sockaddr_in addr;
	std::memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET; // set to ipv4
	/*	htonl and htons set network byte order so protocoll can recieve/send stuff	*/
	addr.sin_addr.s_addr = htonl(INADDR_ANY); // set address of socket to non-specific
	addr.sin_port = htons(port); // set socket to port (420)
	err = bind(socket_d, (struct sockaddr *)&addr, sizeof(addr));
	if (err == -1)
		irc_log(CRITICAL, "bind() failed!");

	/*-----	listen for connections on a socket	-----*/
	err = listen(socket_d, BACKLOG); // marks socket as passive to recieve incomming messages
	if (err == -1)
		irc_log(CRITICAL, "listen() failed!");

	return (socket_d);
}