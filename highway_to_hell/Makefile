all:
	g++ main.cpp  -std=c++0x -lboost_filesystem -lboost_system -lboost_regex -o trigram_calc

gen:
	./trigram_calc -train wiki/data/

clean:
	rm -r af bg de en pl ru uk trigram_calc
