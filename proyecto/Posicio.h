#pragma once
#ifndef POSICIO_H
#define POSICIO_H
#include <string>
#include <fstream>
using namespace std;
const int N_FILES = 8;
const int N_COLUMNES = 8;
class Posicio
{
public:
	Posicio();
	Posicio(const string& pos);
	Posicio(int fila, int columna);
	int getFila() const { return m_fila; }
	int getColumna() const { return m_columna; }
	string toString() const;
private:
	int m_fila, m_columna;
};
#endif // POSICIO_H