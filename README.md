# Warcaby (Checkers)
>  Graficzna gra w warcaby z botem, jako przeciwnikiem.

## Autor
>   **Górka Mateusz**

## Spis treści

- [Autor](#Autor)
- [Zasady](#Zasady)
- [Uruchomienie](#Uruchomienie)
- [Specyfikacja](#Specyfikacja)
- [Dokumentacja](#Dokumentacja)

## Uruchomienie
- Kompilacja (windows)

    Poleceniem `make` z folderu aktualnego projektu. Polecenie wykorzystuje program [make](http://gnuwin32.sourceforge.net/packages/make.htm) i plik `Makefile`.

- Kompilacja (Linux)

    Należy zainstalować blibliotekę SFML poleceniem: `sudo apt-get install libsfml-dev`,
    następnie skompilować program poleceniem: `make linux`.

- Uruchomienie programu

    Program uruchamiamy, za pomocą jego nazwy: `warcaby.exe`.

    Program umożliwia wybranie trybów trudności za pomocą flag.

    | flaga | opis |
    |:-----:|:-----|
    | -a    | Bot gra agresywnie, jest skłonny poświęcać swoje pionki. |
    | -m    | Bot gra zrównoważenie, poświęca pionki, tylko dla większego zysku. |
    | -d    | Bot gra bardzo ostrożnie, a zbicie jego damki jest prawnie niemożliwe. |

## Zasady

1. Nakaz bicia - Gracz musi wykonać bicie, jeżeli ma taką możliwość;
2. Bicie jest możliwe w każdym kierunku;
3. Pionek może ruszać się tylko do przodu, i tylko o jeden, po skosie;
4. Pionek, który dochodzi do końca planszy staje sie "damką" - może poruszać się w każdym kierunku (po skosie) o 1;
5. Gracz, który jako pierwszy utraci możliwość ruchu przegrywa;

## Specyfikacja
- Język: C++;
- OS: Windows 10;
- Kompilator: `minGW`;
- Testowane także na Ubuntu, w kompilatorze `GCC`;
- Wykorzystuje bibliotekę [SFML](https://www.sfml-dev.org/);
- Wykorzystuje czcionkę [Coolvetica](http://typodermicfonts.com/coolvetica/)


## Dokumentacja
Dokumentacje można wygenerwować za pomocą programu [Doxygen](http://doxygen.nl/), wykorzystując plik `Doxyfile.`
