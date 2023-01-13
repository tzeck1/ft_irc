/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: btenzlin <btenzlin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 10:07:36 by tzeck             #+#    #+#             */
/*   Updated: 2022/11/28 19:23:04 by btenzlin         ###   ########.fr       */
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

static std::string	handle_roulette(client_type &clients, size i, channel_type &channels)
{
	std::srand(std::time(0));
	int			r = std::rand() % 2;
	std::string	nick = clients[i].second.get_nick();

	if (r == 0)
	{
		kick_from_channels(clients, channels, clients[i].second.get_nick());
		close_connection(clients, i);
		return (nick + " pushed their luck and was sent to hell\r\n");
	}
	else
	{
		clients[i].second.set_op(true);
		return (nick + " took the risk and got rewarded with operator rights\r\n");
	}
}

static bool	handle_bot_cmd(client_type &clients, size i, channel_type &channels, std::string &msg, std::string ch_name)
{
	std::string	cmd = msg.substr(msg.find(":") + 1);
	std::string	server_ip = SERVER_IP;
	std::string	reply = ":[BOT]WALL-E!WALL-E@" + server_ip + " PRIVMSG #toolbot :";
	bool		was_kicked = false;

	if (cmd == "!brew")
		reply += "rm -rf $HOME/.brew && git clone --depth=1 https://github.com/Homebrew/brew $HOME/.brew && echo 'export PATH=$HOME/.brew/bin:$PATH' >> $HOME/.zshrc && source $HOME/.zshrc && brew update\r\n";
	else if (cmd == "!clean")
		reply += "rm -rf ~/Library/**.42_cache_bak*; rm -rf ~/**.42_cache_bak; brew cleanup\r\n";
	else if (cmd == "!roulette")
	{
		reply += handle_roulette(clients, i, channels);
		if (reply.find("to hell") != std::string::npos)
			was_kicked = true;
	}
	else if (cmd == "!help")
		reply += "Commands: [!brew (how to install brew) | !clean (delete cache and free space) | !roulette (suprise)]\r\n";
	else
		reply += "Invalid command (!help for available commands)\r\n";

	channel_type::iterator	it = channels.find(ch_name);
	for (std::vector<User>::iterator user = (*it).second.begin(); user != (*it).second.end(); user++)
		send((*user).get_fd(), reply.c_str(), reply.size(), 0);
	return (was_kicked);
}

bool		handle_channel_msg(client_type &clients, size i, channel_type &channels, std::string &msg)
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
		if (ch_name == "toolbot")
			return (handle_bot_cmd(clients, i, channels, msg, ch_name));
	}
	else
	{
		irc_log(WARNING, "sending channel message failed");
		reply = build_no_such_nick(ch_name); //needs # before channel name
		send(clients[i].first.fd, reply.c_str(), reply.length(), 0);
	}
	return (false);
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
	if (msg.find(",", 0) != std::string::npos)
	{
		ch_name = msg.substr(0, msg.find(",", 0));
		msg.erase(0, msg.find(",", 0) + 1);
	}
	else
	{
		ch_name = msg.substr(0, msg.find(" ", 0));
		msg.erase(0, msg.find(",", 0));
	}
	if (ch_name.size() == 0)
		return ;
	if (check_name(ch_name) == false)
	{
		irc_log(WARNING, "forbidden character in channel name");
		reply = build_erroneus_chan(ch_name);
		send(clients[i].first.fd, reply.c_str(), reply.length(), 0);
		return ;
	}
	channel_type::iterator it = channels.find(ch_name);

	if (it != channels.end())
	{
		for (std::vector<User>::iterator it_users = it->second.begin(); it_users != it->second.end(); it_users++)
		{
			if (it_users->get_nick() == clients[i].second.get_nick())
			{
				irc_log(WARNING, "user is duplicate");
				return ;
			}
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
		irc_log(TRACE, "channel created");
	}
	else // add user to channel
	{
		(*it).second.push_back(clients[i].second);

		std::vector<User>	users = (*it).second;
		std::vector<User>::iterator	ite = users.begin();
		for (; ite != users.end(); ite++)
		{
			reply = build_prefix(clients[i].second) + " JOIN #" + ch_name + "\r\n";
			send((*ite).get_fd(), reply.c_str(), reply.length(), 0);
		}
	}
	reply = build_users_in_channel(channels, ch_name, clients[i].second);
	send(clients[i].first.fd, reply.c_str(), reply.length(), 0);
}

void		handle_join_channels(client_type &clients, size i, channel_type &channels, std::string &msg)
{
	msg.erase(0, 5);
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

	msg.erase(0, 1);
	if (msg.find(",", 0) != std::string::npos)
	{
		ch_name = msg.substr(0, msg.find(",", 0));
		msg.erase(0, msg.find(",", 0) + 1);
	}
	else
	{
		ch_name = msg.substr(0, msg.find(" ", 0));
		msg.erase(0, msg.find(",", 0));
	}
	if (ch_name.size() == 0)
		return ;
	channel_type::iterator	it = channels.find(ch_name);
	if (it != channels.end()) // channel found
	{
		std::vector<User>::iterator	ite;
		for (ite = (*it).second.begin(); ite != (*it).second.end(); ite++)
		{
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
			irc_log(TRACE, "channel deleted");
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
	}
	msg.erase(0, 5);
	if (msg.find(",", 0) == std::string::npos)
		handle_leave_channel(clients, i, channels, msg, comment);
	else
	{
		while (msg.size())
			handle_leave_channel(clients, i, channels, msg, comment);
	}
}

void		handle_set_op(client_type &clients, int i, std::string &msg)
{
	std::string	nick = msg.substr(5, (msg.find(" ", 5) - 5));
	std::string	op_pwd = msg.substr((msg.find(" ", 5) + 1));
	std::string	reply;

	if (op_pwd != OP_PWD)
	{
		irc_log(WARNING, "wrong oper password!");
		reply = build_bad_pwd(op_pwd);
		send(clients[i].first.fd, reply.c_str(), reply.size(), 0);
	}
	else
	{
		client_type::iterator it = clients.begin();
		for (; it != clients.end(); it++)
		{
			if ((*it).second.get_nick() == nick)
				break ;
		}
		if (it == clients.end())
		{
			irc_log(WARNING, "nick for oper cmd not found!");
			reply = build_no_such_nick(nick);
			send(clients[i].first.fd, reply.c_str(), reply.size(), 0);
		}
		else if ((*it).second.get_op() == false)
		{
			(*it).second.set_op(true);
			reply = build_youre_oper(nick);
			send(clients[i].first.fd, reply.c_str(), reply.size(), 0);
		}
	}
}

bool		handle_kick_user(client_type &clients, size i, channel_type &channels, std::string &msg)
{
	std::string	nick = msg.substr(5, (msg.find(" ", 5) - 5));
	std::string	reason;
	size		n = msg.find(":");
	if (n != std::string::npos)
		reason = msg.substr(n + 1);
	std::string	reply;

	if (clients[i].second.get_op() == false)
	{
		reply = build_no_privileges(clients[i].second.get_nick());
		send(clients[i].first.fd, reply.c_str(), reply.size(), 0);
		irc_log(WARNING, "not a op (kill cmd)");
	}
	else if (clients[i].second.get_nick() == nick)
	{
		reply = build_kill_failed(clients[i].second.get_nick());
		send(clients[i].first.fd, reply.c_str(), reply.size(), 0);
	}
	else
	{
		client_type::iterator it = clients.begin();
		for (; it != clients.end(); it++)
		{
			if ((*it).second.get_nick() == nick)
			{
				// if (clients[i].second.get_nick() != nick)
				// {
					reply = build_kill_done(nick, reason);
					send(clients[i].first.fd, reply.c_str(), reply.size(), 0);
					send((*it).first.fd, reply.c_str(), reply.size(), 0);
				// }
				kick_from_channels(clients, channels, nick);
				close_connection(clients, it);
				return (true);
			}
		}
		reply = build_no_such_nick(nick);
		send(clients[i].first.fd, reply.c_str(), reply.size(), 0);
	}
	return (false);
}
void		handle_kill_server(client_type &clients, int i, channel_type &channels)
{
	if (clients[i].second.get_op() == false)
	{
		std::string	reply = build_no_privileges(clients[i].second.get_nick());
		send(clients[i].first.fd, reply.c_str(), reply.size(), 0);
		irc_log(WARNING, "not a op (die cmd)");
	}
	else
	{
		for (client_type::iterator it = clients.begin() + 1; it != clients.end(); it++)
			close((*it).second.get_fd());
		clients.clear();
		channels.clear();
		// system("leaks ircserv");
		irc_log(INFO, "EXIT_SUCCESS");
		exit(EXIT_SUCCESS);
	}
}
