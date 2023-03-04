/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelInvite.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karisti- <karisti-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 12:20:03 by karisti-          #+#    #+#             */
/*   Updated: 2023/02/27 12:21:36 by karisti-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../Command.hpp"


namespace IRC
{
	class ChannelInvite : public Command
	{
		public:
			ChannelInvite();
			virtual ~ChannelInvite();

			void	exec(IRC::Server* server, IRC::User& user);
	};
}
