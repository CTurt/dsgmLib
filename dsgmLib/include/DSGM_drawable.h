#pragma once

#define DSGM_Color(r, g, b) ARGB16(1, r, g, b)

#define DSGM_BLACK DSGM_Color(0, 0, 0)
#define DSGM_WHITE DSGM_Color(31, 31, 31)
#define DSGM_RED DSGM_Color(31, 0, 0)
#define DSGM_GREEN DSGM_Color(0, 31, 0)
#define DSGM_BLUE DSGM_Color(0, 0, 31)

void DSGM_InitDrawableBackground(DSGM_BackgroundInstance *backgroundInstance);

#define DSGM_DrawPixelToBackground(screen, x, y, color) DSGM_DrawPixelToBackgroundFull(&DSGM_Rooms[DSGM_currentRoom], screen, x, y, color)
void DSGM_DrawPixelToBackgroundFull(DSGM_Room *room, u8 screen, int x, int y, u16 color);

#define DSGM_DrawRectangleToBackground(screen, x, y, width, height, color) DSGM_DrawRectangleToBackgroundFull(&DSGM_Rooms[DSGM_currentRoom], screen, x, y, width, height, color)
void DSGM_DrawRectangleToBackgroundFull(DSGM_Room *room, u8 screen, int x, int y, int width, int height, u16 color);

#define DSGM_DrawLineToBackground(screen, x1, y1, x2, y2, thickness, color) DSGM_DrawLineToBackgroundFull(&DSGM_Rooms[DSGM_currentRoom], screen, x1, y1, x2, y2, thickness, color)
void DSGM_DrawLineToBackgroundFull(DSGM_Room *room, u8 screen, int x1, int y1, int x2, int y2, int thickness, u16 color);
