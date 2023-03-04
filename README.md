# 42_ft_irc

![Demo image](https://github.com/karisti/42_ft_irc/blob/master/demo1.png?raw=true)


## Description
This project is a basic IRC Server following the IRC protocol details. We will discover how to create a server with C++, receive connections, and do it on a non-blocking manner without using threads for it.

## Lessons
- IRC protocol
- Sockets
- Non-blocking fcntl
- Kqueue
- Buffer management

## Prerequisites
- IRC client (CIRC tested)

## Usage
- Run ```make && ./ircserv <SERVER_PORT> <SERVER_PASSWORD>```
- Connect from client. From CIRC: ```/server <SERVER_IP> <SERVER_PORT> <SERVER_PASSWORD>```

## Resources

