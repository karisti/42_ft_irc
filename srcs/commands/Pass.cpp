/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karisti- <karisti-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 21:15:21 by gpernas-          #+#    #+#             */
/*   Updated: 2023/03/02 16:16:54 by karisti-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/Pass.hpp"


IRC::Pass::Pass() {}
IRC::Pass::~Pass() {}

void IRC::Pass::exec(IRC::Server* server, IRC::User& user)
{
	/** CHECK POSSIBLE ERRORS **/
	if (this->args.size() < 1)
		return setReply(ERR_NEEDMOREPARAMS, *server, user, 1, this->command.c_str());
	if (user.isAuthenticated())
		return setReply(ERR_ALREADYREGISTRED, *server, user, 0);
	
	/** SET AND CHECK PASSWORD **/
	user.setPassword(args.substr(0, args.find(" ")));
	if (server->getPassword().size() > 0 && user.getPassword().compare(server->getPassword()) != 0)
		setReply(ERR_PASSWDMISMATCH, *server, user, 0);
}
