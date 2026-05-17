CC = gcc

SRC = main.c
OUT = main

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(SRC) -o $(OUT) && ./$(OUT)

run: $(OUT)
	./$(OUT)

clean:
	rm -rf $(OUT)