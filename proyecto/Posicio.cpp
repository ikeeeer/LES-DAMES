#include "posicio.hpp"
ostream& operator<<(ostream& out, const Posicio& pos) {
	out << pos.toString();
	return out;
}

/*Posicio::Posicio()
{
	m_fila = 0;
	m_columna = 0;
}
Posicio::Posicio(int fila, int columna)
{
	m_fila = fila;
	m_columna = columna;
}
string Posicio::toString() const
{
	string posicio = "a1";
	posicio[0] = 'a' + m_columna;
	posicio[1] = '1' + (N_FILES - 1 - m_fila);
	return posicio;
}
Posicio::Posicio(const string& pos)
{
	m_fila = (N_FILES - 1) - (pos[1] - '1');
	m_columna = pos[0] - 'a';
}
*/