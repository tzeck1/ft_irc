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
	(void)argc;
	(void)argv;
	int sockfd, connfd, n;
	// uint8_t	buff[MAXLINE + 1];
	char	recvline[MAXLINE + 1];

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
		struct sockaddr_in	client_addr;
		socklen_t			addr_len;
		unsigned int		ip_raw;
		std::string			ip_str;
		std::stringstream	ss;
		struct hostent		*host;

		connfd = accept(sockfd, (sockaddr *)&client_addr, &addr_len);
		std::cout << "connfd: " << connfd << std::endl;
		ip_raw = client_addr.sin_addr.s_addr;
		std::cout << "raw ip as int: " << ip_raw << std::endl;
		ss	<< (ip_raw >> 0 & 0xff) << "."
			<< (ip_raw >> 8 & 0xff) << "."
			<< (ip_raw >> 16 & 0xff) << "."
			<< (ip_raw >> 24 & 0xff);
		// 			<< " connected" << std::endl;
		// ip_str =  (ip_raw >> 0 & 0xff) + ".";
		// ip_str += (ip_raw >> 8 & 0xff) + ".";
		// ip_str += (ip_raw >> 16 & 0xff) + ".";
		// ip_str += (ip_raw >> 24 & 0xff);
		ip_str = ss.str();
		std::cout << ip_str << " string ip" << std::endl;

		host = gethostbyname(ip_str.c_str());
		std::cout	<< "name: " << host->h_name
					<< "nick: " << host->h_aliases
					<< std::endl;
		printf("nick address saved: %p\n", host->h_aliases[0]);
					// << "nick[0]: " << host->h_aliases[0] <<std::endl;
		// host->		
		memset(recvline, 0, MAXLINE);
		std::cout << "\nreading lines from client now:" << std::endl;
		sleep(1);
		n = read(connfd, recvline, MAXLINE - 1);
		std::cout << "n is: " << n << std::endl;
		while (n > 0)
		{
			std::cout << recvline << std::endl;

			// usleep(1000000);
			if (std::string(recvline).find("\r\n\r\n") >= 0 /*recvline[n - 1] == '\n'*/) {
				std::cout << "breaking for end thingy" << std::endl;
				break ;
			}
			memset(recvline, 0, MAXLINE);
			n = read(connfd, recvline, MAXLINE - 1);
			std::cout << "n is: " << n << std::endl;
		}
		write(connfd, ":10.11.4.27 001 mmeising :Welcome to the Internet Relay Network mmeising!mmeising@10.11.4.29\r\n\r\n", 96);
		// close(connfd); //check later if we have to close fd or not
	}
	return (EXIT_SUCCESS);
}

/*
need gnl like function
poll lets us know when new info in fd is present
re-read for new line when poll notifies
keep buffer if read without newline
keep reading for new stuff unlike gnl.
https://www.ibm.com/docs/en/i/7.2?topic=designs-using-poll-instead-select

*/
