#pragma once
#include "posicio.hpp"
#include "Moviment.h"
#include "GraphicManager.h"
typedef enum {
	TIPUS_NORMAL,
	TIPUS_DAMA,
	TIPUS_EMPTY
} TipusFitxa;

typedef enum
{
	COLOR_BLANC,
	COLOR_NEGRE,
	COLOR_DEFAULT
} ColorFitxa;

const int MAX_MOVIMENTS = 50;
const int MAX_POSICIONS = 32;


class Fitxa
{
public:
	Fitxa() : m_posicio(0,0),  m_color(COLOR_DEFAULT), m_tipus(TIPUS_EMPTY), m_nMovimentsValids(0) {}
	Fitxa(const Posicio pos, TipusFitxa tipus) : m_posicio(pos), m_color(COLOR_DEFAULT), m_tipus(tipus), m_nMovimentsValids(0) {}
	Fitxa(const Posicio pos, ColorFitxa color, TipusFitxa tipus) : m_posicio(pos), m_color(color), m_tipus(tipus), m_nMovimentsValids(0) {}
	Fitxa(const Posicio pos) { m_posicio = pos; }

	int getNMovimentsValids() const { return m_nMovimentsValids; }
	void setTipus(TipusFitxa tipus) { m_tipus = tipus; }
	void afegeixMoviment(const Moviment& moviment);
	void netejaMoviments() { m_nMovimentsValids = 0; m_movimentsValids.clear(); }
	Moviment getMoviment(int posicio) const { return m_movimentsValids[posicio]; }
	bool esBuida() { return m_tipus == TIPUS_EMPTY; }
	ColorFitxa getColor() const { return m_color; }
	Posicio getPosicio() const { return m_posicio; }
	TipusFitxa getTipus() const { return m_tipus; }
	void getPosicionsValides(Posicio posValides[], int& nPosValides);
	void eliminaRepetides(Posicio posValides[], int& nPosValides);
	bool teMoviments(const int fila, const int col) const;
	bool teMovimentsMatarDiferents(int i) const;
	void visualitza(int x, int y, ColorFitxa color, TipusFitxa tipus);
	int getTorn() const { return m_jugador; }
	void setNMovimentsValids(int n) { m_nMovimentsValids = n;  }
	void setPosicio(const Posicio& p) { m_posicio = p; }
	void setColor(const ColorFitxa& color) { m_color = color; }

	
private:
	Posicio m_posicio;
	ColorFitxa m_color;
	TipusFitxa m_tipus;
	vector<Moviment> m_movimentsValids;
	int m_nMovimentsValids;
	int m_jugador;
};