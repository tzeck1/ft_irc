/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: btenzlin <btenzlin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 10:29:09 by tzeck             #+#    #+#             */
/*   Updated: 2022/11/25 14:20:02 by btenzlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/common.hpp"

static std::string	get_pwd(char *input)
{
	for (int i = 0; input[i] != '\0'; i++)
	{
		if (isalnum(input[i]) == 0)
			irc_log(CRITICAL, "Password contains invalid chars!");
	}
	return (input);
}

int	main(int argc, char **argv)
{
	if (argc != 3)
		irc_log(CRITICAL, "./ircserv <port> <password> expected!");
	int port = atoi(argv[1]);
	if (port == 0)
		irc_log(CRITICAL, "Invalid port given!");
	std::string pwd = get_pwd(argv[2]);
	//	SETUP SOCKET SERVER
	int socket_d = init_server(port);
	irc_log(INFO, "server setup complete");
	//	WAIT FOR INPUT / HANDLE CMDS
	loop_requests(socket_d, pwd);
	return (EXIT_SUCCESS);
}
