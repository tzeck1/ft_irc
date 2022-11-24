/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: btenzlin <btenzlin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 17:06:43 by mmeising          #+#    #+#             */
/*   Updated: 2022/11/22 14:54:40 by btenzlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/*------	INCLUDES	-----*/
#include <sys/socket.h> //socket functions
#include <netinet/in.h> //sockaddr_in struct
#include <poll.h> // poll function and pollfd struct

#include <string> // string function
#include <iostream> // output
#include <fcntl.h>
#include <vector>
#include <map>
#include <unistd.h>
#include <cstdlib> // for atoi
#include <cctype> // for isalnum

/*------	MACROS	-----*/
// #define SERVER_PORT 420
#define BACKLOG		32 // how many connections can queue up (listen function)
// #define SERVER_IP	"172.17.252.83"
#define SERVER_IP	"irc.42.com"
#define OP_PWD		"test"
#define TIMEOUT		(3 * 60 * 1000) //timeout for poll

# define RED			"\033[31m"
# define RED_BOLD		"\033[1;31m"
# define GREEN			"\033[32m"
# define GREEN_BOLD		"\033[1;32m"
# define BLUE			"\033[34m"
# define BLUE_BOLD		"\033[1;34m"
# define PURPLE			"\033[35m"
# define PURPLE_BOLD	"\033[1;35m"
# define YELLOW			"\033[33m"
# define YELLOW_BOLD	"\033[1;33m"
# define RESET			"\033[0m"

/*------	TYPEDEFS	-----*/

/**
 * @param CRITICAL Critical errors for which the only solution is to exit the program.
 * @param ERROR Non-critical errors which may run fine but some parts may break.
 * @param WARNING Unexpected situation which is not technically an error but could cause one.
 * @param INFO General info about the program.
 * @param DEBUG Detailed info about the state of the program.
 * @param TRACE Low-level info like function entry/exit points.
*/
typedef enum e_err	{ CRITICAL = 0, ERROR = 1, WARNING = 2, INFO = 3, DEBUG = 4, TRACE = 5 }	t_err;
