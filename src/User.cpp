/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmeising <mmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 13:14:58 by btenzlin          #+#    #+#             */
/*   Updated: 2022/11/17 17:06:54 by mmeising         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include <sstream>

User::User(void) : _is_complete(false) {}
User::User(std::string str) : _nick(str), _is_complete(false) {}
User::User(in_addr_t raw_ip) : _ip(ip_itostr(raw_ip)), _is_complete(false) {}

static std::string	build_welcome(User user)
{
	std::stringstream	ss;

	ss	<< ":" << SERVER_IP << " 001 " << user.get_nick()
		<< " :Welcome to our ft_irc " << user.get_nick()
		<< "!" << user.get_user() << "@" << user.get_ip() << "\r\n";
	return (ss.str());
}

void	init_user(User &user, std::string client_msg, int fd)
{
	irc_log(TRACE, "called set_data");
	std::cout << "with msg "<< client_msg << std::endl;
	if (client_msg.find("NICK ") == 0)
	{
		user.set_nick(client_msg.substr(5, (client_msg.length() - 5)));
		irc_log(DEBUG, "nick is set to");
		std::cout << user.get_nick() << std::endl;
	}
	else if (client_msg.find("USER ") == 0)
	{
		user.set_user(client_msg.substr(5, client_msg.find(" ", 5) - 5));
		irc_log(DEBUG, "user is set to");
		std::cout << user.get_user() << std::endl;
		user.set_is_complete(true);
		std::string	welcome = build_welcome(user);
		std::cout << "about to send welcome: " << welcome << std::endl;
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

void				User::set_is_complete(const bool is_complete)
{
	_is_complete = is_complete;
}