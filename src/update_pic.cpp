#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include <cgicc/CgiDefs.h>
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>

using namespace std;
using namespace cgicc;

int main(int argc, char const *argv[])
{
	Cgicc formData;

	cout << "Content-type:text/html\n\n";
	cout << "<html>\n";
	cout << "<head>\n";
	cout << "<title>See form data in CGI</title>\n";
	cout << "</head>\n";
	cout << "<body>\n";

	form_iterator fi = formData.getElement("username");
	if ( !fi->isEmpty() && fi != (*formData).end()) {
	   cout << "username: " << **fi << endl;
	} else {
	  cout << "No text entered for first name" << endl;
	}
	cout << "<br/>\n";

	fi = formData.getElement("pswd");
	if ( !fi->isEmpty() && fi != (*formData).end()) {
	   cout << "pswd: " << **fi << endl;
	} else {
	  cout << "No text entered for first name" << endl;
	}
	cout << "<br/>\n";
	cout << "</body>\n";
	cout << "</html>\n";

    return 0;
}
