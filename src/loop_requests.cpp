/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_requests.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmeising <mmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 10:36:57 by tzeck             #+#    #+#             */
/*   Updated: 2022/11/17 13:48:14 by mmeising         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.hpp"
#include <unistd.h>

static void	init_poll(pollfd *poll_fd, int fds_size)
{
	irc_log(TRACE, "waiting for poll ...");
	int err = poll(poll_fd, fds_size, TIMEOUT);
	if (err < 0)
		irc_log(CRITICAL, "poll() failed!");
	else if (err == 0)
		irc_log(CRITICAL, "poll() timed out!");
}

static void	accept_users(std::vector<pollfd> &fds, int socket_d, int &fds_size)
{
	irc_log(TRACE, "accept_users called");
	while (true)
	{
		int	new_fd = accept(socket_d, NULL, NULL);
		if (new_fd == -1)
		{
			if (errno != EWOULDBLOCK)
				irc_log(CRITICAL, "accept() failed!");
			break ; //else nothing more to accept
		}
		fds[fds_size].fd = new_fd;
		fds[fds_size].events = POLLIN;
		fds_size++;
		irc_log(DEBUG, "Accepted a new user");
	}
}

static void	receive_msg(int client_fd)
{
	char	buffer[512];
	int		err;

	irc_log(TRACE, "receive_msg called");
	std::memset(buffer, 0, sizeof(buffer));
	err = recv(client_fd, buffer, sizeof(buffer), 0); //receive msg from fds (except socket fd)
	if (err < 0 && errno != EWOULDBLOCK)
		irc_log(WARNING, "recv() failed");
	if (err == 0) //TODO: should erase vector and 'close' fd
		irc_log(CRITICAL, "connection closed by user"); //just here to avoid infinite loop
	std::cout << "buffer: " << buffer << std::endl;
	//TODO: handle commands in buffer
}

void	loop_requests(int socket_d)
{
	std::vector<pollfd>	fds(200); // init to 200 empty fd
	fds[0].fd = socket_d; // open file
	fds[0].events = POLLIN; // requestet events (POLLIN = there is data to be read)
	int	fds_size = 1;

	while (true)
	{
		init_poll(&fds[0], fds_size); //seems like poll returns with POLLIN but no message when new connection appears
		accept_users(fds, socket_d, fds_size);
		for (int i = 0; i < fds_size; i++)
		{
			std::cout << "REVENTS for fd " << i << " is " << fds[i].revents << std::endl;
			if (fds[i].revents == 0) //skips user first time after accept
				continue ;
			if (fds[i].revents != POLLIN && fds[i].revents != (POLLIN | POLLHUP)) //POLLIN: incoming connection POLLIN|POLLHUP: disconnect by user
			{
				irc_log(TRACE, "revents problem happening at fd ");
				std::cerr << i << std::endl;
				irc_log(CRITICAL, "Error revents problem!");
			}
			if (i != 0) //if not socket_d
				receive_msg(fds[i].fd);
		}
	}
}
