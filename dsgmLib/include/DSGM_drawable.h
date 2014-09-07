#pragma once

#define DSGM_Colour(r, g, b) ARGB16(1, r, g, b)
#define DSGM_Color(r, g, b) DSGM_Colour(r, g, b)

void DSGM_InitDrawableBackground(DSGM_BackgroundInstance *backgroundInstance);

#define DSGM_DrawPixelToBackground(screen, x, y, colour) DSGM_DrawPixelToBackgroundFull(&DSGM_Rooms[DSGM_currentRoom], screen, x, y, colour)
void DSGM_DrawPixelToBackgroundFull(DSGM_Room *room, u8 screen, int x, int y, u16 colour);

#define DSGM_DrawRectangleToBackground(screen, x, y, width, height, colour) DSGM_DrawRectangleToBackgroundFull(&DSGM_Rooms[DSGM_currentRoom], screen, x, y, width, height, colour)
void DSGM_DrawRectangleToBackgroundFull(DSGM_Room *room, u8 screen, int x, int y, int width, int height, u16 colour);

#define DSGM_DrawLineToBackground(screen, x1, y1, x2, y2, thickness, colour) DSGM_DrawLineToBackgroundFull(&DSGM_Rooms[DSGM_currentRoom], screen, x1, y1, x2, y2, thickness, colour)
void DSGM_DrawLineToBackgroundFull(DSGM_Room *room, u8 screen, int x1, int y1, int x2, int y2, int thickness, u16 colour);
