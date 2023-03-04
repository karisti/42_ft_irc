/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelList.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karisti- <karisti-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 19:46:03 by karisti-          #+#    #+#             */
/*   Updated: 2023/03/04 22:29:51 by karisti-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/ChannelList.hpp"


IRC::ChannelList::ChannelList() {}
IRC::ChannelList::~ChannelList() {}

void	IRC::ChannelList::exec(IRC::Server* server, IRC::User& user)
{
	if (!user.isAuthenticated())
		return setReply(ERR_NOTREGISTERED, *server, user, 0);
	
	std::vector<std::string> argsArray = splitString(args, " ");
	std::vector<std::string> channelsArrayStr = splitString(argsArray[0], ",");
	
	/** WE FIND ALL CHANNELS FROM ARGS **/
	IRC::Channel::channels_map channelsFound;
	for (std::vector<std::string>::iterator it = channelsArrayStr.begin(); it != channelsArrayStr.end(); it++)
	{
		IRC::Channel::channels_map::iterator channelIt = server->findChannel(*it);
		if (channelIt != server->getChannels().end())
			channelsFound[channelIt->second.getName()] = channelIt->second;
	}
	
	/** WE REPLY FOUND USER LIST OR ALL OF THEM **/
	if (channelsFound.size() > 0)
		printList(*server, user, channelsFound.begin(), channelsFound.end());
	else
		printList(*server, user, server->getChannels().begin(), server->getChannels().end());
}

void	IRC::ChannelList::printList(IRC::Server server, IRC::User user, IRC::Channel::channels_map::iterator itBegin, IRC::Channel::channels_map::iterator itEnd)
{
	setReply(RPL_LISTSTART, server, user, 0);
	
	for (IRC::Channel::channels_map::iterator it = itBegin; it != itEnd; it++)
	{
		if (it->second.isSecret() && !user.isInChannel(it->second))
			continue ;
		
		std::stringstream ss;
		ss << it->second.getUsers().size();
		std::string sizeStr = ss.str();
		
		setReply(RPL_LIST, server, user, 3, it->second.getName().c_str(), sizeStr.c_str(), it->second.getTopic().c_str());
	}
	
	setReply(RPL_LISTEND, server, user, 0);
}
