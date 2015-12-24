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
    cout << "<form method=\"post\" action=\"./login.cgi\">\n";
    cout << "<input type=\"text\" name=\"username\">\n";
    cout << "<input type=\"password\" name=\"pswd\">\n";
    cout << "<input type=\"submit\" value=\"Login\">\n";
    cout << "</form>\n";
    cout << "</body>\n";
    cout << "<html>\n";

	return 0;
}
