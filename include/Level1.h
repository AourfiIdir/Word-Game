#ifndef LEVEL1_H
#define LEVEL1_H

#include "STATES.h"




extern sqlite3* gameDB;

void loadLevel1(void);
void unloadLevel1(void);
void updateLevel1(void);
void drawLevel1(void);
void initialiseLevel1(void);

// Logic utilities
int checkWordLevel1(word **w, char *input);
int checkFinishedLevel1(word **w);

#endif // LEVEL1_H
