/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelJoin.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karisti- <karisti-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 21:15:21 by gpernas-          #+#    #+#             */
/*   Updated: 2023/03/02 20:54:05 by karisti-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/ChannelJoin.hpp"


IRC::ChannelJoin::ChannelJoin() {}
IRC::ChannelJoin::ChannelJoin(const IRC::ChannelJoin &other) { *this = other; }
IRC::ChannelJoin::~ChannelJoin() {}

IRC::ChannelJoin &IRC::ChannelJoin::operator=(const IRC::ChannelJoin &other)
{
	if (this != &other)
	{
		channelsArray = other.channelsArray;
		passwordsArray = other.passwordsArray;
	}
	return *this;
}

void			IRC::ChannelJoin::exec(IRC::Server* server, IRC::User& user)
{
	/** CHECK POSSIBLE ERRORS **/
	if (!user.isAuthenticated())
		return setReply(ERR_NOTREGISTERED, *server, user, 0);

	std::vector<std::string> argsArray = splitString(args, " ");
	if (argsArray.size() < 1)
		return setReply(ERR_NEEDMOREPARAMS, *server, user, 1, command.c_str());
	
	channelsArray = splitString(argsArray[0], ",");
	if (argsArray.size() > 1)
		passwordsArray = splitString(argsArray[1], ",");

	/** ITERATE EACH PARSED CHANNEL **/
	for (size_t i = 0; i < channelsArray.size(); i++)
	{
		if (channelsArray[i].size() < 2 || channelsArray[i].at(0) != '#')
			return setReply(ERR_BADCHANMASK, *server, user, 1, channelsArray[i].c_str());

		if (user.getJoinedChannels().size() >= MAX_CHANNELS)
			return setReply(ERR_TOOMANYCHANNELS, *server, user, 1, channelsArray[i].c_str());

		/** IF CHANNEL ALREADY EXIST, JOIN. IF DOESNT EXIST, CREATE **/
		IRC::Channel newChannel;
		IRC::Channel::channels_map::iterator found = server->findChannel(channelsArray[i]);
		if (found != server->getChannels().end())
		{
			if (!joinExistingChannel(found->second, *server, user))
				continue;
			newChannel = found->second;
		}
		else
			newChannel = createNewChannel(channelsArray[i], user, server);
		
		/** SEND REPLIES **/
		setActionInReply(user, newChannel, "JOIN :" + newChannel.getName());
		
		if (newChannel.getTopic().size() == 0)
			setReply(RPL_NOTOPIC, *server, user, 1, newChannel.getName().c_str());
		else
			setReply(RPL_TOPIC, *server, user, 2, newChannel.getName().c_str(), newChannel.getTopic().c_str());
			
		setReply(RPL_NAMREPLY, *server, user, 2, newChannel.getName().c_str(), newChannel.getUsersString().c_str());
		setReply(RPL_ENDOFNAMES, *server, user, 1, newChannel.getName().c_str());
	}
}

bool	IRC::ChannelJoin::joinExistingChannel(IRC::Channel& channel, IRC::Server server, IRC::User& user)
{
	if (channel.isInviteOnly() && !user.isInvitedToChannel(channel))
	{
		setReply(ERR_INVITEONLYCHAN, server, user, 1, channel.getName().c_str());
		return false;
	}

	if (!channel.checkPassword("") && (passwordsArray.size() <= 0 || !channel.checkPassword(passwordsArray.at(0))))
	{
		if (passwordsArray.size() > 0)
			passwordsArray.erase(passwordsArray.begin());
		setReply(ERR_BADCHANNELKEY, server, user, 1, channel.getName().c_str());
		return false;
	}
	else if (passwordsArray.size() > 0)
		passwordsArray.erase(passwordsArray.begin());

	if (channel.isFull())
	{
		setReply(ERR_CHANNELISFULL, server, user, 1, channel.getName().c_str());
		return false;
	}

	if (channel.existsUser(user))
		return false;

	user.addJoinedChannel(channel);
	return channel.addUser(user);
}

IRC::Channel	IRC::ChannelJoin::createNewChannel(std::string channelName, IRC::User& user, IRC::Server* server)
{
	IRC::Channel newChannel = Channel(channelName, user);
	server->addChannel(newChannel);
	user.addJoinedChannel(newChannel);

	return newChannel;
}
