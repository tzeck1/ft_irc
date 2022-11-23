/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmds.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: btenzlin <btenzlin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 15:11:13 by mmeising          #+#    #+#             */
/*   Updated: 2022/11/23 19:20:06 by btenzlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prototypes.hpp"
#include "user.hpp"

static void	auth_user(std::vector<client> &clients, std::string input, std::string pwd, size i)
{
	if (check_pwd(input, pwd) == true)
		clients[i].second.set_auth(true);
	else
	{
		std::string reply = build_bad_pwd(input.substr(5, input.size() - 5));
		send(clients[i].first.fd, reply.c_str(), reply.size(), 0);
	}
}

static bool	user_present(std::vector<User> users, std::string nick)
{
	for (std::vector<User>::iterator it_users = users.begin(); it_users != users.end(); it_users++)
	{
		if (it_users->get_nick() == nick)
			return (true);
	}
	return (false);
}

static void	handle_cmd(std::vector<client> &clients, channel_type &channels, std::string &msg, int i)
{
	std::string	reply;

	irc_log(TRACE, "handle_cmd called");
	if (msg.find("PING ") == 0)
	{
		reply = "PONG " + (std::string)SERVER_IP + "\r\n";
		send(clients[i].first.fd, reply.c_str(), reply.length(), 0);
	}
	else if (msg.find("PRIVMSG #") == 0)
	{
		std::string				ch_name = msg.substr(9, (msg.find(" ", 9) - 9));
		channel_type::iterator	it = channels.find(ch_name);
		
		if (it != channels.end() && user_present((*it).second, clients[i].second.get_nick())) // channel found
		{
			std::vector<User>	users = (*it).second;
			std::vector<User>::iterator	ite = users.begin();
			for (; ite != users.end(); ite++)
			{
				if (clients[i].second.get_nick() == (*ite).get_nick())
					continue ;
				reply = build_prefix(clients[i].second) + " " + msg + "\r\n";
				send((*ite).get_fd(), reply.c_str(), reply.length(), 0);
			}
		}
		else
		{
			irc_log(WARNING, "sending channel message failed");
			reply = build_no_such_nick(ch_name); //needs # before channel name
			send(clients[i].first.fd, reply.c_str(), reply.length(), 0);
		}
	}
	else if (msg.find("PRIVMSG ") == 0)
	{
		for (std::vector<client>::iterator it = clients.begin(); it != clients.end(); it++)
		{
			if ((*it).second.get_nick() == get_nick_from_msg(msg))
			{
				reply = build_prefix(clients[i].second) + " " + msg + "\r\n";
				send((*it).first.fd, reply.c_str(), reply.length(), 0);
				return ;
			}
		}
		irc_log(WARNING, "sending private message failed");
		reply = build_no_such_nick(get_nick_from_msg(msg));
		send(clients[i].first.fd, reply.c_str(), reply.length(), 0);
	}
	else if (msg.find("JOIN #") == 0)
	{
		std::string ch_name = msg.substr(6, (msg.find(" ", 6) - 6));
		channel_type::iterator it = channels.find(ch_name);
		for (std::vector<User>::iterator it_users = it->second.begin(); it_users != it->second.end(); it_users++)
		{
			if (it_users->get_nick() == clients[i].second.get_nick())
			{
				irc_log(DEBUG, "user is duplicate");
				return ;
			}
		}
		if (it == channels.end()) // create channel
		{
			std::vector<User> ch_users;

			ch_users.push_back(clients[i].second);
			channel new_ch(ch_name, ch_users);
			channels.insert(new_ch);
			reply = build_prefix(clients[i].second) + " " + msg + "\r\n";
			send(clients[i].first.fd, reply.c_str(), reply.length(), 0);
			irc_log(DEBUG, "channel created");
		}
		else // add user to channel
		{
			// irc_log(DEBUG, "channel exists");
			(*it).second.push_back(clients[i].second);

			std::vector<User>	users = (*it).second;
			std::vector<User>::iterator	ite = users.begin();
			for (; ite != users.end(); ite++)
			{
				reply = build_prefix(clients[i].second) + " " + msg + "\r\n";
				// irc_log(DEBUG, reply);
				send((*ite).get_fd(), reply.c_str(), reply.length(), 0);
			}
		}
		reply = build_users_in_channel(channels, ch_name, clients[i].second);
		send(clients[i].first.fd, reply.c_str(), reply.length(), 0);
	}
	else if (msg.find("PART ") == 0)
	{
		std::string				ch_name = msg.substr(6, (msg.find(" ", 6) - 6));
		channel_type::iterator	it = channels.find(ch_name);
		
		if (it != channels.end()) // channel found
		{
			// std::vector<User>	users = (*it).second;
			irc_log(DEBUG, "channel name is " + (*it).first);
			std::vector<User>::iterator	ite;
			for (ite = (*it).second.begin(); ite != (*it).second.end(); ite++)
			{
				// if (clients[i].second.get_nick() == (*ite).get_nick())
				// 	continue ;
				reply = build_prefix(clients[i].second) + " " + msg + "\r\n";
				send((*ite).get_fd(), reply.c_str(), reply.length(), 0);
			}
			for (ite = (*it).second.begin(); ite != (*it).second.end(); ite++)
			{
				if (clients[i].second.get_nick() == (*ite).get_nick())
				{
					(*it).second.erase(ite);
					break ;
				}
			}
			if ((*it).second.size() == 0)
			{
				irc_log(DEBUG, "channel deleted");
				channels.erase(ch_name);
			}
		}
		else
		{
			irc_log(WARNING, "sending channel message failed");
			reply = build_no_such_nick(ch_name); //needs # before channel name
			send(clients[i].first.fd, reply.c_str(), reply.length(), 0);
		}
	}
	else if (msg.find("OPER ") == 0)
	{
		std::string nick = msg.substr(5, (msg.find(" ", 5) - 5));
		std::string op_pwd = msg.substr((msg.find(" ", 5) + 1));

		if (op_pwd != OP_PWD)
			irc_log(ERROR, "wrong oper password!");
		else
		{
			client_type::iterator it = clients.begin();
			for (; it != clients.end(); it++)
			{
				if ((*it).second.get_nick() == nick)
					break ;
			}
			if (it == clients.end())
				irc_log(ERROR, "nick for oper cmd not found!");
			else
				(*it).second.set_op(true);
		}
	}
	else if (msg.find("KILL ") == 0)
	{
		std::string nick = msg.substr(5, (msg.find(" ", 5) - 5));
		irc_log(INFO, nick);
		// std::string reason = msg.substr((msg.find(" ", 6)));
		std::string reason = msg.substr((msg.find(" ", 5) + 1));
		irc_log(INFO, reason);

		if (clients[i].second.get_op() == false)
			irc_log(ERROR, "not a op (kill cmd)");
		else
		{
			client_type::iterator it = clients.begin();
			for (; it != clients.end(); it++)
			{
				if ((*it).second.get_nick() == nick)
				{
					close_connection(clients, it);
					break ;
				}
			}
		}
	}
	// else if (msg.find("QUIT ") == 0)
		//need to send msg to all members of same channel
}

void	parse_cmds(std::vector<client> &clients, channel_type &channels, std::string &msg, int i, std::string pwd)
{
	std::string	tmp;

	irc_log(TRACE, "called handle_cmds with msg");
	irc_log(DEBUG, msg);
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
			handle_cmd(clients, channels, tmp, i);
	}
}
