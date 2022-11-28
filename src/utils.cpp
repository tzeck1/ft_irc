/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: btenzlin <btenzlin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 13:28:23 by tzeck             #+#    #+#             */
/*   Updated: 2022/11/28 18:49:38 by btenzlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "user.hpp"
#include "common.hpp"
#include "prototypes.hpp"
#include <sstream>
#include <unistd.h>

std::string	ip_itostr(in_addr_t ip_raw)
{
	std::stringstream		ss;

	ss	<< (ip_raw >>  0 & 0xff) << "."
		<< (ip_raw >>  8 & 0xff) << "."
		<< (ip_raw >> 16 & 0xff) << "."
		<< (ip_raw >> 24 & 0xff);
	return (ss.str());
}

bool	nick_in_use(std::string nick, client_type &clients)
{
	for (client_type::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if (nick == it->second.get_nick())
			return (true);
	}
	return (false);
}

#include <stdlib.h>

void	close_connection(client_type &clients, size i)
{
	if (close(clients[i].first.fd) == -1)
		irc_log(CRITICAL, "failed to close file descriptor");
	clients.erase(clients.begin() + i);
}

void	close_connection(client_type &clients, client_type::iterator it)
{
	if (close((*it).second.get_fd()) == -1)
		irc_log(CRITICAL, "failed to close file descriptor");
	clients.erase(it);
}

void	kick_from_channels(client_type &clients, channel_type &channels, const std::string &nick)
{
	channel_type::iterator	it = channels.begin();

	for (; it != channels.end(); it++)
	{
		if (user_present((*it).second, nick))
		{
			for (std::vector<User>::iterator it_users = (*it).second.begin(); it_users != (*it).second.end(); it_users++)
			{
				if (it_users->get_nick() == nick)
				{
					(*it).second.erase(it_users);
					break ;
				}
			}
			std::string	reply = build_prefix_from_nick(clients, nick) + " PART #" + (*it).first + "\r\n";
			for (std::vector<User>::iterator it_users = (*it).second.begin(); it_users != (*it).second.end(); it_users++)
				send((*it_users).get_fd(), reply.c_str(), reply.length(), 0);
		}
	}
}

static std::string	get_names(channel_type channels, std::string ch_name)
{
	std::string	names;

	channel_type::iterator	it = channels.find(ch_name);
	std::vector<User>	users = (*it).second;
	std::vector<User>::iterator	ite = users.begin();
	for (; ite != users.end() - 1; ite++)
		names += (*ite).get_nick() + " ";
	names += (*ite).get_nick();
	return (names);
}

std::string	get_nick_from_msg(std::string msg)
{
	int	i = 0;
	std::string	nick;

	for (; msg[i] != ' '; i++)
		continue ;
	i++;
	for (; msg[i] != ' '; i++)
		nick.push_back(msg[i]);
	return (nick);
}

bool	check_pwd(std::string input, std::string pwd)
{
	if (input.substr(5, input.size() - 5) == pwd)
		return (true);
	else
		return (false);
}

bool	check_name(std::string input)
{
	for (int i = 0; input[i] != '\0'; i++)
	{
		if (isalnum(input[i]) == 0)
		{
			irc_log(WARNING, "Nick/Channel name contains invalid chars!");
			return (false);
		}
	}
	return (true);
}

bool	user_present(std::vector<User> users, std::string nick)
{
	for (std::vector<User>::iterator it_users = users.begin(); it_users != users.end(); it_users++)
	{
		if (it_users->get_nick() == nick)
			return (true);
	}
	return (false);
}

/*--------------	BUILD REPLIES	-------------*/

std::string	build_nick_in_use(User user)
{
	std::stringstream	ss;

	ss	<< ":" << SERVER_IP << " 433 * " << user.get_nick()
		<< " :Nickname is already in use" << "\r\n";
	return (ss.str());
}

std::string	build_no_such_nick(std::string nick)
{
	std::stringstream	ss;

	ss	<< ":" << SERVER_IP << " 401 " << nick
		<< " :No such nick/channel" << "\r\n";
	return (ss.str());
}

std::string	build_erroneus_nick(std::string nick)
{
	std::stringstream	ss;

	ss	<< ":" << SERVER_IP << " 432 " << nick
		<< " :invalid characters in given nick" << "\r\n";
	return (ss.str());
}

std::string	build_erroneus_chan(std::string chan)
{
	std::stringstream	ss;

	ss	<< ":" << SERVER_IP << " 403 " << chan
		<< " :invalid characters in given channel name" << "\r\n";
	return (ss.str());
}

std::string	build_bad_pwd(std::string pwd)
{
	std::stringstream	ss;

	ss	<< ":" << SERVER_IP << " 464 " << pwd
		<< " :Incorrect password" << "\r\n";
	return (ss.str());
}

std::string	build_youre_oper(std::string nick)
{
	std::stringstream	ss;

	ss	<< ":" << SERVER_IP << " 381 " << nick
		<< " :You are now an IRC operator" << "\r\n";
	return (ss.str());
}

std::string	build_no_privileges(std::string nick)
{
	std::stringstream	ss;

	ss	<< ":" << SERVER_IP << " 481 " << nick
		<< " :Permission Denied - You're not an IRC operator" << "\r\n";
	return (ss.str());
}

std::string	build_kill_failed(std::string nick)
{
	std::stringstream	ss;

	ss	<< ":" << SERVER_IP << " 485 " << nick
		<< " :Permission Denied - You can't kick yourself" << "\r\n";
	return (ss.str());
}

std::string	build_kill_done(std::string nick, std::string reason)
{
	std::stringstream	ss;

	ss	<< ":" << SERVER_IP << " 361 " << nick
		<< " :was kicked. (reason: " << reason
		<< ")" << "\r\n";
	return (ss.str());
}

std::string	build_users_in_channel(channel_type &channels, std::string ch_name, User &user)
{
	std::stringstream	ss;

	ss	<< ":" << SERVER_IP << " 353 " << user.get_nick()
		<< " = #" << ch_name << " :" << get_names(channels, ch_name) << "\r\n";
	
	ss	<< ":" << SERVER_IP << " 366 " << user.get_nick()
		<< " #" << ch_name << " :End of NAMES list." << "\r\n";
	return (ss.str());
}

std::string	build_welcome(User user)
{
	std::stringstream	ss;

	ss	<< ":" << SERVER_IP << " 001 " << user.get_nick()
		<< " :Welcome to our ft_irc " << user.get_nick()
		<< "!" << user.get_user() << "@" << user.get_ip() << "\r\n";
	return (ss.str());
}

std::string			build_prefix(User &user)
{
	std::stringstream	ss;

	ss	<< ":" << user.get_nick() << "!" << user.get_user() << "@" << user.get_ip();
	return (ss.str());
}

std::string			build_prefix_from_nick(client_type &clients, const std::string &nick)
{
	client_type::iterator it = clients.begin();

	for (; it != clients.end(); it++)
	{
		if ((*it).second.get_nick() == nick)
			break ;
	}
	return (build_prefix((*it).second));
}
