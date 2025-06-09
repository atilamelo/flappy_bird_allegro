# Makefile para projeto C++ com Allegro
# Estrutura de diretórios esperada:
# .
# ├── assets/
# ├── bin/
# ├── include/
# ├── obj/
# ├── src/
# └── Makefile

# --- Compilador e Flags ---
# Define o compilador C++
CXX = g++

# Flags de compilação:
# -std=c++17: Usa o padrão C++17
# -g:         Adiciona informações de debug (para usar com gdb)
# -Wall:      Habilita a maioria dos avisos do compilador
CXXFLAGS = -std=c++17 -g -Wall

# Flags de linkagem:
# Usa pkg-config para encontrar e adicionar as flags necessárias para a Allegro 5
# Isso torna o Makefile mais portável entre sistemas.
LDFLAGS = $(shell pkg-config --libs allegro-5 allegro_font-5 allegro_image-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5)

# --- Diretórios ---
# Define os nomes dos diretórios para facilitar a manutenção
SRCDIR = src
OBJDIR = obj
BINDIR = bin
INCDIR = include

# Adiciona o diretório 'include' ao caminho de busca do compilador
CXXFLAGS += -I$(INCDIR)

# --- Arquivos ---
# Nome do executável final
TARGET = $(BINDIR)/game

# Encontra todos os arquivos .cpp no diretório 'src'
SOURCES = $(wildcard $(SRCDIR)/*.cpp)

# Gera a lista de arquivos objeto (.o) correspondentes, colocando-os no diretório 'obj'
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))

# --- Regras (Targets) ---

# A regra padrão, que é executada quando você digita apenas 'make'
# Depende do executável e da cópia dos assets.
all: $(TARGET) assets

# Regra para linkar os arquivos objeto e criar o executável final
$(TARGET): $(OBJECTS)
	@echo "Ligando os arquivos para criar o executável..."
	@mkdir -p $(BINDIR) # Cria o diretório 'bin' se ele não existir
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Executável '$(TARGET)' criado com sucesso!"

# Regra de padrão para compilar arquivos .cpp em .o
# Esta regra é acionada para cada arquivo objeto na lista $(OBJECTS)
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR) # Cria o diretório 'obj' se ele não existir
	@echo "Compilando $< -> $@"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regra para copiar a pasta de assets para o diretório bin
assets:
	@echo "Copiando a pasta de assets..."
	@cp -r assets $(BINDIR)/

# Regra para limpar os arquivos gerados (compilados e executável)
clean:
	@echo "Limpando os diretórios de compilação..."
	@rm -rf $(OBJDIR) $(BINDIR)
	@echo "Limpeza concluída."

# Phony targets são regras que não representam arquivos reais.
# Isso evita conflitos se você tiver um arquivo chamado 'clean' ou 'all'.
.PHONY: all clean assets

