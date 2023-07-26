/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdarify <mdarify@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 16:13:58 by mdarify           #+#    #+#             */
/*   Updated: 2023/07/21 10:22:58 by mdarify          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include    <sys/types.h>
#include    <sys/socket.h>
#include    <netdb.h>
#include    <vector>
#include    <iostream>
#include    <string>
#include    <cstdlib>
#include    <string.h>
#include    <unistd.h>
#include    <dirent.h>
#include    <map>
#include    "location.hpp"

class   Location;

class Server
{
    public :
            std::map<std::string, std::string>  elements;
            std::map<std::string, std::string>  Error_page;
            std::map<std::string, Location>     location;

            
            std::string     servername;
            std::string     port;
            int             server_socket;
            struct addrinfo hint;
            struct addrinfo *res;
            Server();
            Server(std::string serv, std::string port);
            ~Server();
            

            
            void                            Content_Configfile();
            bool                            ServerContent(std::string element);
            void                            CreateSocket();
		    std::string 					get_element(std::string key);
            int						        find_element(std::string key);
		    void					        set_element(std::string key, std::string &value);
            Server					        Content_Server(std::ifstream &infile, std::string &line);
};
