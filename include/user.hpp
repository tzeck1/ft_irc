/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmeising <mmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 16:11:20 by mmeising          #+#    #+#             */
/*   Updated: 2022/11/03 18:11:29 by mmeising         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.hpp"

class User {
	public:

		User(std::string user, std::string user_ip, std::string nick)
			:	_user = user,
				_user_ip = user_ip,
				_nick = nick;
				_welcome = false;
		~User();

		std::string	get_user() const					{ return (_user); }
		std::string	get_user_ip() const					{ return (_user_ip); }
		std::string	get_nick() const					{ return (_nick); }
		bool		get_welcome() const					{ return (_welcome); }

		void		set_user(std::string user)			{ _user = user; }
		void		set_user_ip(std::string user_ip)	{ _user_ip = user_ip; }
		void		set_nick(std::string nick)			{ _nick = nick; }
		void		set_welcome(bool welcome)			{ _welcome = welcome; }

	private:
		std::string	_user;
		std::string	_user_ip;
		std::string	_nick;
		bool		_received_welcome;

}
