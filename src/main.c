#include "raylib.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "STATES.h"
#include "Level1.h"
#include "Login.h"
#include "Menu.h"
#define frames 60
sqlite3 *gameDB; //database pointer

stateFunctions states[] = {
    [SIGNIN] = {NULL,NULL,NULL,NULL,false},
    [LOGIN] = {loadLogin, unloadLogin, UpdateLoginScreen, DrawLoginScreen, false},
    [MENU] = {loadMenu, deloadMenu, updateMenu, drawMenu, false},
    [LEVELS] = {NULL, NULL, NULL, NULL, false},
    [LEVEL1] = {loadLevel1, unloadLevel1, updateLevel1, drawLevel1, false},
    [LEVEL2] = {NULL, NULL, NULL, NULL, false},
};



int main(void)
{
    // Initialization
    InitWindow(screenWidth, screenHeight, "WORD GAME");
    SetTargetFPS(frames);
    currentState = LOGIN;
    //audio
    InitAudioDevice();
    //initialise the database
    int rc; 
    int rc2;
     char *errMsg = 0;
     // 1. Open (or create) the database file
    rc = sqlite3_open("database/game.db", &gameDB);

    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(gameDB));
        return 1;
    }

    // 2. SQL to create table only if it doesn't exist
    const char *sql = "CREATE TABLE IF NOT EXISTS users ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "name TEXT NOT NULL,"
                      "password TEXT NOT NULL);";
    const char *sql2 = "CREATE TABLE IF NOT EXISTS words("
    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "word TEXT NOT NULL ,"
    "length INTEGER NOT NULL,"
    "definition TEXT NOT NULL);";
    // 3. Execute SQL
    rc = sqlite3_exec(gameDB, sql, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(gameDB);
        return 1;
    } else {
        printf(" users Table created or already exists.\n");
    }
    
    rc2 = sqlite3_exec(gameDB,sql2,0,0,&errMsg);
    if(rc2!=SQLITE_OK){
        fprintf(stderr,"SQL error: %s\n",errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(gameDB);
    }
    else{
        printf("words Table created or already exists");
    }


    Music music = LoadMusicStream("audio/audio.mp3");
    PlayMusicStream(music);
    while (!WindowShouldClose())
    {
        UpdateMusicStream(music);
        BeginDrawing();
        ClearBackground(RAYWHITE);
        switch (currentState)
        {
        case MENU:
            if (states[currentState].isloaded == false)
            {
                states[currentState].load();
                states[currentState].isloaded = true;
            }
            states[currentState].update();
            states[currentState].draw();
            break;
        case LEVELS:
            if (states[currentState].isloaded == false)
            {
                states[currentState].load();
                states[currentState].isloaded = true;
            }
            states[currentState].update();
            states[currentState].draw();
            break;
        case LEVEL1:
            if (states[currentState].isloaded == false)
            {
                states[currentState].load();
                states[currentState].isloaded = true;
            }
            states[currentState].update();
            states[currentState].draw();
            break;
        case LEVEL2:
            if (states[currentState].isloaded == false)
            {
                states[currentState].load();
                states[currentState].isloaded = true;
            }
            states[currentState].update();
            states[currentState].draw();
            break;
        case LOGIN:
            if (states[currentState].isloaded == false)
            {
                states[currentState].load();
                states[currentState].isloaded = true;
            }
            if(isInit == false){
                initLoginScreen(&loginscreen);
            }
            
            states[currentState].update(&loginscreen);
            states[currentState].draw(loginscreen);
            break;
        default:
            break;
        }
        EndDrawing();
    }
    UnloadMusicStream(music);
    states[currentState].unload();
    sqlite3_close(gameDB);
    CloseWindow();
    
    return 0;
}
