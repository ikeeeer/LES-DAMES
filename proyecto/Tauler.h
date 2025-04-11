#pragma once
#ifndef TAULER_H
#define TAULER_H
#include <fstream>
#include "posicio.h"
using namespace std;
class Tauler {
public:
	void inicialitza(const string& nomFitxer);
	void escriuTauler(const string& nomFitxer);
	void actualitzaMovimentsValids() const;

private:
	char m_tauler[N_FILES][N_COLUMNES];
};

#endif // TAULER_H