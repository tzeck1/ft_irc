/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: btenzlin <btenzlin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 10:19:35 by tzeck             #+#    #+#             */
/*   Updated: 2022/11/15 16:37:47 by btenzlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/*------	INCLUDES	-----*/
#include <sys/socket.h> //socket functions
#include <netinet/in.h> //sockaddr_in struct
#include <poll.h> // poll function and pollfd struct

#include <string> // string functions
#include <iostream> // output
#include <fcntl.h>
#include <vector>




/*------	MACROS	-----*/
#define SERVER_PORT 420
#define BACKLOG		32 // how many connections can queue up (listen function)
#define SERVER_IP	"10.11.4.29"
#define TIMEOUT		(3 * 60 * 1000) //timeout for poll

# define RED			"\033[31m"
# define RED_BOLD		"\033[1;31m"
# define GREEN			"\033[32m"
# define GREEN_BOLD		"\033[1;32m"
# define BLUE			"\033[34m"
# define BLUE_BOLD		"\033[1;34m"
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
typedef enum e_err {
	CRITICAL = 0,
	ERROR = 1,
	WARNING = 2,
	INFO = 3,
	DEBUG = 4,
	TRACE = 5
}	t_err;

/*------	FUNCTIONS	-----*/
int		init_server(void);
void	loop_requests(int socket_d);

t_err	irc_log(enum e_err type, std::string msg);

void	server_error(std::string err);