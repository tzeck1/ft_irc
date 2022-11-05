/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   refactored.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmeising <mmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/05 19:15:00 by mmeising          #+#    #+#             */
/*   Updated: 2022/11/05 19:51:41 by mmeising         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.hpp"

static int	handle_cmds(std::string msg, std::vector<struct pollfd> &client_fds, int i)
{
	int	err = 0;

	if (msg.find("USER ") != std::string::npos)
		err = send(client_fds[i].fd, ":10.11.4.29 001 mmeising :Welcome to the Internet Relay Network mmeising!mmeising@10.11.4.27\r\n", 94, 0);
	else if (msg.find("PING") != std::string::npos)
		err = send(client_fds[i].fd, "PONG 10.11.4.29\r\n", 17, 0);
	return (err);
}

/*
**	initialises socket_fd, 
*/
int	init_socket(void)
{
	int	socket_fd = 0;
	int	err = 0;

	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0)
	{
		std::cerr << "Socket creation failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	err = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, NULL, 0);//don't need "opt" variable as we don't use ioctl
	if (err < 0)
	{
		std::cerr << "Setsockopt failed" << std::endl;
		close(socket_fd);
		exit(EXIT_FAILURE);
	}
	err = fcntl(socket_fd, F_SETFL, O_NONBLOCK);
	if (err < 0)
	{
		std::cerr << "File control failed" << std::endl;
		close(socket_fd);
		exit(EXIT_FAILURE);
	}
	return (socket_fd);
}

void	socket_bind_listen(struct sockaddr_in *socket_addr, int socket_fd)
{
	int err = 0;

	std::memset(socket_addr, 0, sizeof(*socket_addr));
	socket_addr->sin_family = AF_INET;
	socket_addr->sin_addr.s_addr = htonl(INADDR_ANY); // listn to all clients
	socket_addr->sin_port = htons(SERVER_PORT); // our port :)
	err = bind(socket_fd, (struct sockaddr *)socket_addr, sizeof(*socket_addr));
	if (err < 0)
	{
		std::cerr << "Binding failed" << std::endl;
		close(socket_fd);
		exit(EXIT_FAILURE);
	}

	err = listen(socket_fd, 10);
	if (err < 0)
	{
		std::cerr << "Listening failed" << std::endl;
		close(socket_fd);
		exit(EXIT_FAILURE);
	}
}

void	init(void)
{
	int							socket_fd = -1;
	int							timeout;
	struct sockaddr_in			socket_addr;
	std::vector<struct pollfd>	client_fds(200);

	socket_fd = init_socket();
	socket_bind_listen(&socket_addr, socket_fd);
	init_listening_socket();
	client_fds[0].fd = socket_fd;
	client_fds[0].events = POLLIN;
	timeout = (3 * 60 * 1000);


}