/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdarify <mdarify@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 13:16:33 by mdarify           #+#    #+#             */
/*   Updated: 2023/07/23 09:55:47 by mdarify          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef             CONFIG_HPP
#define             CONFIG_HPP

#include                "webserv.hpp"
#include                "Server.hpp"
#include                "location.hpp"
#include                <vector>
#include                <utility>
#include                <string>
#include                <map>
#include                <string>
#include                <sstream>
#include                <fstream>
#include                <iostream>
#include                <dirent.h>
#include                <unistd.h>
#include                <algorithm>
#include                <sys/types.h>
#include                <sys/socket.h>
#include                <fcntl.h>
#include                <netinet/in.h>
#include                <arpa/inet.h>
#include                <cstring>
#include                <cstdlib>
#include                <netdb.h>



class               Config
{
                    private: 
                            Config(const    Config      &OBJConfig);
                            Config  &operator=(const    Config      &OBJConfig);
                    public:
                            Config();
                            ~Config();
                            void        configfile(std::string  conf_file);
                            std::string	Key_Value(std::string &line, std::vector<std::string>& vector);
                            std::vector<Server>         Serv;
                            std::vector<std::string>    Port;
                            void        Content_Configfile();
};

#endif