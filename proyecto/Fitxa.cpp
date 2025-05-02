#include "Fitxa.h"

void Fitxa::afegeixMoviment(const Moviment& moviment) {
	if (m_nMovimentsValids < MAX_MOVIMENTS) {
		m_movimentsValids[m_nMovimentsValids] = moviment;
		m_nMovimentsValids++;
	}
}