CXX       := g++
INCDIR    := include
CXXFLAGS  := -std=c++17 -g -Wall -I$(INCDIR) -MMD -MP -I/usr/local/include
LDFLAGS   := $(shell pkg-config --libs allegro-5 allegro_font-5 allegro_image-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5 allegro_ttf-5)
LDLIBS    := -L/usr/local/lib -lwidgetz
TESTFLAGS := -I./doctest -DTESTING

SRCDIR    := src
TESTDIR   := tests
OBJDIR    := obj
BINDIR    := bin

# --- jogo ---
TARGET := $(BINDIR)/flappy_bird
SRCS   := $(shell find $(SRCDIR) -name '*.cpp')
OBJS   := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/$(SRCDIR)/%.o,$(SRCS))
DEPS   := $(OBJS:.o=.d)

all: $(TARGET) assets

$(TARGET): $(OBJS)
	@mkdir -p $(BINDIR)
	$(CXX) $^ -o $@ $(LDLIBS) $(LDFLAGS)

$(OBJDIR)/$(SRCDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

assets:
	@cp -r assets $(BINDIR)/

run: all
	./$(TARGET)

# --- testes ---
TEST_SRCS  := $(shell find $(TESTDIR) -name '*.cpp')
# Mantém caminho completo para testes em subpastas, apenas nome para testes na raiz
TEST_NAMES := $(foreach test,$(basename $(TEST_SRCS)),$(if $(filter $(TESTDIR)/%,$(test)),$(patsubst $(TESTDIR)/%,%,$(test)),$(notdir $(test))))
TEST_OBJS  := $(patsubst $(TESTDIR)/%.cpp,$(OBJDIR)/$(TESTDIR)/%.o,$(TEST_SRCS))
TEST_DEPS  := $(TEST_OBJS:.o=.d)

# Todos os objetos do jogo, exceto main.o
GAME_OBJS := $(filter-out $(OBJDIR)/$(SRCDIR)/main.o,$(OBJS))

# Compila cada objeto de teste
$(OBJDIR)/$(TESTDIR)/%.o: $(TESTDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(TESTFLAGS) -c $< -o $@

# Função para gerar regras dinâmicas
define MAKE_TEST_RULE
TEST_PATH := $(1)
TEST_NAME := $(if $(findstring /,$(1)),$(subst /,_,$(1)),$(1))

TEST_EXE := $(BINDIR)/tests/$(TEST_PATH)
$$(TEST_EXE): $(OBJDIR)/$(TESTDIR)/$(TEST_PATH).o $$(GAME_OBJS)
	@mkdir -p $$(dir $$@)
	$$(CXX) $$^ -o $$@ $$(LDFLAGS) $$(TESTFLAGS) $$(LDLIBS)

.PHONY: run_$$(TEST_NAME)
run_$$(TEST_NAME): $$(TEST_EXE)
	@./$$<
endef

# Aplica a função para cada teste
$(foreach t,$(TEST_NAMES),$(eval $(call MAKE_TEST_RULE,$(t))))

# Alvo para construir todos os testes
.PHONY: tests
tests: $(addprefix $(BINDIR)/tests/,$(TEST_NAMES))

# Lista todos os alvos run_ disponíveis
.PHONY: list_tests
list_tests:
	@echo "Test targets available:"
	@for t in $(TEST_NAMES); do \
		tname=$$(echo $$t | tr '/' '_'); \
		echo "run_$$tname"; \
	done

clean:
	@rm -rf $(OBJDIR) $(BINDIR)

-include $(DEPS)
-include $(TEST_DEPS)