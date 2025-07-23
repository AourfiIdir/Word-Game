#include "Login.h"
#include "STATES.h"

bool isInit = false;
Texture2D loginBackground;
LoginScreen loginscreen; // Global login screen instance
bool CheckLoginCredentials(LoginScreen *screen, sqlite3 *db);


void loadLogin(void) {
    loginBackground = LoadTexture("graphics/login.png");
    if (loginBackground.id == 0) {
        TraceLog(LOG_WARNING, "Failed to load login background texture");
    }
    initLoginScreen(&loginscreen); // Initialize the global instance
}

void unloadLogin(void) {
    UnloadTexture(loginBackground);
}

void initLoginScreen(LoginScreen* loginScreen) {
    memset(loginScreen->name, 0, sizeof(loginScreen->name));
    memset(loginScreen->password, 0, sizeof(loginScreen->password));
    loginScreen->nameCharCount = 0;
    loginScreen->passCharCount = 0;
    loginScreen->editNameMod = false;
    loginScreen->editPassMod = false;
    
    // Adjusted coordinates to be more visible (verify these match your background)
    loginScreen->nameBox = (Rectangle){ 400, 180, 225, 50 };
    loginScreen->passBox = (Rectangle){ 400, 280, 225, 50 };
    loginScreen->loginBtn = (Rectangle){ 400, 380, 225, 50 };
    isInit = true;
}

void UpdateLoginScreen(LoginScreen *screen) {
    Vector2 mousePos = GetMousePosition();
    
    // Handle mouse clicks for focus
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        bool clickedName = CheckCollisionPointRec(mousePos, screen->nameBox);
        bool clickedPass = CheckCollisionPointRec(mousePos, screen->passBox);
        
        // Exclusive focus - only one field can be active
        screen->editNameMod = clickedName;
        screen->editPassMod = clickedPass && !clickedName;
        
        TraceLog(LOG_DEBUG, "Name active: %d, Pass active: %d", 
                screen->editNameMod, screen->editPassMod);
    }

    // Handle name input
    if (screen->editNameMod) {
        SetMouseCursor(MOUSE_CURSOR_IBEAM);

        int key = GetCharPressed();
        while (key > 0) {
            if ((key >= 32) && (key <= 125) && (screen->nameCharCount < MAX_CHAR_INPUT))
            {
                screen->name[screen->nameCharCount] = (char)key;
                screen->name[screen->nameCharCount+1] = '\0'; // Add null terminator at the end of the string.
                screen->nameCharCount++;
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE) && screen->nameCharCount > 0) {
            screen->name[--screen->nameCharCount] = '\0';
        }
    }else{
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }



    
    // Handle password input - FIXED SECTION
    if (screen->editPassMod) {
        int key = GetCharPressed();
        while (key > 0) {
            if (screen->passCharCount < MAX_CHAR_INPUT) {
                screen->password[screen->passCharCount] = (char)key;
                screen->password[screen->passCharCount+1] = '\0';
                screen->passCharCount++;
            }
            key = GetCharPressed();  // Removed the redeclaration of 'key'
        }
        if (IsKeyPressed(KEY_BACKSPACE) && screen->passCharCount > 0) {
            screen->password[--screen->passCharCount] = '\0';
        }
    }
    
    
    // Tab key navigation between fields
    if (IsKeyPressed(KEY_TAB)) {
        screen->editNameMod = !screen->editNameMod;
        screen->editPassMod = !screen->editNameMod;
    }

    //handle login button click
    if(mousePos.x >= screen->loginBtn.x && mousePos.x <= screen->loginBtn.x + screen->loginBtn.width &&
       mousePos.y >= screen->loginBtn.y && mousePos.y <= screen->loginBtn.y + screen->loginBtn.height) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            TraceLog(LOG_DEBUG, "Login button clicked");
            if (CheckLoginCredentials(screen, gameDB)) {
                TraceLog(LOG_INFO, "Login successful");
                changeState(MENU); 
            } else {
                TraceLog(LOG_WARNING, "Login failed");
            }
        }
    }
}
void DrawLoginScreen(LoginScreen screen) {
    // Draw background
    DrawTexture(loginBackground, 0, 0, WHITE);
    
    // Draw name field
    DrawRectangleRec(screen.nameBox, Fade(LIGHTGRAY, 0.8f));
    DrawRectangleLinesEx(screen.nameBox, 2, screen.editNameMod ? BLUE : DARKGRAY);
    
    DrawText("Username:", screen.nameBox.x - 120, screen.nameBox.y + 15, 20, BLACK); // Changed to BLACK
    
    // Draw username text with padding and clipping
    if (screen.name[0] != '\0') {
        // Calculate text position and width
        int textWidth = MeasureText(screen.name, 20);
        int maxWidth = screen.nameBox.width - 20; // 10px padding on each side
        
        // Draw either full text or clipped version
        if (textWidth <= maxWidth) {
            DrawText(screen.name, screen.nameBox.x + 10, screen.nameBox.y + 15, 20, BLACK); // Changed to BLACK
        } else {
            // Find how many characters we can show
            int charsToShow = 0;
            while (MeasureText(TextSubtext(screen.name, 0, charsToShow), 20) < maxWidth) {
                charsToShow++;
            }
            charsToShow--; // Back off one character
            
            // Draw the visible portion with ellipsis if needed
            const char* visibleText = TextFormat("%.*s...", charsToShow - 3, screen.name);
            DrawText(visibleText, screen.nameBox.x + 10, screen.nameBox.y + 15, 20, BLACK);
        }
    }
    
    // Draw password field
    DrawRectangleRec(screen.passBox, Fade(LIGHTGRAY, 0.8f));
    DrawRectangleLinesEx(screen.passBox, 2, screen.editPassMod ? BLUE : DARKGRAY);
    DrawText("Password:", screen.passBox.x - 120, screen.passBox.y + 15, 20, BLACK); // Changed to BLACK
    
    // Draw password as asterisks
    if (screen.passCharCount > 0) {
        char hiddenPass[MAX_CHAR_INPUT + 1] = {0};
        memset(hiddenPass, '*', screen.passCharCount);
        
        int passWidth = MeasureText(hiddenPass, 20);
        int maxPassWidth = screen.passBox.width - 20;
        
        if (passWidth <= maxPassWidth) {
            DrawText(hiddenPass, screen.passBox.x + 10, screen.passBox.y + 15, 20, BLACK); // Changed to BLACK
        } else {
            // Handle long passwords with ellipsis
            int charsToShow = 0;
            while (MeasureText(TextSubtext(hiddenPass, 0, charsToShow), 20) < maxPassWidth) {
                charsToShow++;
            }
            charsToShow--;
            
            const char* visiblePass = TextFormat("%.*s...", charsToShow - 3, hiddenPass);
            DrawText(visiblePass, screen.passBox.x + 10, screen.passBox.y + 15, 20, BLACK);
        }
    }
    
    // Draw cursors (blinking effect)
    float time = GetTime();
    if ((int)(time * 2) % 2 == 0) {
        if (screen.editNameMod) {
            int cursorX = screen.nameBox.x + 10 + MeasureText(screen.name, 20);
            // Ensure cursor stays within box
            if (cursorX < screen.nameBox.x + screen.nameBox.width - 5) {
                DrawText("|", cursorX, screen.nameBox.y + 15, 20, RED);
            }
        }
        if (screen.editPassMod) {
            char hiddenPass[MAX_CHAR_INPUT + 1] = {0};
            memset(hiddenPass, '*', screen.passCharCount);
            int cursorX = screen.passBox.x + 10 + MeasureText(hiddenPass, 20);
            // Ensure cursor stays within box
            if (cursorX < screen.passBox.x + screen.passBox.width - 5) {
                DrawText("|", cursorX, screen.passBox.y + 15, 20, RED);
            }
        }
    }
    
    // Draw login button
    DrawRectangleRec(screen.loginBtn, SKYBLUE);
    DrawRectangleLinesEx(screen.loginBtn, 2, DARKBLUE);
    DrawText("LOGIN", 
            screen.loginBtn.x + screen.loginBtn.width/2 - MeasureText("LOGIN", 20)/2,
            screen.loginBtn.y + 15, 20, DARKBLUE);
}


bool CheckLoginCredentials(LoginScreen *screen, sqlite3 *db){
    if(screen->nameCharCount == 0 || screen->passCharCount == 0) {
        TraceLog(LOG_WARNING, "Username or password is empty");
        return false; // Cannot login with empty fields
    }
    int rc1 = sqlite3_open("database/game.db", &db);
    if (rc1) {
        TraceLog(LOG_ERROR, "Failed to open database: %s", sqlite3_errmsg(db));
        return false;
    }
    // Prepare SQL statement to check credentials
    if (db == NULL) {
        TraceLog(LOG_ERROR, "Database is not initialized");
        return false;
    }
    sqlite3_stmt *stmt;
    const char *sql = "SELECT COUNT(*) FROM users WHERE name = ? AND password = ?";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        TraceLog(LOG_ERROR, "Failed to prepare statement: %s", sqlite3_errmsg(db));
        return false;
    }

    sqlite3_bind_text(stmt, 1, screen->name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, screen->password, -1, SQLITE_STATIC);
    rc = sqlite3_step(stmt);
    bool isValid = false;   
    if (rc == SQLITE_ROW) {
        int count = sqlite3_column_int(stmt, 0);
        isValid = (count > 0);
        if (isValid) {
            TraceLog(LOG_INFO, "Login successful for user: %s", screen->name);
        } else {
            TraceLog(LOG_WARNING, "Invalid login credentials for user: %s", screen->name);
        }
    } else {
        TraceLog(LOG_ERROR, "Failed to execute statement: %s", sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    // Reset the login screen after checking credentials
    memset(screen->name, 0, sizeof(screen->name));
    memset(screen->password,0, sizeof(screen->password));
    screen->nameCharCount = 0;  
    screen->passCharCount = 0;

    return isValid;

};


