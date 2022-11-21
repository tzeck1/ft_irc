/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_requests.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmeising <mmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 10:36:57 by tzeck             #+#    #+#             */
/*   Updated: 2022/11/21 17:12:20 by mmeising         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.hpp"
#include "User.hpp"

/**
 * @return a vector of the fds connected to be used for poll().
*/
std::vector<pollfd>	get_client_fds(std::vector<client> &clients)
{
	std::vector<pollfd> client_fds;

	for (std::vector<client>::iterator it = clients.begin(); it != clients.end(); it++)
		client_fds.push_back((*it).first);
	return (client_fds);
}

static void	init_poll(std::vector<client> &clients, int fds_size)
{
	std::vector<pollfd>	poll_fd = get_client_fds(clients);

	irc_log(TRACE, "waiting for poll ...");
	int err = poll(poll_fd.data(), fds_size, TIMEOUT);
	if (err < 0)
		irc_log(CRITICAL, "poll() failed!");
	else if (err == 0)
		irc_log(CRITICAL, "poll() timed out!");
	for (size i = 0; i < clients.size(); i++)
		clients.begin()[i].first.revents = poll_fd[i].revents;//copying over changes
}

static void	accept_users(std::vector<client> &clients, int socket_d)
{
	struct sockaddr_in	client_addr;
	socklen_t			len;

	irc_log(TRACE, "accept_users called");
	while (true)
	{
		int	new_fd = accept(socket_d, (sockaddr*)&client_addr, &len);
		if (new_fd == -1)
		{
			if (errno != EWOULDBLOCK)
				irc_log(CRITICAL, "accept() failed!");
			break ; //else nothing more to accept
		}
		std::cout << "new fd for user is " << new_fd << std::endl;
		pollfd	new_pollfd;
		new_pollfd.fd = new_fd;
		new_pollfd.events = POLLIN;
		// new_pollfd.revents = POLLIN;// not needed anymore i hope
		User	new_user(client_addr.sin_addr.s_addr);
		clients.push_back(client(new_pollfd, new_user));
		irc_log(DEBUG, "Accepted a new user");
	}
}

static std::string	receive_msg(int client_fd, std::vector<client> &clients, size i)
{
	char	buffer[512];
	int		err;

	irc_log(TRACE, "receive_msg called");
	std::memset(buffer, 0, sizeof(buffer));
	err = recv(client_fd, buffer, sizeof(buffer), 0); //receive msg from fds (except socket fd)
	if (err < 0 && errno != EWOULDBLOCK)
		irc_log(WARNING, "recv() failed");
	if (err == 0) //TODO: should erase vector and 'close' fd
		close_connection(clients, i);
	std::cout << "buffer: " << buffer << std::endl;
	return (buffer);
}

void	loop_requests(int socket_d)
{
	std::vector<client>	clients;
	User				server("server");
	pollfd	socket;
	std::string			msg;

	socket.fd = socket_d; // open file
	socket.events = POLLIN; // requestet events (POLLIN = there is data to be read)
	clients.push_back(client(socket, server));
	while (true)
	{
		init_poll(clients, clients.size()); //poll returns with POLLIN but no message when new connection appears
		accept_users(clients, socket_d);
		for (size i = 0; i < clients.size(); i++)
		{
			// irc_log(DEBUG, "for loop iterator ");
			// std::cout << i << " revents: " << clients[i].first.revents << " events: " << clients[i].first.events << " fd: " << clients[i].first.fd << std::endl;
			if (clients[i].first.revents == 0) //skips user first time after accept
				continue ;
			if (clients[i].first.revents != POLLIN && clients[i].first.revents != (POLLIN | POLLHUP)) //POLLIN: incoming connection POLLIN|POLLHUP: disconnect by user
			{
				irc_log(TRACE, "revents problem happening at fd ");
				std::cerr << i << std::endl;
				irc_log(CRITICAL, "Error revents problem!");
			}
			if (i != 0) //if not socket_d
			{
				msg = receive_msg(clients[i].first.fd, clients, i);
				std::cout << "msg received is: " << msg << std::endl;
				if (msg.size() != 0)
					parse_cmds(clients, msg, i);
			}
		}
	}
}
