SRC = src
INC = inc
BIN = bin

# Don't forget to remove debug info in prod !!!!
stout: $(SRC) $(INC) $(BIN)
	gcc -g $(SRC)/* -I $(INC) -o $(BIN)/stout

run:
	./bin/tset $(TEST)/test1.t

clean:
	rm -rf $(BIN)/*

$(BIN):
	mkdir -p $@
