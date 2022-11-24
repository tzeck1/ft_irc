/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmeising <mmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 10:07:36 by tzeck             #+#    #+#             */
/*   Updated: 2022/11/24 14:08:39 by mmeising         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.hpp"
#include "user.hpp"
#include "prototypes.hpp"

void		handle_ping(client_type &clients, size i)
{
	std::string reply = "PONG " + (std::string)SERVER_IP + "\r\n";
	send(clients[i].first.fd, reply.c_str(), reply.length(), 0);
}

void		handle_channel_msg(client_type &clients, size i, channel_type &channels, std::string &msg)
{
	std::string reply;
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

void		handle_priv_msg(client_type &clients, size i, std::string &msg)
{
	std::string reply;

	for (client_type::iterator it = clients.begin(); it != clients.end(); it++)
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

static void		handle_join_channel(client_type &clients, size i, channel_type &channels, std::string &msg)
{
	std::string		reply;
	std::string		ch_name;

	msg.erase(0, 1);
	if (msg.find(",", 0) != std::string::npos){
		ch_name = msg.substr(0, msg.find(",", 0));
		msg.erase(0, msg.find(",", 0) + 1);
		irc_log(DEBUG, "message left after erasing first channel name" + msg);
	}
	else
		ch_name = msg.substr(0, msg.find(" ", 0));
	irc_log(DEBUG, "message left after erasing first channel name" + msg);
	if (ch_name.size() == 0)
		return ;
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
		reply = build_prefix(clients[i].second) + " JOIN #" + ch_name + "\r\n";
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
			reply = build_prefix(clients[i].second) + " JOIN #" + ch_name + "\r\n";
			// irc_log(DEBUG, reply);
			send((*ite).get_fd(), reply.c_str(), reply.length(), 0);
		}
	}
	reply = build_users_in_channel(channels, ch_name, clients[i].second);
	send(clients[i].first.fd, reply.c_str(), reply.length(), 0);
}

void		handle_join_channels(client_type &clients, size i, channel_type &channels, std::string &msg)
{
	msg.erase(0, 5);
	irc_log(DEBUG, "msg after cutting JOIN is: " + msg);
	if (msg.find(",", 0) == std::string::npos)
		handle_join_channel(clients, i, channels, msg);
	else
	{
		while (msg.size())
			handle_join_channel(clients, i, channels, msg);
	}
}

static void		handle_leave_channel(client_type &clients, size i, channel_type &channels, std::string &msg, std::string comment)
{
	std::string				reply;
	std::string				ch_name;

	irc_log(DEBUG, "msg in handle_leave: " + msg);
	msg.erase(0, 1);
	if (msg.find(",", 0) != std::string::npos){
		ch_name = msg.substr(0, msg.find(",", 0));
		msg.erase(0, msg.find(",", 0) + 1);
		irc_log(DEBUG, "message left after erasing first channel name" + msg);
	}
	else
	{
		ch_name = msg.substr(0, msg.find(" ", 0));
		irc_log(INFO, "channel name in else is " + ch_name);
		msg.erase(0, msg.size());
	}
	irc_log(DEBUG, "message left after erasing first channel name" + ch_name);
	if (ch_name.size() == 0)
		return ;
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
			reply = build_prefix(clients[i].second) + " PART #" + ch_name + " " + comment + "\r\n";
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

void		handle_leave_channels(client_type &clients, size i, channel_type &channels, std::string &msg)
{
	std::string		comment;

	if (msg.find(":", 0) != std::string::npos)//there is a msg afterwards
	{
		comment = msg.substr(msg.find(":", 0) + 1, msg.size() - (msg.find(":", 0) + 1));//from next spacebar after channel name until end
		msg.erase(msg.find(":", 0) - 1, msg.size() - (msg.find(":", 0) - 1));//delete the parting msg part
		irc_log(TRACE, "msg after erasing comment: " + msg);
	}
	if (comment.size())
		irc_log(INFO, "parting msg is " + comment);

	msg.erase(0, 5);
	irc_log(DEBUG, "msg after cutting LEAVE is: " + msg);
	if (msg.find(",", 0) == std::string::npos)
		handle_leave_channel(clients, i, channels, msg, comment);
	else
	{
		while (msg.size())
			handle_leave_channel(clients, i, channels, msg, comment);
	}
}

void		handle_set_op(client_type &clients, std::string &msg)
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

bool		handle_kick_user(client_type &clients, size i, channel_type &channels, std::string &msg)
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
				kick_from_channels(clients, channels, nick);
				close_connection(clients, it);
				return (true);
			}
		}
	}
	return (false);
}