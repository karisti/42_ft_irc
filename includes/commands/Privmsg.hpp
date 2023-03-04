/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karisti- <karisti-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 23:07:51 by gpernas-          #+#    #+#             */
/*   Updated: 2023/02/27 17:25:40 by karisti-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../Command.hpp"


namespace IRC
{
	class PrivMsg : public Command
	{
		public:
			PrivMsg();
			virtual ~PrivMsg();

			void	exec(IRC::Server* server, IRC::User& user);
	};
}
