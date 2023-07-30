#ifndef CGI_H
#define CGI_H

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <map>
#include <string>
#include "../request/request.hpp"
#include "../respond/respond.hpp"

class Respond;
class request;

std::string run_cgi(request &r,  Respond &res, std::vector<server> server);
std::map<std::string, std::string> get_env(char *file, request req);


#endif