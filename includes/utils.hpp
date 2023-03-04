/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karisti- <karisti-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 10:31:29 by karisti-          #+#    #+#             */
/*   Updated: 2023/02/28 14:06:05 by karisti-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <csignal>
#include <signal.h>


extern bool socketKiller;


std::vector<std::string>	splitString(std::string str, std::string delim, int count = -1);
void						catchSignal(void);
void						sig_handle(int sig);
std::string					pingGenerator(const int &size);
std::string					getCurrentTimestamp();
bool						isNumber(std::string str);
void						printStrVector(std::string name, std::vector<std::string> strs);
