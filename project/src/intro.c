#include <raylib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main(void)
{
  InitWindow(600, 400, "CNAKE - Intro");
  SetTargetFPS(60);

  while (!WindowShouldClose())
  {
    BeginDrawing();
      ClearBackground(RAYWHITE);
      
      const int font_size = 20;
      const int y = 150;
      const int spacing = 20;

      const char *label_title = "Title:";
      const char *label_name = "My name:";
      const char *label_user = "Github/edX:";
      const char *label_address = "City/Country:";
      const char *label_date = "Date:";

      const char *value_title = "CNAKE";
      const char *value_name = "Anders Strandseter";
      const char *value_user = "strandseter/anders_strandseter";
      const char *value_address = "Fredrikstad/Norway";
      const char *value_date = "2024-12-22";

      int max_label_width = 0;
      max_label_width = fmax(max_label_width, MeasureText(label_title, font_size));
      max_label_width = fmax(max_label_width, MeasureText(label_name, font_size));
      max_label_width = fmax(max_label_width, MeasureText(label_user, font_size));
      max_label_width = fmax(max_label_width, MeasureText(label_address, font_size));
      max_label_width = fmax(max_label_width, MeasureText(label_date, font_size));

      const int label_x = 50;
      const int value_x = label_x + max_label_width + 10;

      DrawText(label_title, label_x, y, font_size, RED);
      DrawText(label_name, label_x, y + spacing, font_size, RED);
      DrawText(label_user, label_x, y + spacing * 2, font_size, RED);
      DrawText(label_address, label_x, y + spacing * 3, font_size, RED);
      DrawText(label_date, label_x, y + spacing * 4, font_size, RED);

      DrawText(value_title, value_x, y, font_size, RED);
      DrawText(value_name, value_x, y + spacing, font_size, RED);
      DrawText(value_user, value_x, y + spacing * 2, font_size, RED);
      DrawText(value_address, value_x, y + spacing * 3, font_size, RED);
      DrawText(value_date, value_x, y + spacing * 4, font_size, RED);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}