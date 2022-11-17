/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmeising <mmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 13:28:23 by tzeck             #+#    #+#             */
/*   Updated: 2022/11/17 14:07:18 by mmeising         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.hpp"
#include <sstream>

/**
 * 0 - quiet: 			No messages printed.
 * 1 - standard:		Print only CRITICAL, ERROR, WARNING, INFO.
 * 2 - verbose:			Also print DEBUG.
 * 3 - very verbose:	Also print TRACE.
*/
#define VERBOSITY 3

/**
 * Type can be one of the following:
 * CRITICAL Critical errors, exits afterwards.
 * ERROR Non-critical, may run fine, some parts may break.
 * WARNING Not technically an error but could cause one.
 * INFO General info about the program.
 * DEBUG Detailed info about the state of the program.
 * TRACE Low-level info like function entry/exit points.
*/
t_err	irc_log(enum e_err type, std::string msg)
{
	switch (type)
	{
		case CRITICAL:
			if (VERBOSITY)
				std::cerr	<< RED_BOLD << "FATAL ERROR: " << RESET
							<< RED << msg << RESET << std::endl;
			exit(1);
		case ERROR:
			if (VERBOSITY)
				std::cerr	<< YELLOW_BOLD << "ERROR: " << RESET
							<< YELLOW << msg << RESET << std::endl;
			break;
		case WARNING:
			if (VERBOSITY)
				std::cerr	<< YELLOW_BOLD << "WARNING: " << RESET
							<< YELLOW << msg << RESET << std::endl;
			break;
		case INFO:
			if (VERBOSITY)
				std::cout	<< GREEN_BOLD << "INFO: " << RESET
							<< GREEN << msg << RESET << std::endl;
			break;
		case DEBUG:
			if (VERBOSITY >= 2)
				std::cerr	<< BLUE_BOLD << "DEBUG: " << RESET
							<< BLUE << msg << RESET << std::endl;
			break;
		case TRACE:
			if (VERBOSITY == 3)
				std::cerr	<< BLUE_BOLD << "TRACE: " << RESET
							<< BLUE << msg << RESET << std::endl;
			break;
	}
	return (type);
}

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

std::string	ip_itostr(in_addr_t ip_raw)
{
	std::stringstream		ss;

	ss	<< (ip_raw >>  0 & 0xff) << "."
		<< (ip_raw >>  8 & 0xff) << "."
		<< (ip_raw >> 16 & 0xff) << "."
		<< (ip_raw >> 24 & 0xff);
	return (ss.str());
}
