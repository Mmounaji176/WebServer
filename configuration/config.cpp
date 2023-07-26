/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdarify <mdarify@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 11:46:55 by mdarify           #+#    #+#             */
/*   Updated: 2023/07/23 10:03:37 by mdarify          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include        "../Headers/config.hpp"
#include		"../Headers/location.hpp"

void	Check_errors2(std::string keyword, std::string	Error_msg, int exit_code)
{
	std::cerr << keyword << " "	<< Error_msg << std::endl;
	exit(exit_code);
}

void	Config::Content_Configfile()
{
	for (std::vector<Server>::iterator  it  = Serv.begin(); it != Serv.end(); it++)
	{
		it->Content_Configfile();
		Port.push_back(it->get_element("listen"));
	}
	std::vector<std::string> Vport;
	for (unsigned int l = 0; l < Port.size(); l++)
	{
		Vport.push_back(Port[l]);
	}
	for (unsigned int l = 0; l < Vport.size(); l++)
	{
		if (Vport[l] == Vport[l + 1])
		{
			std::cout 	<< "ERROR ---> : THE BOTH SERVER HAVE THE SAME VALUE " 
						<< Vport[l]	<< " && " << Vport[l + 1]	<< std::endl;
					exit(1);
		}
		else
			continue;
	}
}

void        Config::configfile(std::string      conf_file)
{
    std::ifstream   infile(conf_file);
    if (!infile)
		Check_errors2(conf_file, " file doesn't exist", 1);
    std::string     			line;
    std::string     			word;
	std::vector<std::string>	Vstring;
	Config						Conf;
    Server  Sr;
    while (std::getline(infile, line))
    {
		Sr.elements.clear();
		Sr.Error_page.clear();
		Sr.location.clear();
        word = Conf.Key_Value(line, Vstring);
        if (word  ==  "Server")
        {
            if (Vstring.size() > 0)
				Check_errors2(word, "ERROR --> : WRONG BLOCK SERVER", 1);
            else
                Serv.push_back(Sr.Content_Server(infile, line));
        }
		else
			Check_errors2(word, "ERROR ---> : THE CONFIG FILE DOESN'T START WITH SERVERNAME", 1);
    }
	if (!Serv.size())
		Check_errors2("Server :"," conf file must have at least 1 server block", 1);
	Content_Configfile();
}
