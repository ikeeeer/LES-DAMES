#pragma once
#include "posicio.hpp"
const int MAX_SALTS = 12; //cas màxim de captures possibles(capturar totes les peçes contraries en 1 moviment)
class Moviment
{
public:
	Moviment() { m_nSalts = 0; m_nFitxes = 0; heMatat = false; }
	Moviment(const Posicio& posicio) { m_posicio_incial = posicio; m_nSalts = 0; m_nFitxes = 0; heMatat = false; }
	Moviment(const Posicio& posicioIni, const Posicio& posicioFinal) {
		m_posicio_incial = posicioIni;
		m_nSalts = 1;
		m_trajecte.push_back(posicioFinal);
	}
	void afageixPas(const Posicio& posicio);
	//void afageixFitxaEliminiada(const Posicio& posicio_fitxa);
	Posicio getInici() const { return m_posicio_incial; }
	Posicio getFinal() const { if (m_trajecte.size() == 0) return m_posicio_incial; 
	return m_trajecte[m_trajecte.size() - 1]; }
	bool esSalt() const { return m_nFitxes > 0; }
	bool esBuit() const;
	Moviment duplicaMoviment() const;
	//void afegeixMoviments(Tauler& tauler, Moviment moviments[], int& nMoviments) const;
	int getNumSalts() const { return m_nSalts; }
	Posicio getSalt(int posicio) const { return m_trajecte[posicio]; }
	bool contePosicio(const int fila, const int col) const;
	void setInici(const Posicio posInici) { m_posicio_incial = posInici; }
	bool getHeMatat() const { return heMatat; }
	void setHeMatat(bool matat) { heMatat = matat; }


private:
	Posicio m_posicio_incial;
	vector<Posicio> m_trajecte; //salts maxims + la posicio inicial
	int m_nSalts;
	//Posicio m_fitxes_eliminades[MAX_SALTS];
	int m_nFitxes;
	bool heMatat;
};