/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdarify <mdarify@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 12:54:47 by mdarify           #+#    #+#             */
/*   Updated: 2023/07/13 12:56:06 by mdarify          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma         once


#include        "config.hpp"
#include        "Client.hpp"
#include        "Server.hpp"

#include <csignal>
#include <fstream>
#include <sstream>
#include <vector>

class           Client;
class           Server;

class          Webserv
{
                public:
                    Webserv();
                    // Webserv(const    Webserv &l){};
                    // Webserv     &operator=(const    Webserv &l){};
                    ~Webserv();
                    void    check_inputfile(std::ifstream inputfile, std::vector<std::string> vector);
                    void    Vector_size(std::vector<std::string> vector);
                    void	removeLastLine(std::string boundary, int &contentlength, std::string &str);                  
                    bool	checkFileExistence(std::string str);
                    void	fillFile(std::string boundary, int contentlength, std::string &filename, std::string content);
                    std::string intToString(int number);
                    void	readFromFile(std::string file, std::string &str);
                    void	createConnections(std::vector<Server> &servers);
};