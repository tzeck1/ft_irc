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

## Usage

``make`` to compile all source files <br>
``./ircserv <port> <password>``

``brew install weechat`` (feel free to use your favorite irc client instead (only tested with weechat)) <br>
``weechat`` to open client <br>
in weechat: <br>
``/server add <server_name> <ip>/<port>`` <br>
``/connect <server_name> -password=<password>`` <br>
type ``/help`` for more commands
