#ifndef MY_TREE_HPP
#define MY_TREE_HPP

#include <iostream>
#include <functional>
#include <stdlib.h>
#include <time.h>

/**
 * \brief Drzewo
 * \tparam T - Wartość węzłów
 */
template< typename T >
class myTree {
private:
    myTree *firstborn;    ///< Pierworodne dziecko (pion)
    myTree *siblings;     ///< Rodzeństwo węzła (poziom)
    T value;              ///< Wartość węzła

public:
    /**
     * \brief Konstruktor paramteryczny węzła
     * \param nval - wartośc węzła
     */
    myTree( const T& nval ): firstborn( NULL ), siblings( NULL ), value( nval ){}

    /**
     * \brief Usuwa obiekt, swoje dziecko i rodzeństwo
     *
     */
    ~myTree(){ delete firstborn; }

    /**
     * \brief Zwraca wartość węzła
     * \return T -
     */
    inline T  v() const { return value; }
    inline T& v()       { return value; }

    /**
     * \brief Czy drzewo ma dzieci?
     * \return true - tak
     * \return false - nie
     */
    inline bool haveChild() const { return( firstborn != NULL ); }

////-----------------------------------------------------------------------
    /**
     * \brief Dodaje rodzeństwo
     * \param val - wartość nowego węzła
     * \return myTree* - wsk na dany węzeł
     */
    inline myTree* addSibling( const T& val )
    { return( this->last_sib()->siblings = new myTree( val ) ); }

    /**
     * \brief Dodaje dziecko
     * \param val -
     * \return myTree* -
     */
    inline myTree* addChild( const T& val ){
        if( firstborn == NULL ) return( firstborn = new myTree( val ) );
        else                    return( firstborn->last_sib()->siblings = new myTree( val ) );
    }

    /**
     * \brief Zwraca link do pierwszego dziecka
     * \return T -
     */
    inline myTree* Firstborn() { return firstborn; }

    /**
     * \brief Usuwa wybrane dziecko
     * \param thechild -
     */
    bool rmChild( const myTree* thechild ){
        myTree* last = firstborn;

        if( firstborn == thechild ){
            firstborn = firstborn->siblings;
            delete last;
            return true;
        }

        myTree* tmp = last->siblings;

        while( tmp != NULL ){
            if( tmp == thechild ){
                last->siblings = tmp->siblings;
                delete tmp;
                return true;
            }

            last = tmp;
            tmp = tmp->siblings;
        }
        return false;
    }

    /**
     * \brief Usuwa wszystkie dzieci z wyjątkiem thechild
     * \param thechild - wskaznik na dziecko do zachowania
     */
    void rmChildApart( myTree* const thechild ){
        myTree *tmp, *curr = firstborn;

        while( curr != NULL ){
            tmp = curr;
            curr = curr->siblings;
            if( tmp != thechild ) delete tmp;
        }

        firstborn = thechild;
        firstborn->siblings = NULL;
    }

////-----------------------------------------------------------------------
    /**
     * \brief Zwraca ostatniego z braci
     * \return myTree* -
     */
    myTree* last_sib(){
        if( siblings == NULL ) return this;
        myTree* tmp = siblings;
        while( tmp->siblings != NULL ) tmp = tmp->siblings;
        return tmp;
    }

    /**
     * \brief Usuwa całe rodzeństwo
     */
    void rmSib(){
        myTree *sib = siblings, *tmp;
        while( sib != NULL ){
            tmp = sib;
            sib = sib->siblings;
            delete tmp;
        }
    }

////----------------------------------------------------------------------
    /**
     * \brief Wyznacza najlepszą gałąź według kryterium fun
     * \tparam Fun
     * \param fun - wyrażenie lambda, krytermium: T -> int
     * \param maxim - wyrażenie lambda, czy maksymalizować dla danego węzła: T -> bool
     * \return int - wartość krędzi wyliczona przez kryterium
     * \details
       ```
        int najlepszaSciezka( drzewo, waga, maksymalizuj )
            jeśli drzewo.maSynów() to
                najlepszySyn = drzewo.pierwszySyn
                max = najlepszaSciezka( najlepszySyn, waga, maksymalizuj )
                syn = najlepszySyn.młodszyBrat

                dopóki syn.maMłodszeRodzeństwo() to
                    gałąź = najlepszaSciezka( syn, waga, maksymalizuj )

                    jeśli maksymalizuj( syn )
                        jeśli gałąź > max to
                            max = gałąź
                            najlepszySyn = syn
                    jeśli nie
                        jeśli gałąź < max to
                            max = gałąź
                            najlepszySyn = syn;

                    syn = syn.młodszyBrat

                drzewo.pozostawTylko( najlepszySyn )
                zwróć max + waga( wartośćKorzenia )

            jeśli nie:
                zwróć waga( wartośćKorzenia )
        ```
    */
    int bestBranch( std::function< int( T ) > fun, std::function< bool( T ) > maxim ){

        if( firstborn != NULL ){
            srand( time(NULL) );
            int best, branch;
            myTree *child, *bestChild;

            bestChild = firstborn;
            best = bestChild->bestBranch( fun, maxim );
            child = bestChild->siblings;

            while( child != NULL ){
                branch = child->bestBranch( fun, maxim );

                if( maxim( value ) ){
                    if( branch >= best ){
                        if( branch == best ){
                            if( (rand() + branch) % 2 ){
                                best = branch;
                                bestChild = child;
                            }
                        }
                        else {
                            best = branch;
                            bestChild = child;
                        }
                    }
                }
                else {
                    if( branch <= best ){
                        if( branch == best ){
                            if( (rand() + branch) % 2 ){
                                best = branch;
                                bestChild = child;
                            }
                        }
                        else {
                            best = branch;
                            bestChild = child;
                        }
                    }
                }

                child = child->siblings;
            }

            this->rmChildApart( bestChild );

            return( best + fun( value ) );
        }
        else return fun( value );
    }

    /**
     * \brief Wyświetla drzewo na std::cout
     * \param i -
     */
    void display( int i = 0 ) const {
        using std::cout;
        using std::endl;

        for(int n=0;n<i;++n) cout << ' ';
        cout << ": " << value << endl;

        myTree* child = firstborn;
        int j=0;

        while( child != NULL ){

            cout << j;
            for(int n=0;n<i;++n) cout << ' ';

            child->display( i+1 );

            child = child->siblings;
            ++j;
        }
    }

};

#endif