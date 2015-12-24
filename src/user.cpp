#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
	cout << "Content-type:text/html\n\n";
	cout << "<html>\n";
    cout << "<head>\n";
    cout << "<title>Welcome</title>\n";
    cout << "</head>\n";
    cout << "<body>\n";
    cout << "<div>\n";
    cout << "<form method=\"post\" action=\"./update_nick.cgi\">\n";
    cout << "<input type=\"text\" name=\"nickname\">\n";
    cout << "<input type=\"submit\" value=\"Update Nickname\">\n";
    cout << "</form>\n";
    cout << "</div>\n";
    cout << "<div>\n";
    cout << "<form method=\"post\" enctype=\"multipart/form-data\"";
    cout << " action=\"./update_pic.cgi\">\n";
    cout << "<input type=\"file\" name=\"image\">\n";
    cout << "<input type=\"submit\" value=\"Update Picture\">\n";
    cout << "</form>\n";
    cout << "</div>\n";
    cout << "</body>\n";
    cout << "<html>\n";

	return 0;
}