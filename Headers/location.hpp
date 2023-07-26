/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdarify <mdarify@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 13:23:56 by mdarify           #+#    #+#             */
/*   Updated: 2023/07/11 16:24:59 by mdarify          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef     LOCATION_HPP
#define     LOCATION_HPP

#include        <iostream>
#include		<fstream>
#include        <vector>
#include        <map>
#include		<dirent.h>

class       Location
{
            private:
					int cgi;
					int	l;
            public:
					Location();
					Location(const Location &l);
					Location &operator=(const Location &l);      
					~Location();
					void	ValidMethode();
					void	ValidRPATH();
					void	ValidCgi();
					void	ValidUPload();
					std::map<std::string, std::string>      loc_elements;     
					std::vector<std::string>                allowedM;
					void	set_CgiFlag(int a);
					int		get_CgiFlag();
					void	setl(int l);
					int 	getl();
					int location_elements(const std::string &element);
					std::string	Key_Value(std::string &line, std::vector<std::string>& vector);
					void    	location_fill(std::ifstream &ifs, std::string &line, std::string &name);
					void  must_fill(const std::string &root);
					void	ValidParsingConfig();
};


#endif
