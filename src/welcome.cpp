/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   welcome.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmeising <mmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 14:35:40 by mmeising          #+#    #+#             */
/*   Updated: 2022/11/04 17:16:27 by mmeising         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.hpp"
#include "user.hpp"

/*
	returns true on fail
	saves content of msg between substrings pre and post inside of content
*/
bool	extract_content(std::string msg, std::string& content, 
						std::string pre, std::string post)
{
	std::string::size_type	pos;

	pos = msg.find(pre);
	if (pos == std::string::npos)
		return (true);
	msg.erase(0, pos + pre.size());
	pos = msg.find(post);
	if (pos == std::string::npos)
		return (true);
	content = msg.substr(0, pos);
	return (false);
}

std::string	ip_itostr(in_addr_t ip_raw)
{
	std::stringstream		ss;

	ss	<< (ip_raw >>  0 & 0xff) << "."
		<< (ip_raw >>  8 & 0xff) << "."
		<< (ip_raw >> 16 & 0xff) << "."
		<< (ip_raw >> 24 & 0xff);
	return (ss.str());
}

std::string build_reply(std::string nick, std::string user, std::string ip)
{
	std::stringstream	ss;

	ss	<< SERVER_IP << " 001 " << nick 
		<< " :Welcome to the Internet Relay Network "
		<< nick << "!" << user << "@" << ip << "\r\n";
	return (ss.str());
}

User	welcome_msg(std::string msg, in_addr_t user_ip_raw)
{
	std::string				ret;
	std::string 			server_ip(SERVER_IP);
	std::string 			nick;
	std::string 			user;
	std::string 			user_ip;

	if (extract_content(msg, nick, "NICK ", "\r\n"))
		throw (std::exception());
	if (extract_content(msg, user, "USER ", " "))
		throw (std::exception());
	user_ip = ip_itostr(user_ip_raw);

	User	new_user(user, user_ip, nick);

	// std::cout << "nick: |" << nick << "|\n";
	// std::cout << "user: |" << user << "|\n";
	// std::cout << "ip:   |" << user_ip << "|\n\n";

	return (new_user);
}

/*
<server_ip> 001 <nick> :Welcome to the Internet Relay Network <nick>!<user>@<host(user_ip)>

NICK <nick>
USER <username> * * :<full name>
*/