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

# Busca recursiva pelos arquivos .cpp
SOURCES := $(shell find $(SRCDIR) -name '*.cpp')
TEST_SOURCES := $(shell find $(TESTDIR) -name '*.cpp')

# Gera os objetos espelhando a estrutura de pastas
OBJECTS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))

# Gera lista de arquivos .d a partir dos .o
DEPS := $(OBJECTS:.o=.d)

# --- Regras ---

all: $(TARGET) assets

$(TARGET): $(filter-out $(OBJDIR)/main.o, $(OBJECTS)) $(OBJDIR)/main.o
	@echo "Ligando os arquivos para criar o executável do jogo..."
	@mkdir -p $(BINDIR)
	$(CXX) $(filter-out $(OBJDIR)/main.o, $(OBJECTS)) $(OBJDIR)/main.o -o $(TARGET) $(LDFLAGS)
	@echo "Executável '$(TARGET)' criado com sucesso!"

# Compilação de objetos do jogo
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "Compilando $< -> $@"
	$(CXX) $(CXXFLAGS) -c $< -o $@

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

# --- Regras para Testes Individuais ---
# Lista de nomes de testes (sem extensão)
TEST_NAMES := $(basename $(notdir $(TEST_SOURCES)))

# Alvos phony para execução de testes
.PHONY: $(addprefix run_,$(TEST_NAMES))

# Regra para cada teste individual
define TEST_RULE
run_$(1): $(BINDIR)/$(1)
	@echo "Executando teste: $(1)"
	@./$$<
endef

# Cria regras dinâmicas para cada teste
$(foreach test,$(TEST_NAMES),$(eval $(call TEST_RULE,$(test))))

# Regra para compilar cada teste
$(BINDIR)/%: $(OBJDIR)/$(TESTDIR)/%.o $(filter-out $(OBJDIR)/main.o, $(OBJECTS))
	@mkdir -p $(BINDIR)
	@echo "Construindo teste: $@"
	@$(CXX) $(filter-out $(OBJDIR)/main.o, $(OBJECTS)) $< -o $@ $(LDFLAGS) $(TESTFLAGS)

# Compilação de objetos de teste
$(OBJDIR)/$(TESTDIR)/%.o: $(TESTDIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "Compilando teste: $< -> $@"
	@$(CXX) $(CXXFLAGS) $(TESTFLAGS) -c $< -o $@

.PHONY: all clean assets run

# Inclui os arquivos de dependência, se existirem
-include $(DEPS)