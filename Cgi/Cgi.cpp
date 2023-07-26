#include "CGI.hpp"
#define GCI_BUFFERSIZE 1024

/*--------- Constructors & Destructor --------*/

CGI::CGI(request &request, serverConfig &config) : scriptName("./cgi_bin/php-cgi")
{
	std::map<std::string, std::string> headers = request.getHeaders();
	if (headers.find("Auth-Scheme") != headers.end() && headers["Auth-Scheme"] != "")
		this->env["AUTH_TYPE"] = headers["Authorization"];
	this->env["REDIRECT_STATUS"] = "200";
	this->env["GATEWAY_INTERFACE"] = "CGI/1.1";
	this->env["REQUEST_METHOD"] = request.getMethod();
	this->env["CONTENT_LENGTH"] =  std::to_string(request.getBodyLength());
	this->env["CONTENT_TYPE"] = headers["Content-Type"];
	this->env["REMOTE_ADDR"] = to_string(config.gethostPort().host);
	this->env["QUERY_STRING"] = request.getQuery();
	this->env["REQUEST_URI"] = request.getPath() + request.getQuery();
	this->env["REMOTE_IDENT"] = headers["Authorization"];
	this->env["REMOTE_USER"] = headers["Authorization"];
	if (headers.find("Host") != headers.end())
		this->env["SERVER_NAME"] = headers["Host"];
	else
		this->env["SERVER_NAME"] = "serverName";
	this->env["SERVER_PORT"] = to_string(config.gethostPort().port);
	this->env["SERVER_PROTOCOL"] = "HTTP/1.1";
	this->env["SERVER_SOFTWARE"] = "Weebserv/1.0";
	// HTTP_*
	this->env["HTTP_COOKIE"] = headers["Cookie"];
	this->env["HTTP_ACCEPT"] = headers["Accept"];
	this->env["HTTP_ACCEPT_CHARSET"] = headers["Accept-Charset"];
	this->env["HTTP_ACCEPT_ENCODING"] = headers["Accept-Encoding"];
	this->env["HTTP_ACCEPT_LANGUAGE"] = headers["Accept-Language"];
	this->env["HTTP_CONNECTION"] = headers["Connection"];
}

CGI::CGI(CGI const &src){
	if (this != &src)
		this->env = src.env;
}

CGI::~CGI(void){
}

CGI &CGI::operator=(CGI const &src){
	if (this != &src)
		this->env = src.env;
	return *this;
}

/*--------- Member functions --------*/

char **mapToArray(std::map<std::string, std::string> env)
{
	char **env;
	int j = 0;

	try{
		env = new char *[env.size() + 1];
		for (std::map<std::string, std::string>::const_iterator i = env.begin();
			 i != env.end(); i++)
		{
			std::string line = i->first + "=" + i->second;
			env[j] = new char[line.size() + 1];
			env[j] = strcpy(env[j], line.c_str());
			j++;
		}
		env[j] = NULL;
	}
	catch (std::bad_alloc &e)
	{
		std::cerr << RED << e.what() << RESET << std::endl;
	}
	return env;
}

void deleteArray(char **env)
{
	for (size_t i = 0; env[i]; i++)
		delete[] env[i];
	delete[] env;
}

/*
	main function for executing cgi
*/

std::string CGI::executeCgi(const std::string &_filePath, size_t socket_fd, Response &response, request &request)
{
	std::fstream fileOut;	
	std::string output;
	pid_t pid;
	int savedIn;
	int savedOut;
	char **env;
	std::string filename = "./tmp/body";
	filename += to_string(socket_fd);
	std::fstream _body;
	std::string myline;
	char **argv = new char *[3];
	argv[0] = new char[scriptName.size() + 1];
	argv[0] = strcpy(argv[0], scriptName.c_str());
	argv[1] = new char[_filePath.size() + 1];
	argv[1] = strcpy(argv[1], _filePath.c_str());
	argv[2] = NULL;

	this->env["PATH_INFO"] = _filePath;
	this->env["PATH_TRANSLATED"] = _filePath;
	env = mapToArray(this->env);

	savedIn = dup(STDIN_FILENO);
	savedOut = dup(STDOUT_FILENO);

	response.get_file_change_get() = randomFileName();
	int fdIn = open(filename.c_str(), O_RDWR | O_CREAT, 0666);
	int fdOut = open(response.get_file_change_get().c_str(),  O_RDWR | O_CREAT, 0666);

	lseek(fdIn, 0, SEEK_SET);
	pid = fork();
	if (pid == -1){
		std::cerr << "Fork Error" << std::endl;
		response.set_status_code(500);
	}
	else if (pid == 0){
		dup2(fdIn, STDIN_FILENO);
		dup2(fdOut, STDOUT_FILENO);
		execve(scriptName.c_str(), argv, env);
		response.set_status_code(500);
	}
	else
	{
		char buffer[GCI_BUFFERSIZE] = {0};
		int ret = 1;

		waitpid(pid, NULL, 0);
		lseek(fdOut, 0, SEEK_SET);
		while (ret > 0)
		{
			memset(buffer, 0, GCI_BUFFERSIZE);
			ret = read(fdOut, buffer, GCI_BUFFERSIZE - 1);
			output += buffer;
		}
		dup2(savedIn, STDIN_FILENO);
		dup2(savedOut, STDOUT_FILENO);
	}
	fileOut.close();
	close(fdIn);
	close(fdOut);
	close(savedIn);
	close(savedOut);
	deleteArray(env);
	return addHeader(socket_fd, output, response, request);
}


std::string CGI::addHeader(size_t socket_fd, const std::string &responseData, Response &response, request &request) {
    // Compose the HTTP response message, including the response headers
    std::string httpResponse;

    // Add the status line (HTTP version and status code)
    httpResponse += "HTTP/1.1 " + std::to_string(response.get_status_code()) + " " + response.get_status_message() + "\r\n";

    // Add other necessary headers (e.g., Content-Type, Content-Length, etc.) to the response
    httpResponse += "Content-Type: " + response.get_content_type() + "\r\n";
    httpResponse += "Content-Length: " + std::to_string(responseData.size()) + "\r\n";

    // Add any additional custom headers as needed based on the response and request objects

    // Add a blank line to separate the headers from the response body
    httpResponse += "\r\n";

    // Add the response data to the HTTP response message
    httpResponse += responseData;

    // Send the composed HTTP response back to the client using the socket_fd
    send(socket_fd, httpResponse.c_str(), httpResponse.size(), 0);

    // Return the complete HTTP response for logging or other purposes (not necessary for actual response sending)
    return httpResponse;
}
