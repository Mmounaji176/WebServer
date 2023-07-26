/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdarify <mdarify@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 16:14:17 by mdarify           #+#    #+#             */
/*   Updated: 2023/07/20 18:36:20 by mdarify          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include    "../Headers/Server.hpp"
#include    "../Headers/location.hpp"
#include	"../Headers/config.hpp"

Server::Server()
{}

Server::Server(std::string serv, std::string port)
{
    servername = serv;
    this->port = port;
}

Server::~Server()
{}

void    Server::CreateSocket()
{
	Config		Conf;
	Server		Serv;
    memset(&hint, 0, sizeof(hint));
    hint.ai_family = AF_INET;
    hint.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(servername.c_str(), port.c_str(), &hint, &res) != 0)
    {
        std::cerr << "get addrinfo error !" << std::endl;
        exit(1);
    }
    server_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (bind(server_socket, res->ai_addr, res->ai_addrlen) == -1)
    {
        std::cerr << "Couldn't perform binding !" << std::endl;
        close(server_socket);
    }

    if (listen(server_socket, 400) == -1)
    {
        std::cerr << "Couldn't perfom listening with the Socket !" << std::endl;
        close(server_socket);
    }
}

int	Server::find_element(std::string key)
{
	if (elements.find(key) == elements.end())
		return 1;
	return 0;
}

void	Server::set_element(std::string key, std::string &value)
{
	elements[key] = value;
}

std::string Server::get_element(std::string key)
{
	return(elements[key]);
}

void Server::Content_Configfile()
{
	if (elements.find("root") == elements.end())
		elements["root"] = "/";
	if (elements.find("listen") == elements.end())
		elements["listen"] = "8080";
	if (elements.find("max_body_size") == elements.end())
		elements["max_body_size"] = "";
	if (!(std::all_of(elements["max_body_size"].begin(), elements["max_body_size"].end(), ::isdigit)))
	{
		std::cerr << "the max_body_size must be digits only " << elements["max_body_size"] << std::endl;
		exit(1);
	}
	for (std::map<std::string, Location>::iterator it = location.begin(); it != location.end(); it++)
		it->second.must_fill(elements["root"]);
	if (!(std::all_of(elements["listen"].begin(), elements["listen"].end(), ::isdigit)))
	{
		std::cerr << "Listen  must be digits only" <<  elements["listen"] << std::endl;
		exit(1);
	}
}
