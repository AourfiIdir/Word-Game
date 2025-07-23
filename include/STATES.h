//this file includes  global declarations + the states methods 

#ifndef STATES_H
#define STATES_H
#include <stdbool.h>
#include <sqlite3.h>
#include <raylib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


#define screenWidth 800
#define screenHeight 600
//the game states 
typedef enum{
    SIGNIN,
    LOGIN,
    MENU,
    LEVELS,
    LEVEL1,
    LEVEL2,
}GameStates;

typedef struct inputField{
    char *input;
    int length;
    Rectangle textBox;
    bool curserOnText;
    bool activeBox;
}inputField;

typedef struct button{
    Rectangle rect;
    char *text;
    Color color;
    bool isPressed;
    bool hovered;
}button;
extern GameStates currentState; //current state of the game define in another file
//functions states'the functions needed for each state
typedef struct stateFunctions{
    void (*load)(void); //for loading all the assets
    void (*unload)(void);   //for unloading all the assets
    void (*update)();   //for updating the game logic
    void (*draw)(); //for drawing the game
    bool isloaded; //check if the state is loaded or not
}stateFunctions;

typedef struct word{    
    int length;
    char *word;
    char *definition;
    bool checked;
}word;


typedef struct levelFunctions{
    int(*checkFinished)(word *w);
    void(*initialise)(void);
    int(*checkWord)(word **w , char*input);
}levelFunctions;

extern stateFunctions states[];
extern sqlite3* gameDB;

void changeState(GameStates newState);

#endif
