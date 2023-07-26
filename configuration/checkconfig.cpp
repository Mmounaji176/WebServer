/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkconfig.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdarify <mdarify@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 09:35:08 by mdarify           #+#    #+#             */
/*   Updated: 2023/07/25 15:48:51 by mdarify          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include        "../Headers/config.hpp"
#include		"../Headers/location.hpp"

Config::Config()
{}

void	Check_errors(std::string keyword, std::string	Error_msg, int exit_code)
{
	std::cerr << keyword << " "	<< Error_msg << std::endl;
	exit(exit_code);
}

std::string	Config::Key_Value(std::string &line, std::vector<std::string>& vector)
{
	std::string::iterator it = line.begin();
	std::string word;
	std::string key;
	int flag = 0;

	vector.clear();
	while (it != line.end())
	{
		word.clear();
		while ((*it == ' ' || *it == '\t') && it != line.end())
			it++;
		while ((*it != ' ' && *it != '\t') && it != line.end())
		{
			if (*it)
				word += (*it);
			it++;
		}
		if (word != "" && flag == 0)
		{
			key = word;
			flag = 1;
		}
		else if (word != "")
			vector.push_back(word);
	}
	return key;
}

bool Server::ServerContent(std::string element)
{
	std::map<std::string, std::string> Content_Server;
	Content_Server["index"] = "";
	Content_Server["Error_page"] = "";
	Content_Server["max_body_size"] = "";
	Content_Server["root"] = "";
	Content_Server["servername"] = "";
	Content_Server["listen"] = "";
	Content_Server["upload"] = "";
	if(Content_Server.find(element) != Content_Server.end())
		return 1;
	return 0;
}

Server      Server::Content_Server(std::ifstream   &infile, std::string &line)
{
	Config		Conf;
    std::string Keyword;
	std::vector<std::string> vector;
	std::getline(infile, line);
	Location Klocat;
	Keyword = Conf.Key_Value(line, vector);
	if (Keyword != "{" || vector.size() == 1)
		Check_errors(Keyword, "ERROR --> : block server doesn't start with {", 1);
	while (std::getline(infile, line))
	{
		Keyword = Conf.Key_Value(line, vector);
		Klocat.loc_elements.clear();
		Klocat.loc_elements.clear();
		if (Keyword == "Error_page")
		{
			if (vector.size() != 2 || vector[0].find_first_not_of("0123456789") != std::string::npos)
					Check_errors(Keyword, "arguments of error page are invalid", 1);
			std::ifstream inpfile(vector[1]);
			if (!inpfile || inpfile.fail())
				Check_errors(vector[1], "invalid infile", 1);
			Error_page[vector[0]] = vector[1];
		}
		else if (ServerContent(Keyword) == 1 && vector.size() == 1 )
		{
			if (elements.find(Keyword) != elements.end())
				Check_errors(Keyword, " is already presented in this block ", 1);
			elements[Keyword] = vector[0];
			std::cout << "-->" << elements[Keyword] << std::endl;
		}
		else if (Keyword == "location")
		{
				if (vector.size() != 1)
					Check_errors(Keyword, "location block is wrong", 1);
				else
                {
					if(vector[0].back() != '/')
						vector[0] += '/';
					Klocat.location_fill(infile, line, vector[0]);
					std::cout << vector[0] << std::endl;
					location[vector[0]] = Klocat;
					
				}
		}
		else if (Keyword == "}" && vector.size() == 0)
			break;
		else
			Check_errors(Keyword, "unvalid element in the server block", 1);
	}
	if (Keyword != "}")
		Check_errors(Keyword, "missing ending for the server block", 1);
	return *this;
}

Config::~Config()
{}
