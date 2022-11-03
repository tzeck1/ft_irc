/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmeising <mmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 10:19:35 by tzeck             #+#    #+#             */
/*   Updated: 2022/11/03 15:57:09 by mmeising         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/*------	INCLUDES	-----*/
#include <sys/socket.h> // socket functions
#include <netinet/in.h> // sockaddr struct
 #include <sys/types.h>

#include <netdb.h> // gethostbyname()
#include <iostream>
#include <sstream> // stringstream for int to str
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <exception>
#include <fcntl.h> //file control
#include <vector>
#include <poll.h>
#include <errno.h>
#include <cstring>

void	init(void);

/*------	MACROS	-----*/
#define SERVER_PORT 420
#define MAXLINE		4096
#define SERVER_IP	"10.11.4.29"
