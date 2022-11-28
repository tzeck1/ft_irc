/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: btenzlin <btenzlin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 17:06:43 by mmeising          #+#    #+#             */
/*   Updated: 2022/11/22 14:54:40 by btenzlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/*------	INCLUDES	-----*/
#include <sys/socket.h> //socket functions
#include <netinet/in.h> //sockaddr_in struct
#include <poll.h> // poll function and pollfd struct

#include <string> // string function
#include <iostream> // output
#include <fcntl.h>
#include <vector>
#include <map>
#include <unistd.h>
#include <cstdlib> // for atoi
#include <cctype> // for isalnum

class	User;

typedef std::pair<pollfd, User>							client;
typedef std::pair< std::string, std::vector<User> >		channel;
typedef std::vector<client>::size_type					size;
typedef std::map< std::string, std::vector<User> >		channel_type;
typedef std::vector<client>								client_type;

/*------	MACROS	-----*/
// #define SERVER_PORT 420
#define BACKLOG		32 // how many connections can queue up (listen function)
// #define SERVER_IP	"172.17.252.83"
#define SERVER_IP	"irc.42.com"
#define OP_PWD		"test"
#define TIMEOUT		(3 * 60 * 1000) //timeout for poll

# define RED			"\033[31m"
# define RED_BOLD		"\033[1;31m"
# define GREEN			"\033[32m"
# define GREEN_BOLD		"\033[1;32m"
# define BLUE			"\033[34m"
# define BLUE_BOLD		"\033[1;34m"
# define PURPLE			"\033[35m"
# define PURPLE_BOLD	"\033[1;35m"
# define YELLOW			"\033[33m"
# define YELLOW_BOLD	"\033[1;33m"
# define RESET			"\033[0m"

/*------	TYPEDEFS	-----*/

/**
 * @param CRITICAL Critical errors for which the only solution is to exit the program.
 * @param ERROR Non-critical errors which may run fine but some parts may break.
 * @param WARNING Unexpected situation which is not technically an error but could cause one.
 * @param INFO General info about the program.
 * @param DEBUG Detailed info about the state of the program.
 * @param TRACE Low-level info like function entry/exit points.
*/
typedef enum e_err	{ CRITICAL = 0, ERROR = 1, WARNING = 2, INFO = 3, DEBUG = 4, TRACE = 5 }	t_err;

/*------	HANDLE REQUESTS	-----*/
int			init_server(int port);
void		loop_requests(int socket_d, std::string pwd);
void		parse_cmds(client_type &clients, channel_type &channels, std::string &msg, int i, std::string pwd);
t_err		irc_log(enum e_err type, std::string msg);

/*------	UTIL FUNCTIONS	-----*/

void		close_connection(client_type &clients, size i);
void		close_connection(client_type &clients, client_type::iterator it);
void		kick_from_channels(client_type &clients, channel_type &channels, const std::string &nick);
std::string	ip_itostr(in_addr_t ip_raw);
bool		nick_in_use(std::string nick, client_type &clients);
bool		check_pwd(std::string input, std::string pwd);
bool		check_nick(std::string input);
std::string	get_nick_from_msg(std::string msg);
bool		user_present(std::vector<User> users, std::string nick);

/*------	BUILD REPLY	-----*/

std::string	build_prefix(User &user);
std::string	build_prefix_from_nick(client_type &clients, const std::string &nick);
std::string	build_no_such_nick(std::string nick);
std::string	build_erroneus_nick(std::string nick);
std::string	build_nick_in_use(User user);
std::string	build_welcome(User user);
std::string	build_bad_pwd(std::string pwd);
std::string	build_youre_oper(std::string nick);
std::string	build_no_privileges(std::string nick);
std::string	build_kill_done(std::string nick, std::string reason);
std::string	build_users_in_channel(channel_type &channels, std::string ch_name, User &user);

/*------	HANDLE COMMANDS	-----*/

void		handle_ping(client_type &clients, size i);
bool		handle_channel_msg(client_type &clients, size i, channel_type &channels, std::string &msg);
void		handle_priv_msg(client_type &clients, size i, std::string &msg);
void		handle_join_channels(client_type &clients, size i, channel_type &channels, std::string &msg);
void		handle_leave_channels(client_type &clients, size i, channel_type &channels, std::string &msg);
void		handle_set_op(client_type &clients, int i, std::string &msg);
bool		handle_kick_user(client_type &clients, size i, channel_type &channels, std::string &msg);
void		handle_kill_server(client_type &clients, int i, channel_type &channels);


class	User
{

	public:
		User(void);
		User(std::string str);

		std::string		msg;

		/* ----------------- GETTER ----------------- */

		std::string			get_nick(void) const;
		std::string			get_user(void) const;
		std::string			get_ip(void) const;
		int					get_fd(void) const;
		bool				get_auth(void) const;
		bool				get_op(void) const;
		bool				get_is_complete(void) const;

		/* ----------------- SETTER ----------------- */

		void				set_nick(const std::string nick);
		void				set_user(const std::string user);
		void				set_ip(const std::string ip);
		void				set_fd(int fd);
		void				set_auth(const bool auth);
		void				set_op(bool op);
		void				set_is_complete(const bool is_complete);

	private:

		std::string		_nick;
		std::string		_user;
		std::string		_ip;
		int				_fd;
		bool			_is_complete;
		bool			_auth;
		bool			_op;

};

void		init_user(User &user, std::vector<client> &clients, std::string client_msg, int fd);