/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pong.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karisti- <karisti-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 19:05:45 by gpernas-          #+#    #+#             */
/*   Updated: 2023/03/04 21:28:25 by karisti-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/Pong.hpp"


IRC::Pong::Pong() {}
IRC::Pong::~Pong() {}

void IRC::Pong::exec(IRC::Server* server, IRC::User& user)
{
	if (args.size() < 1)
		return setReply(ERR_NEEDMOREPARAMS, *server, user, 0);
	
	if (user.isPinged() == false)
		return ;

	/** IF KEY IS CORRECT RESET TIMEOUT, ELSE CLOSE CONNECTION **/
	if (args.compare(user.getPingKey()) == 0)
	{
		user.setTimeout(time(NULL));
		user.setPingRequest(false);
	}
	else
		server->closeClient(user, "INCORRECT PING");
}
