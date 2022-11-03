/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   welcome.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmeising <mmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 14:35:40 by mmeising          #+#    #+#             */
/*   Updated: 2022/11/03 16:09:06 by mmeising         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/common.hpp"

std::string	welcome_msg(std::string msg, size_t len, in_addr_t user_ip_raw)
{
	std::string	ret;
	std::string part;
	std::string server_ip(SERVER_IP);
	std::string nick;
	std::string user;
	std::string user_ip;

	
	return (ret);
}
/*
NICK <nick>
USER <username> * * :<full name>

<server_ip> 001 <nick> :Welcome to the Internet Relay Network <nick>!<user>@<host(user_ip)>
*/
