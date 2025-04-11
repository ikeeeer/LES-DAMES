#include <iostream>
#include <fstream>
#include "string"
#include "Tauler.h"
using namespace std;
void Tauler::inicialitza(const string& nomFitxer)
{
	ifstream fitxer;
	fitxer.open(nomFitxer);
	if (fitxer.is_open()) {
		for (int i = 0; i < N_FILES; i++) {
			for (int j = 0; j < N_COLUMNES; j++) { //inicialitzem tot el tauler buit
				m_tauler[i][j] = ' ';
			}
		}
		char valor; 
		string pos;
		while (fitxer >> valor >> pos) { //llegeix els valors del fitxer
			char tipusFitxa = pos[0]; //diferenciem el tipus i posicio de la fitxa
			char posicioFitxa = pos[1];
			int col = tipusFitxa - 'a'; //ho posem a dins dels marges 0-7 en comptes de 1-8
			int fila = 8 - (posicioFitxa - '0');
			if (fila >= 0 && col >= 0 && col < N_COLUMNES && fila < N_FILES){
				m_tauler[fila][col] = valor;
		}
	}
	fitxer.close();
}
}
void Tauler::escriuTauler(const string& nomFitxer)
{
	ofstream fitxer;
	fitxer.open(nomFitxer);
	if (fitxer.is_open()) {
		for (int i = 0; i < N_FILES; i++) {
			for (int j = 0; j < N_COLUMNES; j++) {
				if (m_tauler[i][j] != ' ') {
					//covertir els indexs a posicions del tauler
					char col = 'a' + j;  //columna a la lletra del tauler
					char fila = '0' + (8 - i); //la fila 0 correspon a la 8 i aixi succesivament per tant convertim l'index a la numeració del tauler
					fitxer << m_tauler[i][j] << " " << col << fila << endl;
				}
			}
		}
		fitxer.close();
	}
}
void Tauler::actualitzaMovimentsValids() const {

}
