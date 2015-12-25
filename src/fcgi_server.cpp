#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include "fcgio.h"
#include "backend_connection.h"

using namespace std;

// Maximum bytes
const unsigned long STDIN_MAX = 1000000;

string get_request_content(const FCGX_Request & request) {
    char * content_length_str = FCGX_GetParam("CONTENT_LENGTH", request.envp);
    unsigned long content_length = STDIN_MAX;

    if (content_length_str)
    {
        content_length = strtol(content_length_str, &content_length_str, 10);
        if (*content_length_str)
        {
            cerr << "Can't Parse 'CONTENT_LENGTH='"
                 << FCGX_GetParam("CONTENT_LENGTH", request.envp)
                 << "'. Consuming stdin up to " << STDIN_MAX << endl;
        }
        if (content_length > STDIN_MAX)
        {
            content_length = STDIN_MAX;
        }
    }
    else
    {
        // Do not read from stdin if CONTENT_LENGTH is missing
        content_length = 0;
    }

    char * content_buffer = new char[content_length];
    cin.read(content_buffer, content_length);
    content_length = cin.gcount();

    // Chew up any remaining stdin - this shouldn't be necessary
    // but is because mod_fastcgi doesn't handle it correctly.

    // ignore() doesn't set the eof bit in some versions of glibc++
    // so use gcount() instead of eof()...
    do cin.ignore(1024); while (cin.gcount() == 1024);

    string content(content_buffer, content_length);
    delete [] content_buffer;
    return content;
}

string get_request_cookie(const FCGX_Request & request)
{
    char * cookie_str = FCGX_GetParam("HTTP_COOKIE", request.envp);
    if (cookie_str)
    {
        return string(cookie_str);
    }
    else
    {
        return "";
    }
}

vector<string> &split_str(const string &s, char delim, vector<string> &elems)
{
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim))
    {
        elems.push_back(item);
    }
    return elems;
}

int check_user_signed_in(const FCGX_Request & request)
{
    int username = -1;
    string cookie = get_request_cookie(request);
    if (cookie == "")
    {
        return username;
    }
    vector<string> items;
    split_str(cookie, '&', items);
    for (int i = 0; i < items.size(); i++)
    {
        vector<string> record;
        split_str(items[i], '=', record);
        if (record.size() >= 2 && record[0] == "username")
        {
            if (record[1].find_first_not_of("0123456789") == string::npos)
            {
                username = atoi(record[1].c_str());
                return username;
            }
            else
            {
                return username;
            }
        }
    }
    return username;
}

void dump_all_env(const FCGX_Request & request)
{
    cout << "<PRE>\n";
    char **env = request.envp;
    while (*(++env))
    {
        cout << *env << "\n";
    }
    cout << "</PRE>\n";
}

void respond_welcome_page(const FCGX_Request & request)
{
    cout << "<html>\n";
    cout << "<head>\n";
    cout << "<title>Welcome</title>\n";
    cout << "</head>\n";
    cout << "<body>\n";
    cout << "<div style=\"text-align: center; margin: auto;\">\n";
    cout << "<p>Please login with your username and password</p>\n";
    cout << "<form method=\"post\" action=\"./login\">\n";
    cout << "<input type=\"text\" name=\"username\"><br>\n";
    cout << "<input type=\"password\" name=\"pswd\"><br>\n";
    cout << "<input type=\"submit\" value=\"Login\">\n";
    cout << "</form>\n";
    cout << "</div>";
    cout << "</body>\n";
    cout << "<html>\n";
}

void welcome_user(const FCGX_Request & request)
{
    cout << "Content-type:text/html\n\n";
    respond_welcome_page(request);
}

void login_user(const FCGX_Request & request, backend_connection & con)
{
    string params = get_request_content(request);
    string backend_response = con.communicate_with_backend("check_user\n" + params);
    cout << "Content-type:text/html\n";
    if (backend_response.find_first_not_of("0123456789") == string::npos) {
        cout << "Set-Cookie: username=" << backend_response << "\n\n";
    } else {
        cout << "Set-Cookie: username=nonuser\n\n";
        respond_welcome_page(request);
        return;
    }
    cout << "<html>\n";
    cout << "<head>\n";
    cout << "<title>Welcome to user system</title>\n";
    cout << "</head>\n";
    cout << "<body>\n";
    cout << "<div style=\"text-align: center; margin: auto;\">\n";
    cout << "You can see <a href=\"./user\">user page</a> now.";
    cout << "</div>";
    cout << "</body>\n";
    cout << "<html>\n";
}

void logout_user(const FCGX_Request & request)
{
    cout << "Content-type:text/html\n";
    cout << "Set-Cookie: username=nonuser\n\n";
    respond_welcome_page(request);
}

void display_user(const FCGX_Request & request)
{
    int username = check_user_signed_in(request);
    if (username == -1)
    {
        welcome_user(request);
        return ;
    }
    cout << "Content-type:text/html\n\n";
    cout << "<html>\n";
    cout << "<head>\n";
    cout << "<title>Hello, "<< username << "</title>\n";
    cout << "</head>\n";
    cout << "<body>\n";
    cout << "<div style=\"text-align: center; margin: auto;\">\n";
    cout << "<form method=\"post\" action=\"./update_nick\">\n";
    cout << "<input type=\"text\" name=\"nickname\"><br>\n";
    cout << "<input type=\"submit\" value=\"Update Nickname\"><br>\n";
    cout << "</form>\n";
    cout << "</div><br>\n";
    cout << "<div style=\"text-align: center; margin: auto;\">\n";
    cout << "<form method=\"post\" enctype=\"multipart/form-data\"";
    cout << " action=\"./update_pic\">\n";
    cout << "<input type=\"file\" name=\"image\" accept=\"image/png\"><br>\n";
    cout << "<input type=\"submit\" value=\"Update Picture\"><br>\n";
    cout << "</form>\n";
    cout << "</div>\n";
    cout << "<div style=\"text-align: center; margin: auto;\">\n";
    cout << "You can <a href=\"./logout\">logout</a> now.";
    cout << "</div>\n";
    cout << "</body>\n";
    cout << "<html>\n";
}

void update_nick(const FCGX_Request & request, backend_connection & con)
{
    int username = check_user_signed_in(request);
    if (username == -1)
    {
        welcome_user(request);
        return ;
    }
    string params = get_request_content(request);
    params = params + "&username=" + to_string(username);
    string backend_response = con.communicate_with_backend("update_nick\n" + params);
    cout << "Content-type:text/html\n\n";
    cout << "<html>\n";
    cout << "<head>\n";
    cout << "<meta http-equiv=\"refresh\" content=\"3;url=./user\">";
    cout << "<title>Welcome</title>\n";
    cout << "</head>\n";
    cout << "<body>\n";
    if (backend_response == "OK")
    {
        cout << "The update is " << backend_response << "<br>\n";
    }
    else
    {
        cout << "The update is " << "NOT OK" << "<br>\n";
    }
    cout << "You will be redirected to <a href=\"./user\">user</a> page";
    cout << "<div style=\"text-align: center; margin: auto;\">\n";
    cout << "You can <a href=\"./logout\">logout</a> now.";
    cout << "</div>\n";
    cout << "</body>\n";
    cout << "<html>\n";
}

void update_pic(const FCGX_Request & request)
{
    int username = check_user_signed_in(request);
    if (username == -1)
    {
        welcome_user(request);
        return ;
    }
    string params = get_request_content(request);
    stringstream ss(params);
    cout << "Content-type:text/html\n\n";
    cout << "<html>\n";
    cout << "<head>\n";
    // cout << "<meta http-equiv=\"refresh\" content=\"3;url=./user\">";
    cout << "<title>Welcome</title>\n";
    cout << "</head>\n";
    cout << "<body>\n";
    string line;
    vector<string> lines;
    while (getline(ss, line))
    {
        lines.push_back(line);
    }
    lines.erase(lines.begin() + 0, lines.begin() + 2);
    lines.erase(lines.begin() + (lines.size() - 1));
    string filename = to_string(username) + ".png";
    ofstream oFile;
    oFile.open(filename.c_str(), ios::binary);
    for (int i = 0; i < lines.size(); i++)
    {
        cout << lines[i] << "<br>\n";
        oFile << lines[i];
    }
    oFile.close();
    cout << "<div style=\"text-align: center; margin: auto;\">\n";
    cout << "You can <a href=\"./logout\">logout</a> now.";
    cout << "</div>\n";
    cout << "</body>\n";
    cout << "<html>\n";
}

void path_not_found(const FCGX_Request & request)
{
    cout << "Content-type:text/html\n\n";
    cout << "<html>\n";
    cout << "<head>\n";
    cout << "<title>Welcome</title>\n";
    cout << "</head>\n";
    cout << "<p><a href=\"./welcome\">welcome</a></p>";
    cout << "</body>\n";
    cout << "<html>\n";
}

void process_uri(const char *uri, const FCGX_Request & request, backend_connection & con)
{
    if (strcmp(uri, "/welcome") == 0 || strcmp(uri, "/index") == 0 || strcmp(uri, "/") == 0)
    {
        welcome_user(request);
    }
    else if (strcmp(uri, "/login") == 0)
    {
        login_user(request, con);
    }
    else if (strcmp(uri, "/logout") == 0)
    {
        logout_user(request);
    }
    else if (strcmp(uri, "/user") == 0)
    {
        display_user(request);
    }
    else if (strcmp(uri, "/update_nick") == 0)
    {
        update_nick(request, con);
    }
    else if (strcmp(uri, "/update_pic") == 0)
    {
        update_pic(request);
    }
    else
    {
        path_not_found(request);
    }
}

int main(void) {
    // Backup the stdio streambufs
    streambuf * cin_streambuf  = cin.rdbuf();
    streambuf * cout_streambuf = cout.rdbuf();
    streambuf * cerr_streambuf = cerr.rdbuf();
    backend_connection backend;

    FCGX_Request request;

    FCGX_Init();
    FCGX_InitRequest(&request, 0, 0);

    while (FCGX_Accept_r(&request) == 0)
    {
        fcgi_streambuf cin_fcgi_streambuf(request.in);
        fcgi_streambuf cout_fcgi_streambuf(request.out);
        fcgi_streambuf cerr_fcgi_streambuf(request.err);

        cin.rdbuf(&cin_fcgi_streambuf);
        cout.rdbuf(&cout_fcgi_streambuf);
        cerr.rdbuf(&cerr_fcgi_streambuf);

        const char *uri = FCGX_GetParam("REQUEST_URI", request.envp);
        process_uri(uri, request, backend);
        // Note: the fcgi_streambuf destructor will auto flush
    }

    // restore stdio streambufs
    cin.rdbuf(cin_streambuf);
    cout.rdbuf(cout_streambuf);
    cerr.rdbuf(cerr_streambuf);

    return 0;
}
