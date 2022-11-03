/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_server.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: btenzlin <btenzlin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 10:52:51 by btenzlin          #+#    #+#             */
/*   Updated: 2022/11/03 18:42:44 by btenzlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/common.hpp"

static int	handle_cmds(std::string msg, std::vector<struct pollfd> &client_fds, int i)
{
	int	err = 0;

	if (msg.find("USER") != std::string::npos)
		err = send(client_fds[i].fd, ":10.11.4.29 001 mmeising :Welcome to the Internet Relay Network mmeising!mmeising@10.11.4.27\r\n", 94, 0);
	else if (msg.find("PING") != std::string::npos)
		err = send(client_fds[i].fd, "PONG 10.11.4.29\r\n", 17, 0);
	return (err);
}

void	init(void)
{
	int							socket_fd = -1, new_fd = -1;
	int							err, len, opt = 1;
	int							current_size = 0, client_count = 1;
	int							timeout;
	struct sockaddr_in			socket_addr;
	std::vector<struct pollfd>	client_fds(200);
	bool						end_server = false, compress_array = false, close_conn;
	char						buffer[80];
	std::stringstream			recv_msg;

	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0)
	{
		std::cerr << "Socket creation failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	err = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));
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

	std::memset(&socket_addr, 0, sizeof(socket_addr));
	socket_addr.sin_family = AF_INET;
	socket_addr.sin_addr.s_addr = htonl(INADDR_ANY); // listn to all clients
	socket_addr.sin_port = htons(SERVER_PORT); // our port :)
	err = bind(socket_fd, (struct sockaddr *)&socket_addr, sizeof(socket_addr));
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

	client_fds[0].fd = socket_fd;
	client_fds[0].events = POLLIN;
	timeout = (3 * 60 * 1000);
	do
	{
		std::cout << "Waiting on poll..." << std::endl;
		err = poll(&client_fds[0], client_count, timeout);
		if (err < 0)
		{
			std::cerr << "Poll failed" << std::endl;
			break ;
		}
		else if (err == 0)
		{
			std::cerr << "Poll timed out" << std::endl;
			break ;
		}

		current_size = client_count;
		for (int i = 0; i < current_size; i++)
		{
			if (client_fds[i].revents == 0)
				continue ;

			if (client_fds[i].revents != POLLIN)
			{
				std::cerr << "Error. Revents = " << client_fds[i].revents << std::endl;
				//testing
				err = recv(client_fds[i].fd, buffer, sizeof(buffer), 0);
				if (err < 0)
				{
					if (errno != EWOULDBLOCK)
					{
						std::cerr << "Receive failed" << std::endl;
						close_conn = true;
					}
					break ;
				}

				std::cout << "Received: " << buffer << " from " << client_fds[i].fd << std::endl;
				//testing
				end_server = true;
				break ;
			}
			if (client_fds[i].fd == socket_fd)
			{
				std::cerr << "Listening socket is readable" << std::endl;

				do
				{
					new_fd = accept(socket_fd, NULL, NULL);
					if (new_fd < 0)
					{
						if (errno != EWOULDBLOCK)
						{
							std::cerr << "Accept failed" << std::endl;
							end_server = true;
						}
						break ;
					}

					std::cout << "New incoming connection = " << new_fd << std::endl;
					client_fds[client_count].fd = new_fd;
					client_fds[client_count].events = POLLIN;
					client_count++;

				} while (new_fd != -1);
			}
			else
			{
				std::cout << "Descriptor " << client_fds[i].fd << " is readable" << std::endl;
				close_conn = false;

				do
				{
					err = recv(client_fds[i].fd, buffer, sizeof(buffer), 0);
					if (err < 0)
					{
						if (errno != EWOULDBLOCK)
						{
							std::cerr << "Receive failed" << std::endl;
							close_conn = true;
						}
						break ;
					}
					recv_msg << buffer;

					if (err == 0)
					{
						std::cerr << "Connection closed" << std::endl;
						close_conn = true;
						break ;
					}
					len = err;
					std::cout << len << " bytes received" << std::endl;

					// err = send(client_fds[i].fd, buffer, len, 0);
					if (recv_msg.str().find("\r\n") != std::string::npos) /* TODO seperate received messages by ending sequence */
					{
						std::cout << "Received: " << recv_msg.str() << " from " << client_fds[i].fd << std::endl;
						for (unsigned long n = 0; n <= recv_msg.str().length() + 3; n++)
						{
							if (recv_msg.str().c_str()[n] == 32)
								std::cout << "   ";
							else
								printf("%d ", recv_msg.str().c_str()[n]);
						}
						err = handle_cmds(recv_msg.str(), client_fds, i);
						if (err < 0)
						{
							std::cerr << "Sending failed" << std::endl;
							close_conn = true;
							break ;
						}
						recv_msg.str(std::string());
					}
				} while (true);
				if (close_conn)
				{
					close(client_fds[i].fd);
					client_fds[i].fd = -1;
					compress_array = true;
				}
			}
		}
		if (compress_array)
		{
			compress_array = false;
			for (int i = 0; i < client_count; i++)
			{
				if (client_fds[i].fd == -1)
				{
					for (int j = i; j < client_count; j++)
						client_fds[j].fd = client_fds[j + 1].fd;
					i--;
					client_count--;
				}
			}
		}
	} while (!end_server);
	for (int i = 0; i < client_count; i++)
	{
		if (client_fds[i].fd >= 0)
			close(client_fds[i].fd);
	}
}
