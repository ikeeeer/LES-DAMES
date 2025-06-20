#include "Moviment.h"
/*void Moviment::afegeixMoviments(Tauler& tauler, Moviment movimentsValids[], int& nMoviments) const {
    
    Moviment movimentsPendents[MAX_MOVIMENTS];
    int inici = 0;
    int fi = 0;
    Moviment movimentActual = *this;
    movimentsPendents[fi++] = movimentActual; // Afegim el moviment inicial a la cua
    Posicio posActual = getInici();

    do
    {
        movimentActual = movimentsPendents[inici++];
        Posicio posValides[MAX_POSICIONS];
        int nPosValides = 0;
        tauler.getPosicionsPossibles(posActual, nPosValides, posValides);
        while (nPosValides > 0)
        {
            // Afegim la primera posici� v�lida al movimentActual
            movimentActual.afageixPas(posValides[0]);

            // Per la resta de posicions v�lides, dupliquem moviment
            for (int i = 1; i < nPosValides; i++)
            {
                Moviment movimentPendent = movimentActual; // dupliquem
                movimentPendent.afageixPas(posValides[i]);
                movimentsPendents[fi++] = movimentPendent;
            }
            posActual = posValides[0];
            movimentsPendents[fi++] = movimentActual;
            tauler.getPosicionsPossibles(posActual, nPosValides, posValides);
        }
        if (movimentActual.getInici().getFila() != movimentActual.getFinal().getFila() ||
            movimentActual.getInici().getColumna() != movimentActual.getFinal().getColumna())
            movimentsValids[nMoviments++] = movimentActual;
    } while (inici < fi);
}*/
void Moviment::afageixPas(const Posicio& posicio) {

    if (m_nSalts < MAX_SALTS)
    {
        m_trajecte.push_back(posicio);
        m_nSalts++;
        if (posicio.mataFitxa()) {
            heMatat = true; //marquem que ha matat una fitxa
            m_nFitxes++;
        }
    }
}
bool Moviment::esBuit() const {
    return (m_nSalts == 0);
    //if (m_nSalts == 0)
      //  return true;
    //return //!(m_posicio_incial.getFila() != m_trajecte[m_nSalts - 1].getFila() ||
      //  m_posicio_incial.getColumna() != m_trajecte[m_nSalts - 1].getColumna());
}
Moviment Moviment::duplicaMoviment() const {
    Moviment copia;
    copia.m_posicio_incial = m_posicio_incial;
    copia.m_nSalts = m_nSalts;
    copia.m_nFitxes = m_nFitxes;
    copia.heMatat = heMatat;
    for (int i = 0; i < m_trajecte.size(); i++)
        copia.m_trajecte.push_back(m_trajecte[i]);
    return copia;
}
bool Moviment::contePosicio(const int fila, const int col) const {
    if (m_posicio_incial.getFila() == fila && m_posicio_incial.getColumna() == col)
        return true;
    for (int i = 0; i < m_nSalts; i++) {
        if (m_trajecte[i].getFila() == fila && m_trajecte[i].getColumna() == col)
            return true;
    }
    return false;
}