# ft_irc
This project is about creating your own IRC server. <br>
You will use an actual IRC client to connect to your server and test it. <br>
Internet is ruled by solid standards protocols that allow connected computers to interact
with each other. <br>
<br>
Here you can read more about the concept https://en.wikipedia.org/wiki/Internet_Relay_Chat <br>
<br>
<img width="200" alt="Screenshot 2023-01-13 at 15 15 41" src="https://user-images.githubusercontent.com/65648486/212340953-70706bbf-6571-4ffb-a366-e1e1d57c3523.png">
<br>

## Introduction

**- philo number:** how many philosophers are sitting at the table <br>
**- time to die:** time in milliseconds until a philo dies after his last meal <br>
**- time to eat:** time in milliseconds it takes a philo to eat <br>
**- time to die:** time in milliseconds it takes a philo to sleep <br>
**- number must eat:** how many times a philo must at least have eaten to end the simulation <br>
<br>

A philosopher has to announce its current status in the STDOUT, which can be: <br>
- philo n has taken a fork <br>
- philo n is eating <br>
- philo n is sleeping <br>
- philo n is thinking <br>
- philo n died <br>

## Usage

``make`` to compile all source files <br>
``./ircserv <port> <password>``

``brew install weechat`` (feel free to use your favorite irc client instead (only tested with weechat))
in weechat:
``/server add <server_name> <ip>/<port>``
``/connect <server_name> -password=<password>``
type ``/help`` for more commands
