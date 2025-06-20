CXX = g++
CXXFLAGS = -std=c++11 -Iinclude -I. -Wall -Wextra -Wno-type-limits

# Alvos principais
all: score_system

# Executável principal
score_system: src/main.cpp src/score.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

# Testes
run_tests: tests/test_score.cpp src/score.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@
	./$@

# Limpeza
clean:
	rm -f score_system run_tests *.csv

.PHONY: all run_tests clean