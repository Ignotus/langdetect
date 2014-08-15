all:
	g++ main.cpp  -std=c++0x -lboost_filesystem -lboost_system -lboost_regex -fopenmp -o trigram_calc
	unzip trained_data/trained_data.zip

gen:
	./trigram_calc -train wiki/data/

clean:
	rm -r af bg de en pl ru uk trigram_calc
	rm -rf testset wiki/data/
