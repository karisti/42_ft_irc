/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karisti- <karisti-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 02:14:55 by gpernas-          #+#    #+#             */
/*   Updated: 2023/02/27 18:14:06 by karisti-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../Command.hpp"


namespace IRC
{
	class Nick : public Command
	{
		public:
			Nick();
			virtual ~Nick();

			void	exec(IRC::Server* server, IRC::User& user);
	};
}
