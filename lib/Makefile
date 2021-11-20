CFLAGS = -std=c99 -g -O0

TOOLS = ../code/tools/*.c 
TDAs= ../code/TDAs/**/*.c 
INTERFAZ = ../code/interfaz/interfaz.c

build: $(TOOLS) $(TDAs) $(INTERFAZ)
	
	$(CC) $(CFLAGS) -c $(TOOLS)
	$(CC) $(CFLAGS) -c $(TDAs) $(TOOLS)
	$(CC) $(CFLAGS) -c $(INTERFAZ) $(TOOLS)
