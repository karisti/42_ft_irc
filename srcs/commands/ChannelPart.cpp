/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelPart.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karisti- <karisti-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 17:19:43 by karisti-          #+#    #+#             */
/*   Updated: 2023/03/02 20:54:05 by karisti-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/ChannelPart.hpp"


IRC::ChannelPart::ChannelPart() {}
IRC::ChannelPart::~ChannelPart() {}

void	IRC::ChannelPart::exec(IRC::Server* server, IRC::User& user)
{
	/** CHECK POSSIBLE ERRORS **/
	if (!user.isAuthenticated())
		return setReply(ERR_NOTREGISTERED, *server, user, 0);

	std::vector<std::string> argsArray = splitString(args, " ", 1);
	if (argsArray.size() < 1)
		return setReply(ERR_NEEDMOREPARAMS, *server, user, 1, command.c_str());
	
	std::vector<std::string> channelsArray = splitString(argsArray[0], ",");

	/** ITERATE EACH PARSED CHANNEL **/
	for (std::vector<std::string>::iterator it = channelsArray.begin(); it != channelsArray.end(); it++)
	{
		IRC::Channel::channels_map::iterator channelIt = server->findChannel(*it);
		if (channelIt == server->getChannels().end())
			return setReply(ERR_NOSUCHCHANNEL, *server, user, 1, (*it).c_str());
		
		if (!channelIt->second.existsUser(user))
			return setReply(ERR_NOTONCHANNEL, *server, user, 1, (*it).c_str());
		
		/** REMOVE USER FROM CHANNEL **/
		if (argsArray.size() > 1 && argsArray[1].size() > 0)
			setActionInReply(user, channelIt->second, "PART " + channelIt->second.getName() + " :\"" + argsArray[1] + "\"");
		else
			setActionInReply(user, channelIt->second, "PART :" + channelIt->second.getName());
		channelIt->second.removeUser(server, user);
		user.removeJoinedChannel(channelIt->second);
	}
}
