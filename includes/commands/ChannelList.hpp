/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelList.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karisti- <karisti-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 19:46:06 by karisti-          #+#    #+#             */
/*   Updated: 2023/03/02 17:56:04 by karisti-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../Command.hpp"
#include <sstream>


namespace IRC
{
	class ChannelList : public Command
	{
		public:
			ChannelList();
			virtual ~ChannelList();

			void	exec(Server* server, User& user);

		private:
			void	printList(Server server, User user, IRC::Channel::channels_map::iterator itBegin, IRC::Channel::channels_map::iterator itEnd);
	};
}
