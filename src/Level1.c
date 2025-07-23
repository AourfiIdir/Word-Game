#include "Level1.h"

#define levelLength 5
#define MAX_LENGTH 30
#define MAX_LEVEL_WORD 5

extern sqlite3 *gameDB;

Texture2D backgroundLevel1;
inputField input;
button btn;
word **wordList;

void freeWordList(word **list, int size) {
    if (!list) return;
    for (int i = 0; i < size; i++) {
        if (list[i]) {
            if (list[i]->word) free(list[i]->word);
            if (list[i]->definition) free(list[i]->definition);
            free(list[i]);
        }
    }
    free(list);
}

void trimInput(char *str) {
    int len = strlen(str);
    while (len > 0 && isspace((unsigned char)str[len - 1])) str[--len] = '\0';
}

void loadLevel1(void) {
     printf("LEVEL1: loadLevel1() called\n");
    backgroundLevel1 = LoadTexture("graphics/level1.png");
    if (!backgroundLevel1.id) {
        TraceLog(LOG_WARNING, "Failed to load level 1 background texture");
    }
    initialiseLevel1();
}

void unloadLevel1(void) {
    UnloadTexture(backgroundLevel1);

    if (input.input) {
        free(input.input);
        input.input = NULL;
    }

    if (wordList) {
        freeWordList(wordList, MAX_LEVEL_WORD);
        wordList = NULL;
    }
}

void updateLevel1() {
    Vector2 mousePos = GetMousePosition();

    if (CheckCollisionPointRec(mousePos, input.textBox)) {
        input.curserOnText = true;
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            input.activeBox = true;
        }
    } else {
        input.curserOnText = false;
        input.activeBox = false;
    }

    if (input.activeBox) {
        if (IsKeyPressed(KEY_BACKSPACE)) {
            input.length--;
            if (input.length < 0) input.length = 0;
            input.input[input.length] = '\0';
        }

        int key = GetCharPressed();
        while (key > 0) {
            if ((key >= 32) && (key <= 125) && (input.length < MAX_LENGTH - 1)) {
                input.input[input.length] = (char)key;
                input.length++;
                input.input[input.length] = '\0';
            }
            key = GetCharPressed();
        }
    }

    if (CheckCollisionPointRec(mousePos, btn.rect)) {
        btn.hovered = true;
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            btn.isPressed = true;
            if (input.length > 0) {
                trimInput(input.input);
                int result = checkWordLevel1(wordList, input.input);
                if (result != -1) {
                    if (!wordList[result]->checked) {
                        wordList[result]->checked = true;
                        TraceLog(LOG_INFO, "Word '%s' found and marked as checked", input.input);
                    } else {
                        TraceLog(LOG_INFO, "Word '%s' already submitted", input.input);
                    }
                }
                input.length = 0;
                memset(input.input, 0, sizeof(char) * MAX_LENGTH);
            }
        }
    } else {
        btn.hovered = false;
    }
}

void drawLevel1() {
    DrawTexture(backgroundLevel1, 0, 0, WHITE);

    DrawRectangleRec(input.textBox, Fade(LIGHTGRAY, 0.8f));
    DrawRectangleLinesEx(input.textBox, 2, input.curserOnText ? BLUE : DARKGRAY);

    if (input.length > 0) {
        DrawText(input.input, input.textBox.x + 10, input.textBox.y + 15, 20, BLACK);
    }

    /*
    // Optional cursor blinking logic
    static int frameCount = 0;
    frameCount++;
    if (input.activeBox && (frameCount / 30) % 2 == 0) {
        DrawText("|", input.textBox.x + 10 + MeasureText(input.input, 20), input.textBox.y + 15, 20, BLACK);
    }
    */

    DrawRectangleRec(btn.rect, btn.color);
    DrawRectangleLinesEx(btn.rect, 2, DARKBLUE);
    DrawText(btn.text, btn.rect.x + btn.rect.width / 2 - MeasureText(btn.text, 20) / 2,
             btn.rect.y + 15, 20, DARKBLUE);

    for (int i = 0; i < MAX_LEVEL_WORD; i++) {
    if (wordList[i] && wordList[i]->definition) {
        DrawRectangle(8, 10 + i * 30 + 2, MeasureText(wordList[i]->definition, 20), 20, WHITE);
        DrawText(wordList[i]->definition, 10, 10 + i * 30, 20, BLACK);

        if (wordList[i]->checked) {
            DrawText(" Y ", 10 + MeasureText(wordList[i]->definition, 20), 10 + i * 30, 20, GREEN);
            DrawText(wordList[i]->word, 10 + MeasureText("✓", 20) + MeasureText(wordList[i]->definition, 20), 10 + i * 30, 20, BLACK);
        } else {
            DrawText(" ✗ ", 10 + MeasureText(wordList[i]->definition, 20), 10 + i * 30, 20, RED);
        }
    }
}
}

void initialiseLevel1() {
    input.input = malloc(sizeof(char) * MAX_LENGTH);
    if (!input.input) {
        TraceLog(LOG_WARNING, "Failed to allocate input buffer");
        return;
    }
    memset(input.input, 0, sizeof(char) * MAX_LENGTH);
    input.length = 0;
    input.textBox = (Rectangle){screenWidth / 2 - 100, screenHeight / 2 + 20, 200, 40};
    input.curserOnText = false;
    input.activeBox = false;

    btn.rect = (Rectangle){screenWidth / 2 - 50, screenHeight / 2 + 61, 100, 40};
    btn.text = "Submit";
    btn.color = SKYBLUE;
    btn.isPressed = false;
    btn.hovered = false;

    wordList = malloc(sizeof(word*) * MAX_LEVEL_WORD);
    if (!wordList) {
        TraceLog(LOG_WARNING, "Failed to allocate wordList array");
        free(input.input);
        return;
    }

    for (int i = 0; i < MAX_LEVEL_WORD; i++) {
        wordList[i] = NULL;
    }

    const char *sql = "SELECT word, definition FROM words WHERE length = ?";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(gameDB, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(gameDB));
        free(wordList);
        free(input.input);
        return;
    }

    rc = sqlite3_bind_int(stmt, 1, levelLength);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to bind parameter: %s\n", sqlite3_errmsg(gameDB));
        sqlite3_finalize(stmt);
        free(wordList);
        free(input.input);
        return;
    }

    int index = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW && index < MAX_LEVEL_WORD) {
        wordList[index] = malloc(sizeof(word));
        if (!wordList[index]) {
            TraceLog(LOG_WARNING, "Failed to allocate memory for word struct");
            break;
        }

        const unsigned char *wordText = sqlite3_column_text(stmt, 0);
        const unsigned char *defText = sqlite3_column_text(stmt, 1);

        wordList[index]->word = strdup((const char *)wordText);
        wordList[index]->definition = strdup((const char *)defText);

        if (!wordList[index]->word || !wordList[index]->definition) {
            TraceLog(LOG_WARNING, "Failed to duplicate word/definition");
            if (wordList[index]->word) free(wordList[index]->word);
            if (wordList[index]->definition) free(wordList[index]->definition);
            free(wordList[index]);
            wordList[index] = NULL;
            continue;
        }

        wordList[index]->checked = false;
        index++;
    }

    sqlite3_finalize(stmt);
}

int checkWordLevel1(word **w, char *input) {
    for (int i = 0; i < MAX_LEVEL_WORD; i++) {
        if (w[i] && strcmp(w[i]->word, input) == 0) {
            return i;
        }
    }
    return -1;
}

int checkFinishedLevel1(word **w) {
    for (int i = 0; i < MAX_LEVEL_WORD; i++) {
        if (w[i] && !w[i]->checked) {
            return 0;
        }
    }
    return 1;
}
