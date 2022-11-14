/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzeck <@student.42heilbronn.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 10:19:35 by tzeck             #+#    #+#             */
/*   Updated: 2022/11/14 11:36:10 by tzeck            ###   ########.fr       */
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
#define SERVER_IP	"10.11.4.31"

# define RED		"\033[31m"
# define RED_BOLD	"\033[1;31m"
# define GREEN		"\033[32m"
# define BLUE		"\033[34m"
# define YELLOW		"\033[33m"
# define RESET		"\033[0m"

/*------	FUNCTIONS	-----*/
int		init_server(void);
void	loop_requests(int socket_d);

void	server_error(std::string err);