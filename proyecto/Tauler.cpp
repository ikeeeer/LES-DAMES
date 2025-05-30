#include "tauler.hpp"

void Tauler::inicialitza(const string& nomFitxer)
{
	ifstream fitxer;
	fitxer.open(nomFitxer);
	if (fitxer.is_open()) {
		for (int i = 0; i < N_FILES; i++) {
			for (int j = 0; j < N_COLUMNES; j++) { //inicialitzem tot el tauler buit
				m_tauler[i][j].setTipus(TIPUS_EMPTY);
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
				m_tauler[fila][col] = creaFitxaAPartirDeChar(valor, fila, col); //pasar de tipus char a tipusFitxa
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
	for (int fila = 0; fila < N_FILES; fila++) {
		for (int col = 0; col < N_COLUMNES; col++) {
			//primer creem la fitxa basada en la posicio del tauler, i també dos arrays de moviments pendents i moviments valids
			Fitxa& fitxa = m_tauler[fila][col];
			if (!fitxa.esBuida()) { //mirem que hi hagi una fitxa
				TipusFitxa tipusOrigin = fitxa.getTipus();
				ColorFitxa colorOrigin = fitxa.getColor();
				Moviment movimentsPendents[MAX_MOVIMENTS];
				//Moviment movimentsValids[MAX_MOVIMENTS];
				int nMovimentsPendents = 0;
				//int nMovimentsValids = 0;
				//int inici = 0;
				//int fi = 0;

				Moviment movimentActual; //creem el primer moviment
				Posicio posActual(fila, col); //agafem la posicio actual de la fitxa, per després mirar els moviments possibles
				movimentActual.setInici(posActual); //li afegim la posicio actual al moviment
				movimentsPendents[nMovimentsPendents] = movimentActual; // Afegim el moviment inicial a la cua de moviments pendents
				nMovimentsPendents++;
				//aquests pasos són només per el primer movimentActual, deprés en el bucle ja és va iterant

				//recorrem tot el array de movimentsPendents fins que estigui buida
				do
				{
					movimentActual = movimentsPendents[nMovimentsPendents-1]; //li assignem al moviment actual el primer, després el segon i així consecutivament
					nMovimentsPendents--;

					posActual = movimentActual.getFinal(); //la posicio actual serà l'última dels moviments fets
					Posicio posValides[MAX_POSICIONS];
					int nPosValides = 0;
					getPosicionsPossiblesDirectes(posActual, nPosValides, posValides, tipusOrigin, colorOrigin, movimentActual); //agafem totes les possibles posicions desde la posicioActual
					int iniciPosValida = 0;
					//mentre hi hagi posicions vàlides, recorrem posValides
					while (nPosValides > 0){
						// Afegim la primera posició vàlida al movimentActual
						movimentActual.afageixPas(posValides[iniciPosValida++]);

						// Per la resta de posicions vàlides, dupliquem moviment i les afegim a moviments pendents
						for (int i = 1; i < nPosValides; i++)
						{
							Moviment movimentPendent = movimentActual.duplicaMoviment();
							movimentPendent.afageixPas(posValides[i]);
							movimentsPendents[nMovimentsPendents++] = movimentPendent;
						}
						//posicioActual amb la primer posicio valida
						posActual = posValides[iniciPosValida-1];
						iniciPosValida--;

						// Afegim el moviment actual ja modificat a la cua de pendents per seguir expandint-lo
						getPosicionsPossiblesDirectes(posActual, nPosValides, posValides, tipusOrigin, colorOrigin, movimentActual); //agafem totes les possibles posicions desde la posicioActual i actualitzem nPosValides
					}
					// Si el moviment no està buit (ha fet algun pas), l'afegim com a vàlid
					if (movimentActual.esBuit())
						fitxa.afegeixMoviment(movimentActual);
				} while (nMovimentsPendents>0); //continuem mentre el array de moviments pendents no s'hagi acabat
			}
		}
	}
}
void Tauler::getPosicionsPossibles(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[]) {
	Fitxa& fitxa = m_tauler[origen.getFila()][origen.getColumna()];
	fitxa.getPosicionsValides(posicionsPossibles,nPosicions);
}
void Tauler::getPosicionsPossiblesDirectes(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[],
	TipusFitxa tipusOrigin, ColorFitxa colorOrigin, Moviment& movimentActual) {

	nPosicions = 0;
	int fila = origen.getFila(); //agafem les coordenades del origen
	int col = origen.getColumna();

	const Fitxa& fitxa = getFitxa(fila, col);
	ColorFitxa color = fitxa.getColor();
	TipusFitxa tipus = fitxa.getTipus();
	if (tipus == TIPUS_NORMAL)
		possiblesPosicionsNormal(fitxa, fila, col, posicionsPossibles, nPosicions, color);
	else if (tipus == TIPUS_DAMA || tipusOrigin == TIPUS_DAMA) {
		possiblesPosicionsDama(fitxa, fila, col, posicionsPossibles, nPosicions, colorOrigin, movimentActual);
	}
	else if (tipusOrigin == TIPUS_NORMAL) {
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
void Tauler::possiblesPosicionsNormal(const Fitxa& fitxa, int fila, int col, Posicio posicionsPossibles[], int& nPosicions, ColorFitxa color) {
	int direccio[2][2] = { {obtenirDireccio(fitxa, fila, col, color), -1}, {obtenirDireccio(fitxa, fila, col, color), 1} };
	//obtenim el color de la fitxa i restem o sumem depenent de si es blanc o negre

	//fitxa normal, sense captura

	for (int i = 0; i < 2; i++)
	{
		int f = fila + direccio[i][0]; //agafem la fila i columna i els hi sumem els casos adalt dreta/esquerra o abaix dreta/esquerra
		int c = col + direccio[i][1];
		if (estaADinsDelTauler(f,c) && getFitxa(f, c).esBuida()) {
			posicionsPossibles[nPosicions] = Posicio(f, c); //afegim la posicio si es possible
			nPosicions++;
		}
	}
	//fitxa normal, amb captura
	possiblesPosicionsMatar(fitxa, fila, col, posicionsPossibles, nPosicions, color);
}
void Tauler::possiblesPosicionsMatar(const Fitxa& fitxa, int fila, int col, Posicio posicionsPossibles[], int& nPosicions, ColorFitxa color) {
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
			if (!rival.esBuida() && rival.getColor() != fitxa.getColor() && desti.esBuida()) { //si el rival exsiteix de veritat i el desti és buit, agragem
				posicionsPossibles[nPosicions] = Posicio(f2, c2);
				nPosicions++;
			}
		}
	}
}
bool Tauler::estaADinsDelTauler(int fila, int col) {
	return (fila >= 0 && fila < N_FILES && col >= 0 && col < N_COLUMNES);
}
void Tauler::possiblesPosicionsDama(const Fitxa& fitxa, int fila, int col, Posicio posicionsPossibles[], int& nPosicions, ColorFitxa color,  Moviment& movimentActual) {
	int dirDama[4][2] = { {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };
	if (movimentActual.getNumSalts() == 0) {
		// Primer moviment: explorar totes les direccions
		for (int i = 0; i < 4; ++i) {
			int f = fila + dirDama[i][0];
			int c = col + dirDama[i][1];
			//mirar posicions possibles consecutives
			if(estaADinsDelTauler(f, c) && getFitxa(f, c).esBuida() && !movimentActual.contePosicio(f, c)) {
				posicionsPossibles[nPosicions++] = Posicio(f, c);
			}
		}
		possiblesPosicionsMatarDama(fitxa, fila, col, posicionsPossibles, nPosicions, movimentActual, color, true);
	}
	else {
		// Si ja s'ha fet un salt, seguir en la mateixa direcció que el primer salt
		dirDama[0][0] = movimentActual.getSalt(0).getFila() - movimentActual.getInici().getFila();
		dirDama[0][1] = movimentActual.getSalt(0).getColumna() - movimentActual.getInici().getColumna();

		int f = fila + dirDama[0][0];
		int c = col + dirDama[0][1];
		if (estaADinsDelTauler(f, c) && !movimentActual.contePosicio(f, c)) {
			if (getFitxa(f, c).esBuida()) {
				posicionsPossibles[nPosicions++] = Posicio(f, c);
			}
			else {
				if (getFitxa(f, c).getColor() != color) {
					int fAux = f + dirDama[0][0];
					int cAux = c + dirDama[0][1];
					if (estaADinsDelTauler(fAux, cAux) && getFitxa(fAux, cAux).esBuida()) {
						posicionsPossibles[nPosicions++] = Posicio(fAux, cAux);
						movimentActual.setHeMatat(true);
					}
					possiblesPosicionsMatarDama(getFitxa(fAux,cAux), fAux, cAux, posicionsPossibles, nPosicions, movimentActual, color, false);
				} else 
					possiblesPosicionsMatarDama(getFitxa(f,c), f, c, posicionsPossibles, nPosicions, movimentActual, color, false);
			}
		}
	}

		/*while (estaADinsDelTauler(f2, c2))
		{
			Fitxa fitxaRival = getFitxa(f2, c2);
			if (fitxaRival.esBuida()) {
				if (!rival)
					posicionsPossibles[nPosicions++] = Posicio(f2, c2);
				else
					possiblesPosicionsMatarDama(fitxa, fila, col, posicionsPossibles, nPosicions, color);
				posicionsPossibles[nPosicions++] = Posicio(f2, c2);
				break;
			}
		}
		else {
			if (fitxaRival.getColor() == color)
				break;
			if (!rival)
				rival = true;
			else
				break;
		}
				f2 += dirDama[i][0];
				c2 += dirDama[i][1];
			}
		}
	}*/
}
void Tauler::possiblesPosicionsMatarDama(const Fitxa& fitxa, int fila, int col, Posicio posicionsPossibles[], int& nPosicions, Moviment& movimentActual, ColorFitxa colorOrigin, bool primerCop) {
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
							posicionsPossibles[nPosicions++] = Posicio(f, c);
							movimentActual.setHeMatat(true);
						}
					}
				}
			}
		}
	}
}


bool Tauler::mouFitxa(const Posicio& origen, const Posicio& desti) {
	Fitxa& fitxa = m_tauler[origen.getFila()][origen.getColumna()];

	// Comprobamos si el movimiento es válido
	if (!esMovimentValid(origen, desti)) {
		return false;
	}

	// Eliminamos las piezas que se capturan
	eliminarFitxesCapturades(fitxa, origen, desti);

	// Movemos la pieza a la nueva posición
	m_tauler[desti.getFila()][desti.getColumna()] = fitxa;
	m_tauler[origen.getFila()][origen.getColumna()] = Fitxa(Posicio(origen.getFila(), origen.getColumna()), TIPUS_EMPTY);

	// Comprobamos si la pieza ha llegado al final del tablero y debe convertirse en dama
	convertirEnDama(m_tauler[desti.getFila()][desti.getColumna()], desti);

	// Comprobamos si el movimiento ha sido el mejor (máxima captura)
	if (!esMovimentCapturaMaxima(fitxa, desti)) {
		bufarFitxa(desti);
	}

	return true;
}

bool Tauler::esMovimentValid(const Posicio& origen, const Posicio& desti) const {
	const Fitxa& fitxa = m_tauler[origen.getFila()][origen.getColumna()];

	// Validación simple: la pieza se mueve a una casilla adyacente y está vacía
	int filaDiff = abs(origen.getFila() - desti.getFila());
	int colDiff = abs(origen.getColumna() - desti.getColumna());

	if (filaDiff == 1 && colDiff == 1) {
		const Fitxa& dest = m_tauler[desti.getFila()][desti.getColumna()];
		return dest.getTipus() == TIPUS_EMPTY;  // Comprobamos si la casilla está vacía
	}

	// Validación si es una captura (salta una pieza contraria)
	if (filaDiff == 2 && colDiff == 2) {
		int filaMitja = (origen.getFila() + desti.getFila()) / 2;
		int colMitja = (origen.getColumna() + desti.getColumna()) / 2;
		const Fitxa& fitxaIntermedia = m_tauler[filaMitja][colMitja];
		return fitxaIntermedia.getTipus() != TIPUS_EMPTY && fitxaIntermedia.getColor() != fitxa.getColor(); // Comprobamos que haya una pieza intermedia
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

void Tauler::eliminarFitxesCapturades(const Fitxa& fitxa, const Posicio& origen, const Posicio& desti) {
	// Calcular la posición intermedia entre origen y destino
	int filaMitja = (origen.getFila() + desti.getFila()) / 2;
	int colMitja = (origen.getColumna() + desti.getColumna()) / 2;

	// Comprobar si se ha capturado una pieza contraria
	Fitxa& possibleCaptura = m_tauler[filaMitja][colMitja];

	if (possibleCaptura.getTipus() != TIPUS_EMPTY && possibleCaptura.getColor() != fitxa.getColor()) {
		// Eliminar la pieza capturada
		possibleCaptura.setTipus(TIPUS_EMPTY);
	}
}

bool Tauler::esMovimentCapturaMaxima(const Fitxa& fitxa, const Posicio& desti) const {
	// Inicializamos el contador de piezas capturadas en el movimiento actual
	int capturasActuales = 0;

	// Calculamos la posición intermedia entre origen y destino
	int filaMitja = (fitxa.getPosicio().getFila() + desti.getFila()) / 2;
	int colMitja = (fitxa.getPosicio().getColumna() + desti.getColumna()) / 2;

	// Verificamos si hay una pieza en la posición intermedia
	const Fitxa& fitxaIntermedia = m_tauler[filaMitja][colMitja];  // Uso de referencia constante
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
				const Fitxa& fitxaIntermediaSimulada = m_tauler[filaMitjaSimulada][colMitjaSimulada];  // Uso de referencia constante
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



void Tauler::bufarFitxa(const Posicio& pos) {
	// Elimina la pieza del tablero
	m_tauler[pos.getFila()][pos.getColumna()] = Fitxa(Posicio(pos.getFila(), pos.getColumna()), TIPUS_EMPTY);
}

string Tauler::toString() const {
	stringstream ss;

	// Recorremos el tablero y generamos la representación visual
	for (int i = 0; i < N_FILES; i++) {
		ss << (8 - i) << ": ";
		for (int j = 0; j < N_COLUMNES; j++) {
			const Fitxa& f = m_tauler[i][j];
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