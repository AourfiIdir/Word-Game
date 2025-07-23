#include "raylib.h"

int main(void) {
    // Initialization
    InitWindow(800, 450, "Play Music Example");
    InitAudioDevice();      // 👈 Required for audio

    Music music = LoadMusicStream("C:/c_projects/testraylib/audio/audio.mp3");  // Your file
    PlayMusicStream(music);                      // 👈 Start playing

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        UpdateMusicStream(music);                // 👈 Must be called each frame

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Playing music...", 200, 200, 20, LIGHTGRAY);
        EndDrawing();
    }

    // Cleanup
    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
