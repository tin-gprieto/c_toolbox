V = valgrind
CFLAGS = -std=c99 -g -O0
GFLAGS = -tui
WFLAGS = -Wall -Wconversion -Werror
VFLAGS = --leak-check=full --track-origins=yes --show-reachable=yes

TOOLS = code/tools/tools.c
INTERFAZ = code/interfaz/interfaz.c

test/interfaz.exe: $(INTERFAZ) $(TOOLS)
	$(CC) test/test_interfaz.c $(INTERFAZ) $(TOOLS) $(CFLAGS) $(WFLAGS) -o test/interfaz.exe

exe interfaz: test/interfaz.exe $(INTERFAZ)
	$(V) $(VFLAGS) ./test/interfaz.exe