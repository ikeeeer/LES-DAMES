#pragma once
#include "Posicio.h"
#include "Moviment.h"
typedef enum {
	TIPUS_NORMAL,
	TIPUS_DAMA,
	TIPUS_EMPTY
} TipusFitxa;

typedef enum
{
	COLOR_NEGRE,
	COLOR_BLANC
} ColorFitxa;
const int MAX_MOVIMENTS = 1000;
class Fitxa
{
public:
	
private:
	Posicio m_posicio;
	ColorFitxa m_color;
	TipusFitxa m_tipus;
	Moviment movimentsValids[MAX_MOVIMENTS];
	int m_nMovimentsValids;
};