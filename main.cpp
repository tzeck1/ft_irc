/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmeising <mmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 10:29:09 by tzeck             #+#    #+#             */
/*   Updated: 2022/11/10 13:41:13 by mmeising         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.hpp"

int	main(void)
{

}

/*
need gnl like function
poll lets us know when new info in fd is present
re-read for new line when poll notifies
keep buffer if read without newline
keep reading for new stuff unlike gnl.
https://www.ibm.com/docs/en/i/7.2?topic=designs-using-poll-instead-select
nc -l 420 to listen for input
*/
