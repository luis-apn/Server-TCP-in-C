all: monitor
	@echo Compilando...
	@g++ server.cpp Monitor.o -o server
monitor:
	@g++ -c Monitor.cpp
clean:
	@echo Limpando Arquivos...
	@rm server Monitor.o

run:
	@echo Executando...
	@./server

clear:
	@clear