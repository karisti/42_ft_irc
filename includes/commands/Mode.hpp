/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karisti- <karisti-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 10:00:31 by gpernas-          #+#    #+#             */
/*   Updated: 2023/02/07 12:01:01 by karisti-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../Command.hpp"
#include "../Channel.hpp"
#include <sstream>


namespace IRC
{
	class Server;
	class Command;

	class Mode : public Command
	{
		public:
			Mode();
			virtual ~Mode();

			void	exec(IRC::Server* server, IRC::User& user);
		
		private:
			std::string		printChannelMax(IRC::Channel recept);
			std::string		printPassword(IRC::Channel recept);
	};
}
