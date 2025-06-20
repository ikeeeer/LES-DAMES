#pragma once
#include "vector"
#include "Moviment.h"
using namespace std;

struct Node {
    Moviment mov;
    Node* seguent;
};
class CuaMoviments
{

public:
    void buida();
    void carregaMovimentsDeFitxer(const string& nomFitxer) {
        buida();
        ifstream fitxer;
        fitxer.open(nomFitxer);
        if (fitxer.isOpen()) {
            while (fitxer.eof()) {

            }
        }
        fitxer.close();
    }

private:
    Node* m_primer;
    Node* m_darrer;
};
