//Aurthor: Alexei Kohovec, var: 9

#pragma once

//This file contains information about structs birthday, address and patient

#include <stdio.h>
#include <stdlib.h>


typedef struct farm_struct{
	char address[100];
	char director_surname[100];
	int phone_number;
} farm;

typedef struct product_struct{
	int farm_id;
	char name[100];
	char sort[100];
	int count;
	int cost;
} product;

typedef struct auction_struct{
	int product_id;
	int farm_id;
	char sort[100];
	int count;
	int cost;
	char customer_category[100];
} auction;
