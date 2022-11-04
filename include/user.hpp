/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmeising <mmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 16:11:20 by mmeising          #+#    #+#             */
/*   Updated: 2022/11/04 17:16:51 by mmeising         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.hpp"

class User {
	public:

		User(std::string user, std::string user_ip, std::string nick)
			:	_user(user),
				_user_ip(user_ip),
				_nick(nick) {}
		~User() {};

		std::string	get_user() const					{ return (_user); }
		std::string	get_user_ip() const					{ return (_user_ip); }
		std::string	get_nick() const					{ return (_nick); }
		// bool		get_welcome() const					{ return (_welcome); }

		void		set_user(std::string user)			{ _user = user; }
		void		set_user_ip(std::string user_ip)	{ _user_ip = user_ip; }
		void		set_nick(std::string nick)			{ _nick = nick; }
		// void		set_welcome(bool welcome)			{ _welcome = welcome; }

		std::string build_reply()
		{
			std::stringstream	ss;

			ss	<< SERVER_IP << " 001 " << get_nick() 
				<< " :Welcome to the Internet Relay Network "
				<< get_nick() << "!" << get_user() << "@" << get_user_ip() << "\r\n";
			return (ss.str());
		}

	private:
		std::string	_user;
		std::string	_user_ip;
		std::string	_nick;
		// bool		_welcome;
};

User	welcome_msg(std::string msg, in_addr_t user_ip_raw);
