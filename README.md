# 42_ft_irc

[![Demo image](https://i.gyazo.com/175f5e05408824fa0f46a7feccb27697.gif)](https://gyazo.com/175f5e05408824fa0f46a7feccb27697)


## Description
This project is a basic IRC Server following the IRC protocol details. We will discover how to create a server with C++, receive connections, and do it on a non-blocking manner without using threads for it. Developed by @gpernas- and @karisti-.

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
- [RFC-IRC PROTOCOL](https://www.ietf.org/rfc/rfc1459.txt "RFC-IRC PROTOCOL")
- [RFC-IRC PROTOCOL (modern)](https://modern.ircdocs.horse/#client-to-server "RFC-IRC PROTOCOL (modern)")
- [Creating a TCP Server in C++ (Linux / Code Blocks) [YouTube]](https://www.youtube.com/watch?v=cNdlrbZSkyQ "Creating a TCP Server in C++ (Linux / Code Blocks) [YouTube]")
- [How to use Sockets in C++ for TCP/UDP in MacOS [YouTube]](https://www.youtube.com/watch?v=F3iIGUiW27Q "How to use Sockets in C++ for TCP/UDP in MacOS [YouTube]")
- [A brief intro to TCP/IP and a basic client-server setup in C++](https://lenngro.github.io/how-to/2021/01/05/Simple-TCPIP-Server-Cpp/ "A brief intro to TCP/IP and a basic client-server setup in C++")
- [I/O Multiplexing](https://nima101.github.io/io_multiplexing "I/O Multiplexing")
- [Blocking and Non-Blocking I/0](https://www.linuxtoday.com/blog/blocking-and-non-blocking-i-0/ "Blocking and Non-Blocking I/0")
- [Socket man](https://linux.die.net/man/7/socket "Socket man")
- [fcntl man](https://man7.org/linux/man-pages/man2/fcntl.2.html "fcntl man")
- [A TCP server with kqueue](https://dev.to/frevib/a-tcp-server-with-kqueue-527 "A TCP server with kqueue")
- [kqueue tutorial](https://wiki.netbsd.org/tutorials/kqueue_tutorial/ "kqueue tutorial")
- [Handling TCP Connections with Kqueue Event Notification](http://eradman.com/posts/kqueue-tcp.html "Handling TCP Connections with Kqueue Event Notification")
- [I/O Multiplexing](https://nima101.github.io/io_multiplexing "I/O Multiplexing")
