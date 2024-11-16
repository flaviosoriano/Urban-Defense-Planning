# Makefile

# Compilador e flags
CC = g++
CFLAGS = -Wall -Wextra -Wpedantic -Wformat-security -Wconversion -Werror -Iinclude
DEBUGFLAGS = -g

# Diretórios
SRCDIR = src
BUILDDIR = obj
INCDIR = include

# Arquivo executável
EXEC = tp2.out

# Busca todos os arquivos .cpp no diretório src
SRC = $(wildcard $(SRCDIR)/*.cpp)
OBJ = $(SRC:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)

# Regra padrão
all: $(EXEC)

# Cria o executável
$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

# Compila arquivos .cpp
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compilação em modo de depuração
debug: CFLAGS += $(DEBUGFLAGS)
debug: clean all

# Limpa arquivos objetos e binários
clean:
	rm -rf $(BUILDDIR)/*.o $(BINDIR)/$(EXEC)

.PHONY: all clean debug
