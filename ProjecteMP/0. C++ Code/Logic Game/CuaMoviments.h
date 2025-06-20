#pragma once
#include "Moviment.h"
class Node {
public:
	Moviment getMov() { return m_mov; }
	Node* getNext() { return m_seguent; }
	void setMov(Moviment mov) { m_mov = mov; }
	void setNext(Node* next) { m_seguent = next; }
private:
	Moviment m_mov;
	Node* m_seguent;
};
class CuaMoviments {
public:
	void buida();
	void carregaMoviments(const string& nomFitxer);
	void carregaMovimentsAFitxer(const string& nomFitxer);
	void guardaMoviment(Moviment mov);
	bool estaBuida() { return m_next == nullptr; }
	Moviment getPrimer() { return m_next->getMov(); }
	void eliminaPrimer();
private:
	Node* m_next;
};