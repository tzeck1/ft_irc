/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzeck <@student.42heilbronn.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 10:19:35 by tzeck             #+#    #+#             */
/*   Updated: 2022/11/02 12:41:03 by tzeck            ###   ########.fr       */
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

/*------	MACROS	-----*/
#define SERVER_PORT 420
#define MAXLINE		4096