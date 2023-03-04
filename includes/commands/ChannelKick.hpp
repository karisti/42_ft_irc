/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelKick.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karisti- <karisti-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 12:19:57 by karisti-          #+#    #+#             */
/*   Updated: 2023/02/27 12:22:16 by karisti-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../Command.hpp"


namespace IRC
{
	class ChannelKick : public Command
	{
		public:
			ChannelKick();
			virtual ~ChannelKick();

			void	exec(IRC::Server* server, IRC::User& user);
	};
}
