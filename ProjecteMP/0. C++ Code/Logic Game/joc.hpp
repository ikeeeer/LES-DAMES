#ifndef JOC_H
#define JOC_H

#include <stdio.h>
#include "info_joc.hpp"

using namespace std;


class Joc 
{

public:
    Joc() : m_partidaAcabada(false), m_fitxaSeleccionada(false), m_nomFitxerTauler(""), m_torn(COLOR_BLANC) {}
    void inicialitza(ModeJoc mode, const string& nomFitxerTauler, const string& nomFitxerMoviments);
    bool actualitza(int mousePosX, int mousePosY, bool mouseStatus);
    void finalitza();
    void menu(ModeJoc& mode);
    ModeJoc& getMode() { return m_mode; }
    void mostraPosMouse(int x, int y);
    ColorFitxa canviaTorn(ColorFitxa tornActual);
    void netejaFitxer(const string& nomFitxer);
    void visualitzaTorn();
    ColorFitxa getTorn() { return m_torn; }



    
    
private:
    Tauler m_tauler;
    CuaMoviments m_cua;
    ModeJoc m_mode;
    bool m_partidaAcabada;
    bool m_fitxaSeleccionada;
    string m_nomFitxerTauler;
    string m_nomFitxerMoviments;
    Posicio m_posicioSeleccionada;
    vector<Posicio> m_posicionsValides;
    ColorFitxa m_torn;
    bool m_clicProcesat = false;

};

#endif 
