//Authors: Alexei Kohovec, Nickita Makhnitskiy
#include <stdio.h>
#include <stdlib.h>
#include "../include/sqlite3.h"
#include "../include/structs.h"

static int callback(void *data, int argc, char **argv, char **azColName);

int requests(sqlite3* db);

int print_information_about_farm_with_exp_product(sqlite3* db);
int print_sum_and_count_by_customer(sqlite3* db);

int farm_profit(sqlite3* db);
int print_max_profit(sqlite3* db);

int good_auction(sqlite3* db);
int get_profit_from_auction(sqlite3* db, int id);;

int get_profit_less_plan(sqlite3* db, char* per);
int insert_into_farms(sqlite3* db);
int insert_into_products(sqlite3* db);
int insert_into_auction(sqlite3* db);
int insert_into_database(sqlite3* db);

int delete_from_database(sqlite3* db);

int update_database(sqlite3* db);
