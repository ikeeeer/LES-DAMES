#include "tauler.hpp"

void Tauler::inicialitza(const string& nomFitxer)
{
	ifstream fitxer;
	fitxer.open(nomFitxer);
	if (fitxer.is_open()) {
		for (int i = 0; i < N_FILES; i++) {
			for (int j = 0; j < N_COLUMNES; j++) { //inicialitzem tot el tauler buit
				Fitxa* f = new Fitxa(Posicio(i, j), TIPUS_EMPTY);
				m_tauler[i][j] = f; //posem totes les fitxes a buides
			}
		}
		char valor;
		string pos;
		while (fitxer >> valor >> pos) { //llegeix els valors del fitxer
			char tipusFitxa = pos[0]; //diferenciem el tipus i posicio de la fitxa
			char posicioFitxa = pos[1];
			int col = tipusFitxa - 'a'; //ho posem a dins dels marges 0-7 en comptes de 1-8
			int fila = 8 - (posicioFitxa - '0');
			if (fila >= 0 && col >= 0 && col < N_COLUMNES && fila < N_FILES) {
				*m_tauler[fila][col] = creaFitxaAPartirDeChar(valor, fila, col); //pasar de tipus char a tipusFitxa
			}
		}
		fitxer.close();
	}
}
Fitxa Tauler::creaFitxaAPartirDeChar(char c, int fila, int columna) const
{
	Posicio pos(fila, columna);
	switch (c)
	{
	case '.':
		return Fitxa(pos, TIPUS_EMPTY); // fitxa buida
	case 'O':
		return Fitxa(pos, COLOR_BLANC, TIPUS_NORMAL); // fitxa blanca normal
	case 'D':
		return Fitxa(pos, COLOR_BLANC, TIPUS_DAMA);   // dama blanca
	case 'X':
		return Fitxa(pos, COLOR_NEGRE, TIPUS_NORMAL); // fitxa negra normal
	case 'R':
		return Fitxa(pos, COLOR_NEGRE, TIPUS_DAMA);   // dama negra
	default:
		return Fitxa(pos, TIPUS_EMPTY); // si el caràcter no es coneix, deixem buit
	}
}
/*void Tauler::escriuTauler(const string& nomFitxer)
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
}*/
void Tauler::actualitzaMovimentsValids() {
	//metode purament basat en l'algorisme de la presentacio
	//per totes les posicions del tauler
	podenMatarBlanques = false; //inicialitzem les variables de si poden matar a cada actualitzar
	podenMatarNegres = false;
	for (int fila = 0; fila < N_FILES; fila++) {
		for (int col = 0; col < N_COLUMNES; col++) {
			//primer creem la fitxa basada en la posicio del tauler, i també dos arrays de moviments pendents i moviments valids
			Fitxa& fitxa = *m_tauler[fila][col];
			fitxa.netejaMoviments();
			if (!fitxa.esBuida()) { //mirem que hi hagi una fitxa
				TipusFitxa tipusOrigin = fitxa.getTipus();
				ColorFitxa colorOrigin = fitxa.getColor();
				vector<Moviment> movimentsPendents;
				
				int nMovimentsPendents = 0;
				

				Moviment movimentActual; //creem el primer moviment
				Posicio posActual(fila, col); //agafem la posicio actual de la fitxa, per després mirar els moviments possibles
				movimentActual.setInici(posActual); //li afegim la posicio actual al moviment
				movimentsPendents.push_back(movimentActual); // Afegim el moviment inicial a la cua de moviments pendents
				nMovimentsPendents++;
				//aquests pasos són només per el primer movimentActual, deprés en el bucle ja és va iterant

				//recorrem tot el array de movimentsPendents fins que estigui buida
				do
				{
					movimentActual = movimentsPendents[movimentsPendents.size() - 1]; //li assignem al moviment actual el primer, després el segon i així consecutivament
					nMovimentsPendents--;


					posActual = movimentActual.getFinal(); //la posicio actual serà l'última dels moviments fets
					vector<Posicio> posValides;
					int nPosValides = 0;
					getPosicionsPossiblesDirectes(posActual, nPosValides, posValides, tipusOrigin, colorOrigin, movimentActual); //agafem totes les possibles posicions desde la posicioActual
					int iniciPosValida = 0;
					//mentre hi hagi posicions vàlides, recorrem posValides
					while (nPosValides > 0){
						// Afegim la primera posició vàlida al movimentActual
						movimentActual.setHeMatat(posValides[iniciPosValida].mataFitxa()); //marquem en moure a aquesta posicio, si ha matat alguna fitxa
						movimentActual.afageixPas(posValides[iniciPosValida]);
						iniciPosValida++;

						// Per la resta de posicions vàlides, dupliquem moviment i les afegim a moviments pendents
						for (int i = 1; i < nPosValides; i++)
						{
							Moviment movimentPendent = movimentActual.duplicaMoviment();
							movimentPendent.setHeMatat(posValides[i].mataFitxa());
							movimentPendent.afageixPas(posValides[i]);
							movimentsPendents.push_back(movimentPendent);
							nMovimentsPendents++;
						}
						//posicioActual amb la primer posicio valida
						posActual = posValides[iniciPosValida - 1];
						iniciPosValida--;

						// Afegim el moviment actual ja modificat a la cua de pendents per seguir expandint-lo
						posValides.clear();
						getPosicionsPossiblesDirectes(posActual, nPosValides, posValides, tipusOrigin, colorOrigin, movimentActual); //agafem totes les possibles posicions desde la posicioActual i actualitzem nPosValides
					}
					// Si el moviment no està buit (ha fet algun pas), l'afegim com a vàlid
					if (!movimentActual.esBuit())
						fitxa.afegeixMoviment(movimentActual);

				} while (nMovimentsPendents>0); //continuem mentre el array de moviments pendents no s'hagi acabat

			}
		}
	}
}
void Tauler::getPosicionsPossibles(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[]) {
	Fitxa& fitxa = *m_tauler[origen.getFila()][origen.getColumna()];
	fitxa.getPosicionsValides(posicionsPossibles,nPosicions);
}
void Tauler::getPosicionsPossiblesDirectes(const Posicio& origen, int& nPosicions, vector<Posicio>& posicionsPossibles,
	TipusFitxa tipusOrigin, ColorFitxa colorOrigin, Moviment &movimentActual) {

	nPosicions = 0;
	int fila = origen.getFila(); //agafem les coordenades del origen
	int col = origen.getColumna();

	Fitxa fitxa = getFitxa(fila, col);
	ColorFitxa color = fitxa.getColor();
	TipusFitxa tipus = fitxa.getTipus();
	if (tipus == TIPUS_NORMAL)
		possiblesPosicionsNormal(fitxa, fila, col, posicionsPossibles, nPosicions, color);
	else if (tipus == TIPUS_DAMA || tipusOrigin == TIPUS_DAMA) {
		possiblesPosicionsDama(fitxa, fila, col, posicionsPossibles, nPosicions, colorOrigin, movimentActual);
	}
	else if (tipusOrigin == TIPUS_NORMAL && movimentActual.getHeMatat()) {
		possiblesPosicionsMatar(fitxa, fila, col, posicionsPossibles, nPosicions, colorOrigin);
	}

}

int Tauler::obtenirDireccio(const Fitxa& fitxa, int fila, int columna, ColorFitxa color) const
{
	if (color == COLOR_BLANC)
		return -1;
	else
		return 1;
}
void Tauler::possiblesPosicionsNormal(const Fitxa& fitxa, int fila, int col, vector<Posicio>& posicionsPossibles, int& nPosicions, ColorFitxa color) {
	int direccio[2][2] = { {obtenirDireccio(fitxa, fila, col, color), -1}, {obtenirDireccio(fitxa, fila, col, color), 1} };
	//obtenim el color de la fitxa i restem o sumem depenent de si es blanc o negre

	//fitxa normal, sense captura

	for (int i = 0; i < 2; i++)
	{
		int f = fila + direccio[i][0]; //agafem la fila i columna i els hi sumem els casos adalt dreta/esquerra o abaix dreta/esquerra
		int c = col + direccio[i][1];
		if (estaADinsDelTauler(f,c) && getFitxa(f, c).esBuida()) {
			posicionsPossibles.push_back(Posicio(f, c)); //afegim la posicio si es possible
			nPosicions++;
		}
	}
	//fitxa normal, amb captura
	possiblesPosicionsMatar(fitxa, fila, col, posicionsPossibles, nPosicions, color);
}
void Tauler::possiblesPosicionsMatar(const Fitxa& fitxa, int fila, int col, vector<Posicio>& posicionsPossibles, int& nPosicions, ColorFitxa color) {
	int direccio[2][2] = { {obtenirDireccio(fitxa, fila, col,color), -1}, {obtenirDireccio(fitxa, fila, col,color), 1} };
	for (int i = 0; i < 2; i++)
	{
		int f = fila + direccio[i][0]; //agafem la fila i columna i els hi sumem els casos adalt dreta/esquerra o abaix dreta/esquerra
		int c = col + direccio[i][1];
		int f2 = fila + 2 * direccio[i][0]; // posició després (ha d'estar buida)
		int c2 = col + 2 * direccio[i][1];
		if (estaADinsDelTauler(f, c) && estaADinsDelTauler(f2, c2)) {
			Fitxa rival = getFitxa(f, c); //creem una possible fitxa rival a la posicio on es mou la peça de normal
			Fitxa desti = getFitxa(f2, c2); //creem la possible fitxa on acabarà
			if (!rival.esBuida() && rival.getColor() != color && desti.esBuida()) { //si el rival exsiteix de veritat i el desti és buit, agragem
				Posicio posicioMatar(f2, c2);
				posicioMatar.setMataFitxa(true); //marquem que mata una fitxa
				posicionsPossibles.push_back(posicioMatar); //afegim la posicio de matar
				nPosicions++;
				marcarMatarSegonsColor(fitxa.getColor(), Posicio(fila, col)); //marquem que pot matar segons el color
			}

		}
	}
}
bool Tauler::estaADinsDelTauler(int fila, int col) const {
	return (fila >= 0 && fila < N_FILES && col >= 0 && col < N_COLUMNES);
}
void Tauler::possiblesPosicionsDama(const Fitxa& fitxa, int fila, int col, vector<Posicio>& posicionsPossibles, int& nPosicions, ColorFitxa color,  Moviment& movimentActual) {
	int dirDama[4][2] = { {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };
	if (movimentActual.getNumSalts() == 0 || movimentActual.getHeMatat()) {

		// Primer moviment: explorar totes les direccions
		for (int i = 0; i < 4; ++i) {
			int f = fila + dirDama[i][0];
			int c = col + dirDama[i][1];
			//mirar posicions possibles consecutives
			if(estaADinsDelTauler(f, c) && !movimentActual.contePosicio(f, c)) {
				if (getFitxa(f, c).esBuida() && !movimentActual.getHeMatat()) {
					// Cas que no he matat cap fitxa
					// i la posició està buida, afegim la posició
					posicionsPossibles.push_back(Posicio(f, c));
					nPosicions++;
				}
				else {
					// Si hi ha una fitxa rival, mirar si es pot matar
					// i afegir la posició de salt
					
					if (getFitxa(f, c).getColor() != color && !getFitxa(f, c).esBuida()) {
						int fAux = f + dirDama[i][0];
						int cAux = c + dirDama[i][1];
						if (estaADinsDelTauler(fAux, cAux) && getFitxa(fAux, cAux).esBuida() && !movimentActual.contePosicio(fAux, cAux)) {
							Posicio posicioMatar(fAux, cAux);
							posicioMatar.setMataFitxa(true); //marquem que mata una fitxa
							posicionsPossibles.push_back(posicioMatar);
							nPosicions++;
							marcarMatarSegonsColor(color, fitxa.getPosicio());
						}
					}
					else {
						//Cas que mata una fitxa i pot matar a una altre en la diagonal
						bool trobat = false;
						while (!trobat && estaADinsDelTauler(f, c)) { //buscar si hi ha alguna fitxa per matar en la diagonal f-fila c-col
							int fAux = f + dirDama[i][0];
							int cAux = c + dirDama[i][1];
							if (getFitxa(f, c).getColor() != color && !getFitxa(f, c).esBuida()) {
								if (estaADinsDelTauler(fAux, cAux) && getFitxa(fAux, cAux).esBuida() && !movimentActual.contePosicio(fAux, cAux)) {
									Posicio posicioMatar(fAux, cAux);
									posicioMatar.setMataFitxa(true); //marquem que mata una fitxa
									posicionsPossibles.push_back(posicioMatar);
									nPosicions++;
									marcarMatarSegonsColor(color, fitxa.getPosicio());
									trobat = true;
								}
							}
							f = fAux;
							c = cAux;
						}
					}
				}
			}
		}

	}
	else {
		// Si ja s'ha fet un salt, seguir en la mateixa direcció que el primer salt
		dirDama[0][0] = (fila - movimentActual.getInici().getFila()) / abs(fila - movimentActual.getInici().getFila());
		dirDama[0][1] = (col - movimentActual.getInici().getColumna()) / abs(col - movimentActual.getInici().getColumna());

		int f = fila + dirDama[0][0];
		int c = col + dirDama[0][1];
		if (estaADinsDelTauler(f, c) && !movimentActual.contePosicio(f, c)) {
			if (getFitxa(f, c).esBuida()) {
				posicionsPossibles.push_back(Posicio(f, c));
				nPosicions++;
			}
			else {
				if (getFitxa(f, c).getColor() != color) {
					int fAux = f + dirDama[0][0];
					int cAux = c + dirDama[0][1];
					if (estaADinsDelTauler(fAux, cAux) && getFitxa(fAux, cAux).esBuida()) {
						Posicio posicioMatar(fAux, cAux);
						posicioMatar.setMataFitxa(true); //marquem que mata una fitxa
						posicionsPossibles.push_back(posicioMatar);
						nPosicions++;
						marcarMatarSegonsColor(color, fitxa.getPosicio());
					}
				}
			}
		}
	}
}
void Tauler::possiblesPosicionsMatarDama(const Fitxa& fitxa, int fila, int col, vector<Posicio>& posicionsPossibles, int& nPosicions, Moviment &movimentActual, ColorFitxa colorOrigin, bool primerCop) {
	int dirDama[4][2] = { {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };
	if (movimentActual.getHeMatat() || primerCop) {
		for (int d = 0; d < 4; ++d) {
			int df = dirDama[d][0];
			int dc = dirDama[d][1];
			int f = fila + df;
			int c = col + dc;

			if (estaADinsDelTauler(f, c)) {
				Fitxa fActual = getFitxa(f, c);

				if (!fActual.esBuida()) {
					if (fActual.getColor() != colorOrigin) {
						f += df;
						c += dc;
						Fitxa arribada = getFitxa(f, c);
						if (estaADinsDelTauler(f, c) && arribada.esBuida() && !movimentActual.contePosicio(f, c)) {
							Posicio posicioMatar(f, c);
							posicioMatar.setMataFitxa(true); //marquem que mata una fitxa
							posicionsPossibles.push_back(posicioMatar);
							nPosicions++;
							marcarMatarSegonsColor(colorOrigin, fitxa.getPosicio());
						}
					}
				}
			}
		}
	}
}


bool Tauler::mouFitxa(const Posicio& origen, const Posicio& desti) {
	Fitxa fitxa = *m_tauler[origen.getFila()][origen.getColumna()];

	// Eliminamos las piezas que se capturan
	bool bufar = false;
	bool valid = validarIEliminarFitxesCapturades(fitxa, origen, desti, bufar);

	if (valid) { // Movemos la pieza a la nueva posición
		// Comprobamos si el movimiento ha sido el mejor (máxima captura)
		
		if (bufar && (posicioABuffar.getFila()!= origen.getFila() || posicioABuffar.getColumna() != origen.getColumna())) {
			// Cas que sigui la feitxa a bufer diferent de l'origen
			*m_tauler[posicioABuffar.getFila()][posicioABuffar.getColumna()] = Fitxa(posicioABuffar, TIPUS_EMPTY);
			*m_tauler[desti.getFila()][desti.getColumna()] = fitxa;
			m_tauler[desti.getFila()][desti.getColumna()]->setPosicio(desti);
			*m_tauler[origen.getFila()][origen.getColumna()] = Fitxa(Posicio(origen.getFila(), origen.getColumna()), TIPUS_EMPTY);

		} else 
			if (bufar) {
				// Cas que sigui la posicio origen la que s'ha de bufar
				*m_tauler[origen.getFila()][origen.getColumna()] = Fitxa(Posicio(origen.getFila(), origen.getColumna()), TIPUS_EMPTY);
			}	
			else  
			{
				// Cas de no buffar
			*m_tauler[desti.getFila()][desti.getColumna()] = fitxa;
			m_tauler[desti.getFila()][desti.getColumna()]->setPosicio(desti);
			*m_tauler[origen.getFila()][origen.getColumna()] = Fitxa(Posicio(origen.getFila(), origen.getColumna()), TIPUS_EMPTY);

			}		
		
		// Comprobamos si la pieza ha llegado al final del tablero y debe convertirse en dama
		convertirEnDama(*m_tauler[desti.getFila()][desti.getColumna()], desti);
		
		
	}

	return valid;
}

bool Tauler::esMovimentValid(const Posicio origen, const Posicio desti) const {
	Fitxa fitxa = *m_tauler[origen.getFila()][origen.getColumna()];

	int filaDesti = desti.getFila();
	int columnaDesti = desti.getColumna();
	// Cal recorrer tots els moviments valids de la fitxa i comprovar que la posicio desti hi estigui inclosa
	if (fitxa.esBuida() || !estaADinsDelTauler(filaDesti, columnaDesti)) {
		return false; // Si la casilla origen está vacía o destino está fuera del tablero, no es válido
	} else {
		bool trobat = false;
		int i = 0;
		while (i < fitxa.getNMovimentsValids() && !trobat) {
			int j = 0;
			Moviment m = fitxa.getMoviment(i);
			// Comprovar si la posició desti està entre els moviments valids
			while (j< m.getNumSalts() && !trobat) {
				if (m.getSalt(j).getFila() == filaDesti && m.getSalt(j).getColumna() == columnaDesti) {
					trobat = true; // Si la posició desti està entre els moviments valids, es pot moure
				} else {
					j++;
				}
			} 
			if (!trobat) 
				i++;
		}
		return trobat;
	}
	return false;
}

void Tauler::convertirEnDama(Fitxa& fitxa, const Posicio& pos) {
	// Solo las piezas que llegan al final del tablero se convierten en dama
	if (fitxa.getTipus() == TIPUS_DAMA)
		return;

	if (fitxa.getColor() == COLOR_BLANC && pos.getFila() == 0)
		fitxa.setTipus(TIPUS_DAMA);
	else if (fitxa.getColor() == COLOR_NEGRE && pos.getFila() == N_FILES - 1)
		fitxa.setTipus(TIPUS_DAMA);
}

bool Tauler::validarIEliminarFitxesCapturades(const Fitxa& fitxa, const Posicio& origen, const Posicio& desti, bool &bufarFitxa) {
	// Trobar primer el trajecte que conté la posició destí i recorrer-lo per eliminar les fitxes capturades
	bool trobat = false;
	int i = 0;
	int j = 0;
	// LA fitxa es pot moure al desti??? Si desti no esta a cap salt de cap moviment possible de la fitxa-> retorna fals (la fitxa no pot fer aquest moviment
	while (i < fitxa.getNMovimentsValids() && !trobat) {
		j = 0;
		Moviment m = fitxa.getMoviment(i);
		int filaDesti = desti.getFila();
		int columnaDesti = desti.getColumna();
		// Comprovar si la posició desti està entre els moviments valids
		while (j< m.getNumSalts() && !trobat) {
			if (m.getSalt(j).getFila() == filaDesti && m.getSalt(j).getColumna() == columnaDesti) {
				trobat = true; // Si la posició desti està entre els moviments valids, es pot moure
			} else {
				j++;
			}
		} 
		if (!trobat) 
			i++;
	}
	if (!trobat) {
		return false; // Si no se ha encontrado el movimiento que conté el desti, no es un moviemnt valid d'aquella fitxa
	} else {
		Moviment m = fitxa.getMoviment(i);
			
		if (m.getNumSalts() == 0) {
			bufarFitxa = false;
			return true; // Si no hay saltos, no hay piezas capturadas
		} 
		// Cal comprovar si es bufa la fitxa o no es fa el moviment sencer de matar

		if (!m.getHeMatat() && podenMatar(fitxa.getColor()) ) {
			bufarFitxa = true;
			return true;
		} else {
			if (m.getHeMatat() && ((j< m.getNumSalts() - 1) || fitxa.teMovimentsMatarDiferents(i) || maximMovimentsMatats(fitxa,m))) {
				bufarFitxa = true;
				posicioABuffar = origen; // Marquem la posició a bufar
				
			} 

			// Ja es pot matar
			// Recorrer el salts del moviment per eliminar les fitxes capturades
			// Cal comprovar també el primer salt que va de l'origen al salt[0]
			
			// Si se ha encontrado el movimiento, eliminamos las piezas capturadas
			// Sabem que es el moviment -i- i que cal saltar fins a -j- salts
			
			// El primer salt es de origen a m.getSalt(0)
			if (m.getHeMatat()) {

				int filaOrigen = origen.getFila();
				int columnaOrigen = origen.getColumna();
				int filaPrimerSalt = m.getSalt(j).getFila();
				int columnaPrimerSalt = m.getSalt(j).getColumna();
				if (filaOrigen != filaPrimerSalt || columnaOrigen != columnaPrimerSalt) {
					// Cal eliminar la fitxa capturada entre origen i el primer salt
 					int filaMitja = (filaOrigen + filaPrimerSalt) / 2;
					int colMitja = (columnaOrigen + columnaPrimerSalt) / 2;

					Fitxa& possibleCaptura = *m_tauler[filaMitja][colMitja];
					if (!possibleCaptura.esBuida() && possibleCaptura.getColor() != fitxa.getColor()) {
						// Eliminar la pieza capturada
						possibleCaptura.setTipus(TIPUS_EMPTY);
						possibleCaptura.setColor(COLOR_DEFAULT);
					}
				}
				for (int k = 0; k < j; k++) {
					int filaMitja = (m.getSalt(k).getFila() + m.getSalt(k+1).getFila()) / 2;
					int colMitja = (m.getSalt(k).getColumna() + m.getSalt(k+1).getColumna()) / 2;


					Fitxa& possibleCaptura = *m_tauler[filaMitja][colMitja];
					if (!possibleCaptura.esBuida() && possibleCaptura.getColor() != fitxa.getColor()) {
						// Eliminar la pieza capturada
						possibleCaptura.setTipus(TIPUS_EMPTY);
						possibleCaptura.setColor(COLOR_DEFAULT);
					}
					
				}
			}
		
	
	}

				
	}
	return true;

}

bool Tauler::esMovimentCapturaMaxima(const Fitxa& fitxa, const Posicio& desti) const {
	// Inicializamos el contador de piezas capturadas en el movimiento actual
	int capturasActuales = 0;

	// Calculamos la posición intermedia entre origen y destino
	int filaMitja = (fitxa.getPosicio().getFila() + desti.getFila()) / 2;
	int colMitja = (fitxa.getPosicio().getColumna() + desti.getColumna()) / 2;

	// Verificamos si hay una pieza en la posición intermedia
	const Fitxa& fitxaIntermedia = *m_tauler[filaMitja][colMitja];  // Uso de referencia constante
	if (fitxaIntermedia.getTipus() != TIPUS_EMPTY && fitxaIntermedia.getColor() != fitxa.getColor()) {
		// Si hay una pieza contraria en la casilla intermedia, contamos como captura
		capturasActuales++;
	}

	// Ahora, comprobamos si existen otros posibles movimientos que puedan realizar capturas
	// Para eso, vamos a simular los movimientos posibles y contar cuántas piezas pueden ser capturadas
	int maxCapturasPosibles = 0;

	// Recorremos todas las casillas del tablero en busca de los movimientos posibles de la pieza
	for (int i = 0; i < N_FILES; i++) {
		for (int j = 0; j < N_COLUMNES; j++) {
			Posicio pos(i, j);
			if (esMovimentValid(fitxa.getPosicio(), pos)) {
				// Simulamos la captura que se podría hacer
				int filaMitjaSimulada = (fitxa.getPosicio().getFila() + pos.getFila()) / 2;
				int colMitjaSimulada = (fitxa.getPosicio().getColumna() + pos.getColumna()) / 2;

				// Verificamos si hay una pieza contraria en la casilla intermedia
				const Fitxa& fitxaIntermediaSimulada = *m_tauler[filaMitjaSimulada][colMitjaSimulada];  // Uso de referencia constante
				if (fitxaIntermediaSimulada.getTipus() != TIPUS_EMPTY && fitxaIntermediaSimulada.getColor() != fitxa.getColor()) {
					// Si hay una pieza contraria, incrementamos el contador de capturas
					maxCapturasPosibles++;
				}
			}
		}
	}

	// Si las capturas actuales son menores que las capturas máximas posibles, significa que el movimiento actual no fue el mejor
	if (capturasActuales < maxCapturasPosibles) {
		return false; // El movimiento no fue el mejor, por lo tanto se debe bufar una pieza
	}

	return true; // El movimiento es el que permite la máxima captura
}



	

string Tauler::toString() const {
	stringstream ss;

	// Recorremos el tablero y generamos la representación visual
	for (int i = 0; i < N_FILES; i++) {
		ss << (8 - i) << ": ";
		for (int j = 0; j < N_COLUMNES; j++) {
			const Fitxa& f = *m_tauler[i][j];
			char simbol = '_';
			if (f.getTipus() != TIPUS_EMPTY) {
				if (f.getColor() == COLOR_BLANC) {
					simbol = (f.getTipus() == TIPUS_DAMA) ? 'D' : 'O';
				}
				else {
					simbol = (f.getTipus() == TIPUS_DAMA) ? 'R' : 'X';
				}
			}
			ss << simbol << " ";
		}
		ss << endl;
	}
	ss << "A B C D E F G H\n";
	return ss.str();
}

void Tauler::marcarMatarSegonsColor(ColorFitxa color, Posicio posicio) {
	if (color == COLOR_BLANC) {
		podenMatarBlanques = true;
		fitxaBlancaQuePotMatar = *m_tauler[posicio.getFila()][posicio.getColumna()];
	} else if (color == COLOR_NEGRE) {
		podenMatarNegres = true;
		fitxaNegraQuePotMatar = *m_tauler[posicio.getFila()][posicio.getColumna()];
	}
}
bool Tauler::podenMatar(ColorFitxa color) {
	if (color == COLOR_BLANC) {
		if (podenMatarBlanques) {

			Posicio p(fitxaBlancaQuePotMatar.getPosicio().getFila(),
				fitxaBlancaQuePotMatar.getPosicio().getColumna());
			posicioABuffar = p;
			posicioABuffar.setMataFitxa(true); // Marquem que pot matar
			return podenMatarBlanques;
		}
	} else if (color == COLOR_NEGRE) {
		if (podenMatarNegres) {
			Posicio p(fitxaNegraQuePotMatar.getPosicio().getFila(),
				fitxaNegraQuePotMatar.getPosicio().getColumna());
			posicioABuffar = p;
			posicioABuffar.setMataFitxa(true); // Marquem que pot matar
			return podenMatarNegres;
		}
	}
	return false;
}
void Tauler::visualitza(bool fitxaSeleccionada, vector<Posicio> posicionsValides, int posXTauler, int posYTauler, int casellaInicialX, int casellaInicialY, int ampladaCasella, int alcadaCasella) {
	for (int fila = 0; fila < N_FILES; fila++) {
		for (int col = 0; col < N_COLUMNES; col++) {
			int posX = posXTauler + casellaInicialX + ((col) * ampladaCasella);
			int posY = posYTauler + casellaInicialY + ((fila) * alcadaCasella);
			Fitxa fitxa = *m_tauler[fila][col];
			fitxa.visualitza(posX, posY, fitxa.getColor(), fitxa.getTipus());
		}
	}

	if (fitxaSeleccionada && !posicionsValides.empty()) {
		for (int i = 0; i < posicionsValides.size(); ++i) {
			Posicio p = posicionsValides[i];
			int f = posXTauler + casellaInicialX + ((p.getColumna())*ampladaCasella);
			int c = posYTauler + casellaInicialY + ((p.getFila()) * alcadaCasella);
			GraphicManager::getInstance()->drawSprite(GRAFIC_POSICIO_VALIDA, f,c);
		}
	}
}
bool Tauler::esBuit(ColorFitxa torn) {
	for (int fila = 0; fila < N_FILES; ++fila) {
		for (int col = 0; col < N_COLUMNES; ++col) {
			Fitxa f = *m_tauler[fila][col];
			if (f.getColor() == torn)
				return false;
		}
	}
	return true;
}
bool Tauler::maximMovimentsMatats(const Fitxa& fitxa, Moviment mov) {
	int maxSaltsFitxa = mov.getNumSalts();
	for (int i = 0; i < N_FILES; i++) {
		for (int j = 0; j < N_COLUMNES; j++) {
			if (m_tauler[i][j] != nullptr) {
				Fitxa aux = *m_tauler[i][j];
				if (fitxa.getColor() == aux.getColor() && fitxa.getPosicio() != aux.getPosicio()) {
					for (int k = 0; k < aux.getNMovimentsValids(); k++) {
						Moviment mov2 = aux.getMoviment(k);
						if (mov2.getHeMatat() && mov2.getNumSalts() > maxSaltsFitxa && mov2.getPrimer() == mov.getPrimer())
							return true;
					}
				}
			}
		}
	}
	return false;
}
