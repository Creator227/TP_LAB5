bin/Auction: obj/main.o obj/db_worker.o obj/sqlite3.o
	gcc obj/main.o obj/db_worker.o obj/sqlite3.o -lpthread -ldl -o bin/Auction

obj/sqlite3.o: src/sqlite3.c
	gcc -c src/sqlite3.c -o obj/sqlite3.o -lpthread -ldl

obj/main.o: src/main.c
	gcc -c src/main.c -o obj/main.o

obj/db_worker.o: src/db_worker.c
	gcc -c src/db_worker.c -o obj/db_worker.o

clean:
	rm -f obj/*.o bin/Auction
