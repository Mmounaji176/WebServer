
#ifndef __CGI_HPP__
#define __CGI_HPP__

#include <iostream>
#include <string>
#include <map>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

class Response;
class request;

class CGI
{
	private:
		std::map<std::string, std::string>	env;
		std::string							filePath;
		std::string							scriptName;
	public:
		CGI(CGI const &);
		~CGI(void);
		CGI	&operator=(CGI const &src);
		CGI( request &request,  serverConfig &server);
		std::string		executeCgi(const std::string&, size_t, Response&, request &);
		std::string		getTheCompletePath(const std::string&);
		std::string		addHeader(int fd, std::string, Response &, request &);
};

#endif