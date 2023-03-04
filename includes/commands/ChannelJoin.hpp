/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelJoin.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karisti- <karisti-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 13:07:58 by karisti-          #+#    #+#             */
/*   Updated: 2023/02/28 23:39:04 by karisti-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../Command.hpp"
#include <algorithm>


#define MAX_CHANNELS 10


namespace IRC
{
	class ChannelJoin : public Command
	{
		private:
			std::vector<std::string> channelsArray;
			std::vector<std::string> passwordsArray;

		public:
			ChannelJoin();
			ChannelJoin(const ChannelJoin& other);
			virtual ~ChannelJoin();
			ChannelJoin& operator=(const ChannelJoin& other);

			void	exec(IRC::Server* server, IRC::User& user);

		private:
			bool			joinExistingChannel(IRC::Channel& channel, IRC::Server server, IRC::User &user);
			IRC::Channel	createNewChannel(std::string channelName, IRC::User& user, IRC::Server* server);
	};
}
