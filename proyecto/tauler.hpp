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
using namespace std;
class Tauler {
public:
	Fitxa creaFitxaAPartirDeChar(char c, int fila, int columna) const;
	void inicialitza(const string& nomFitxer);
	//void escriuTauler(const string& nomFitxer);
	void actualitzaMovimentsValids();
	Fitxa getFitxa(int fila, int columna) const { return m_tauler[fila][columna]; }
	void getPosicionsPossibles(const Posicio& origen, int& nPosicions,
		Posicio posicionsPossibles[]);
	int obtenirDireccio(const Fitxa& fitxa, int fila, int columna) const;
	void possiblesPosicionsNormal(const Fitxa& fitxa, int fila, int col, Posicio posicionsPossibles[], int& nPosicions, ColorFitxa color);
	void possiblesPosicionsDama(const Fitxa& fitxa, int fila, int col, Posicio posicionsPossibles[], int& nPosicions, ColorFitxa color);
	bool mouFitxa(const Posicio& origen, const Posicio& desti);
	string toString() const;
	bool estaADinsDelTauler(int fila, int col);

private:
	Fitxa m_tauler[N_FILES][N_COLUMNES];

	bool esMovimentValid(const Posicio& origen, const Posicio& desti) const;
	void convertirEnDama(Fitxa& fitxa, const Posicio& pos);
	void eliminarFitxesCapturades(const Fitxa& fitxa, const Posicio& origen, const Posicio& desti);
	bool esMovimentCapturaMaxima(const Fitxa& fitxa, const Posicio& desti) const;
	void bufarFitxa(const Posicio& pos);
};
#endif //TAULER_H