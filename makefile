# Makefile

# Compilador e flags
CC = g++
CFLAGS = -Wall -Iinclude

# Diretórios
SRCDIR = src
BUILDDIR = obj
BINDIR = bin
INCDIR = include

# Arquivo executável
EXEC = tp2.out

# Busca todos os arquivos .cpp no diretório src
SRC = $(wildcard $(SRCDIR)/*.cpp)
OBJ = $(SRC:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)

# Regra padrão
all: $(BINDIR)/$(EXEC)

# Cria o executável
$(BINDIR)/$(EXEC): $(OBJ) | $(BINDIR)
	$(CC) $(CFLAGS) $(OBJ) -o $@

# Compila arquivos .cpp
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Limpa arquivos objetos e binários
clean:
	rm -rf $(BUILDDIR)/*.o $(BINDIR)/$(EXEC)

.PHONY: all clean
