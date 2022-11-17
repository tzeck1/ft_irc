/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmds.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmeising <mmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 15:11:13 by mmeising          #+#    #+#             */
/*   Updated: 2022/11/17 17:19:31 by mmeising         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.hpp"
#include "User.hpp"

static void	handle_cmd(std::vector<pollfd> &fds, std::string msg,
					int i, std::vector<User> &users)
{
	std::string	reply;
	(void)users;

	if (msg.find("PING ") == 0)
	{
		reply = "PONG " + (std::string)SERVER_IP + "\r\n";
		send(fds[i].fd, reply.c_str(), reply.length(), 0);
	}
	// else if (msg.find("QUIT "))
	// if (msg.find("NICK "))
}

void	parse_cmds(std::vector<pollfd> &fds, std::string msg,
					int i, std::vector<User> &users)
{
	std::string	tmp;

	irc_log(TRACE, "called handle_cmds");
	for (std::string::size_type	pos = msg.find("\r\n");
			pos != std::string::npos; pos = msg.find("\r\n"))
	{
		if (pos == 0)
			irc_log(WARNING, "escape sequence at beginning of message");
		tmp = msg.substr(0, pos);
		msg.erase(0, pos + 2);
		if ((users.begin() + i)->get_is_complete() == false)
			init_user(*(users.begin() + i), tmp, fds[i].fd);
		else
			handle_cmd(fds, tmp, i, users);
	}
}
