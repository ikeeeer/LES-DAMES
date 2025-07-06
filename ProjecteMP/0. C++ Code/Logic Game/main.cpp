//
//  main.cpp
//
//  Copyright � 2018 Compiled Creations Limited. All rights reserved.
//

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined  (_WIN64)

#include <iostream>
//Definicio necesaria per poder incloure la llibreria i que trobi el main
#define SDL_MAIN_HANDLED
#include <windows.h>
//Llibreria grafica
#include "../Graphic Lib/libreria.h"
#include "../Graphic Lib/NFont/NFont.h"
#include <conio.h>      /* getch */ 

#elif __APPLE__
//Llibreria grafica
#include "../Graphic Lib/libreria.h"
#include "../Graphic Lib/NFont/NFont.h"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#include <SDL2/SDL.h>
#pragma clang diagnostic pop

#endif

#include "./joc.hpp"
#include "./info_joc.hpp"
#include <chrono>
#include <thread>

int main(int argc, const char* argv[])
{

    //Instruccions necesaries per poder incloure la llibreria i que trobi el main
    SDL_SetMainReady();
    SDL_Init(SDL_INIT_VIDEO);

    //Inicialitza un objecte de la classe Screen que s'utilitza per gestionar la finestra grafica
    Screen pantalla(TAMANY_PANTALLA_X, TAMANY_PANTALLA_Y);
    //Mostrem la finestra grafica
    pantalla.show();

    Joc joc;
    joc.menu(joc.getMode());

    joc.inicialitza(joc.getMode(), "./data/Games/tauler_inicial.txt", "./data/Games/moviments.txt");
    bool final = false;
    do
    {
        // Captura tots els events de ratolí i teclat de l'ultim cicle
        pantalla.processEvents();

        bool mouseStatus = Mouse_getBtnLeft();
        int mousePosX = Mouse_getX();
        int mousePosY = Mouse_getY();
        final = joc.actualitza(mousePosX, mousePosY, mouseStatus);

        // Actualitza la pantalla
        pantalla.update();

    } while (!Keyboard_GetKeyTrg(KEYBOARD_ESCAPE) && !final);
    // Sortim del bucle si pressionem ESC
    joc.finalitza();
    int posTextX = POS_X_TAULER;
    int posTextY = POS_Y_TAULER + (ALCADA_CASELLA * NUM_FILES_TAULER) + 220;
    string msg = "";
    if (joc.getTorn() != COLOR_BLANC)
        msg = "Ha guanyat BLANC!";
    else
        msg = "Ha guanyat NEGRE!";
    GraphicManager::getInstance()->drawFont(FONT_WHITE_30, posTextX, posTextY, 0.8, msg);
    pantalla.update();
    this_thread::sleep_for(chrono::seconds(3));
    //Instruccio necesaria per alliberar els recursos de la llibreria 
    SDL_Quit();
    return 0;
}

