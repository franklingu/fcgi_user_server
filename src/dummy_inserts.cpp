#include <stdio.h>
#include "db_connection.h"

int main(int argc, char const *argv[])
{
	db_connection con;
	int NUM_USERS = 10000000;
	for (int i = 0; i < NUM_USERS; i++)
	{
		con.insert_user("pwsd", "nick");
	}
	printf("Done with inserts\n");

	return 0;
}
