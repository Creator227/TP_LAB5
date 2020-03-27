//Authors: Alexei Kohovec, Nickita Makhnitskiy
//This code orginise work with database
#include <stdio.h>
#include <stdlib.h>
#include "../include/sqlite3.h"
#include "../include/db_worker.h"

//Print each string in record
static int callback(void *data, int argc, char **argv, char **azColName)
{
	for(int i = 0; i<argc; i++)
	{
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

int database_request(sqlite3* db, char* sql)
{
  char *zErrMsg = 0;
	int rc = sqlite3_exec(db, sql, callback, (void*)NULL, &zErrMsg);
  if(rc !=SQLITE_OK)
  {
                printf("Problem\n");
                printf(stderr, "Failed to create table\n");
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
								return 1;
  }
	return 0;
}

int requests(sqlite3* db)
{
	printf("\n\nChoose requests: "\
			"1.Information about farm with the most expensive product\n"\
			"2.Sum and count by customer category\n"\
			"3.Profit by farm\n"\
			"4.Farm with max profit\n"\
			"5.Good deals\n"\
			"6.Profit by farm id\n"\
			"7.Where profit less than plan\n");
	int answer;
	char per[100];
	scanf("%d", &answer);
	int i;
	switch(answer)
	{
		case 1:
			print_information_about_farm_with_exp_product(db);
			break;
		case 2:
			print_sum_and_count_by_customer(db);
			break;
		case 3:
			farm_profit(db);
			break;
		case 4:
			print_max_profit(db);
			break;
		case 5:
			good_auction(db);
			break;
		case 6:
			printf("Enter id\n");
			scanf("%d", &i);
			printf("%d\n", get_profit_from_auction(db, i));
			break;
		case 7:
			printf("Enter percent\n");

			scanf("%s", per);
			get_profit_less_plan(db, per);
			break;
		default:
			break;
	return 0;
	}
}

int print_information_about_farm_with_exp_product(sqlite3* db)
{
  database_request(db, "SELECT Animal_farm.id, Animal_farm.address, Animal_farm.director_surname, "\
						"Animal_farm.phone_number, max(Auction_result.auction_cost)  as max_cost "\
						"from Animal_farm join Auction_result"\
						" on Animal_farm.id=Auction_result.farm_id");
	return 0;
}

int print_sum_and_count_by_customer(sqlite3* db)
{
  database_request(db, "SELECT Auction_result.customer_category, "\
						"sum( Auction_result.sold_count) as count, "\
						"sum(Auction_result.auction_cost*Auction_result.sold_count) as sum_cost "\
						"from Auction_result GROUP BY Auction_result.customer_category ");
	return 0;
}

int farm_profit(sqlite3* db)
{
  database_request(db, "SELECT Animal_farm.address, Animal_farm.director_surname, "\
						"sum(Auction_result.auction_cost*Auction_result.sold_count) as profit "\
						"from Auction_result join Animal_farm "\
						"on Animal_farm.id = Auction_result.farm_id "\
						"GROUP BY Animal_farm.id");
	return 0;
}

int print_max_profit(sqlite3* db)
{
	database_request(db, "select address, director_surname, sold, "\
							"max(profit) from ( SELECT Animal_farm.address, Animal_farm.director_surname, "\
							"sum(Auction_result.auction_cost*Auction_result.sold_count) as profit, "\
							"sum(Auction_result.sold_count) as sold from Auction_result join Animal_farm "\
							"on Animal_farm.id = Auction_result.farm_id GROUP BY Animal_farm.id)");
	return 0;
}

int good_auction(sqlite3* db)
{
	database_request(db, "select Product.name, Animal_farm.address, Animal_farm.director_surname, "\
							"Animal_farm.phone_number, Product.cost, Auction_result.auction_cost "\
							"from Auction_result "\
							"join Product on Product.id=Auction_result.product_id "\
							"join Animal_farm on Animal_farm.id=Auction_result.farm_id "\
							"where Product.cost < Auction_result.auction_cost group by Product.id");
	return 0;
}

int get_profit_from_auction(sqlite3* db, int id)
{
	int profit = 0;
	int profit_callback(void *data, int argc, char **argv, char **azColName)
	{
		profit = atoi(argv[0]);
		return 0;
	}

	char *zErrMsg = 0;
	char sql[500];
	sprintf(sql, "select sum(Auction_result.auction_cost*Auction_result.sold_count), "\
							"Auction_result.farm_id from Auction_result where Auction_result.farm_id=%d "\
						  "group by Auction_result.farm_id", id);
	int rc = sqlite3_exec(db, sql, profit_callback, (void*)NULL, &zErrMsg);
	return profit;
}

int get_profit_less_plan(sqlite3* db, char* per)
{
	char sql[500];
	sprintf(sql, "select * from( select Auction_result.farm_id, "\
							"sum(Auction_result.auction_cost*Auction_result.sold_count) "\
							"as profit, sum(Product.cost*Product.count) as plan "\
							"from Auction_result join Product on Auction_result.product_id=Product.id 	"\
							"group by Auction_result.farm_id) where (profit-plan)/plan < %s", per);
  database_request(db, sql);
	return 0;
}
//////////////////////////////////////////////////////////////////////

int insert_into_farms(sqlite3* db)
{
	farm f;
	printf("Enter address\n");
	scanf("%s", f.address);

	printf("Enter director's surname\n");
	scanf("%s", f.director_surname);

	printf("Enter phone\n");
	scanf("%d", &f.phone_number);

	char* sql = "INSERT into Animal_farm(address, director_surname, phone_number) "\
						"values('%s', '%s', %d)";
	char request[100];
	sprintf(request, sql, f.address, f.director_surname, f.phone_number);

  database_request(db, request);
}

int insert_into_products(sqlite3* db)
{
	product p;

	printf("Enter farm id\n");
	scanf("%d", &p.farm_id);

	printf("Enter name\n");
	scanf("%s", p.name);

	printf("Enter sort\n");
	scanf("%s", p.sort);

	printf("Enter count\n");
	scanf("%d", &p.count);

	printf("Enter cost\n");
	scanf("%d", &p.cost);

	char* sql = "INSERT into Product(farm_id, name, sort, count, cost) "\
						"values(%d, '%s', '%s', %d, %d)";
	char request[100];
	sprintf(request, sql, p.farm_id, p.name, p.sort, p.count, p.cost);

  database_request(db, request);
}

int check_insert_into_auction(sqlite3*db, int product_id, int add_num)
{
	int current_num = 0;
	int max_num = 0;
	int profit_callback(void *data, int argc, char **argv, char **azColName)
	{
		current_num = atoi(argv[2]);
		max_num = atoi(argv[1]);
		return 0;
	}

	char *zErrMsg = 0;
	char sql[500];
	sprintf(sql, "select Product.id, Product.count, sum(Auction_result.sold_count) "\
							"from Product join Auction_result on Product.id=Auction_result.product_id "\
							"group by Product.id having Product.id = %d", product_id);
	int rc = sqlite3_exec(db, sql, profit_callback, (void*)NULL, &zErrMsg);

	if(current_num+add_num <= max_num)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int insert_into_auction(sqlite3* db)
{
	auction a;

	printf("Enter product id\n");
	scanf("%d", &a.product_id);

	printf("Enter farm id\n");
	scanf("%d", &a.farm_id);

	printf("Enter sort\n");
	scanf("%s", a.sort);

	printf("Enter count\n");
	scanf("%d", &a.count);

	printf("Enter cost\n");
	scanf("%d", &a.cost);

	printf("Enter customer category\n");
	scanf("%s", a.customer_category);

	if(check_insert_into_auction(db, a.product_id, a.count) == 1)
	{
		printf("Invalid count \n");
		return 1;
	}

	char* sql = "INSERT into Auction_result(product_id, farm_id, sort, sold_count, "\
							"auction_cost, customer_category) values(%d, %d, '%s', %d, %d, '%s')";
	char request[500];
	sprintf(request, sql, a.product_id, a.farm_id, a.sort, a.count,
		a.cost, a.customer_category);

	database_request(db, request);
}

int insert_into_database(sqlite3* db)
{
	int answer;
	printf("\n\nChoose table to insert: 1.Farms\n2.Products\n3.Auction\n");
	scanf("%d", &answer);
	switch(answer)
	{
		case 1:
			insert_into_farms(db);
			break;
		case 2:
			insert_into_products(db);
			break;
		case 3:
			insert_into_auction(db);
			break;
		default:
			break;
	}
}

int delete_from_database(sqlite3* db)
{
	int answer;
	printf("\n\nChoose table to delete record: 1.Farms\n2.Products\n3.Auction\n");
	scanf("%d", &answer);
	char* sql;
	switch(answer)
	{
		case 1:
			sql = "DELETE from Animal_farm where Animal_farm.id=%d";
			break;
		case 2:
			sql = "DELETE from Product where Product.id=%d";
			break;
		case 3:
			sql = "DELETE from Auction_result where Auction_result.deal_id=%d";
			break;
		default:
			break;
	}

	printf("Enter id\n");
	int id;
	scanf("%d", &id);
	char request[100];
	sprintf(request, sql, id);
	database_request(db, request);
	return 0;
}

int update_database(sqlite3* db)
{
	int answer;
	printf("\n\nChoose what update: 1.Product cost\n2.Auction cost\n3.Auction count\n");
	scanf("%d", &answer);
	char* sql;
	switch(answer)
	{
		case 1:
			sql = "UPDATE Product SET cost = %d where id=%d";
			break;
		case 2:
			sql = "UPDATE Auction_result SET auction_cost = %d where deal_id=%d";
			break;
		case 3:
			sql = "UPDATE Auction_result SET sold_count = %d where deal_id=%d";;
			break;
		default:
			break;
	}

	printf("Enter id\n");
	int id;
	scanf("%d", &id);

	printf("Enter new val\n");
	int val;
	scanf("%d", &val);

	char request[100];
	sprintf(request, sql, val, id);
	database_request(db, request);
	return 0;
}
