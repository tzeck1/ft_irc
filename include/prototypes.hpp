/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prototypes.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzeck <@student.42heilbronn.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 15:13:40 by btenzlin          #+#    #+#             */
/*   Updated: 2022/11/24 10:32:57 by tzeck            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "common.hpp"
#include "user.hpp"

/*------	HANDLE REQUESTS	-----*/
int			init_server(int port);
void		loop_requests(int socket_d, std::string pwd);
void		parse_cmds(std::vector<client> &clients, channel_type &channels, std::string &msg, int i, std::string pwd);
t_err		irc_log(enum e_err type, std::string msg);

/*------	UTIL FUNCTIONS	-----*/

void		close_connection(std::vector<client> &clients, size i);
void		close_connection(std::vector<client> &clients, client_type::iterator it);
std::string	ip_itostr(in_addr_t ip_raw);
bool		nick_in_use(std::string nick, std::vector<client> &clients);
bool		check_pwd(std::string input, std::string pwd);
std::string	get_nick_from_msg(std::string msg);
bool		user_present(std::vector<User> users, std::string nick);

/*------	BUILD REPLY	-----*/

std::string	build_prefix(User &user);
std::string	build_no_such_nick(std::string nick);
std::string	build_nick_in_use(User user);
std::string	build_welcome(User user);
std::string	build_bad_pwd(std::string pwd);
std::string	build_users_in_channel(channel_type &channels, std::string ch_name, User &user);

/*------	HANDLE COMMANDS	-----*/

void		handle_ping(client_type &clients, size i);
void		handle_channel_msg(client_type &clients, size i, channel_type &channels, std::string &msg);
void		handle_priv_msg(client_type &clients, size i, std::string &msg);
void		handle_join_channel(client_type &clients, size i, channel_type &channels, std::string &msg);
void		handle_leave_channel(client_type &clients, size i, channel_type &channels, std::string &msg);
void		handle_set_op(client_type &clients, std::string &msg);
void		handle_kick_user(client_type &clients, size i, std::string &msg);