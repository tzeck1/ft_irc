/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzeck <@student.42heilbronn.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 10:29:09 by tzeck             #+#    #+#             */
/*   Updated: 2022/11/23 10:49:13 by tzeck            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prototypes.hpp"

std::string	get_pwd(char *input)
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
	int socket_d = init_server(port);
	irc_log(INFO, "Server setup complete");
	loop_requests(socket_d, pwd);
	return (EXIT_SUCCESS);
}
