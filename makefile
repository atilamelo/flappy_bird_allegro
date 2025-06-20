# --- Compilador e Flags ---
CXX = g++
CXXFLAGS = -std=c++17 -g -Wall
LDFLAGS = $(shell pkg-config --libs allegro-5 allegro_font-5 allegro_image-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5)
TESTFLAGS = -I./doctest -DTESTING 

# --- Diretórios ---
SRCDIR = src
OBJDIR = obj
BINDIR = bin
INCDIR = include
TESTDIR = tests

CXXFLAGS += -I$(INCDIR)
CXXFLAGS += -MMD -MP

# --- Arquivos ---
TARGET = $(BINDIR)/flappy_bird
TEST_TARGET = $(BINDIR)/test_runner

# Busca recursiva pelos arquivos .cpp
SOURCES := $(shell find $(SRCDIR) -name '*.cpp')
TEST_SOURCES := $(shell find $(TESTDIR) -name '*.cpp')

# Gera os objetos espelhando a estrutura de pastas
OBJECTS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))
TEST_OBJECTS := $(patsubst $(TESTDIR)/%.cpp,$(OBJDIR)/$(TESTDIR)/%.o,$(TEST_SOURCES))

# Gera lista de arquivos .d a partir dos .o
DEPS := $(OBJECTS:.o=.d) $(TEST_OBJECTS:.o=.d)

# --- Regras ---

all: $(TARGET) assets

test: $(TEST_TARGET)
	@echo "Executando testes..."
	./$(TEST_TARGET)

$(TARGET): $(filter-out $(OBJDIR)/main.o, $(OBJECTS)) $(OBJDIR)/main.o
	@echo "Ligando os arquivos para criar o executável do jogo..."
	@mkdir -p $(BINDIR)
	$(CXX) $(filter-out $(OBJDIR)/main.o, $(OBJECTS)) $(OBJDIR)/main.o -o $(TARGET) $(LDFLAGS)
	@echo "Executável '$(TARGET)' criado com sucesso!"

$(TEST_TARGET): $(filter-out $(OBJDIR)/main.o, $(OBJECTS)) $(TEST_OBJECTS)
	@echo "Construindo executável de testes..."
	@mkdir -p $(BINDIR)
	$(CXX) $(filter-out $(OBJDIR)/main.o, $(OBJECTS)) $(TEST_OBJECTS) -o $(TEST_TARGET) $(LDFLAGS) $(TESTFLAGS)
	@echo "Executável de testes '$(TEST_TARGET)' pronto!"

# Compilação de objetos do jogo
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "Compilando $< -> $@"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compilação de objetos de teste
$(OBJDIR)/$(TESTDIR)/%.o: $(TESTDIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "Compilando teste: $< -> $@"
	$(CXX) $(CXXFLAGS) $(TESTFLAGS) -c $< -o $@

assets:
	@echo "Copiando a pasta de assets..."
	@cp -r assets $(BINDIR)/

clean:
	@echo "Limpando os diretórios de compilação..."
	@rm -rf $(OBJDIR) $(BINDIR)
	@echo "Limpeza concluída."

run: all
	@echo "Executando o jogo..."
	./bin/flappy_bird

.PHONY: all clean assets run test

# Inclui os arquivos de dependência, se existirem
-include $(DEPS)