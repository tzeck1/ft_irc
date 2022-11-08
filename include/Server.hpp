/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: btenzlin <btenzlin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/05 20:50:27 by mmeising          #+#    #+#             */
/*   Updated: 2022/11/08 17:56:20 by btenzlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.hpp"

class Server
{

	public:

		Server(int port); //TODO password
		~Server(void);

		void	init(void);

	private:

		Server(void);

		/* FUNCTIONS */
		void						init_socket(void);
		void						bind_and_listen(void);
		void						handle_connections(void);
		int							check_connection(int i);
		void						accept_connection(void);
		void						receive_msg(int i);

		/* ATTRIBUTES */
		std::string					_recv_msg;
		bool						_end_server;
		bool						_close_conn;
		bool						_compress_array;
		int							_server_port;
		int							_socket_fd;
		int							_client_count;
		static const int			_timeout = (3 * 60 * 1000);
		struct sockaddr_in			_socket_addr;
		std::vector<struct pollfd>	_client_fds;

};
