/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Checklocation.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdarify <mdarify@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 12:44:16 by mdarify           #+#    #+#             */
/*   Updated: 2023/07/11 16:14:12 by mdarify          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include        "../Headers/location.hpp"

Location::Location()
{}

Location::Location(const Location &l)
{
	*this = l;
}

Location	&Location::operator=(const Location &l)
{
	this->cgi = l.cgi;
	this->l	  = l.l;
	return (*this);
}
Location::~Location()
{}

void	Location::set_CgiFlag(int a)
{
	cgi = a;
}

int		Location::get_CgiFlag()
{
	return cgi;
}

void	Location::setl(int l)
{
	this->l = l;
}

int Location::getl()
{
	return l;
}

std::string		Location::Key_Value(std::string &line, std::vector<std::string>& vector)
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

int Location::location_elements(const std::string &element)
{
	std::map<std::string, std::string> location_ele;

	location_ele["index"] = "";
	location_ele["auto_index"] = "";
	location_ele["root"] = "";
	location_ele["upload"] = "";
	location_ele["return"] = "";
	location_ele["upload"] = "";
	location_ele["upload_dir"] = "";
	location_ele["cgi"] = ""; // I add CGI to location elemets ;)
	if(location_ele.find(element) != location_ele.end())
		return 1;
	return 0;
}

void Location::must_fill(const std::string &root)
{
	if (loc_elements.find("root") == loc_elements.end())
		loc_elements["root"] = root;
	if (loc_elements.find("auto_index") == loc_elements.end())
		loc_elements["auto_index"] = "off";
	if (loc_elements.find("upload") == loc_elements.end())
		loc_elements["upload"] = "off";
	if (loc_elements.find("upload_dir") == loc_elements.end())
		loc_elements["upload_dir"] = "./upload";
	if (loc_elements["upload_dir"].back() != '/')
		loc_elements["upload_dir"] += "/";
	if (loc_elements["root"].back() != '/')
		loc_elements["root"] += "/";
}
