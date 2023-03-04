/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karisti- <karisti-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 16:55:46 by karisti-          #+#    #+#             */
/*   Updated: 2023/03/04 22:17:32 by karisti-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include "Channel.hpp"


namespace IRC
{
	class Channel;
	
	class User
	{
		public:
			typedef std::map<std::string, Channel>	channels_map;
			typedef std::map<int, User>				users_map;
			
		private:
			sockaddr_in							address;
			int									socket;
			std::string							password;
			std::string							nick;
			std::string							user;
			std::string							realname;
			std::string							hostname;
			std::string							servername;
			bool								authenticated;
			bool								pingReq;
			std::string							pingKey;
			time_t								timeout;
			channels_map						joinedChannels;
			channels_map						invitedChannels;
			std::string							buffer;

		public:
			User();
			User(const User& other);
			~User();
			User &operator=(const User &other);
			
			/* -- Getters -- */
			sockaddr_in							getAddress(void) const;
			int									getSocket(void) const;
			std::string							getPassword(void) const;
			std::string							getNick(void) const;
			std::string							getUser(void) const;
			std::string							getRealname(void) const;
			std::string							getHostname(void) const;
			std::string							getServername(void) const;
			bool								isAuthenticated(void) const;
			bool								isPinged(void) const;
			std::string							getPingKey(void) const;
			time_t								getTimeout(void) const;
			channels_map&						getJoinedChannels(void);
			channels_map&						getinvitedChannels(void);
			std::string							getBuffer(void) const;
			
			/* -- Setters -- */
			void					setPassword(std::string password);
			void					setNick(std::string nick);
			void					setUser(std::string user);
			void					setRealname(std::string realname);
			void					setHostname(std::string realname);
			void					setServername(std::string realname);
			void					setAuthenticated(bool value);
			void					setPingRequest(bool req);
			void					setPingKey(std::string key);
			void					setTimeout(time_t timeout);

			/* -- Modifiers -- */
			void					addJoinedChannel(Channel& channel);
			void					removeJoinedChannel(Channel channel);
			void					addInvitedChannel(Channel& channel);
			void					removeInvitedChannel(Channel channel);
			void					appendBuffer(std::string str);
			void					clearBuffer(void);

			/* -- Member functions -- */
			void					startListeningSocket(int serverSocket);
			std::string				getJoinedChannelsString(void) const;
			bool					isInChannel(Channel channel) const;
			bool					isInvitedToChannel(Channel channel) const;
			void					sendMessage(std::string message) const;
	};
	
	/* -- Non-member functions -- */
	bool								operator== (const User lhs, const User rhs);
	IRC::User::users_map::iterator		findUserByNick(IRC::User::users_map& users, std::string nick);
	void								printUser(User user);
	void								printUsers(IRC::User::users_map users);
}
