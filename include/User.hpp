/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: btenzlin <btenzlin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 13:11:31 by btenzlin          #+#    #+#             */
/*   Updated: 2022/11/17 14:05:03 by btenzlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "common.hpp"

class	User {

public:

	User(void);

	/* ----------------- GETTER ----------------- */

	const std::string	get_nick(void) const;
	const std::string	get_user(void) const;
	const std::string	get_ip(void) const;
	const bool			get_is_complete(void) const;

	/* ----------------- SETTER ----------------- */

	void				set_nick(const std::string nick);
	void				set_user(const std::string user);
	void				set_ip(const std::string ip);
	void				set_is_complete(const bool is_complete);

private:

	std::string	_nick;
	std::string	_user;
	std::string	_ip;
	bool		_is_complete;
	//maybe save fd
};

void	set_data(User user, std::string client_msg);
