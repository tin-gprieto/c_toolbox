V = valgrind
CFLAGS = -std=c99 -g -O0
GFLAGS = -tui
WFLAGS = -Wall -Wconversion -Werror
VFLAGS = --leak-check=full --track-origins=yes --show-reachable=yes

TOOLS = code/tools/tools.c
INTERFAZ = code/interfaz/interfaz.c

test/tools.exe:	$(TOOLS)
	$(CC) test/test_tools.c $(TOOLS) $(CFLAGS) $(WFLAGS) -o test/tools.exe

tools: test/tools.exe $(TOOLS)
	$(V) $(VFLAGS) ./test/tools.exe

test/interfaz.exe: $(INTERFAZ)
	$(CC) test/test_interfaz.c $(INTERFAZ) $(CFLAGS) $(WFLAGS) -o test/interfaz.exe

interfaz: test/interfaz.exe $(INTERFAZ)
	$(V) $(VFLAGS) ./test/interfaz.exe