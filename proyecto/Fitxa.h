#pragma once
#include "posicio.hpp"
#include "Moviment.h"
typedef enum {
	TIPUS_NORMAL,
	TIPUS_DAMA,
	TIPUS_EMPTY
} TipusFitxa;

typedef enum
{
	COLOR_NEGRE,
	COLOR_BLANC,
	COLOR_DEFAULT
} ColorFitxa;

const int MAX_MOVIMENTS = 50;
const int MAX_POSICIONS = 32;


class Fitxa
{
public:
	Fitxa() : m_posicio(0,0), m_tipus(TIPUS_EMPTY), m_nMovimentsValids(0), m_color(COLOR_DEFAULT) {}
	Fitxa(const Posicio& pos, TipusFitxa tipus) : m_posicio(pos), m_tipus(tipus), m_nMovimentsValids(0), m_color(COLOR_DEFAULT) {}
	Fitxa(const Posicio& pos, ColorFitxa color, TipusFitxa tipus) : m_posicio(pos), m_color(color), m_tipus(tipus), m_nMovimentsValids(0) {}
	
	int getNMovimentsValids() const { return m_nMovimentsValids; }
	void setTipus(TipusFitxa tipus) { m_tipus = tipus; }
	void afegeixMoviment(const Moviment& moviment);
	void netejaMoviments() { m_nMovimentsValids = 0; }
	bool esBuida() { return m_tipus == TIPUS_EMPTY; }
	ColorFitxa getColor() const { return m_color; }
	Posicio getPosicio() const { return m_posicio; }
	TipusFitxa getTipus() const { return m_tipus; }



private:
	Posicio m_posicio;
	ColorFitxa m_color;
	TipusFitxa m_tipus;
	Moviment m_movimentsValids[MAX_MOVIMENTS];
	int m_nMovimentsValids;
};