#pragma once
#include "Fitxa.h"
#include "Posicio.h"

class ChessBoard {
public:
    ChessBoard();

    bool mouFitxa(const Posicio& origen, const Posicio& desti);
    // Mueve la ficha si el movimiento es valido, y haria todas las cosas que se le pide (mover dama, eliminar piezas...), si no es valido devuelve false

    string toString() const;
    // Muestra el estado del tablero como en el undir la flota: X/D/O/R/_

private:
    Fitxa m_tauler[N_FILES][N_COLUMNES];

    bool esMovimentValid(const Posicio& origen, const Posicio& desti) const;
    // comprueba si desde la posicion origen se puede ir a la destino
    // Iker-->creo que ya hay una funcion asi en tauler, pero no lo se

    void convertirEnDama(Fitxa& fitxa, const Posicio& pos);
    // convierte en dama (se cambiaria en la clase TipusFitxa del tipo normal al tipo dama)

    void eliminarFitxesCapturades(const Fitxa& fitxa, const Posicio& origen, const Posicio& desti);
    // Calcula quines fitxes han estat capturades durant el moviment i les elimina

    void comprobaMoviment(const Fitxa& fitxa, const Posicio& origen, const Posicio& desti);
    // comprueba si el jugador ha hecho el mejor movimiento y bufa la ficha si no es el caso
    // abajo he dividido las funciones en dos, por si no quieres hacerlo todo en una

    /*
    bool movimentEsCapturaMaxima(const Fitxa& fitxa, const Posicio& desti) const;
    // Comprova si el moviment fet és el que mata més peces (el millor moviment possible)

    void bufarFitxa(const Posicio& pos);
    // Elimina la fitxa que hi ha a la posició indicada (penalització per no fer el moviment correcte)
    */


    void eliminaFitxa(const Posicio& pos);
    // elmina ficha del tablero que se ha matado (de X/O/D/R -->)
};