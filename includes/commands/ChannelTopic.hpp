/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelTopic.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karisti- <karisti-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 18:29:32 by gpernas-          #+#    #+#             */
/*   Updated: 2023/02/27 16:46:23 by karisti-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../Command.hpp"


namespace IRC
{
	class ChannelTopic : public Command
	{
		public:
			ChannelTopic();
			virtual ~ChannelTopic();

			void	exec(IRC::Server* server, IRC::User& user);
	};
}
