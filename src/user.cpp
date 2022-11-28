/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: btenzlin <btenzlin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 13:14:58 by btenzlin          #+#    #+#             */
/*   Updated: 2022/11/28 12:27:58 by btenzlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "user.hpp"
#include "prototypes.hpp"
#include <sstream>

User::User(void) : _is_complete(false), _auth(false), _op(false)	{ }
User::User(std::string str) : _nick(str), _is_complete(false)		{ }

void	init_user(User &user, std::vector<client> &clients, std::string client_msg, int fd)
{
	irc_log(TRACE, "called init_user");
	if (client_msg.find("NICK ") == 0)
	{
		std::string nick = client_msg.substr(5, (client_msg.length() - 5));
		if (nick_in_use(nick, clients) == true)
		{
			irc_log(WARNING, "nickname already in use");
			std::string	error = build_nick_in_use(user);
			send(fd, error.c_str(), error.length(), 0);
		}
		else if (check_name(nick) == false)
		{
			irc_log(WARNING, "forbidden character in nickname");
			std::string	error = build_erroneus_nick(nick);
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

std::string	User::get_nick(void) const			{ return (_nick); }
std::string	User::get_user(void) const			{ return (_user); }
std::string	User::get_ip(void) const			{ return (_ip); }
int			User::get_fd(void) const			{ return (_fd); }
bool		User::get_auth(void) const			{ return (_auth); }
bool		User::get_op(void) const			{ return (_op); }
bool		User::get_is_complete(void) const	{ return (_is_complete); }

/* ----------------- SETTER ----------------- */

void		User::set_nick(const std::string nick)			{ _nick = nick; }
void		User::set_user(const std::string user)			{ _user = user; }
void		User::set_ip(const std::string ip)				{ _ip = ip; }
void		User::set_fd(const int fd)						{ _fd = fd; }
void		User::set_auth(const bool auth)					{ _auth = auth; }
void		User::set_op(const bool op)						{ _op = op; }
void		User::set_is_complete(const bool is_complete)	{ _is_complete = is_complete; }
