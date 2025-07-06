#pragma once
#ifndef POSICIO_H
#define POSICIO_H
#include <string>
#include <fstream>
#include <vector>

using namespace std;
const int N_FILES = 8;
const int N_COLUMNES = 8;
class Posicio
{
public:
	Posicio() : m_fila(0), m_columna(0), mata(false) {}
	Posicio(const string& pos) 
	{
		m_fila = (N_FILES - 1) - (pos[1] - '1');
		m_columna = pos[0] - 'a';
		mata  = false; // Inicialitzem mata a false
	}
	Posicio(int fila, int columna) : m_fila(fila), m_columna(columna) { mata = false;}
	int getFila() const { return m_fila; }
	int getColumna() const { return m_columna; }
	void setFila(int fila) { m_fila = fila; }
	void setColumna(int col) { m_columna = col; }
	string toString() const {
		string posicio = "a1";
		posicio[0] = 'a' + m_columna;
		posicio[1] = '1' + (N_FILES - 1 - m_fila);
		return posicio;
	}
	bool mataFitxa() const { return mata; }
	void setMataFitxa(bool mataFitxa) { mata = mataFitxa; }
	bool operator==(const Posicio& posicio) const { 
		return m_fila == posicio.m_fila && m_columna == posicio.m_columna; }
	Posicio& operator=(const Posicio& pos) {
		if (this != &pos) {
			m_fila = pos.m_fila;
			m_columna = pos.m_columna;
		}
		return *this;
	}
	bool operator!=(const Posicio& pos) const {
		return !(pos.m_fila == m_fila && pos.m_columna == m_columna);
	}


private:
	int m_fila, m_columna;
	bool mata;
};

ostream& operator<<(ostream& out, const Posicio& pos);

#endif // POSICIO_H