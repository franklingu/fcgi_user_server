#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <string>
// #include "fcgi_stdio.h"
// #include "fcgiapp.h"
#include "fcgio.h"

using namespace std;

// Maximum bytes
const unsigned long STDIN_MAX = 1000000;

string get_request_content(const FCGX_Request & request) {
    char * content_length_str = FCGX_GetParam("CONTENT_LENGTH", request.envp);
    unsigned long content_length = STDIN_MAX;

    if (content_length_str) {
        content_length = strtol(content_length_str, &content_length_str, 10);
        if (*content_length_str) {
            cerr << "Can't Parse 'CONTENT_LENGTH='"
                 << FCGX_GetParam("CONTENT_LENGTH", request.envp)
                 << "'. Consuming stdin up to " << STDIN_MAX << endl;
        }

        if (content_length > STDIN_MAX) {
            content_length = STDIN_MAX;
        }
    } else {
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

void welcome_user(const FCGX_Request & request)
{
    cout << "Content-type:text/html\n\n";
    cout << "<html>\n";
    cout << "<head>\n";
    cout << "<title>Welcome</title>\n";
    cout << "</head>\n";
    cout << "<body>\n";
    cout << "<form method=\"post\" action=\"./login\">\n";
    cout << "<input type=\"text\" name=\"username\">\n";
    cout << "<input type=\"password\" name=\"pswd\">\n";
    cout << "<input type=\"submit\" value=\"Login\">\n";
    cout << "</form>\n";
    cout << "</body>\n";
    cout << "<html>\n";
}

void login_user(const FCGX_Request & request)
{
    string params = get_request_content(request);
    cout << "Content-type:text/html\n\n";
    cout << "<html>\n";
    cout << "<head>\n";
    cout << "<title>Welcome</title>\n";
    cout << "</head>\n";
    cout << "<body>\n";
    cout << params;
    cout << "</body>\n";
    cout << "<html>\n";
}

void display_user(const FCGX_Request & request)
{
    string username = "1";
    cout << "Content-type:text/html\n\n";
    cout << "<html>\n";
    cout << "<head>\n";
    cout << "<title>Hello, "<< username << "</title>\n";
    cout << "</head>\n";
    cout << "<body>\n";
    cout << "<div>\n";
    cout << "<form method=\"post\" action=\"./update_nick\">\n";
    cout << "<input type=\"text\" name=\"nickname\">\n";
    cout << "<input type=\"submit\" value=\"Update Nickname\">\n";
    cout << "</form>\n";
    cout << "</div>\n";
    cout << "<div>\n";
    cout << "<form method=\"post\" enctype=\"multipart/form-data\"";
    cout << " action=\"./update_pic\">\n";
    cout << "<input type=\"file\" name=\"image\">\n";
    cout << "<input type=\"submit\" value=\"Update Picture\">\n";
    cout << "</form>\n";
    cout << "</div>\n";
    cout << "</body>\n";
    cout << "<html>\n";
}

void update_nick(const FCGX_Request & request)
{
    string params = get_request_content(request);
    cout << "Content-type:text/html\n\n";
    cout << "<html>\n";
    cout << "<head>\n";
    cout << "<title>Welcome</title>\n";
    cout << "</head>\n";
    cout << "<body>\n";
    cout << params;
    cout << "</body>\n";
    cout << "<html>\n";
}

void update_pic(const FCGX_Request & request)
{
    string params = get_request_content(request);
    cout << "Content-type:text/html\n\n";
    cout << "<html>\n";
    cout << "<head>\n";
    cout << "<title>Welcome</title>\n";
    cout << "</head>\n";
    cout << "<body>\n";
    cout << params;
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

void process_uri(const char *uri, const FCGX_Request & request)
{
    if (strcmp(uri, "/welcome") == 0 || strcmp(uri, "/index") == 0 || strcmp(uri, "/") == 0)
    {
        welcome_user(request);
    }
    else if (strcmp(uri, "/login") == 0)
    {
        login_user(request);
    }
    else if (strcmp(uri, "/user") == 0)
    {
        display_user(request);
    }
    else if (strcmp(uri, "/update_nick") == 0)
    {
        update_nick(request);
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

    FCGX_Request request;

    FCGX_Init();
    FCGX_InitRequest(&request, 0, 0);

    while (FCGX_Accept_r(&request) == 0) {
        fcgi_streambuf cin_fcgi_streambuf(request.in);
        fcgi_streambuf cout_fcgi_streambuf(request.out);
        fcgi_streambuf cerr_fcgi_streambuf(request.err);

        cin.rdbuf(&cin_fcgi_streambuf);
        cout.rdbuf(&cout_fcgi_streambuf);
        cerr.rdbuf(&cerr_fcgi_streambuf);

        const char *uri = FCGX_GetParam("REQUEST_URI", request.envp);
        process_uri(uri, request);
        // Note: the fcgi_streambuf destructor will auto flush
    }

    // restore stdio streambufs
    cin.rdbuf(cin_streambuf);
    cout.rdbuf(cout_streambuf);
    cerr.rdbuf(cerr_streambuf);

    return 0;
}
