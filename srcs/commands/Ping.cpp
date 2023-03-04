/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karisti- <karisti-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 19:05:45 by gpernas-          #+#    #+#             */
/*   Updated: 2023/02/28 12:30:18 by karisti-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/Ping.hpp"


IRC::Ping::Ping() {}
IRC::Ping::~Ping() {}

void	IRC::Ping::exec(IRC::Server* server, IRC::User& user)
{
	/** ANSWER WITH PONG **/
	if (args.size() > 0)
	{
		args = ":" + server->getHostname() +  " PONG " + this->args;
		user.sendMessage(args);
	}
}
