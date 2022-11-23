/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prototypes.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzeck <@student.42heilbronn.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 15:13:40 by btenzlin          #+#    #+#             */
/*   Updated: 2022/11/23 09:27:42 by tzeck            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "common.hpp"
#include "user.hpp"

/*------	FUNCTIONS	-----*/
int			init_server(int port);
void		loop_requests(int socket_d);

std::string	ip_itostr(in_addr_t ip_raw);

t_err		irc_log(enum e_err type, std::string msg);
void		server_error(std::string err);
std::string	get_nick_from_msg(std::string msg);
std::string	build_prefix(User &user);
std::string	build_no_such_nick(std::string nick);
std::string	build_nick_in_use(User user);
std::string	build_welcome(User user);
