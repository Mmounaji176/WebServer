#include "cgi.hpp"

std::map<std::string, std::string> Cgi::get_env(char *file, request &req)
{
    std::map<std::string, std::string> env;

    env["REQUEST_METHOD"] = req.get_method();
    env["CONTENT_TYPE"] = req.get_header("Content-Type");
    env["CONTENT_LENGTH"] = std::to_string(req.get_body().size());
    env["QUERY_STRING"] = req.get_query();
    env["REDIRECT_STATUS"] = "200";
    env["SCRIPT_FILENAME"] = file;
    return env;
}


std::string Cgi::run_cgi(request &r, Respond &res, std::vector<server> server)
{
    // Extract the CGI file path and path info from the Respond object
    char *file, *path;
    file = strdup(res.get_file_cgi().c_str());
    path = strdup(res.get_path_info_founded().c_str());

    // Prepare the command to execute the CGI script
    char *cmd[3] = {path, file, NULL};

    // Create temporary file descriptors to capture CGI script output
    std::string cgi_str;
    FILE *temp = std::tmpfile();
    FILE *temp1 = std::tmpfile();
    int fdtemp = fileno(temp);
    int fdtemp1 = fileno(temp1);

    // Get the environment variables for the CGI script
    std::map<std::string, std::string> env = get_env(file, r);
    std::string method = env["REQUEST_METHOD"];

    // Prepare the environment variables as an array of C strings
    char **envp = (char **)malloc(sizeof(char *) * (env.size() + 1));
    if (!envp)
    {
        res.handle_error_response(server, 500);
        free(file);
        free(path);
        return res.get_response_status(res.get_status_code());
    }
    int i = 0;
    for (std::map<std::string, std::string>::iterator it = env.begin(); it != env.end(); it++)
    {
        std::string e = it->first + "=" + it->second;
        envp[i++] = strdup(e.c_str());
    }
    envp[i] = NULL;

    // Fork a child process to execute the CGI script
    int pid = fork();
    if (pid == -1)
    {
        res.handle_error_response(server, 500);
        free_all(file, path, envp, env.size());
        return "";
    }
    else if (pid == 0)
    {
        // Child process (execute the CGI script)

        // If the request method is POST, redirect the standard input to read from the temp1 file descriptor
        if (method == "POST")
        {
            if (dup2(fdtemp1, STDIN_FILENO) == -1)
            {
                res.handle_error_response(server, 500);
                free_all(file, path, envp, env.size());
                return "";
            }
            write(fdtemp1, r.get_body().c_str(), r.get_body().size());
            std::rewind(temp1);
        }

        // Redirect the standard output to write to the temp file descriptor
        if (dup2(fdtemp, STDOUT_FILENO) == -1)
        {
            res.handle_error_response(server, 500);
            free_all(file, path, envp, env.size());
            return "";
        }

        // Set an alarm to limit the execution time of the CGI script (2 seconds in this case)
        alarm(2);

        // Execute the CGI script using execve
        execve(cmd[0], cmd, envp);

        // If the execution reaches this point, there was an error executing the CGI script
        exit(1);
    }

    // Parent process (web server)

    // Wait for the child process to complete
    int status;
    waitpid(pid, &status, 0);

    // Check if the child process was terminated by a signal or if it exited with a non-zero status (indicating an error)
    if (WIFSIGNALED(status) || status != 0)
    {
        res.handle_error_response(server, 500);
        free_all(file, path, envp, env.size());
        return "";
    }

    // Read the output generated by the CGI script from the temp file descriptor and store it in the 'content' string
    char buf[1];
    int byt;
    std::string content;
    std::rewind(temp);
    while ((byt = read(fdtemp, buf, 1)) > 0)
        content.append(buf, 1);

    // Check for errors while reading the output
    if (byt == -1)
    {
        res.set_status_code(500);
        res.set_response_body(res.get_response_status(res.get_status_code()));
        free_all(file, path, envp, env.size());
        return res.get_response_status(res.get_status_code());
    }

    // Close the temporary file descriptors and free allocated memory
    close(fdtemp);
    close(fdtemp1);
    free_all(file, path, envp, env.size());

    // Process the CGI script output to set response headers and body
    set_headers_cgi(content, res);

    // Return the CGI script output as a string
    return content;
}

bool isCGIRequest(const std::string& url) {
    // Check if the URL ends with the ".cgi" extension
    return url.length() >= 4 && url.substr(url.length() - 4) == ".cgi";
}
