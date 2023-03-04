/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karisti- <karisti-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 17:36:07 by karisti-          #+#    #+#             */
/*   Updated: 2023/03/04 22:57:36 by karisti-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"


IRC::Server::Server() {}

IRC::Server::Server(std::string password)
{
	this->password = password;
	this->creationTimestamp = getCurrentTimestamp();
}

IRC::Server::Server(const IRC::Server &other) { *this = other; }

IRC::Server::~Server() {}

IRC::Server &IRC::Server::operator=(const IRC::Server &other)
{
	if (this != &other)
	{
		this->ip = other.ip;
		this->sSocket = other.sSocket;
		this->kq = other.kq;
		this->password = other.password;
		this->users = other.users;
		this->channels = other.channels;
		this->hostname = other.hostname;
	}

	return *this;
}

/* -- Getters -- */
std::string								IRC::Server::getIp(void) const { return this->ip; }
int										IRC::Server::getSocket(void) const { return this->sSocket; }
std::string								IRC::Server::getPassword(void) const { return this->password; }
IRC::Server::users_map&					IRC::Server::getUsers(void) { return this->users; }
IRC::Server::channels_map&				IRC::Server::getChannels(void) { return this->channels; }
std::string								IRC::Server::getHostname(void) const { return this->hostname; }

IRC::Server::channels_map::iterator		IRC::Server::findChannel(std::string name)
{
	return this->channels.find(name);
}

/* -- Modifiers -- */
void	IRC::Server::setHostname(std::string hostname)
{
	this->hostname = hostname;
}

void	IRC::Server::addChannel(IRC::Channel& channel)
{
	this->channels[channel.getName()] = channel;
}

void	IRC::Server::removeChannel(IRC::Channel channel)
{
	this->channels.erase(channel.getName());
}

void	IRC::Server::updateUserInChannels(IRC::User user)
{
	for (IRC::Server::channels_map::iterator channelIt = this->channels.begin(); channelIt != this->channels.end(); channelIt++)
	{
		IRC::Channel::chanusers_map::iterator userIt = channelIt->second.getUsers().find(user.getSocket());
		if (userIt != channelIt->second.getUsers().end())
			userIt->second.first = user;
	}
}

int IRC::Server::createNetwork(std::string *args)
{
	if ((this->sSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		return throwError("Error opening socket");

	/** Bind the Socket to any free IP / Port **/
	sockaddr_in hint;
	hint.sin_family = AF_INET; // IPv4 type
	hint.sin_port = htons(atoi(args[0].c_str())); // Little Endian (for bigger numbers) | Host To Network Short
	inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
	int yes = 1;
	
	if (setsockopt(this->sSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		return throwError("Error could not reuse address");

	if (bind(this->sSocket, (struct sockaddr *)&hint, sizeof(hint)) < 0)
		return throwError("Error binding socket");

	/** Listen for new connections **/
	if (listen(this->sSocket, SOMAXCONN) == -1)
		return throwError("Error listen");

	if (fcntl(this->sSocket, F_SETFL, O_NONBLOCK) < 0)
		return throwError("Error making server socket non blocking");

	/** Initialize a kqueue **/
	if ((this->kq = kqueue()) == -1)
		return throwError("kqueue");
	
	/** Add action to listen for **/
	EV_SET(&this->eventSet, this->sSocket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
	if (kevent(this->kq, &this->eventSet, 1, NULL, 0, NULL) == -1)
		return throwError("kevent add server socket");

	if (saveIp() == -1)
		return -1;
	std::cout << "--- IP: " << this->ip << " ---" << std::endl;

	return 0;
}

int IRC::Server::loop(void)
{
	int	newEvents;

	/** SERVER LOOP **/
	catchSignal();
	while (!socketKiller)
	{
		struct timespec keventTime = {KQUEUE_TIMEOUT, 0};
		
		/** Wait for events **/
		if ((newEvents = kevent(this->kq, NULL, 0, this->eventList, KQUEUE_SIZE, &keventTime)) == -1)
			if (!socketKiller)
				return throwError("kevent check events");

		/** Kqueue events loop **/
		for (int i = 0; i < newEvents; i++)
		{
			int eventFd = this->eventList[i].ident;

			/** Client disconnected **/
			if (this->eventList[i].flags & EV_EOF)
				clientDisconnected(eventFd);
			/** New client connected **/
			else if (eventFd == getSocket())
				clientConnected();
			/** New message from client **/
			else if (this->eventList[i].filter & EVFILT_READ)
				receiveMessage(eventFd);
		}
		catchPing();
	}
	return 0;
}

void	IRC::Server::closeClient(IRC::User& user, std::string message)
{
	/** Remove user from joined channels and send QUIT message to them **/
	for (IRC::Server::channels_map::iterator itChannel = user.getJoinedChannels().begin(); itChannel != user.getJoinedChannels().end(); itChannel++)
	{
		IRC::Server::channels_map::iterator channel = this->channels.find(itChannel->second.getName());
		if (channel != this->channels.end())
		{
			channel->second.sendMessageToUsers(user, ":" + user.getNick() + " QUIT :" + message);
			channel->second.removeUser(this, user);
		}
	}
	
	/** Close client socket **/
	if (close(user.getSocket()) == -1)
		throwError("Client close error");
	else
		std::cout << "Client (" << user.getSocket() << ") closed" << std::endl;

	removeUser(user);
}

/* -- Private Member functions */
void	IRC::Server::removeUser(IRC::User& user)
{
	this->users.erase(user.getSocket());
}

int		IRC::Server::saveIp(void)
{
	char				host[256];
	struct hostent*		hostEntry;

	if (gethostname(host, sizeof(host)) == -1)
		return throwError("Error getting ip: gethostname");
	
	hostEntry = gethostbyname(host);
	if (!hostEntry)
	{
		this->ip = "127.0.0.1";
		this->hostname = "localhost";
	}
	else
	{
		this->ip = inet_ntoa(*((struct in_addr*) hostEntry->h_addr_list[0]));

		std::cout << ">> Hostname: " << hostEntry->h_name << std::endl;
		std::string s(hostEntry->h_name);
		this->hostname = s;
	}
	
	return 0;
}

void	IRC::Server::terminateServer(void)
{
	/** Close client sockets **/
	for (IRC::Server::users_map::iterator userIt = this->users.begin(); userIt != this->users.end(); ++userIt)
	{
		if (close(userIt->second.getSocket()) == -1)
			throwError("Client close error");
		else
			std::cout << "Client (" << userIt->second.getSocket() << ") closed" << std::endl;
	}
	
	/** Close server socket **/
	if (close(getSocket()) == -1)
		throwError("Server close error");
	else
		std::cout << "Server (" << getSocket() << ") closed" << std::endl;
}

int		IRC::Server::clientConnected(void)
{
	IRC::User user;
	
	user.startListeningSocket(this->sSocket);

	/** Add action to listen for **/
	EV_SET(&this->eventSet, user.getSocket(), EVFILT_READ, EV_ADD, 0, 0, NULL);
	if (kevent(kq, &this->eventSet, 1, NULL, 0, NULL) == -1)
		return throwError("kevent add client socket");

	this->users[user.getSocket()] = user;
	return 0;
}

void	IRC::Server::clientDisconnected(int eventFd)
{
	IRC::Server::users_map::iterator userIt = this->users.find(eventFd);
	if (userIt == this->users.end())
		return ;

	closeClient(userIt->second, "Quit: Connection closed");
}

int		IRC::Server::receiveMessage(int eventFd)
{
	char buf[4096];
	int bytesRec;

	memset(buf, 0, 4096);

	/** Receive data **/
	if ((bytesRec = recv(eventFd, buf, 4096, 0)) == -1)
	{
		std::cout << "Error in recv(). Quitting" << std::endl;
		return -1;
	}

	IRC::Server::users_map::iterator found = this->users.find(eventFd);
	if (found == this->users.end())
		return -1;

	IRC::User& user = found->second;
	
	/** Manage user buffer and split commands **/
	std::string message(buf);
	message.erase(remove(message.begin(), message.end(), '\r'), message.end());
	user.appendBuffer(message);
	
	if (PRINT_DEBUG)
		std::cout << "Command: '" << user.getBuffer() << "'" << std::endl;

	std::vector<std::string> messageSplit = splitString(user.getBuffer(), "\n");
	if (messageSplit.size() == 0)
		return 0;

	user.clearBuffer();
	
	if (messageSplit[messageSplit.size() - 1].size() == 0)
		messageSplit.erase(messageSplit.end() - 1);
	else
	{
		user.appendBuffer(messageSplit[messageSplit.size() - 1]);
		messageSplit.erase(messageSplit.end() - 1);
	}

	/** Manage each command **/
	for (std::vector<std::string>::iterator it = messageSplit.begin(); it != messageSplit.end(); it++)
	{
		if (!user.isAuthenticated())
			registration(user, *it);
		else
		{
			IRC::Command cmd(*it);
			cmd.detectCommand(this, user);
		}
		
		if (PRINT_DEBUG)
		{
			printUsers(this->users);
			printChannels(this->channels);
			std::cout << std::endl << "*** *** *** *** *** *** *** *** *** *** ***" << std::endl << std::endl;
		}
	}
	
	messageSplit.clear();

	return 0;
}

void	IRC::Server::registration(IRC::User& user, std::string message)
{
	IRC::Command cmd(message);
	cmd.detectCommand(this, user);

	/** Registration completed **/
	if (user.getPassword().size() > 0 && user.getNick().size() > 0 && user.getUser().size() > 0 && !user.isAuthenticated())
	{
		user.setAuthenticated(true);
		
		user.sendMessage(":" + this->getHostname() + " 001 " + user.getNick() + " :Welcome to the 42 IRC Network, " + user.getNick() + "!" + user.getUser() + "@" + user.getHostname());
		user.sendMessage(":" + this->getHostname() + " 002 " + user.getNick() + " :Your host is ircserv, running version 1.0");
		user.sendMessage(":" + this->getHostname() + " 003 " + user.getNick() + " :This server was created " + this->creationTimestamp);
		user.sendMessage(":" + this->getHostname() + " 004 " + user.getNick() + " :ircserv 1.0 ositnmlvk iso");
		user.sendMessage(":" + this->getHostname() + " 005 " + user.getNick() +  " :CASEMAPPING=<ascii> MAXTARGETS=1 CHANLIMIT=#:10 PREFIX=(ov)@+ :are supported by this server");
	}
}

int		IRC::Server::throwError(std::string message)
{
	perror(message.c_str());
	return -1;
}

void	IRC::Server::catchPing(void)
{
	if (PRINT_DEBUG)
		std::cout << "> Catch Ping: " << std::endl;

	/** Auxiliary vector to store the iterators that need to be removed **/
	IRC::Server::user_map_iters_remove iteratorsToRemove;
	for (IRC::Server::users_map::iterator userIt = this->users.begin(); userIt != this->users.end(); ++userIt)
	{
		if (PRINT_DEBUG)
			std::cout << userIt->second.getNick() << " (" << userIt->second.getSocket() << ") -> " << REG_TIMEOUT + userIt->second.getTimeout() - time(NULL) << "s" << std::endl;
		
		if (userIt->second.isPinged() && time(NULL) - userIt->second.getTimeout() > PING_TIMEOUT)
			iteratorsToRemove.push_back(std::pair<IRC::Server::users_map::iterator, std::string>(userIt, "PING ERROR"));
		else if (!userIt->second.isPinged() && time(NULL) - userIt->second.getTimeout() > REG_TIMEOUT)
		{
			if (!userIt->second.isAuthenticated())
				iteratorsToRemove.push_back(std::pair<IRC::Server::users_map::iterator, std::string>(userIt, "REGISTRATION TIMEOUT"));
			else
			{
				userIt->second.setPingKey(pingGenerator(5));
				userIt->second.setPingRequest(true);
				userIt->second.sendMessage("PING " + userIt->second.getPingKey());
			}
		}
	}

	/** Remove the iterators that need to be removed from the map **/
	for (IRC::Server::user_map_iters_remove::iterator it = iteratorsToRemove.begin(); it != iteratorsToRemove.end(); ++it)
		closeClient(it->first->second, it->second);
}
