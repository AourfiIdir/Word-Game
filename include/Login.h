#ifndef LOGIN_H
#define LOGIN_H
#include "STATES.h"
#define MAX_CHAR_INPUT 22

typedef struct LoginScreen{
    char name[MAX_CHAR_INPUT + 1];
    char password[MAX_CHAR_INPUT + 1];
    int nameCharCount;
    int passCharCount;
    bool editNameMod;
    bool editPassMod;
    Rectangle nameBox;
    Rectangle passBox;
    Rectangle loginBtn;
}LoginScreen;

extern LoginScreen loginscreen;
extern bool isInit;
void initLoginScreen(LoginScreen* loginScreen);
void loadLogin(void);
void unloadLogin(void);
void UpdateLoginScreen(LoginScreen *screen);
void DrawLoginScreen(LoginScreen screen);
bool CheckLoginCredentials(LoginScreen *screen, sqlite3 *db);
#endif