/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karisti- <karisti-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 17:51:40 by karisti-          #+#    #+#             */
/*   Updated: 2023/03/04 21:24:17 by karisti-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"


IRC::Channel::Channel()
{
	this->name = "";
	this->topic = "";
	this->password = "";
	this->inviteOnlyMode = false;
	this->secretMode = false;
	this->freeTopicMode = true;
	this->publicMsgMode = false;
	this->moderatedMode = false;
	this->maxUsers = 0;
}

IRC::Channel::Channel(std::string name)
{
	this->name = name;
	this->topic = "";
	this->password = "";
	this->inviteOnlyMode = false;
	this->secretMode = false;
	this->freeTopicMode = true;
	this->publicMsgMode = false;
	this->moderatedMode = false;
	this->maxUsers = 0;
}

IRC::Channel::Channel(std::string name, IRC::User createdBy)
{
	this->name = name;
	this->topic = "";
	this->password = "";
	this->inviteOnlyMode = false;
	this->secretMode = false;
	this->freeTopicMode = true;
	this->publicMsgMode = false;
	this->moderatedMode = false;
	this->maxUsers = 0;
	
	this->users[createdBy.getSocket()] = std::pair<IRC::User, UserRol>(createdBy, USER_OPERATOR);
}

IRC::Channel::Channel(const IRC::Channel &other) { *this = other; }
IRC::Channel::~Channel() {}

IRC::Channel &IRC::Channel::operator=(const IRC::Channel &other)
{
	if (this != &other)
	{
		this->name = other.name;
		this->topic = other.topic;
		this->password = other.password;
		this->users = other.users;
		this->inviteOnlyMode = other.inviteOnlyMode;
		this->secretMode = other.secretMode;
		this->freeTopicMode = other.freeTopicMode;
		this->publicMsgMode = other.publicMsgMode;
		this->moderatedMode = other.moderatedMode;
		this->maxUsers = other.maxUsers;
	}
	
	return *this;
}

/* -- Getters -- */
std::string						IRC::Channel::getName(void) const { return this->name; }
std::string						IRC::Channel::getTopic(void) const { return this->topic; }
std::string						IRC::Channel::getPassword(void) const { return this->password; }
IRC::Channel::chanusers_map&	IRC::Channel::getUsers(void) { return this->users; }
bool							IRC::Channel::isInviteOnly(void) const { return this->inviteOnlyMode; }
bool							IRC::Channel::isSecret(void) const { return this->secretMode; }
bool							IRC::Channel::isFreeTopic(void) const { return this->freeTopicMode; }
bool							IRC::Channel::isPublicMsg(void) const { return this->publicMsgMode; }
bool							IRC::Channel::isModerated(void) const { return this->moderatedMode; }
int								IRC::Channel::getMaxUsers(void) const { return this->maxUsers; }

/* -- Setters -- */
void						IRC::Channel::setName(std::string newName) { this->name = newName; }
void						IRC::Channel::setTopic(std::string newTopic) { this->topic = newTopic; }
void						IRC::Channel::setPassword(std::string password) { this->password = password; }
void						IRC::Channel::setInviteOnly(bool value) { this->inviteOnlyMode = value; }
void						IRC::Channel::setSecrecy(bool value) { this->secretMode = value; }
void						IRC::Channel::setFreeTopic(bool value) { this->freeTopicMode = value; }
void						IRC::Channel::setPublicMsg(bool value) { this->publicMsgMode = value; }
void						IRC::Channel::setModerated(bool value) { this->moderatedMode = value; }
void						IRC::Channel::setMaxUsers(int size) { if (size < 0) { size = 0; } this->maxUsers = size; }

/* -- Modifiers -- */
void	IRC::Channel::addOperator(IRC::User user, IRC::Server* server)
{
	(void) server;
	
	IRC::Channel::chanusers_map::iterator userIt = this->users.find(user.getSocket());
	if (userIt != this->users.end())
	{
		if (userIt->second.second == USER_MODERATOR)
			userIt->second.second = USER_MODOPER;
		else
			userIt->second.second = USER_OPERATOR;
	}
	server->updateUserInChannels(userIt->second.first);
}

void	IRC::Channel::removeOperator(IRC::User user, IRC::Server* server)
{
	(void) server;
	
	IRC::Channel::chanusers_map::iterator userIt = this->users.find(user.getSocket());
	if (userIt != this->users.end())
	{
		if (userIt->second.second == USER_MODOPER)
			userIt->second.second = USER_MODERATOR;
		else
			userIt->second.second = USER_NORMAL;
	}
	server->updateUserInChannels(userIt->second.first);
}

void	IRC::Channel::addModerator(IRC::User user, IRC::Server* server)
{
	(void) server;
	
	IRC::Channel::chanusers_map::iterator userIt = this->users.find(user.getSocket());
	if (userIt != this->users.end())
	{
		if (userIt->second.second == USER_OPERATOR)
			userIt->second.second = USER_MODOPER;
		else
			userIt->second.second = USER_MODERATOR;
	}
	server->updateUserInChannels(userIt->second.first);
}

void	IRC::Channel::removeModerator(IRC::User user, IRC::Server* server)
{
	(void) server;
	
	IRC::Channel::chanusers_map::iterator userIt = this->users.find(user.getSocket());
	if (userIt != this->users.end())
	{
		if (userIt->second.second == USER_MODOPER)
			userIt->second.second = USER_OPERATOR;
		else
			userIt->second.second = USER_NORMAL;
	}
	server->updateUserInChannels(userIt->second.first);
}

bool	IRC::Channel::addUser(IRC::User& user)
{
	std::pair<chanusers_map::iterator, bool> ret;
	ret = this->users.insert(std::pair<int, std::pair<IRC::User, UserRol> >(user.getSocket(), std::pair<IRC::User, UserRol>(user, USER_NORMAL)) );
	
	return ret.second;
}

void	IRC::Channel::removeUser(IRC::Server* server, IRC::User& user)
{
	this->users.erase(user.getSocket());

	if (isEmpty())
		server->removeChannel(*this);
}

/* -- Member functions -- */
bool	IRC::Channel::isOperator(IRC::User user)
{
	IRC::Channel::chanusers_map::iterator userIt = this->users.find(user.getSocket());
	if (userIt != this->users.end())
		return userIt->second.second == USER_MODOPER || userIt->second.second == USER_OPERATOR;
	return false;
}

bool	IRC::Channel::isModerator(IRC::User user)
{
	IRC::Channel::chanusers_map::iterator userIt = this->users.find(user.getSocket());
	if (userIt != this->users.end())
		return userIt->second.second == USER_MODOPER || userIt->second.second == USER_MODERATOR;
	return false;
}

bool	IRC::Channel::existsUser(IRC::User user)
{
	return this->users.find(user.getSocket()) != this->users.end();
}

std::string	IRC::Channel::getUsersString(void)
{
	std::string usersString = "";

	for (IRC::Channel::chanusers_map::iterator userIt = this->users.begin(); userIt != this->users.end(); userIt++)
	{
		if (userIt != this->users.begin())
			usersString += " ";

		if (userIt->second.second == USER_MODOPER || userIt->second.second == USER_OPERATOR)
			usersString += "@";
			
		if (userIt->second.second == USER_MODOPER || userIt->second.second == USER_MODERATOR)
			usersString += "+";
	
		usersString += userIt->second.first.getNick();
	}

	return usersString;
}

bool	IRC::Channel::hasPassword(void) const 
{
	return this->password.size() > 0;
}

bool	IRC::Channel::checkPassword(std::string pass) const
{
	return (!hasPassword() || this->password.compare(pass) == 0);
}

bool	IRC::Channel::hasMax(void) const
{
	return this->maxUsers > 0;
}

bool	IRC::Channel::isFull(void) const
{
	return (hasMax() && (int)this->users.size() >= getMaxUsers());
}

bool	IRC::Channel::isEmpty(void) const
{
	return (this->users.empty());
}

void	IRC::Channel::sendMessageToUsers(IRC::User sender, std::string message)
{
	for (IRC::Channel::chanusers_map::iterator userIt = this->users.begin(); userIt != this->users.end(); userIt++)
	{
		if (!(userIt->second.first == sender))
			userIt->second.first.sendMessage(message);
	}
}

/* -- Non-member functions -- */
bool	IRC::operator== (const IRC::Channel lhs, const IRC::Channel rhs)
{
	if (lhs.getName().compare(rhs.getName()) == 0)
		return true;
	return false;
}

void	IRC::printChannels(IRC::Channel::channels_map& channels)
{
	if (channels.size() == 0)
		return ;
	std::cout << "------- Channels -------" << std::endl;
	for (IRC::Channel::channels_map::iterator it = channels.begin(); it != channels.end(); ++it)
		printChannel(it->second);
	std::cout << "------------------------" << std::endl;
}

void	IRC::printChannel(IRC::Channel& channel)
{	
	std::cout << channel.getName() << ":" << std::endl;
	std::cout << "	> Topic: " << channel.getTopic() << std::endl;
	std::cout << "	> Password: " << channel.getPassword() << std::endl;
	std::cout << "	> Mode: ";
	
	std::string modes = "";
	modes += (channel.isInviteOnly()) ? "+i" : "-i";
	modes += " ";
	modes += (channel.isSecret()) ? "+s" : "-s";
	modes += " ";
	modes += (channel.isFreeTopic()) ? "+t" : "-t";
	modes += " ";
	modes += (channel.isPublicMsg()) ? "+n" : "-n";
	modes += " ";
	modes += (channel.isModerated()) ? "+m" : "-m";
	modes += " ";
	
	std::cout << modes << std::endl;
	std::cout << "	> Max users: " << channel.getMaxUsers() << std::endl;
	std::cout << "	> Users: " << channel.getUsersString() << std::endl;
	std::cout << std::endl;
}
