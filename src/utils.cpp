/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzeck <@student.42heilbronn.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 13:28:23 by tzeck             #+#    #+#             */
/*   Updated: 2022/11/14 11:52:27 by tzeck            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.hpp"

void	server_error(std::string err)
{
	std::cout << RED_BOLD << "FATAL ERROR: " << RESET << RED << err << std::endl;
	exit(EXIT_FAILURE);
}

void	loop_error(std::string err, bool &end_server, bool kill)
{
	std::cout << RED_BOLD << "RUNTIME ERROR: " << RESET << RED << err << std::endl;
	end_server = kill;
}