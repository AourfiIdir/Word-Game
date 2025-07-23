#include <stdio.h>
#include "Menu.h"


Texture2D background;
Texture2D PlaytBtn;
Texture2D exitBtn;
Texture2D LevelsBtn;

Vector2 PlayBtnPos = {320, 220};
Vector2 exitBtnPos = {320, 400};
Vector2 LevelsBtnPos = {320, 310};
Vector2 mousePos = {0, 0};
void loadMenu(void){
    background = LoadTexture("graphics/MENU.png");
    PlaytBtn = LoadTexture("graphics/PLAY.png");
    exitBtn = LoadTexture("graphics/EXIT.png");
    LevelsBtn = LoadTexture("graphics/LEVELS.png");
}
void updateMenu(void){
    mousePos = GetMousePosition();
    //the play button
    if(CheckCollisionPointRec(mousePos, (Rectangle){PlayBtnPos.x, PlayBtnPos.y, PlaytBtn.width, PlaytBtn.height})){
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            printf("Play button pressed\n");
            changeState(LEVEL1); // Change to LOGIN state when Play button is pressed
            //currentState = LOGIN; // Set the current state to LOGIN
        }
        //the exit button
    }else if(CheckCollisionPointRec(mousePos, (Rectangle){exitBtnPos.x, exitBtnPos.y, exitBtn.width, exitBtn.height})){
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            CloseWindow();
        }
        //the levels button
    }else if(CheckCollisionPointRec(mousePos, (Rectangle){LevelsBtnPos.x, LevelsBtnPos.y, LevelsBtn.width, LevelsBtn.height})){
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            printf("Levels button pressed\n");
            currentState = LOGIN;
        }
    }
    if (IsKeyPressed(KEY_ESCAPE)){
        CloseWindow();
    }

}

void deloadMenu(void){
    UnloadTexture(background);
    UnloadTexture(PlaytBtn);
    UnloadTexture(exitBtn);
    UnloadTexture(LevelsBtn);
}

void drawMenu(void){
    DrawTexture(background,0,0,WHITE);
    DrawTexture(PlaytBtn,320,220,WHITE);
    DrawTexture(exitBtn,320,400,WHITE);
    DrawTexture(LevelsBtn,320,310,WHITE);
}
