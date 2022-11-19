/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzeck <@student.42heilbronn.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 13:11:31 by btenzlin          #+#    #+#             */
/*   Updated: 2022/11/19 15:34:10 by tzeck            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "common.hpp"

class	User {

public:
	// static size_t	_user_count;

	User(void);
	User(std::string str);
	User(in_addr_t raw_ip);

	/* ----------------- GETTER ----------------- */

	 std::string	get_nick(void) const;
	 std::string	get_user(void) const;
	 std::string	get_ip(void) const;
	 bool			get_is_complete(void) const;

	/* ----------------- SETTER ----------------- */

	void				set_nick(const std::string nick);
	void				set_user(const std::string user);
	void				set_ip(const std::string ip);
	void				set_is_complete(const bool is_complete);

private:

	std::string		_nick;
	std::string		_user;
	std::string		_ip;
	bool			_is_complete;
	//maybe save fd
};

typedef std::pair<pollfd, User>	client;	
typedef std::vector<client>::size_type size;

void	init_user(User &user, std::vector<client> &clients, std::string client_msg, int fd);
void	parse_cmds(std::vector<client> &clients, std::string msg, int i);
