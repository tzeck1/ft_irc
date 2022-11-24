/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzeck <@student.42heilbronn.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 09:47:50 by tzeck             #+#    #+#             */
/*   Updated: 2022/11/24 09:49:00 by tzeck            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.hpp"

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
				std::cerr	<< PURPLE_BOLD << "DEBUG: " << RESET
							<< PURPLE << msg << RESET << std::endl;
			break;
		case TRACE:
			if (VERBOSITY == 3)
				std::cerr	<< BLUE_BOLD << "TRACE: " << RESET
							<< BLUE << msg << RESET << std::endl;
			break;
	}
	return (type);
}