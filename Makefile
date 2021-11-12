V = valgrind
CFLAGS = -std=c99 -g -O0
GFLAGS = -tui
WFLAGS = -Wall -Wconversion -Werror
VFLAGS = --leak-check=full --track-origins=yes --show-reachable=yes

TOOLS = code/tools/*.c 
INTERFAZ = code/interfaz/interfaz.c
TDAs=code/TDAs/**/*.c 
TDAsTEST= test/TDAs/*.c

test/tools.exe:	$(TOOLS)
	$(CC) test/test_tools.c $(TOOLS) $(CFLAGS) $(WFLAGS) -o test/tools.exe

tools: test/tools.exe $(TOOLS)
	$(V) $(VFLAGS) ./test/tools.exe

test/interfaz.exe: $(INTERFAZ) $(TOOLS)
	$(CC) test/test_interfaz.c $(INTERFAZ) $(TOOLS) $(CFLAGS) $(WFLAGS) -o test/interfaz.exe

interfaz: test/interfaz.exe $(INTERFAZ)
	$(V) $(VFLAGS) ./test/interfaz.exe

test/tdas.exe: $(TDAs) $(TDAsTEST) $(TOOLS)
	$(CC) test/test_tdas.c $(TDAsTEST) $(TDAs) $(TOOLS) $(CFLAGS) $(WFLAGS) -o test/tdas.exe

tdas: test/tdas.exe $(TDAs)
	$(V) $(VFLAGS) ./test/tdas.exe