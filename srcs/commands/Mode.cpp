/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karisti- <karisti-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 10:01:18 by gpernas-          #+#    #+#             */
/*   Updated: 2023/03/04 22:34:08 by karisti-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/Mode.hpp"


IRC::Mode::Mode() {}
IRC::Mode::~Mode() {}

void	IRC::Mode::exec(IRC::Server* server, IRC::User& user)
{
	/** CHECK POSSIBLE ERRORS **/
	if (!user.isAuthenticated())
		return setReply(ERR_NOTREGISTERED, *server, user, 0);

	if (this->args.size() == 0)
		return setReply(ERR_NEEDMOREPARAMS, *server, user, 1, command.c_str());

	/** CHANNEL MODES **/
	if (this->args.at(0) == '#')
	{
		/** CHANNEL MODE ERRORS **/
		std::vector<std::string> argSplit = splitString(this->args, " ");
		IRC::Channel::channels_map::iterator receptor = server->findChannel(argSplit[0]);
		if (receptor == server->getChannels().end())
			return setReply(ERR_NOSUCHCHANNEL, *server, user, 1, argSplit[0].c_str());
		
		if (!user.isInChannel(receptor->second))
			return setReply(ERR_NOTONCHANNEL, *server, user, 1, argSplit[0].c_str());

		if(!receptor->second.isOperator(user))
			return setReply(ERR_CHANOPRIVSNEEDED, *server, user, 1, argSplit[0].c_str());

		if (argSplit.size() > 1 && argSplit[1].size() > 0)
		{
			/** MANAGE ALL MODE CHANGES **/
			for (size_t i = 1; i < argSplit[1].size(); i++)
			{
				if (argSplit[1].at(i) == 'o')
				{
					if (argSplit.size() < 3)
						return setReply(ERR_NEEDMOREPARAMS, *server, user, 1, command.c_str());
					if (argSplit[1].at(0) == '+')
					{
						IRC::User::users_map::iterator userIt = findUserByNick(server->getUsers(), argSplit[2]);
						if (userIt != server->getUsers().end())
						{
							receptor->second.addOperator(userIt->second, server);
							setActionInReply(user, receptor->second, "MODE " + receptor->second.getName() + " +o :" + argSplit[2]);
						}
					}
					else if (argSplit[1].at(0) == '-')
					{
						IRC::User::users_map::iterator userIt = findUserByNick(server->getUsers(), argSplit[2]);
						if (userIt != server->getUsers().end())
							receptor->second.removeOperator(userIt->second, server);
						setActionInReply(user, receptor->second, "MODE " + receptor->second.getName() + " -o :" + argSplit[2]);
					}
				}
				else if (argSplit[1].at(i) == 's')
				{
					if (argSplit[1].at(0) == '+' && !receptor->second.isSecret())
					{
						receptor->second.setSecrecy(true);
						setActionInReply(user, receptor->second, "MODE " + receptor->second.getName() + " +s :" + receptor->second.getName());
					}
					else if (argSplit[1].at(0) == '-' && receptor->second.isSecret())
					{
						receptor->second.setSecrecy(false);
						setActionInReply(user, receptor->second, "MODE " + receptor->second.getName() + " -s :" + receptor->second.getName());
					}
				}
				else if (argSplit[1].at(i) == 'i')
				{
					if (argSplit[1].at(0) == '+' && !receptor->second.isInviteOnly())
					{
						receptor->second.setInviteOnly(true);
						setActionInReply(user, receptor->second, "MODE " + receptor->second.getName() + " +i :" + receptor->second.getName());
					}
					else if (argSplit[1].at(0) == '-' && receptor->second.isInviteOnly())
					{
						receptor->second.setInviteOnly(false);
						setActionInReply(user, receptor->second, "MODE " + receptor->second.getName() + " -i :" + receptor->second.getName());
					}
				}
				else if (argSplit[1].at(i) == 't')
				{
					if (argSplit[1].at(0) == '+' && !receptor->second.isFreeTopic())
					{
						receptor->second.setFreeTopic(true);
						setActionInReply(user, receptor->second, "MODE " + receptor->second.getName() + " +t :" + receptor->second.getName());
					}
					else if (argSplit[1].at(0) == '-' && receptor->second.isFreeTopic())
					{
						receptor->second.setFreeTopic(false);
						setActionInReply(user, receptor->second, "MODE " + receptor->second.getName() + " -t :" + receptor->second.getName());
					}
				}
				else if (argSplit[1].at(i) == 'n')
				{
					if (argSplit[1].at(0) == '+' && !receptor->second.isPublicMsg())
					{
						receptor->second.setPublicMsg(true);
						setActionInReply(user, receptor->second, "MODE " + receptor->second.getName() + " +n :" + receptor->second.getName());
					}
					else if (argSplit[1].at(0) == '-' && receptor->second.isPublicMsg())
					{
						receptor->second.setPublicMsg(false);
						setActionInReply(user, receptor->second, "MODE " + receptor->second.getName() + " -n :" + receptor->second.getName());
					}
				}
				else if (argSplit[1].at(i) == 'm')
				{
					if (argSplit[1].at(0) == '+' && !receptor->second.isModerated())
					{
						receptor->second.setModerated(true);
						setActionInReply(user, receptor->second, "MODE " + receptor->second.getName() + " +m :" + receptor->second.getName());
					}
					else if (argSplit[1].at(0) == '-' && receptor->second.isModerated())
					{
						receptor->second.setModerated(false);
						setActionInReply(user, receptor->second, "MODE " + receptor->second.getName() + " -m :" + receptor->second.getName());
					}
				}
				else if (argSplit[1].at(i) == 'l')
				{
					if (argSplit[1].at(0) == '+')
					{
						if (argSplit.size() < 3)
							return setReply(ERR_NEEDMOREPARAMS, *server, user, 1, command.c_str());
						receptor->second.setMaxUsers(atoi(argSplit[2].c_str())); 
						setActionInReply(user, receptor->second, "MODE " + receptor->second.getName() + " +l :" + argSplit[2]);
					}
					else if (argSplit[1].at(0) == '-')
					{
						receptor->second.setMaxUsers(0);
						setActionInReply(user, receptor->second, "MODE " + receptor->second.getName() + " -l :" + "0");
					}
				}
				else if (argSplit[1].at(i) == 'v')
				{
					if (argSplit.size() < 3)
						return setReply(ERR_NEEDMOREPARAMS, *server, user, 1, command.c_str());
					if (argSplit[1].at(0) == '+')
					{
						IRC::User::users_map::iterator userIt = findUserByNick(server->getUsers(), argSplit[2]);
						if (userIt != server->getUsers().end())
						{
							receptor->second.addModerator(userIt->second, server);
							setActionInReply(user, receptor->second, "MODE " + receptor->second.getName() + " +v :" + argSplit[2]);
						}
					}
					else if (argSplit[1].at(0) == '-')
					{
						IRC::User::users_map::iterator userIt = findUserByNick(server->getUsers(), argSplit[2]);
						if (userIt != server->getUsers().end())
							receptor->second.removeModerator(userIt->second, server);
						setActionInReply(user, receptor->second, "MODE " + receptor->second.getName() + " -v :" + argSplit[2]);
					}
				}
				else if (argSplit[1].at(i) == 'k')
				{
					if (argSplit.size() < 3)
						return setReply(ERR_NEEDMOREPARAMS, *server, user, 1, command.c_str());
					if (argSplit[1].at(0) == '+')
					{
						receptor->second.setPassword(argSplit[2]);
						setActionInReply(user, receptor->second, "MODE " + receptor->second.getName() + " +k :" + argSplit[2]);
					}
					else if (argSplit[1].at(0) == '-')
					{
						receptor->second.setPassword("");
						setActionInReply(user, receptor->second, "MODE " + receptor->second.getName() + " -k :" + argSplit[2]);
					}
				}
				else
					return setReply(ERR_UNKNOWNMODE, *server, user, 1, argSplit[i].c_str());
			}
		}
		else
		{
			/** SHOW MODES STATUS **/
			std::string mode = " +";
			receptor->second.isSecret()? mode += "s": "";
			receptor->second.isFreeTopic()? mode += "t": "";
			receptor->second.isPublicMsg()? mode += "n": "";
			receptor->second.isModerated()? mode += "m": "";
			receptor->second.isInviteOnly()? mode += "i": "";
			receptor->second.hasPassword()? mode += "k": "";
			receptor->second.hasMax()? mode += "l": "";
			receptor->second.hasPassword()? mode += printPassword(receptor->second): "";
			receptor->second.hasMax()? mode += printChannelMax(receptor->second): "";
			if (mode.length() > 0)
				mode.erase(0, 1);

			setReply(RPL_CHANNELMODEIS, *server, user, 2, receptor->second.getName().c_str(), mode.c_str());
		}
	}
}

std::string		IRC::Mode::printChannelMax(IRC::Channel recept)
{
	std::stringstream ss;
	ss << " :";
	ss << recept.getMaxUsers();
	return ss.str();
}

std::string		IRC::Mode::printPassword(IRC::Channel recept)
{
	std::string str(" ");
	str += recept.getPassword();
	return str;
}
