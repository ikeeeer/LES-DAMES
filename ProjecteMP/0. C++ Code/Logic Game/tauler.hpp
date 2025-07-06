#pragma once
#ifndef TAULER_H
#define TAULER_H
#include <iostream>
#include <fstream>
#include <sstream>
#include "string"
#include "posicio.hpp"
#include "Fitxa.h"
#include "Moviment.h"
#include <vector>
using namespace std;
class Tauler {
public:
	Fitxa creaFitxaAPartirDeChar(char c, int fila, int columna) const;
	void inicialitza(const string& nomFitxer);
	//void escriuTauler(const string& nomFitxer);
	void actualitzaMovimentsValids();
	Fitxa getFitxa(int fila, int columna) const { return* m_tauler[fila][columna]; }
	void getPosicionsPossiblesDirectes(const Posicio& origen, int& nPosicions,
		vector<Posicio>& posicionsPossibles, TipusFitxa tipusOrigin, ColorFitxa colorOrigin,  Moviment &movimentActual);
	void getPosicionsPossibles(const Posicio& origen, int& nPosicions,
		Posicio posicionsPossibles[]);
	int obtenirDireccio(const Fitxa& fitxa, int fila, int columna, ColorFitxa color) const;
	void possiblesPosicionsNormal(const Fitxa& fitxa, int fila, int col, vector<Posicio>& posicionsPossibles, int& nPosicions, ColorFitxa color);
	void possiblesPosicionsDama(const Fitxa& fitxa, int fila, int col, vector<Posicio>& posicionsPossibles, int& nPosicions, ColorFitxa color,  Moviment &movimentActual);
	bool mouFitxa(const Posicio& origen, const Posicio& desti);
	string toString() const;
	bool estaADinsDelTauler(int fila, int col) const;
	void possiblesPosicionsMatar(const Fitxa& fitxa, int fila, int col, vector<Posicio>& posicionsPossibles, int& nPosicions, ColorFitxa color);
	void possiblesPosicionsMatarDama(const Fitxa& fitxa, int fila, int col, vector<Posicio>& posicionsPossibles, int& nPosicions, Moviment& movimentActual, ColorFitxa colorOrigin, bool primerCop);

	void visualitza(bool fitxaSeleccionada, vector<Posicio> posicionsValides, int posXTauler, int posYTauler, int casellaInicialX, int casellaInicialY, int ampladaCasella, int alcadaCasella);
		Posicio getPosicioTauler(int x, int y) { return m_tauler[x][y]->getPosicio(); }
	bool esBuit(ColorFitxa torn);
	bool maximMovimentsMatats(const Fitxa& fitxa, Moviment mov);


private:
	Fitxa* m_tauler[N_FILES][N_COLUMNES];
	bool  podenMatarBlanques;
	bool  podenMatarNegres;
	Fitxa  fitxaBlancaQuePotMatar;
	Fitxa  fitxaNegraQuePotMatar;
	Posicio posicioABuffar;
	
	bool esMovimentValid(const Posicio origen, const Posicio desti) const;
	void convertirEnDama(Fitxa& fitxa, const Posicio& pos);
	bool validarIEliminarFitxesCapturades(const Fitxa& fitxa, const Posicio& origen, const Posicio& desti, bool &buffar);
	bool esMovimentCapturaMaxima(const Fitxa& fitxa, const Posicio& desti) const;
	void bufarFitxa(const Posicio& pos);
	void marcarMatarSegonsColor(ColorFitxa color, Posicio posicio);
	bool podenMatar(ColorFitxa color) ;
	Fitxa getFitxaQuePotMatar(ColorFitxa color) const {
		return (color == COLOR_BLANC) ? fitxaBlancaQuePotMatar : fitxaNegraQuePotMatar;
	}
};
#endif //TAULER_H