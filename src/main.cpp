/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tzeck <@student.42heilbronn.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 10:29:09 by tzeck             #+#    #+#             */
/*   Updated: 2022/11/14 14:10:38 by tzeck            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.hpp"

int	main(void)
{
	int socket_d = init_server();
	std::cout << "socket setup -> " << GREEN << "[success]" << RESET << std::endl; // debug only
	loop_requests(socket_d);
	return (EXIT_SUCCESS);
}
