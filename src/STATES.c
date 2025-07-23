#include "STATES.h"
#include <stdio.h>
#include <stdbool.h>
GameStates currentState; //current state of the game


void changeState(GameStates newState){
    // Unload current state if loaded
    if (states[currentState].isloaded && states[currentState].unload != NULL) {
        states[currentState].unload();
        states[currentState].isloaded = false;
    }
    currentState = newState;
}