CFLAGS=-W -Wall -pedantic

all: main
main: bin/alphabet.o bin/lecture.o bin/pile.o bin/arbre.o bin/nfa.o bin/dfa.o bin/dfa_min.o src/main.c
	gcc -o bin/main $^
bin/%.o: src/%.c src/%.h
	gcc -o $@ -c $< $(CFLAGS)
clean:
	rm -f bin/*.o
mrproper: clean
	rm -f bin/main dot_src/*.tree dot_src/*.nfa dot_src/*.dfa pdf/*_tree.pdf pdf/*_nfa.pdf pdf/*_dfa.pdf
