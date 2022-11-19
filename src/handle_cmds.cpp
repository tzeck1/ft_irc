/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmds.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzeck <@student.42heilbronn.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 15:11:13 by mmeising          #+#    #+#             */
/*   Updated: 2022/11/19 15:32:17 by tzeck            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.hpp"
#include "User.hpp"

static void	handle_cmd(std::vector<client> &clients, std::string msg, int i)
{
	std::string	reply;

	if (msg.find("PING ") == 0)
	{
		reply = "PONG " + (std::string)SERVER_IP + "\r\n";
		send(clients[i].first.fd, reply.c_str(), reply.length(), 0);
	}
	// else if (msg.find("QUIT "))
	// if (msg.find("NICK "))
}

void	parse_cmds(std::vector<client> &clients, std::string msg, int i)
{
	std::string	tmp;

	irc_log(TRACE, "called handle_cmds");
	for (std::string::size_type	pos = msg.find("\r\n"); pos != std::string::npos; pos = msg.find("\r\n"))
	{
		if (pos == 0)
			irc_log(WARNING, "escape sequence at beginning of message");
		tmp = msg.substr(0, pos);
		msg.erase(0, pos + 2);
		if (clients[i].second.get_is_complete() == false)
			init_user(clients[i].second, clients, tmp, clients[i].first.fd);
		else
			handle_cmd(clients, tmp, i);
	}
}
