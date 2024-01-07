#include "math.h"
#include "raylib.h"
#include "stdlib.h"

struct GridTile {
    Vector2 screen_coordinates;
    bool is_visible;
};
struct Triangle {
    Vector2 A;
    Vector2 B;
    Vector2 C;
};

void GenerateHexGrid(double radius, double width, double height,
                     struct GridTile** grid) {
    double offset_x = 100;
    double offset_y = 100;
    float horiz = sqrtf(3) * radius;
    float vert = (3.0 / 2.0) * radius;

    for (int y = 0; y < height; y++) {
        for (int x = -(int)(y * 0.5); x < width - (int)(y * 0.5); x++) {
            double hex_x = (horiz * (x + (y * 0.5))) + offset_x;
            double hex_y = (vert * y) + offset_y;
            grid[y][x].is_visible = true;
            grid[y][x].screen_coordinates.x = hex_x;
            grid[y][x].screen_coordinates.y = hex_y;
        }
    }
};

void DrawHexGrid(double radius, double width, double height,
                 struct GridTile** grid) {
    for (int y = 0; y < height; y++) {
        for (int x = -(int)(y * 0.5); x < width - (int)(y * 0.5); x++) {
            DrawPoly((Vector2){grid[y][x].screen_coordinates.x,
                               grid[y][x].screen_coordinates.y},
                     6, radius, 90, LIGHTGRAY);
        }
    }
};

void DrawHexGridOutline(double radius, double width, double height,
                        struct GridTile** grid) {
    for (int y = 0; y < height; y++) {
        for (int x = -(int)(y * 0.5); x < width - (int)(y * 0.5); x++) {
            double hex_x = grid[y][x].screen_coordinates.x;
            double hex_y = grid[y][x].screen_coordinates.y;
            DrawPolyLines((Vector2){hex_x, hex_y}, 6, radius, 90, BLACK);
        }
    }
};

void DrawDebugCoordinatesOnHexGrid(double radius, double width, double height,
                                   struct GridTile** grid) {
    for (int y = 0; y < height; y++) {
        for (int x = -(int)(y * 0.5); x < width - (int)(y * 0.5); x++) {
            double hex_x = grid[y][x].screen_coordinates.x;
            double hex_y = grid[y][x].screen_coordinates.y;
            DrawText(TextFormat("x: %d", (int)x + (int)(height / 2)),
                     hex_x - (radius / 2), hex_y, 20, RED);
            DrawText(TextFormat("y: %d", (int)y), hex_x - (radius / 2),
                     hex_y - (radius / 2), 20, BLUE);
        }
    }
};

void DrawMouseOnGrid(double radius, double width, double height,
                     struct GridTile** grid) {
    float horiz = sqrtf(3) * radius;
    float vert = (3.0 / 2.0) * radius;
    Vector2 mouse_position = GetMousePosition();
    for (int y = 0; y < height; y++) {
        for (int x = -(int)(y * 0.5); x < width - (int)(y * 0.5); x++) {
            double hex_x = grid[y][x].screen_coordinates.x;
            double hex_y = grid[y][x].screen_coordinates.y;
            double mouse_hex_center_distance =
                sqrt(pow(hex_x - mouse_position.x, 2) +
                     pow(hex_y - mouse_position.y, 2));
            if (mouse_hex_center_distance < (radius * 0.8)) {
                DrawPolyLinesEx((Vector2){hex_x, hex_y}, 6, radius, 90, 7,
                                GRAY);
                // up, left right
                DrawTriangle(
                    (Vector2){hex_x, hex_y},
                    (Vector2){hex_x, hex_y + (horiz * 0.5)},
                    (Vector2){hex_x + (vert * 0.5), hex_y + (horiz * 0.25)},
                    YELLOW);
                DrawTriangle(
                    (Vector2){hex_x, hex_y},
                    (Vector2){hex_x - (vert * 0.5), hex_y + (horiz * 0.25)},
                    (Vector2){hex_x, hex_y + (horiz * 0.5)}, ORANGE);
                DrawTriangle(
                    (Vector2){hex_x, hex_y},
                    (Vector2){hex_x - (vert * 0.5), hex_y - (horiz * 0.25)},
                    (Vector2){hex_x - (vert * 0.5), hex_y + (horiz * 0.25)},
                    RED);
                DrawTriangle(
                    (Vector2){hex_x, hex_y},
                    (Vector2){hex_x, hex_y - (horiz * 0.5)},
                    (Vector2){hex_x - (vert * 0.5), hex_y - (horiz * 0.25)},
                    PURPLE);
                DrawTriangle(
                    (Vector2){hex_x, hex_y},
                    (Vector2){hex_x + (vert * 0.5), hex_y - (horiz * 0.25)},
                    (Vector2){hex_x, hex_y - (horiz * 0.5)}, BLUE);
                DrawTriangle(
                    (Vector2){hex_x, hex_y},
                    (Vector2){hex_x + (vert * 0.5), hex_y + (horiz * 0.25)},
                    (Vector2){hex_x + (vert * 0.5), hex_y - (horiz * 0.25)},
                    GREEN);
            }
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
    int grid_width = width + (height / 2);
    int grid_height = height;
    double radius = 40;
    // memory allocation
    ///////////////////////////////////////////////////////
    struct GridTile** grid =
        (struct GridTile**)malloc(grid_height * sizeof(struct GridTile*));
    for (int x = 0; x < grid_height; x++) {
        grid[x] =
            (struct GridTile*)malloc(grid_width * sizeof(struct GridTile));
    }
    ///////////////////////////////////////////////////////
    for (int y = 0; y < grid_height; y++) {
        for (int x = 0; x < grid_width; x++) {
            grid[y][x].is_visible = false;
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
        GenerateHexGrid(radius, width, height, grid);
        DrawHexGrid(radius, width, height, grid);
        DrawMouseOnGrid(radius, width, height, grid);
        DrawHexGridOutline(radius, width, height, grid);
        DrawDebugCoordinatesOnHexGrid(radius, width, height, grid);
        DrawCircleV(ballPosition, 3, ballColor);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
