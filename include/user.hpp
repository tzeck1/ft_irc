/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzeck <@student.42heilbronn.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 13:11:31 by btenzlin          #+#    #+#             */
/*   Updated: 2022/11/24 10:03:35 by tzeck            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "common.hpp"

class	User
{

	public:
		User(void);
		User(std::string str);

		std::string		msg;

		/* ----------------- GETTER ----------------- */

		std::string			get_nick(void) const;
		std::string			get_user(void) const;
		std::string			get_ip(void) const;
		int					get_fd(void) const;
		bool				get_auth(void) const;
		bool				get_op(void) const;
		bool				get_is_complete(void) const;

		/* ----------------- SETTER ----------------- */

		void				set_nick(const std::string nick);
		void				set_user(const std::string user);
		void				set_ip(const std::string ip);
		void				set_fd(int fd);
		void				set_auth(const bool auth);
		void				set_op(bool op);
		void				set_is_complete(const bool is_complete);

	private:

		std::string		_nick;
		std::string		_user;
		std::string		_ip;
		int				_fd;
		bool			_is_complete;
		bool			_auth;
		bool			_op;

};

typedef std::pair<pollfd, User>							client;
typedef std::pair< std::string, std::vector<User> >		channel;
typedef std::vector<client>::size_type					size;
typedef std::map< std::string, std::vector<User> >		channel_type;
typedef std::vector<client>								client_type;

void		init_user(User &user, std::vector<client> &clients, std::string client_msg, int fd);
