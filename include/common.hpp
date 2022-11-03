/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: btenzlin <btenzlin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 10:19:35 by tzeck             #+#    #+#             */
/*   Updated: 2022/11/03 12:16:57 by btenzlin         ###   ########.fr       */
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

void	init(void);

/*------	MACROS	-----*/
#define SERVER_PORT 420
#define MAXLINE		4096