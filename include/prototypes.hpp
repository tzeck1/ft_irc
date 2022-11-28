/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prototypes.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: btenzlin <btenzlin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 15:13:40 by btenzlin          #+#    #+#             */
/*   Updated: 2022/11/28 12:27:56 by btenzlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "common.hpp"
#include "user.hpp"

/*------	HANDLE REQUESTS	-----*/
int			init_server(int port);
void		loop_requests(int socket_d, std::string pwd);
void		parse_cmds(client_type &clients, channel_type &channels, std::string &msg, int i, std::string pwd);
t_err		irc_log(enum e_err type, std::string msg);

/*------	UTIL FUNCTIONS	-----*/

void		close_connection(client_type &clients, size i);
void		close_connection(client_type &clients, client_type::iterator it);
void		kick_from_channels(client_type &clients, channel_type &channels, const std::string &nick);
std::string	ip_itostr(in_addr_t ip_raw);
bool		nick_in_use(std::string nick, client_type &clients);
bool		check_pwd(std::string input, std::string pwd);
bool		check_name(std::string input);
std::string	get_nick_from_msg(std::string msg);
bool		user_present(std::vector<User> users, std::string nick);

/*------	BUILD REPLY	-----*/

std::string	build_prefix(User &user);
std::string	build_prefix_from_nick(client_type &clients, const std::string &nick);
std::string	build_no_such_nick(std::string nick);
std::string	build_erroneus_nick(std::string nick);
std::string	build_erroneus_chan(std::string chan);
std::string	build_nick_in_use(User user);
std::string	build_welcome(User user);
std::string	build_bad_pwd(std::string pwd);
std::string	build_youre_oper(std::string nick);
std::string	build_no_privileges(std::string nick);
std::string	build_kill_failed(std::string nick);
std::string	build_kill_done(std::string nick, std::string reason);
std::string	build_users_in_channel(channel_type &channels, std::string ch_name, User &user);

/*------	HANDLE COMMANDS	-----*/

void		handle_ping(client_type &clients, size i);
bool		handle_channel_msg(client_type &clients, size i, channel_type &channels, std::string &msg);
void		handle_priv_msg(client_type &clients, size i, std::string &msg);
void		handle_join_channels(client_type &clients, size i, channel_type &channels, std::string &msg);
void		handle_leave_channels(client_type &clients, size i, channel_type &channels, std::string &msg);
void		handle_set_op(client_type &clients, int i, std::string &msg);
bool		handle_kick_user(client_type &clients, size i, channel_type &channels, std::string &msg);
void		handle_kill_server(client_type &clients, int i, channel_type &channels);