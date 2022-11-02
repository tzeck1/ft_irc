/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzeck <@student.42heilbronn.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 10:29:09 by tzeck             #+#    #+#             */
/*   Updated: 2022/11/02 12:40:25 by tzeck            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/common.hpp"

int main(int argc, char **argv)
{
	int sockfd, connfd, n;
	uint8_t	buff[MAXLINE + 1];
	uint8_t	recvline[MAXLINE + 1];
	
	// AF_INET = internet connection; SOCK_STREAM = R/W Stream; 0 = TCP
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET; // IP-Category -> IPV4
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // listn to all clients
	servaddr.sin_port = htons(SERVER_PORT); // our port :)

	bind(sockfd, (sockaddr *)&servaddr, sizeof(servaddr)); // assign address to our socket
	listen(sockfd, 10); // socket can recieve connections

	std::cout << "server running ..." << std::endl;
	while(true)
	{
		struct sockaddr_in	addr;
		socklen_t			addr_len;

		connfd = accept(sockfd, (sockaddr *)NULL, NULL);
		memset(recvline, 0, MAXLINE);
		
		while ((n = read(connfd, recvline, MAXLINE - 1)) > 0)
		{
			std::cout << recvline << std::endl;
			if (recvline[n - 1] == '\n')
				break ;
			memset(recvline, 0, MAXLINE);
		}
		write(connfd, "HELLO!", 6);
		close(connfd);
	}
	return (EXIT_SUCCESS);
}