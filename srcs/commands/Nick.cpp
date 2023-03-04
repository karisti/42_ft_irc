/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karisti- <karisti-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 02:21:33 by gpernas-          #+#    #+#             */
/*   Updated: 2023/03/04 22:17:32 by karisti-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/Nick.hpp"


IRC::Nick::Nick() {}
IRC::Nick::~Nick() {}

void	IRC::Nick::exec(IRC::Server* server, IRC::User& user)
{
	/** CHECK ALL POSSIBLE ERRORS **/
	if (server->getPassword().size() > 0 && user.getPassword() != server->getPassword())
		return ;
	if (this->args.size() < 1)
		return setReply(ERR_NONICKNAMEGIVEN, *server, user, 0);
		
	std::transform(this->args.begin(), this->args.end(), this->args.begin(), ::tolower);
	
	for (size_t i = 0; i != this->args.size(); i++)
		if (!std::isprint(this->args.at(i)) || this->args.at(i) == '#')
			return setReply(ERR_ERRONEUSNICKNAME, *server, user, 1, this->args.c_str());
			
	if (findUserByNick(server->getUsers(), this->args) != server->getUsers().end())
		return setReply(ERR_NICKNAMEINUSE, *server, user, 1, this->args.c_str());

	/** UPDATE USER NICK AND ALSO IN CHANNELS USER LISTS **/
	user.setNick(splitString(this->args, " ").at(0));
	server->updateUserInChannels(user);
}
