/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: btenzlin <btenzlin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 13:14:58 by btenzlin          #+#    #+#             */
/*   Updated: 2022/11/17 13:30:18 by btenzlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User(void) : _is_complete(false) {}

void	set_data(User user, std::string client_msg)
{
	if (!user.get_is_complete())
	{
		if (client_msg.find("NICK") != std::string::npos)
			user.set_nick(client_msg.substr(5, (client_msg.length() - 5)));
		else if (client_msg.find("USER") != std::string::npos)
		{
			user.set_user(client_msg.substr(5, (client_msg.length() - 5)));
			user.set_is_complete(true);
		}
	}
}

/* ----------------- GETTER ----------------- */

const std::string	User::get_nick(void) const
{
	return (_nick);
}

const std::string	User::get_user(void) const
{
	return (_user);
}

const std::string	User::get_ip(void) const
{
	return (_ip);
}

const bool	User::get_is_complete(void) const
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
