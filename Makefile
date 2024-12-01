all:
	gcc -o final main.c -pthread
clean:
	rm -rf final myfifo