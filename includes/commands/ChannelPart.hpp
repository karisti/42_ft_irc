/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelPart.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karisti- <karisti-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 17:20:00 by karisti-          #+#    #+#             */
/*   Updated: 2023/02/27 17:09:59 by karisti-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../Command.hpp"


namespace IRC
{
	class ChannelPart : public Command
	{
		public:
			ChannelPart();
			virtual ~ChannelPart();

			void	exec(IRC::Server* server, IRC::User& user);
	};
}
