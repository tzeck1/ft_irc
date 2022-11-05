/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmeising <mmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/05 20:50:27 by mmeising          #+#    #+#             */
/*   Updated: 2022/11/05 20:53:51 by mmeising         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.hpp"

class Server
{
	public:
		int					_client_count;
		static const int	_timeout = (3 * 60 * 1000);
	private:

};