/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelInvite.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karisti- <karisti-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 12:20:01 by karisti-          #+#    #+#             */
/*   Updated: 2023/03/04 22:17:32 by karisti-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/ChannelInvite.hpp"


IRC::ChannelInvite::ChannelInvite() {}
IRC::ChannelInvite::~ChannelInvite() {}

void	IRC::ChannelInvite::exec(IRC::Server* server, IRC::User& user)
{
	/** CHECK ALL POSSIBLE ERRORS **/
	if (!user.isAuthenticated())
		return setReply(ERR_NOTREGISTERED, *server, user, 0);
	
	std::vector<std::string> argsArray = splitString(args, " ");
	if (argsArray.size() < 2)
		return setReply(ERR_NEEDMOREPARAMS, *server, user, 1, this->command.c_str());

	IRC::Channel::channels_map::iterator channel = server->findChannel(argsArray[1]);
	if (channel == server->getChannels().end())
		return setReply(ERR_NOSUCHCHANNEL, *server, user, 1, argsArray[1].c_str());

	IRC::User::users_map::iterator newUserIt = findUserByNick(server->getUsers(), argsArray[0]);
	if (newUserIt == server->getUsers().end())
		return setReply(ERR_NOSUCHNICK, *server, user, 1, argsArray[0].c_str());

	if (!user.isInChannel(channel->second))
		return setReply(ERR_NOTONCHANNEL, *server, user, 1, channel->second.getName().c_str());

	if (channel->second.isInviteOnly() && !channel->second.isOperator(user))
		return setReply(ERR_CHANOPRIVSNEEDED, *server, user, 1, channel->second.getName().c_str());

	if (channel->second.existsUser(newUserIt->second))
		return setReply(ERR_USERONCHANNEL, *server, user, 2, newUserIt->second.getNick().c_str(), channel->second.getName().c_str());

	/** USER IS INVITED AND REPLY IS SENT **/
	std::string action = ":" + user.getNick() + "!" + user.getUser() + "@" + user.getHostname() + " INVITE " + newUserIt->second.getNick() + " :" + channel->second.getName();
	newUserIt->second.addInvitedChannel(channel->second);
	newUserIt->second.sendMessage(action);
	
	setReply(RPL_INVITING, *server, user, 2, newUserIt->second.getNick().c_str(), channel->second.getName().c_str());
}
