/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: btenzlin <btenzlin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 13:14:58 by btenzlin          #+#    #+#             */
/*   Updated: 2022/11/23 11:20:30 by btenzlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "user.hpp"
#include "prototypes.hpp"
#include <sstream>

User::User(void) : _is_complete(false), _auth(false) {}
User::User(std::string str) : _nick(str), _is_complete(false) {}
// User::User(in_addr_t raw_ip) : _ip(ip_itostr(raw_ip)), _is_complete(false) {}

static bool	nick_in_use(std::string nick, std::vector<client> &clients)
{
	for (std::vector<client>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if (nick == it->second.get_nick())
			return (true);
	}
	return (false);
}

void	init_user(User &user, std::vector<client> &clients, std::string client_msg, int fd)
{
	irc_log(TRACE, "called init_user");
	if (client_msg.find("NICK ") == 0)
	{
		if (nick_in_use(client_msg.substr(5, (client_msg.length() - 5)), clients) == true)
		{
			irc_log(WARNING, "nickname already in use");
			std::string	error = build_nick_in_use(user);
			send(fd, error.c_str(), error.length(), 0);
		}
		else
			user.set_nick(client_msg.substr(5, (client_msg.length() - 5)));
	}
	else if (client_msg.find("USER ") == 0)
		user.set_user(client_msg.substr(5, client_msg.find(" ", 5) - 5));
	if (user.get_nick().size() && user.get_user().size())
	{
		user.set_is_complete(true);
		std::string	welcome = build_welcome(user);
		send(fd, welcome.c_str(), welcome.length(), 0);
	}
}

/* ----------------- GETTER ----------------- */

std::string	User::get_nick(void) const
{
	return (_nick);
}

std::string	User::get_user(void) const
{
	return (_user);
}

std::string	User::get_ip(void) const
{
	return (_ip);
}

bool	User::get_auth(void) const
{
	return (_auth);
}

// std::string	User::get_msg(void) const
// {
// 	return (_msg);
// }

bool	User::get_is_complete(void) const
{
	return (_is_complete);
}

/* ----------------- SETTER ----------------- */

void				User::set_nick(const std::string nick)
{
	_nick = nick;
}

void				User::set_user(const std::string user)
{
	_user = user;
}

void				User::set_ip(const std::string ip)
{
	_ip = ip;
}

void				User::set_auth(const bool auth)
{
	_auth = auth;
}

// void				User::set_msg(const std::string msg)
// {
// 	_msg = _msg;
// }

void				User::set_is_complete(const bool is_complete)
{
	_is_complete = is_complete;
}
