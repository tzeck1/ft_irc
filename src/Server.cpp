/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: btenzlin <btenzlin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 12:05:31 by btenzlin          #+#    #+#             */
/*   Updated: 2022/11/08 19:12:46 by btenzlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

Server::Server(int port)
:	_end_server(false),
	_close_conn(false),
	_compress_array(false),
	_server_port(port),
	_socket_fd(-1),
	_client_fds(200)
{
	
}

Server::~Server(void)
{
	
}

void	Server::init(void)
{
	init_socket();
	bind_and_listen();
	handle_connections();
}

void	Server::init_socket(void)
{
	int	err;
	int	opt = 1;

	_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket_fd < 0)
	{
		std::cerr << "Socket creation failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	err = setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));
	if (err < 0)
	{
		std::cerr << "Setsockopt failed" << std::endl;
		close(_socket_fd);
		exit(EXIT_FAILURE);
	}

	err = fcntl(_socket_fd, F_SETFL, O_NONBLOCK);
	if (err < 0)
	{
		std::cerr << "File control failed" << std::endl;
		close(_socket_fd);
		exit(EXIT_FAILURE);
	}

	std::memset(&_socket_addr, 0, sizeof(_socket_addr));
	_socket_addr.sin_family = AF_INET;
	_socket_addr.sin_addr.s_addr = htonl(INADDR_ANY); // listn to all clients
	_socket_addr.sin_port = htons(_server_port); // our port :)
	std::cout << "Port: " << _server_port << std::endl;
	std::cout << "End of init socket" << std::endl;
}

void	Server::bind_and_listen(void)
{
	int	err;

	err = bind(_socket_fd, (struct sockaddr *)&_socket_addr, sizeof(_socket_addr));
	if (err < 0)
	{
		std::cerr << "Binding failed" << std::endl;
		close(_socket_fd);
		exit(EXIT_FAILURE);
	}

	err = listen(_socket_fd, 10);
	if (err < 0)
	{
		std::cerr << "Listening failed" << std::endl;
		close(_socket_fd);
		exit(EXIT_FAILURE);
	}

	_client_fds[0].fd = _socket_fd;
	_client_fds[0].events = POLLIN;
	std::cout << "End of bind and listen" << std::endl;
}

int		Server::check_connection(int i)
{
	if (_client_fds[i].revents == 0)
		return (1);
	else if (_client_fds[i].revents != POLLIN)
	{
		std::cerr << "Error. Revents = " << _client_fds[i].revents << std::endl;
		_end_server = true;
		return (0);
	}
	std::cout << "End of check connection" << std::endl;
	return (2);
}

void		Server::accept_connection(void)
{
	int	new_fd;

	std::cerr << "Listening socket is readable" << std::endl;
	do
	{
		new_fd = accept(_socket_fd, NULL, NULL);
		if (new_fd < 0)
		{
			if (errno != EWOULDBLOCK)
			{
				std::cerr << "Accept failed" << std::endl;
				_end_server = true;
			}
			break ;
		}

		std::cout << "New incoming connection = " << new_fd << std::endl;
		_client_fds[_client_count].fd = new_fd;
		_client_fds[_client_count].events = POLLIN;
		_client_count++;

	} while (new_fd != -1);
	std::cout << "End of accept" << std::endl;
}

void	Server::receive_msg(int i)
{
	char		buffer[80];
	int			err;
	int			pos;
	std::string	temp;
	// std::string	_recv_msg;

	std::cout << "Descriptor " << _client_fds[i].fd << " is readable" << std::endl;
	_close_conn = false;

	do
	{
		sleep(1);
		err = recv(_client_fds[i].fd, buffer, sizeof(buffer), 0);
		if (err < 0)
		{
			if (errno != EWOULDBLOCK)
			{
				std::cerr << "Receive failed" << std::endl;
				_close_conn = true;
			}
			break ;
		}
		else if (err == 0)
		{
			std::cerr << "Connection closed" << std::endl;
			_close_conn = true;
			break ;
		}
		else if (err < 80)
			buffer[err] = '\0';
		_recv_msg += buffer;
		std::cout << "Recv_msg: " << _recv_msg;
		std::cout << err << " bytes received" << std::endl;
	} while (true);
	pos = _recv_msg.find("\r\n");
	while (pos >= 0)
	{
		temp = _recv_msg.substr(0, pos + 2);
		// printf("%i, %i\n", _recv_msg.c_str()[pos], _recv_msg.c_str()[pos + 1]);
		_recv_msg = _recv_msg.erase(0, pos + 2);
		std::cout << "Temp: " << temp;
		pos = _recv_msg.find("\r\n");
	}
	// _recv_msg.clear();
	if (_close_conn)
	{
		close(_client_fds[i].fd);
		_client_fds[i].fd = -1;
		_compress_array = true;
	}
	std::cout << "End of receive" << std::endl;
}

void	Server::handle_connections(void)
{
	int	err;
	int	current_size;

	do {
		std::cout << "Waiting on poll..." << std::endl;
		err = poll(&_client_fds[0], _client_count, _timeout);
		std::cout << "Poll was called" << std::endl;
		if (err < 0)
		{
			std::cerr << "Poll failed" << std::endl;
			break ;
		}
		else if (err == 0)
		{
			std::cerr << "Poll timed out" << std::endl;
			break ;
		}
		current_size = _client_count;
		for (int i = 0; i < current_size; i++)
		{
			err = check_connection(i);
			if (err == 0)
				break ;
			else if (err == 1)
				continue ;
			if (_client_fds[i].fd == _socket_fd)
				accept_connection();
			else
				receive_msg(i);
		}
		if (_compress_array)
		{
			_compress_array = false;
			for (int i = 0; i < _client_count; i++)
			{
				if (_client_fds[i].fd == -1)
				{
					for (int j = i; j < _client_count; j++)
						_client_fds[j].fd = _client_fds[j + 1].fd;
					i--;
					_client_count--;
				}
			}
		}
	} while (!_end_server);
	for (int i = 0; i < _client_count; i++)
	{
		if (_client_fds[i].fd >= 0)
			close(_client_fds[i].fd);
	}
}
