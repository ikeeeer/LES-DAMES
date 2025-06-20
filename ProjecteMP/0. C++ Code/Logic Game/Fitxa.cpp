#include "Fitxa.h"

void Fitxa::afegeixMoviment(const Moviment& moviment) {
	if (m_nMovimentsValids < MAX_MOVIMENTS) {
		m_movimentsValids.push_back(moviment);
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

bool Fitxa::teMovimentsMatarDiferents(int i) const {
	for (int j= 0; j< m_nMovimentsValids; j++) {
		if ((m_movimentsValids[j].getHeMatat() && j != i) && m_movimentsValids[j].getNumSalts() > m_movimentsValids[i].getNumSalts()){
			return true;
		}
	}
	return false;
}
void Fitxa::visualitza(int x, int y, ColorFitxa color, TipusFitxa tipus) {
	if (tipus != TIPUS_EMPTY) {
		if (color == COLOR_BLANC) {
			if (tipus == TIPUS_NORMAL) {
				GraphicManager::getInstance()->drawSprite(GRAFIC_FITXA_BLANCA, x, y);
			}
			else {
				GraphicManager::getInstance()->drawSprite(GRAFIC_DAMA_BLANCA, x, y);
			}
		}
		else if (color == COLOR_NEGRE) {
			if (tipus == TIPUS_NORMAL) {
				GraphicManager::getInstance()->drawSprite(GRAFIC_FITXA_NEGRA, x, y);
			}
			else {
				GraphicManager::getInstance()->drawSprite(GRAFIC_DAMA_NEGRA, x, y);
			}
		}
	}
}
