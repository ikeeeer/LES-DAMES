//
//  CurrentGame.cpp
//  LearnChess
//
//  Created by Enric Vergara on 21/2/22.
//

#include "joc.hpp"
#include <iostream>
#include <fstream>
#include "GraphicManager.h"




void Joc::menu(ModeJoc& mode) {
	int opcio;
	cout << "Escull el mode de joc:" << endl;
	cout << "1. Mode normal" << endl;
	cout << "2. Mode replay" << endl;
	cin >> opcio;
	if (opcio == 1)
		mode = MODE_JOC_NORMAL;
	else
		mode = MODE_JOC_REPLAY;
}
void Joc::mostraPosMouse(int x, int y) {
	int posTextX = POS_X_TAULER;
	int posTextY = POS_Y_TAULER + (ALCADA_CASELLA * NUM_FILES_TAULER) + 120;
	string msg = "PosX: " + to_string(x) + ", PosY: " + to_string(y);
	GraphicManager::getInstance()->drawFont(FONT_WHITE_30, posTextX, posTextY, 0.8, msg);
}
ColorFitxa Joc::canviaTorn(ColorFitxa tornActual) {
	if (tornActual == COLOR_BLANC)
		return m_torn = COLOR_NEGRE;
	else
		return m_torn = COLOR_BLANC;
}
bool Joc::actualitza(int mousePosX, int mousePosY, bool mouseStatus) 
{
	//Per cada click del mous visulitzem de nou el tauler ja que igual l'hem actualitzat
	GraphicManager::getInstance()->drawSprite(GRAFIC_FONS, 0, 0);
	GraphicManager::getInstance()->drawSprite(GRAFIC_TAULER, POS_X_TAULER, POS_Y_TAULER);
	visualitzaTorn();
	m_tauler.visualitza(m_fitxaSeleccionada, m_posicionsValides, POS_X_TAULER, POS_Y_TAULER, CASELLA_INICIAL_X, CASELLA_INICIAL_Y, AMPLADA_CASELLA, ALCADA_CASELLA);
	if (!mouseStatus) {
		m_clicProcesat = false;
		return false;
	}
	int auxY = (mousePosX - POS_X_TAULER - CASELLA_INICIAL_X) / AMPLADA_CASELLA;
	int auxX = (mousePosY - POS_Y_TAULER - CASELLA_INICIAL_Y) / ALCADA_CASELLA;
	visualitzaTorn();
	//mostraPosMouse(auxX, auxY);
	if (m_tauler.estaADinsDelTauler(auxX,auxY)) {
		Posicio posMouse = m_tauler.getPosicioTauler(auxX, auxY);
		//Mirem si esta dins del tauler i quin mode es
		if (m_mode == MODE_JOC_NORMAL) {
			Fitxa fitxa = m_tauler.getFitxa(auxX, auxY);
			if (fitxa.getColor() == m_torn || fitxa.getTipus() == TIPUS_EMPTY) {
				//Mirem si estem seleccionant la fitxa o la posicio on la volem moure(fitxaSeleccionada logica al reves)
				if (fitxa.getTipus() != TIPUS_EMPTY && !m_fitxaSeleccionada) {
					//Actualitzem els movimentsvalids del tauler i de la fitxa i els afegim a posicionsValides
					m_tauler.actualitzaMovimentsValids();
					m_fitxaSeleccionada = true;
					int nPos = 0;
					Posicio possibles[MAX_POSICIONS];
					fitxa.getPosicionsValides(possibles, nPos);
					for (int i = 0; i < nPos; ++i) {
						m_posicionsValides.push_back(possibles[i]);
					}
					//visualitzem les posicions valides
					m_tauler.visualitza(m_fitxaSeleccionada, m_posicionsValides, POS_X_TAULER, POS_Y_TAULER, CASELLA_INICIAL_X, CASELLA_INICIAL_Y, AMPLADA_CASELLA, ALCADA_CASELLA);
					m_posicioSeleccionada = fitxa.getPosicio();
				}
				else {
					//cas que movem fitxa
					bool movimentValid = false;
					if (m_fitxaSeleccionada && fitxa.getTipus() == TIPUS_EMPTY) {
						//recorrem posicions valides fins trobar la que coincideix amb la posicio del mouse
						for (int i = 0; i < m_posicionsValides.size(); i++) {
							if (posMouse == m_posicionsValides[i]) {
								//Quan trobem la oosicio movem la fitxa a aquella posicio
								m_tauler.mouFitxa(m_posicioSeleccionada, posMouse);
								GraphicManager::getInstance()->drawSprite(GRAFIC_FONS, 0, 0);
								GraphicManager::getInstance()->drawSprite(GRAFIC_TAULER, POS_X_TAULER, POS_Y_TAULER);
								m_tauler.visualitza(m_fitxaSeleccionada, m_posicionsValides, POS_X_TAULER, POS_Y_TAULER, CASELLA_INICIAL_X, CASELLA_INICIAL_Y, AMPLADA_CASELLA, ALCADA_CASELLA);
								Moviment mov(m_posicioSeleccionada, posMouse);
								m_cua.guardaMoviment(mov); //guardem el moviment a la cua de moviments
								//cas de que s'ha acabat la partida/canvia torn
								if (m_tauler.esBuit(canviaTorn(m_torn))) {
									int posTextX = POS_X_TAULER;
									int posTextY = POS_Y_TAULER + (ALCADA_CASELLA * NUM_FILES_TAULER) + 120; 
									string msg = "";
									if(m_torn == COLOR_BLANC)
										msg = "Ha gunayat BLANC!";
									else
										msg = "Ha gunayat NEGRE!";
									GraphicManager::getInstance()->drawFont(FONT_WHITE_30, posTextX, posTextY, 0.8, msg);
									m_partidaAcabada = true;
									finalitza();
									return true;
								}
								//si no s'ha acabat la partida
								m_fitxaSeleccionada = false;
								
								m_posicionsValides.clear();
								m_tauler.actualitzaMovimentsValids();
								movimentValid = true;
								break;
							}
						}
						//si no s'ha trobat la posicio a posicions valides vol dir que es una posicio que no pots anar
						if (!movimentValid) {
							m_fitxaSeleccionada = false;
							m_posicionsValides.clear();
							m_tauler.visualitza(m_fitxaSeleccionada, m_posicionsValides, POS_X_TAULER, POS_Y_TAULER, CASELLA_INICIAL_X, CASELLA_INICIAL_Y, AMPLADA_CASELLA, ALCADA_CASELLA);
						}
					}
				}
			}
		}
		else if (m_mode == MODE_JOC_REPLAY) {
			if (!m_cua.estaBuida() && !m_clicProcesat) {
				Moviment primerMov = m_cua.getPrimer();
				m_cua.eliminaPrimer();
				m_tauler.mouFitxa(primerMov.getInici(), primerMov.getFinal());
				//m_tauler.visualitza(false, m_posicionsValides);
				if (m_tauler.esBuit(canviaTorn(m_torn))) {
					m_partidaAcabada = true;
					GraphicManager::getInstance()->drawSprite(GRAFIC_FONS, 0, 0);
					GraphicManager::getInstance()->drawSprite(GRAFIC_TAULER, POS_X_TAULER, POS_Y_TAULER);
					m_tauler.visualitza(m_fitxaSeleccionada, m_posicionsValides, POS_X_TAULER, POS_Y_TAULER, CASELLA_INICIAL_X, CASELLA_INICIAL_Y, AMPLADA_CASELLA, ALCADA_CASELLA);
					finalitza();
					return true;
				}
				m_tauler.actualitzaMovimentsValids();
				m_clicProcesat = true;
			}
		}
	}
	//TODO 1: Interactuar amb la crida per dibuixar gràfics (sprites).
	// 	      Dibuixar a pantalla el fons i el gràfic amb el tauler buit.
	//------------------------------------------------------------------

	//TODO 1.1 Afegir l'include de GraphicManager --> #include "GraphicManager.h"
	//TODO 1.2 Fer la crida de dibuixar un sprite --> GraphicManager::getInstance()->drawSprite(image, posX, posY);
	//	    Per començar podem cridar el drawSprite amb els params --> 
	//          (GRAFIC_FONS,0,0) i 
	//          (GRAFIC_TAULER, POS_X_TAULER, POS_Y_TAULER)

 


	//TODO 1.3: Dibuixar a pantalla el gràfic amb el tauler i una fitxa blanca a la posició (fila, columna ) del tauler


	//TODO 2: Interactuar amb el ratolí.
	//------------------------------------------------------------------
	//TODO 2.1: Dibuixar la fitxa blanca al tauler només si estem pressionant el botó del ratolí



    //TODO 2.2: Dibuixar la fitxa blanca al tauler només si estem pressionant el botó del ratolí i el ratolí
    // està dins del límits del tauler

 

    //TODO 2.3: Dibuixar la fitxa blanca al tauler només si estem pressionant el botó del ratolí i el ratolí
    // està dins del límits del tauler. Dibuixa la fitxa a la casella on està el ratolí



    //TODO 2.4: Dibuixar la fitxa blanca a la casella on cliquem al ratolí. La fitxa s'ha de mantenir dibuixada
    // a la casella quan deixem de clicar amb el ratolí. Quan cliquem a una altra casella, la fitxa canvia de posició
    // a la nova casella

        
        
    // TODO 3: Imprimir text per pantalla
    //------------------------------------------
    // TODO 3.1: Mostrar la posició actual del ratolí a sota del tauler



	return false;
}
void Joc::inicialitza(ModeJoc mode, const string& nomFitxerTauler, const string& nomFitxerMoviments){
	m_mode = mode;
	m_nomFitxerTauler = nomFitxerTauler;
	m_nomFitxerMoviments = nomFitxerMoviments;
	m_fitxaSeleccionada = false;
	m_tauler.inicialitza(nomFitxerTauler);
	m_tauler.actualitzaMovimentsValids();
	if (mode == MODE_JOC_NORMAL) {
		m_cua.buida();
		m_partidaAcabada = false;

	}
	else if (mode == MODE_JOC_REPLAY) {
		m_cua.carregaMoviments(nomFitxerMoviments);
		m_partidaAcabada = false;
	}
}
void Joc::finalitza() {
	/*int posTextX = POS_X_TAULER;
	int posTextY = POS_Y_TAULER + (ALCADA_CASELLA * NUM_FILES_TAULER) + 200;
	string msg = "";
	if (m_torn == COLOR_BLANC)
		msg = "Ha gunayat BLANC!";
	else
		msg = "Ha gunayat NEGRE!";
	GraphicManager::getInstance()->drawFont(FONT_WHITE_30, posTextX, posTextY, 0.8, msg);
	this_thread::sleep_for(chrono::seconds(3));
	*/
	if (m_mode == MODE_JOC_NORMAL) {
		netejaFitxer(m_nomFitxerMoviments);
		m_cua.carregaMovimentsAFitxer(m_nomFitxerMoviments);
	}
}
void Joc::netejaFitxer(const string& nomFitxer) {
	ofstream fitxer(nomFitxer, ios::out);
	fitxer.close();
}

void Joc::visualitzaTorn() {
	int x = POS_X_TAULER;
	int y = POS_Y_TAULER + (ALCADA_CASELLA * NUM_FILES_TAULER) + 170;
	string mode = "";
	if (m_mode == MODE_JOC_NORMAL)
		mode = "MODE NORMAL";
	else
		mode = "MODE REPLAY";
	GraphicManager::getInstance()->drawFont(FONT_WHITE_30, x, y, 0.8, mode);

	int posTextX = POS_X_TAULER;
	int posTextY = POS_Y_TAULER + (ALCADA_CASELLA * NUM_FILES_TAULER) + 120;
	string msg = "";
	if (m_torn == COLOR_BLANC)
		msg = "Torn de blanques";
	else
		msg = "Torn de negres";
	GraphicManager::getInstance()->drawFont(FONT_WHITE_30, posTextX, posTextY, 0.8, msg);
}




