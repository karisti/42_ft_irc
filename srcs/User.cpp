/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karisti- <karisti-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 16:56:26 by karisti-          #+#    #+#             */
/*   Updated: 2023/03/04 22:17:32 by karisti-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/User.hpp"


IRC::User::User(void)
{
	this->socket = 0;
	this->password = "";
	this->nick = "";
	this->user = "";
	this->realname = "";
	this->hostname =  "";
	this->servername = "";
	this->authenticated = false;
	this->pingReq = false;
	this->pingKey = "";
	this->timeout = time(NULL);
	this->buffer = "";
	
}

IRC::User::User(const IRC::User& other) { *this = other; }
IRC::User::~User() {}

IRC::User& IRC::User::operator=(const IRC::User &other)
{
	this->address = other.address;
	this->socket = other.socket;
	this->password = other.password;
	this->nick = other.nick;
	this->user = other.user;
	this->realname = other.realname;
	this->hostname = other.hostname;
	this->servername = other.servername;
	this->authenticated = other.authenticated;
	this->pingReq = other.pingReq;
	this->pingKey = other.pingKey;
	this->timeout = other.timeout;
	this->joinedChannels = other.joinedChannels;
	this->invitedChannels = other.invitedChannels;
	this->buffer = other.buffer;
	
	return *this;
}

/* -- Getters -- */
sockaddr_in								IRC::User::getAddress(void) const { return this->address; }
int										IRC::User::getSocket(void) const { return this->socket; }
std::string								IRC::User::getPassword(void) const { return this->password; }
std::string								IRC::User::getNick(void) const { return this->nick; }
std::string								IRC::User::getUser(void) const { return this->user; }
std::string								IRC::User::getRealname(void) const { return this->realname; }
std::string								IRC::User::getHostname(void) const { return this->hostname; }
std::string								IRC::User::getServername(void) const { return this->servername; }
bool									IRC::User::isAuthenticated(void) const { return this->authenticated; }
bool									IRC::User::isPinged(void) const { return this->pingReq; }
std::string								IRC::User::getPingKey(void) const { return this->pingKey; }
time_t									IRC::User::getTimeout(void) const { return this->timeout; }
IRC::User::channels_map&				IRC::User::getJoinedChannels(void) { return this->joinedChannels; }
IRC::User::channels_map&				IRC::User::getinvitedChannels(void) { return this->invitedChannels; }
std::string								IRC::User::getBuffer(void) const { return this->buffer; }

/* -- Setters -- */
void	IRC::User::setPassword(std::string password) { this->password = password; }
void	IRC::User::setNick(std::string nick) { this->nick = nick; }
void	IRC::User::setUser(std::string user) { this->user = user; }
void	IRC::User::setRealname(std::string realname) { this->realname = realname; }
void	IRC::User::setHostname(std::string hostname) { this->hostname = hostname; }
void	IRC::User::setServername(std::string servername) { this->servername = servername; }
void	IRC::User::setAuthenticated(bool value) { this->authenticated = value; }
void	IRC::User::setTimeout(time_t timeout) { this->timeout = timeout; }
void	IRC::User::setPingKey(std::string key) { this->pingKey = key; }

/* -- Modifiers -- */
void	IRC::User::addJoinedChannel(IRC::Channel& channel)
{
	this->joinedChannels[channel.getName()] = channel;
	removeInvitedChannel(channel);
}

void	IRC::User::removeJoinedChannel(IRC::Channel channel)
{
	this->joinedChannels.erase(channel.getName());
}

void	IRC::User::addInvitedChannel(IRC::Channel& channel)
{
	this->invitedChannels[channel.getName()] = channel;
}

void	IRC::User::removeInvitedChannel(IRC::Channel channel)
{
	this->invitedChannels.erase(channel.getName());
}

void	IRC::User::appendBuffer(std::string str) { this->buffer.append(str); }
void	IRC::User::clearBuffer(void) { this->buffer.clear(); }

/* -- Member functions -- */
void			IRC::User::startListeningSocket(int serverSocket)
{
	char host[NI_MAXHOST];
	char service[NI_MAXSERV];
	memset(host, 0, NI_MAXHOST);
	memset(service, 0, NI_MAXSERV);
	
	socklen_t addressSize = sizeof(this->address);
	
	/** Accept client connection **/
	this->socket = accept(serverSocket, (struct sockaddr *)&this->address, &addressSize);
	if (this->socket == -1)
	{
		perror("Accept socket error");
		return ;
	}

	/** Make non-blocking **/
	if (fcntl(this->socket, F_SETFL, O_NONBLOCK) < 0)
	{
		perror("Error making client socket non blocking");
		return ;
	}

	if (getnameinfo((struct sockaddr *) &this->address, addressSize, host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
		std::cout << "Host1: " << host << " connected on port " << service << std::endl;
	else 
	{
		inet_ntop(AF_INET, &this->address.sin_addr, host, NI_MAXHOST);
		std::cout << "Host: " << host << " connected on port " << ntohs(this->address.sin_port) << std::endl;
	}
	
	std::string s(host);
	this->hostname = s;
}

std::string		IRC::User::getJoinedChannelsString(void) const
{
	std::string channelsString = "";
	
	for (IRC::User::channels_map::const_iterator channelIt = this->joinedChannels.begin(); channelIt != this->joinedChannels.end(); ++channelIt)
	{
		if (channelIt != this->joinedChannels.begin())
			channelsString += " ";
		channelsString += channelIt->second.getName();
	}

	return channelsString;
}

bool			IRC::User::isInChannel(IRC::Channel channel) const
{
	return (this->joinedChannels.find(channel.getName()) != this->joinedChannels.end());
}

bool			IRC::User::isInvitedToChannel(IRC::Channel channel) const
{
	return (this->invitedChannels.find(channel.getName()) != this->invitedChannels.end());
}

void			IRC::User::sendMessage(std::string message) const
{
	message += "\r\n";
	send(getSocket(), message.c_str(), message.size(), 0);
}

/* -- Non-member functions -- */
bool	IRC::operator== (const IRC::User lhs, const IRC::User rhs)
{
	return (lhs.getSocket() == rhs.getSocket());
}

IRC::User::users_map::iterator	IRC::findUserByNick(IRC::User::users_map& users, std::string nick)
{
	IRC::User::users_map::iterator it = users.begin();
	
	for (; it != users.end(); it++)
	{
		if (it->second.getNick().compare(nick) == 0)
			return it;
	}
	return it;
}

void	IRC::printUser(IRC::User user)
{
	std::cout << "Socket: " << user.getSocket();
	std::cout << ", Password: " << user.getPassword();
	std::cout << ", Nick: " << user.getNick();
	std::cout << ", User: " << user.getUser();
	std::cout << ", Real name: " << user.getRealname();
	std::cout << ", Authenticated: " << user.isAuthenticated();
	std::cout << ", Channels: " << user.getJoinedChannelsString();
	std::cout << std::endl;
}

void	IRC::printUsers(IRC::User::users_map users)
{
	if (users.size() == 0)
		return ;

	std::cout << "------- Users -------" << std::endl;
	IRC::User::users_map::iterator it = users.begin();
	for (; it != users.end(); it++)
		printUser(it->second);
	std::cout << "---------------------" << std::endl;
}

void	IRC::User::setPingRequest(bool req)
{
	this->pingReq = req;
}
