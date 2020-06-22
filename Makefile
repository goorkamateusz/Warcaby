#
# Gorka Mateusz - szablon makefile
#
# Konfiguracja:
SZABLONY=d:/_BIBLIOTEKA/CODE/_szablony/C_CPP
DOX=d:/_PROGRAMY/doxygen/doxygen.exe

# Kompilacja:
FLAGI=-Iinc -Wall -pedantic -std=c++0x
NAZWA_PROG=warcaby.exe
LISTA_PLIKOW= \
	obj/Vector2D.o \
	obj/Move.o \
	obj/Board.o \
	obj/Board_AI.o \
	obj/Interface.o \
	obj/main.o

SFML_INSTR= -DSFML_STATIC \
	-I .\SFML\include \
	-L .\SFML\lib \
	-lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lwinmm -lgdi32 -lfreetype

# Komendy:
__start__: build
	./${NAZWA_PROG}

build: mkobj KOMPILACJA_SKL KONSOLIDACJA

rebuild: clean build

debug: clean mkobj KOMPILACJA_SKL DEGUB_
	gdb ./${NAZWA_PROG} -ex run

rmobj:
	rm -f obj/*

clean: rmobj
	rm -f ${NAZWA_PROG}

doxygen:
	cd dox; ${DOX} Doxyfile 2> /dev/null; cd ..;\

MINIMAL:
	rm -fr obj dox/html; \

linux: clean KOMPILACJA_SKL
	g++ ${FLAGI} -o ${NAZWA_PROG} ${LISTA_PLIKOW} -lsfml-graphics -lsfml-window -lsfml-system

# Podkomendy kompilacji:
KOMPILACJA_SKL: ${LISTA_PLIKOW}

KONSOLIDACJA:
	g++ ${FLAGI} -o ${NAZWA_PROG} ${LISTA_PLIKOW} ${SFML_INSTR}

DEGUB_:
	g++ -g ${FLAGI} -o ${NAZWA_PROG} ${LISTA_PLIKOW}

mkobj:
	mkdir -p obj

# Skladowe - DO UZUPELNIENIA
obj/main.o: src/main.cpp
	g++ -c ${FLAGI} -o obj/main.o src/main.cpp ${SFML_INSTR}

obj/Interface.o: src/Interface.cpp inc/Interface.hpp
	g++ -c ${FLAGI} -o obj/Interface.o src/Interface.cpp ${SFML_INSTR}

obj/Board.o: src/Board.cpp inc/Board.hpp
	g++ -c ${FLAGI} -o obj/Board.o src/Board.cpp

obj/Board_AI.o: src/Board_AI.cpp inc/Board.hpp
	g++ -c ${FLAGI} -o obj/Board_AI.o src/Board_AI.cpp ${SFML_INSTR}

obj/Vector2D.o: src/Vector2D.cpp inc/Vector2D.hpp
	g++ -c ${FLAGI} -o obj/Vector2D.o src/Vector2D.cpp

obj/Move.o: src/Move.cpp inc/Move.hpp
	g++ -c ${FLAGI} -o obj/Move.o src/Move.cpp