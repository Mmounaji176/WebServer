/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdarify <mdarify@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 16:43:15 by mdarify           #+#    #+#             */
/*   Updated: 2023/07/23 10:11:16 by mdarify          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include        "Headers/webserv.hpp"
#include		"Headers/config.hpp"


int main(int argc, char		*argv[])
{
	if (argc != 2)
	{
		std::cerr << "ERROR ---> : INVALID NUMBER OF ARGUMENTS" << std::endl;
		exit(1);
	}
	Config		Configrat;
	
	Configrat.configfile(argv[1]);
    Webserv     webserv;
    std::vector<Server> servers;
    std::vector<Client> clients;
    servers.push_back(Server("localhost", "8080"));
    servers.push_back(Server("localhost", "9090"));
    webserv.createConnections(servers);
    fd_set  rd;
    fd_set  wr;
    while (1)
    {
        FD_ZERO(&rd);
        FD_ZERO(&wr);
        int     MaxSocket   = -1;
        std::vector<Server>::iterator   IT  = servers.begin();
        while (IT != servers.end())
		{
			FD_SET((*IT).server_socket, &rd);
			if ((*IT).server_socket > MaxSocket)
				MaxSocket = (*IT).server_socket;
			IT++;
		}
       // ADD NEW CLIENT TO THE LIST
       std::vector<Client>::iterator    IT1 =   clients.begin();
       while (IT1 != clients.end())
       {
            FD_SET((*IT1).client_socket, &rd);
            FD_SET((*IT1).client_socket, &wr);
            if ((*IT1).client_socket > MaxSocket)
                MaxSocket = (*IT1).client_socket;
            IT1++;
       }
       if (select(MaxSocket + 1, &rd, &wr, NULL, NULL) == -1)
       {
            std::cerr   << "ERROR ----> : IN SELECT"  << std::endl;
            exit(1);
       }
       else
       {
            int l = 0;
            IT  = servers.begin();
            while (IT != servers.end())
			{
				if (FD_ISSET((*IT).server_socket, &rd))
				{
					Client newClient;
					newClient.client_socket = accept((*IT).server_socket, NULL,
							NULL);
					if (newClient.client_socket == -1)
					{
						std::cerr << "Failed to accept client connection" << std::endl;
						continue ;
					}
					newClient.server_id = l;
					clients.push_back(newClient);
					std::cout << "Hello client number "
								<< newClient.client_socket << " from "
								<< servers[newClient.server_id].servername << ":" 
                                << servers[newClient.server_id].port << std::endl;
				}
				IT++;
				l++;
			}
            IT1 = clients.begin();
            while (IT1 != clients.end())
            {
                if (FD_ISSET((*IT1).client_socket, &rd))
                {
                    int r = read((*IT1).client_socket, (*IT1).data, sizeof((*IT1).data));
                    if (r <= 0)
                    {
						std::cerr   << "An error has occured during reading request from a client" 
                                    << std::endl;
						close((*IT1).client_socket);
						(*IT1).client_socket = -1;
						if (IT1 != clients.end())
						{
							clients.erase(IT1);
							IT1--;
						}
					}
                    else
                    {
                        (*IT1).data[r] = '\0';
						if ((*IT1).method.empty())
							(*IT1).datastr += (*IT1).data;
                        else
                        {
							// std::cout << "Hello from inside" << std::endl;
							(*IT1).length += r;
							if ((*IT1).length + 1 > (*IT1).contentlength)
							{
								//std::cout << "Hello from outside" << std::endl;
								int x = (*IT1).contentlength - ((*IT1).length
										- r);
								(*IT1).data[x] = '\0';
								(*IT1).response += std::string((*IT1).data, x);
								(*IT1).if_header = 1;
								size_t t = (*IT1).response.find("filename=\"");
								if (t != std::string::npos)
								{
									t += 10;
									int p = (*IT1).response.find("\"", t);
									(*IT1).filename = (*IT1).response.substr(t,
											p - t);
								}
								int u = (*IT1).response.find("\r\n\r\n");
								u += 4;
								std::cout << "Content : " << (*IT1).contentlength << std::endl;
								std::cout << "Content : " << (*IT1).length << std::endl;
								std::cout << "Content : " << sizeof((*IT1).response) << std::endl;
								(*IT1).length -= u;
								webserv.fillFile((*IT1).boundary, (*IT1).length,
										(*IT1).filename,
										(*IT1).response.substr(u,
											(*IT1).response.length() - u));
								//std::cout << (*IT1).response << std::endl;
								//exit(6);
							}
							else
							{
								(*IT1).data[r] = '\0';
								(*IT1).response += std::string((*IT1).data, r);
							}
						}
                        memset(&(*IT1).data, 0, sizeof((*IT1).data));
						if ((*IT1).if_header)
						{
							/*std::cout << "<<<" << (*IT1).response << ">>>" << std::endl;
                            std::cout << "<<<" << (*IT1).datastr << ">>>" << std::endl;
                            std::cout << "filename : " << (*IT1).filename << std::endl;
                            std::cout << "boundary : " << (*IT1).boundary << std::endl;*/
						}
						size_t f = (*IT1).datastr.find("\r\n\r\n");
						if (f != std::string::npos)
						{
							(*IT1).findmethod();
							if ((*IT1).method != "POST")
							{
								(*IT1).if_header = 1;
								/*(*IT1).file.open((*IT1).method.c_str());
                                if (!(*IT1).file.is_open())
                                    std::cerr << "Failed to create a file." << std::endl;*/
							}
							else
							{
								if (!(*IT1).getleftdata)
								{
									f += 4;
									int d = (*IT1).datastr.length() - f;
									(*IT1).response = (*IT1).datastr.substr(f,
											d);
									(*IT1).datastr = (*IT1).datastr.substr(0,
											(*IT1).datastr.length()
											- ((*IT1).datastr.length() - f));
									(*IT1).length += d;
									(*IT1).getleftdata = 1;
									std::cout << "Hoi" << std::endl;
								}
							}
						}
                    }
                }
                else if (FD_ISSET((*IT1).client_socket, &wr)
						&& (*IT1).if_header)
				{
					std::cout << (*IT1).location << std::endl;
					std::cout << (*IT1).method << std::endl;
					std::string responseBody;
					if (webserv.checkFileExistence((*IT1).location))
						webserv.readFromFile((*IT1).location, responseBody);
					else
						std::cout << "Sadge <" << (*IT1).location << ">" << std::endl;
					std::string responseHeader = "HTTP/1.1 200 OK\r\n";
					responseHeader += "Content-Type: text/html\r\n";
					responseHeader += "Content-Length: "
						+ webserv.intToString(responseBody.length()) + "\r\n";
					responseHeader += "\r\n";
					responseHeader += responseBody;
					ssize_t bytesSent = send((*IT1).client_socket,
							responseHeader.c_str(), responseHeader.length(),
							0);
					if (bytesSent == -1)
					{
						std::cerr << "Failed to send response header." << std::endl;
					}
					else
					{
						std::cout << "Sent " << bytesSent << " bytes of response header." << std::endl;
					}
					std::cout << (*IT1).client_socket << "is writable" << std::endl;
					close((*IT1).client_socket);
					(*IT1).client_socket = -1;
					if (IT1 != clients.end())
					{
						clients.erase(IT1);
						IT1--;
					}
                }
                    IT1++;
                    
            }
       }
    }
    return (1);
}