/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdarify <mdarify@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 12:57:43 by mdarify           #+#    #+#             */
/*   Updated: 2023/07/08 20:06:53 by mdarify          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include            "webserv.hpp"

Webserv::Webserv()
{}

Webserv::~Webserv()
{}

void    Webserv::removeLastLine(std::string boundary, int &contentlength,std::string &str)
{
    std::size_t f = str.find_last_of(boundary);
	if (f != std::string::npos)
	{
		std::size_t d = str.find_last_of("\r\n", f);

		if (d != std::string::npos)
		{
			contentlength -= str.length() - d;
			str = str.substr(0, str.length() - d);
		}
	}
}

bool    Webserv::checkFileExistence(std::string str)
{
	return (access(str.c_str(), F_OK) == 0);
}

void    Webserv::fillFile(std::string boundary, int contentlength, std::string &filename,
		std::string content)
{
	// std::stringstream ss;
	removeLastLine(boundary, contentlength, content);
	//removeLastLine(contentlength, content);
	std::cout << "<<" << content << "     xdlol>>" << std::endl;
	std::cout << "content size : " << contentlength << std::endl;
	std::cout << "File size : " << content.size() << std::endl;
	//ss << content;
	std::ofstream file(filename.c_str(), std::ios::binary);
	if (file.is_open())
	{
		file.write(content.c_str(), contentlength);
		/*std::string line;
        while (std::getline(ss, line)
		&& line.find(boundary) == std::string::npos)
            file << line;*/
	}
}

std::string     Webserv::intToString(int number)
{
	std::stringstream ss;
	ss << number;
	std::string strNumber = ss.str();
	return (strNumber);
}


void	Webserv::check_inputfile(std::ifstream	inputfile, std::vector<std::string> vector)
{
	if (!inputfile || inputfile.fail() || vector.size() != 2)
	{
		std::cerr << "ERROR ---> : INVALID INPUT FILE" << std::endl;
		exit(1);
	}
}

void    Webserv::readFromFile(std::string   file, std::string   &str)
{
    std::ifstream inputFile(file.c_str());
	if (!inputFile)
	{
		std::cerr << "Failed to open the file." << std::endl;
		return ;
	}
	std::string line;
	while (std::getline(inputFile, line))
		str += line + "\n";
	inputFile.close();
}

void    Webserv::createConnections(std::vector<Server> &servers)
{
	std::vector<Server>::iterator it = servers.begin();
	while (it != servers.end())
	{
		(*it).CreateSocket();
		it++;
	}
}

