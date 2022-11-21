/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmeising <mmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 10:19:35 by tzeck             #+#    #+#             */
/*   Updated: 2022/11/21 17:10:39 by mmeising         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "base.hpp"

/*------	FUNCTIONS	-----*/
int			init_server(void);
void		loop_requests(int socket_d);

std::string	ip_itostr(in_addr_t ip_raw);

t_err		irc_log(enum e_err type, std::string msg);
void		server_error(std::string err);
