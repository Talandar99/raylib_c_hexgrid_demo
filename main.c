#include "math.h"
#include "raylib.h"

struct HexGridTile{
    Vector2 screen_coordinates;
    double radius;
    bool is_visible;
};

void DrawHexagon(double radius, double width, double height,struct HexGridTile grid[][grid_height],int grid_height) {
    // first line
    double offset_x = 100;
    double offset_y = 100;
    float horiz = sqrtf(3) * radius;
    float vert = (3.0 / 2.0) * radius;
    double x = 0, y = 0;

    Vector2 mouse_position = GetMousePosition();

    for (int y = 0; y < height; y++) {
        for (int x = -(int)(y * 0.5); x < width - (int)(y * 0.5); x++) {
            double hex_x = (horiz * (x + (y * 0.5))) + offset_x;
            double hex_y = (vert * y) + offset_y;
            double mouse_hex_center_distance =
                sqrt(pow(hex_x - mouse_position.x, 2) +
                     pow(hex_y - mouse_position.y, 2));

            if (mouse_hex_center_distance < (radius * 0.8)) {
                DrawPoly((Vector2){hex_x, hex_y}, 6, radius, 90, GRAY);
            } else {
                DrawPoly((Vector2){hex_x, hex_y}, 6, radius, 90, LIGHTGRAY);
            }
            DrawText(TextFormat("x: %d", (int)x + (int)(height / 2)),
                     hex_x - (radius / 2), hex_y, 20, RED);
            DrawText(TextFormat("y: %d", (int)y), hex_x - (radius / 2),
                     hex_y - (radius / 2), 20, BLUE);
        }
    }
};

int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------------
    // 720p
    const int screenWidth = 1280;
    const int screenHeight = 720;
    int width = 16;
    int height = 9;
    int grid_width = width+(height / 2);
    int grid_height = height;

    struct HexGridTile grid[grid_width][grid_height];
    for (int x=0; x<grid_width; x++) {
        for (int y=0; y<grid_height; y++) {
            grid[x][y].is_visible=false;
        }
    }
    InitWindow(screenWidth, screenHeight, "hex grid demo");
    SetTargetFPS(60);
    Vector2 ballPosition = {-100.0f, -100.0f};
    Color ballColor = DARKBLUE;
    while (!WindowShouldClose()) {
        // Update
        //----------------------------------------------------------------------------------
        ballPosition = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            ballColor = MAROON;
        else if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE))
            ballColor = LIME;
        else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
            ballColor = DARKBLUE;
        else if (IsMouseButtonPressed(MOUSE_BUTTON_SIDE))
            ballColor = PURPLE;
        else if (IsMouseButtonPressed(MOUSE_BUTTON_EXTRA))
            ballColor = YELLOW;
        else if (IsMouseButtonPressed(MOUSE_BUTTON_FORWARD))
            ballColor = ORANGE;
        else if (IsMouseButtonPressed(MOUSE_BUTTON_BACK))
            ballColor = BEIGE;
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(RAYWHITE);
        double radius = 40.;

        DrawHexagon(radius, width, height, grid, grid_height);
        DrawCircleV(ballPosition, 10, ballColor);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
