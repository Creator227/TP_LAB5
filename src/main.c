//Authors: Alexei Kohovec, Nickita Makhnitskiy
#include <stdio.h>
#include <stdlib.h>
#include "../include/sqlite3.h"
#include "../include/db_worker.h"
#include "../include/structs.h"


//Show menu and orginize work of system
int main(int argc, char* argv[])
{
	sqlite3* db;
	int rc = sqlite3_open("database3.db", &db);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return 1;
	}

	int stop = 1;
	while(stop)
	{
		int answer;
		printf("Choose action: \n1.INSERT\n2.DELETE\n3.Update\n4.Requests\n5.Exit\n");
		scanf("%d", &answer);

		int id = 0;
		switch(answer)
		{
			case 1:
				insert_into_database(db);
				break;
			case 2:
				delete_from_database(db);
				break;
			case 3:
				update_database(db);
				break;
			case 4:
				requests(db);
				break;
			case 5:
				stop = 0;
				break;
			default:
				break;
		}
	}
	sqlite3_close(db);
}
