#include "CuaMoviments.h"
void CuaMoviments::buida() { //buidem la cua
    while (m_next != nullptr) {
        Node* seguent = m_next->getNext();
        delete m_next;
        m_next = seguent;
    }
}
void CuaMoviments::carregaMoviments(const string& nomFitxer) {
	ifstream fitxer;
	fitxer.open(nomFitxer);
	if (fitxer.is_open()) {
		buida();
        string origenStr, destiStr;
        Node* ultim = nullptr;
        //Mentre el fitxer llegeixi dos strings, seguim en el while, perque amb eof igual pots fer una iteració de més
        while (fitxer >> origenStr >> destiStr){
            //Creem un nou node i li agragem el nou moviment i el next a nullptr
            Posicio origen(origenStr);
            Posicio desti(destiStr);
            Moviment mov(origen, desti);
            Node* nou = new Node();
            nou->setMov(mov);
            nou->setNext(nullptr);
            //en el primer cas posem el nou
            if (m_next == nullptr)
                m_next = nou;
            else
                ultim->setNext(nou);
            //pels altres casos anem posant al seguent del ultim node

            ultim = nou;
        }
        fitxer.close();
	}
}
void CuaMoviments::carregaMovimentsAFitxer(const string& nomFitxer) {
    ofstream fitxer;
    fitxer.open(nomFitxer);
    if (fitxer.is_open()) {
        Node* actual = m_next;
        //Anem posant tota la cua al fitxer fins que no hi hagin mes nodes a la cua
        while (m_next != nullptr) {
            Moviment mov = actual->getMov();
            fitxer << mov.getInici().toString() << " " << mov.getFinal().toString() << endl;
            actual = actual->getNext(); //iterem al següent node
        }
        fitxer.close();
    }
}

void CuaMoviments::guardaMoviment(Moviment mov) { //guardar noviment a la cua
    //Creem nou node per posar a la cua com a ultim
    Node* nou = new Node;
    nou->setMov(mov);
    nou->setNext(nullptr);
    //Si no hi ha res al actual, l'actualitzem
    if (m_next == nullptr)
        m_next = nou;
    else {
        //anem buscant el seguent fins que arribem a l'ultim node i li assignem el nou moviment
        Node* actual = m_next;
        while (actual->getNext() != nullptr)
            actual = actual->getNext();
        actual->setNext(nou);
    }
}
void CuaMoviments::eliminaPrimer() {
    Node* primer = m_next;
    m_next = primer->getNext();
    delete primer;
}
