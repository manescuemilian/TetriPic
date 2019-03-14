CC = gcc
CFLAGS = -g -Wall -Wextra
HEADERS = tema3.c functii.c
EXEC = tema3

.PHONY = build clean

build: $(EXEC)

$(EXEC): $(HEADERS)
	$(CC) $(CFLAGS) $(HEADERS) -o $@

run_task1: build
	./$(EXEC) 1

run_task2: build
	./$(EXEC) 2

run_task3: build
	./$(EXEC) 3

run_task4: build
	./$(EXEC) 4

run_bonus: build
	./$(EXEC) 5

clean:
	rm -f $(EXEC)