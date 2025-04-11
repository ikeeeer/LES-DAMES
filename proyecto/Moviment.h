#pragma once
#include "Posicio.h"
const int MAX_SALTS = 12; //cas màxim de captures possibles(capturar totes les peçes contraries en 1 moviment)
class Moviment
{
public:
	Moviment() { m_nSalts = 0; m_nFitxes = 0; }
	Moviment(const Posicio& posicio) { m_posicio_incial = posicio; m_nSalts = 0; m_nFitxes = 0; }
	void afageixPas(const Posicio& posicio);
	void afageixFitxaEliminiada(const Posicio& posicio_fitxa);
	Posicio getInici() const { return m_posicio_incial; }
	Posicio getFinal() const { return m_trajecte[m_nSalts - 1]; }
	bool esSalt() const { return m_nFitxes > 0; }
private:
	Posicio m_posicio_incial;
	Posicio m_trajecte[MAX_SALTS+1]; //salts maxims + la posicio inicial
	int m_nSalts;
	Posicio m_fitxes_eliminades[MAX_SALTS];
	int m_nFitxes;
};