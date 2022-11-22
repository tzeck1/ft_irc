/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmds.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: btenzlin <btenzlin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 15:11:13 by mmeising          #+#    #+#             */
/*   Updated: 2022/11/22 13:56:22 by btenzlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.hpp"
#include "User.hpp"
#include <fstream>
#include <unistd.h>

static void	handle_cmd(std::vector<client> &clients, std::string &msg, int i)
{
	std::string	reply;

	irc_log(TRACE, msg.c_str());
	if (msg.find("PING ") == 0)
	{
		reply = "PONG " + (std::string)SERVER_IP + "\r\n";
		send(clients[i].first.fd, reply.c_str(), reply.length(), 0);
	}
	else if (msg.find("PRIVMSG ") == 0)
	{
		irc_log(ERROR, msg);
		for (std::vector<client>::iterator it = clients.begin(); it != clients.end(); it++)
		{
			if ((*it).second.get_nick() == get_nick_from_msg(msg))
			{
				reply = build_prefix(clients[i].second) + " " + msg + "\r\n";
				irc_log(DEBUG, "sending privmsg");
				std::cout << "nickname in clients: " << (*it).second.get_nick() << std::endl;
				std::cout << "nickname in msg: " << get_nick_from_msg(msg) << std::endl;
				send((*it).first.fd, reply.c_str(), reply.length(), 0);
				return ;
			}
		}
		irc_log(DEBUG, "sending privmsg failed");
		reply = build_no_such_nick(get_nick_from_msg(msg));
		send(clients[i].first.fd, reply.c_str(), reply.length(), 0);
	}
	// else if (msg.find("QUIT ") == 0)
		//need to send msg to all members of same channel
	// if (msg.find("NICK "))
}

void	parse_cmds(std::vector<client> &clients, std::string &msg, int i)
{
	std::string	tmp;

	irc_log(TRACE, "called handle_cmds");
	for (std::string::size_type	pos = msg.find("\r\n"); pos != std::string::npos; pos = msg.find("\r\n"))
	{
		irc_log(DEBUG, "inside for loop of parse_cmds");
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
