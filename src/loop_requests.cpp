/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_requests.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzeck <@student.42heilbronn.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 10:36:57 by tzeck             #+#    #+#             */
/*   Updated: 2022/11/14 14:51:54 by tzeck            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.hpp"

void	loop_requests(int socket_d)
{
	std::vector<pollfd>	fds(200); // init to 200 empty fd
	fds[0].fd = socket_d; // open file
	fds[0].events = POLLIN; // requestet events (POLLIN = there is data to be read)
	int timeout = (3 * 60 * 1000); // after 3 minutes timeout

	for (bool end_server = false; end_server != true;)
	{
		std::cout << YELLOW << "waiting for poll ..." << RESET << std::endl;
		int err = poll(&fds[0], fds.size(), timeout);
		if (err < 0)
			server_error("poll() failed!");
		else if (err == 0)
			server_error("poll() timed out!");
		
	}
}
