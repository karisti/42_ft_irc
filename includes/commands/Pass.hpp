/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karisti- <karisti-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 15:20:40 by karisti-          #+#    #+#             */
/*   Updated: 2023/02/27 18:14:15 by karisti-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../Command.hpp"


namespace IRC
{
	class Pass : public Command
	{
		public:
			Pass();
			virtual ~Pass();

			void	exec(IRC::Server* server, IRC::User& user);
	};
}
