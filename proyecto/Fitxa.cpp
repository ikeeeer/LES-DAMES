#include "Fitxa.h"

void Fitxa::afegeixMoviment(const Moviment& moviment) {
	if (m_nMovimentsValids < MAX_MOVIMENTS) {
		m_movimentsValids[m_nMovimentsValids] = moviment;
		m_nMovimentsValids++;
	}
}
void Fitxa::getPosicionsValides(Posicio posValides[], int& nPosValides) {
	nPosValides = 0;
	for (int i = 0; i < m_nMovimentsValids; i++) {
		int nSalts = m_movimentsValids[i].getNumSalts();
		for (int j = 0; j < nSalts; j++) {
			posValides[nPosValides++] = m_movimentsValids[i].getSalt(j);
		}
	}
	eliminaRepetides(posValides,nPosValides);
}
void Fitxa::eliminaRepetides(Posicio posValides[], int& nPosValides) {
	Posicio posAux[MAX_POSICIONS];
	for (int i = 0; i < nPosValides; i++) {
		int j = i + 1;
		while(j < nPosValides){
			if (posValides[i] == posValides[j]) {
				for (int k = j; k < nPosValides - 1; ++k)
					posValides[k] = posValides[k + 1];
				--nPosValides;
			}
			else
				j++;
		}
	}
}
bool Fitxa::teMoviments(const int fila, const int col) const{
	for (int i = 0; i < m_nMovimentsValids; i++) {
		if (m_movimentsValids[i].contePosicio(fila, col))
			return true;
	}
	return false;
}