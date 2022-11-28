/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmds.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: btenzlin <btenzlin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 15:11:13 by mmeising          #+#    #+#             */
/*   Updated: 2022/11/28 18:51:43 by btenzlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prototypes.hpp"
#include "user.hpp"

static void	auth_user(client_type &clients, std::string input, std::string pwd, size i)
{
	if (check_pwd(input, pwd) == true)
		clients[i].second.set_auth(true);
	else
	{
		std::string reply = build_bad_pwd(input.substr(5, input.size() - 5));
		send(clients[i].first.fd, reply.c_str(), reply.size(), 0);
	}
}

static bool	handle_cmd(client_type &clients, channel_type &channels, std::string &msg, int i)
{
	std::string	reply;

	irc_log(TRACE, "handle_cmd called");
	if (msg.find("PING ") == 0)
		handle_ping(clients, i);
	else if (msg.find("PRIVMSG #") == 0)
		return (handle_channel_msg(clients, i, channels, msg));
	else if (msg.find("PRIVMSG ") == 0)
		handle_priv_msg(clients, i, msg);
	else if (msg.find("JOIN #") == 0)
		handle_join_channels(clients, i, channels, msg);
	else if (msg.find("PART ") == 0)
		handle_leave_channels(clients, i, channels, msg);
	else if (msg.find("OPER ") == 0)
		handle_set_op(clients, i, msg);
	else if (msg.find("KILL ") == 0)
		return (handle_kick_user(clients, i, channels, msg));
	else if (msg == "DIE")
		handle_kill_server(clients, i, channels);
	return (false);
}

void	parse_cmds(client_type &clients, channel_type &channels, std::string &msg, int i, std::string pwd)
{
	std::string	tmp;

	irc_log(INFO, "called handle_cmds with msg: " + msg);
	for (std::string::size_type	pos = msg.find("\r\n"); pos != std::string::npos; pos = msg.find("\r\n"))
	{
		if (pos == 0)
			irc_log(WARNING, "escape sequence at beginning of message");
		tmp = msg.substr(0, pos);
		msg.erase(0, pos + 2);
		if (clients[i].second.get_is_complete() == false)
		{
			if (tmp.find("PASS ") == 0)
				auth_user(clients, tmp, pwd, i);
			else if (clients[i].second.get_auth() == true)
				init_user(clients[i].second, clients, tmp, clients[i].first.fd);
		}
		else
		{
			if (handle_cmd(clients, channels, tmp, i))
				break ;
		}
	}
}
