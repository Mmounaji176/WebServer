/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdarify <mdarify@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 16:10:18 by mdarify           #+#    #+#             */
/*   Updated: 2023/06/28 16:11:16 by mdarify          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include    "../Headers/Client.hpp"

Client::Client()
{
    if_header = 0;
    getleftdata = 0;
    length = 0;
}
Client::Client(int sock)
{
    client_socket = sock;
}

void Client::findmethod()
{
    int f = datastr.find(" ");
    method = datastr.substr(0, f);
    int d = datastr.find("/", f);
    int a = datastr.find(" ", d);    
    location = datastr.substr(d+1, a - d - 1);
    if (method == "POST")
    {
        int c = datastr.find("Content-Length: ");
        int b = datastr.find("\r\n", c);
        c += 16;
        std::string k = datastr.substr(c, b - c).c_str();
        contentlength = std::atoi(k.c_str());
    }
    size_t r = datastr.find("boundary=");
    if (r != std::string::npos)
    {
        int e = datastr.find("\r\n", r);
        r += 9;
        boundary = datastr.substr(r, e - r);
    }
}

Client::~Client()
{}
