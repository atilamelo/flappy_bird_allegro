# --- Compilador e Flags ---
CXX = g++
CXXFLAGS = -std=c++17 -g -Wall
LDFLAGS = $(shell pkg-config --libs allegro-5 allegro_font-5 allegro_image-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5)

# --- Diretórios ---
SRCDIR = src
OBJDIR = obj
BINDIR = bin
INCDIR = include

CXXFLAGS += -I$(INCDIR)

# --- Arquivos ---
TARGET = $(BINDIR)/flappy_bird

# Busca recursiva pelos arquivos .cpp
SOURCES := $(shell find $(SRCDIR) -name '*.cpp')

# Gera os objetos espelhando a estrutura de pastas de src → obj
OBJECTS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))

# --- Regras ---

all: $(TARGET) assets

$(TARGET): $(OBJECTS)
	@echo "Ligando os arquivos para criar o executável..."
	@mkdir -p $(BINDIR)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Executável '$(TARGET)' criado com sucesso!"

# Compilação individual com criação do diretório correspondente
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

.PHONY: all clean assets run