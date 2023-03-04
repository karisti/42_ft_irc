/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karisti- <karisti-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 17:52:13 by karisti-          #+#    #+#             */
/*   Updated: 2023/03/04 21:23:19 by karisti-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "User.hpp"
#include "Server.hpp"


namespace IRC
{
	class Server;
	class User;

	class Channel
	{
		public:
			enum UserRol {
				USER_NORMAL=1,
				USER_MODERATOR=2,
				USER_OPERATOR=3,
				USER_MODOPER=4
			};
			
			typedef std::map<std::string, Channel> 				channels_map;
			typedef std::map<int, std::pair<User, UserRol> >	chanusers_map;

		private:
			std::string				name;
			std::string				topic;
			std::string				password;
			chanusers_map			users;
			bool					inviteOnlyMode;
			bool					secretMode;
			bool					freeTopicMode;
			bool					publicMsgMode;
			bool					moderatedMode;
			int						maxUsers;

		public:
			Channel();
			Channel(std::string name);
			Channel(std::string name, User createdBy);
			Channel(const Channel& other);
			~Channel();
			Channel& operator=(const Channel& other);

			/* -- Getters -- */
			std::string				getName(void) const;
			std::string				getTopic(void) const;
			std::string				getPassword(void) const;
			chanusers_map&			getUsers(void);
			bool					isInviteOnly(void) const;
			bool					isSecret(void) const;
			bool					isFreeTopic(void) const;
			bool					isPublicMsg(void) const;
			bool					isModerated(void) const;
			int						getMaxUsers(void) const;

			/* -- Setters -- */
			void					setName(std::string newName);
			void					setTopic(std::string newTopic);
			void					setPassword(std::string password);
			void					setInviteOnly(bool value);
			void					setSecrecy(bool value);
			void					setFreeTopic(bool value);
			void					setPublicMsg(bool value);
			void					setModerated(bool value);
			void					setMaxUsers(int size);
			
			/* -- Modifiers -- */
			void					addOperator(User user, Server* server);
			void					removeOperator(User user, Server* server);
			void					addModerator(User user, Server* server);
			void					removeModerator(User user, Server* server);
			bool					addUser(User& user);
			void					removeUser(Server* server, User& user);

			/* -- Member functions -- */
			bool					isOperator(User user);
			bool					isModerator(User user);
			bool					existsUser(User user);
			std::string				getUsersString(void);
			bool					hasPassword(void) const;
			bool					checkPassword(std::string pass) const;
			bool					hasMax(void) const;
			bool					isFull(void) const;
			bool					isEmpty(void) const;
			void					sendMessageToUsers(User sender, std::string message);
	};

	/* -- Non-member functions -- */
	bool	operator== (const Channel lhs, const Channel rhs);
	void	printChannels(IRC::Channel::channels_map& channels);
	void	printChannel(Channel& channel);
}
