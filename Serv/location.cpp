 /* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdarify <mdarify@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 16:50:20 by mdarify           #+#    #+#             */
/*   Updated: 2023/07/11 17:19:56 by mdarify          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include    "../Headers/location.hpp"

void	Check_errors_location(std::string keyword, std::string	Error_msg, int exit_code)
{
	std::cerr <<  keyword << " " << Error_msg << std::endl;
	exit(exit_code);
}

void	Location::ValidMethode()
{
	for (std::vector<std::string>::iterator Mit = allowedM.begin(); Mit != allowedM.end(); Mit++)
	{
		if ((*Mit) != "GET" && (*Mit) != "POST" && (*Mit) != "DELETE")
		{	
			std::cerr << (*Mit) << " Unvalid method in the Configuration file" << std::endl;
				exit(1);
		}
	}
}

void	Location::ValidRPATH()
{
	Location	Valid;
	const char	*path;

	path = loc_elements["root"].c_str();
	if (path != nullptr)
	{
		DIR* directory = opendir(path);
		if (directory == nullptr)
		{
			std::cout << "ERROR ----> : " << path << ": INVALID PATH" << std::endl; 
			closedir(directory);
			exit(1);
		}
		else

			loc_elements["root"] = path;
	}
}

void	Location::ValidCgi()
{
	std::string Error = "Error : ";
	if (!loc_elements["cgi"].empty()) 
	{
		if (loc_elements["cgi"] == "on" || loc_elements["cgi"] == "off")
		{
			if (loc_elements["cgi"] == "on")
				set_CgiFlag(1);
			else if (loc_elements["cgi"] == "off")
				set_CgiFlag(0);
		}
		else
			Check_errors_location(loc_elements["cgi"], " is invalid cgi option", 1);
	}
	else 
		Check_errors_location(loc_elements["cgi"], "cgi don't have value", 1);
}

void	Location::ValidUPload()
{
	std::string		Upload;
	Upload	=	loc_elements["upload"];
	if (!loc_elements["upload"].empty())
	{
		if (Upload == "on" || Upload == "off")
			return ;
		else
		{
			std::cout << "ERROR ---> : " << Upload << "invalid upload setting" << std::endl;
			exit(1);
		}
	}
}

void	Location::ValidParsingConfig()
{
	ValidMethode();
	ValidRPATH();
	ValidCgi();
	ValidUPload();
}

void	Location::location_fill(std::ifstream &infile, std::string &line, std::string &name)
{
	std::vector<std::string>	vector;
	std::string word;
	int allowed;

	allowed = 0;
	loc_elements["name"] = name;
	std::getline(infile, line);
	word = Key_Value(line,vector);
	if (word != "{" || vector.size() >= 1)
		Check_errors_location(word, "the location block don't start with {", 1);
	while (std::getline(infile, line))
	{
		word = Key_Value(line, vector);
		if (word == "allowedMethods")
		{
			if (allowed == 1)
				Check_errors_location(word, " is already presented in this block", 1);
			allowedM = vector;
			allowed = 1;
		}
		else if (location_elements(word) && vector.size() == 1)
		{
			if (loc_elements.find(word) != loc_elements.end())
				Check_errors_location(word, " is already presented in this block", 1);
			loc_elements[word] = vector[0];
			std::cout << loc_elements[word] << std::endl;
		}
		else if (word == "}" && vector.size() == 0)
			break;
		else
			Check_errors_location(word, " unvalid element  in the location block", 1);
	}
	if (word != "}")
		Check_errors_location(word, "missing ending for the location block", 1);
	ValidParsingConfig();
}
