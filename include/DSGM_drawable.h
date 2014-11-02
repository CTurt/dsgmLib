#pragma once

#define DSGM_DRAWABLE_BACKGROUND (void *)2

#define DSGM_Color(r, g, b) ARGB16(1, r, g, b)
#define DSGM_RandomColor() DSGM_Random(0, DSGM_Color(31, 31, 31))

#define DSGM_BLACK DSGM_Color(0, 0, 0)
#define DSGM_GRAY DSGM_Color(15, 15, 15)
#define DSGM_WHITE DSGM_Color(31, 31, 31)
#define DSGM_RED DSGM_Color(31, 0, 0)
#define DSGM_GREEN DSGM_Color(0, 31, 0)
#define DSGM_BLUE DSGM_Color(0, 0, 31)
#define DSGM_YELLOW DSGM_Color(31, 31, 0)
#define DSGM_MAGENTA DSGM_Color(31, 0, 31)
#define DSGM_BRIGHT_BLUE DSGM_Color(0, 31, 31)

void DSGM_InitDrawableBackground(DSGM_Layer *layer);

#define DSGM_ClearDrawableBackground(screen) DSGM_ClearDrawableBackgroundFull(&DSGM_Rooms[DSGM_currentRoom], screen)
inline void DSGM_ClearDrawableBackgroundFull(DSGM_Room *room, u8 screen);

#define DSGM_DrawPixelToBackground(screen, x, y, color) DSGM_DrawPixelToBackgroundFull(&DSGM_Rooms[DSGM_currentRoom], screen, x, y, color)
inline void DSGM_DrawPixelToBackgroundFull(DSGM_Room *room, u8 screen, int x, int y, u16 color);

#define DSGM_GetDrawablePixel(screen, x, y) DSGM_GetDrawablePixelFull(&DSGM_Rooms[DSGM_currentRoom], screen, x, y)
inline u16 DSGM_GetDrawablePixelFull(DSGM_Room *room, u8 screen, int x, int y);

#define DSGM_DrawRectangleToBackground(screen, x, y, width, height, thickness, color) DSGM_DrawRectangleToBackgroundFull(&DSGM_Rooms[DSGM_currentRoom], screen, x, y, width, height, thickness, color)
inline void DSGM_DrawRectangleToBackgroundFull(DSGM_Room *room, u8 screen, int x, int y, int width, int height, int thickness, u16 color);

#define DSGM_DrawFilledRectangleToBackground(screen, x, y, width, height, color) DSGM_DrawFilledRectangleToBackgroundFull(&DSGM_Rooms[DSGM_currentRoom], screen, x, y, width, height, color)
inline void DSGM_DrawFilledRectangleToBackgroundFull(DSGM_Room *room, u8 screen, int x, int y, int width, int height, u16 color);

#define DSGM_DrawLineToBackground(screen, x1, y1, x2, y2, thickness, color) DSGM_DrawLineToBackgroundFull(&DSGM_Rooms[DSGM_currentRoom], screen, x1, y1, x2, y2, thickness, color)
inline void DSGM_DrawLineToBackgroundFull(DSGM_Room *room, u8 screen, int x1, int y1, int x2, int y2, int thickness, u16 color);

#define DSGM_DrawLineAtAngleToBackground(screen, x, y, angle, length, thickness, color) DSGM_DrawLineAtAngleToBackgroundFull(&DSGM_Rooms[DSGM_currentRoom], screen, x, y, angle, length, thickness, color)
inline void DSGM_DrawLineAtAngleToBackgroundFull(DSGM_Room *room, u8 screen, int x, int y, int angle, int length, int thickness, u16 color);

#define DSGM_DrawCircleToBackground(screen, x, y, radius, thickness, color) DSGM_DrawCircleToBackgroundFull(&DSGM_Rooms[DSGM_currentRoom], screen, x, y, radius, thickness, color)
inline void DSGM_DrawCircleToBackgroundFull(DSGM_Room *room, u8 screen, int x, int y, int radius, int thickness, u16 color);

#define DSGM_DrawFilledCircleToBackground(screen, x, y, radius, color) DSGM_DrawFilledCircleToBackgroundFull(&DSGM_Rooms[DSGM_currentRoom], screen, x, y, radius, color)
inline void DSGM_DrawFilledCircleToBackgroundFull(DSGM_Room *room, u8 screen, int x, int y, int radius, u16 color);

#define DSGM_DrawTileToBackground(screen, x, y, font, tile, size, color) DSGM_DrawTileToBackgroundFull(&DSGM_Rooms[DSGM_currentRoom], screen, x, y, font, tile, size, color)
inline void DSGM_DrawTileToBackgroundFull(DSGM_Room *room, u8 screen, int x, int y, DSGM_Background *font, char tile, int size, u16 color);

#define DSGM_DrawTextToBackground(screen, x, y, font, size, color, format, ...) DSGM_DrawTextToBackgroundFull(&DSGM_Rooms[DSGM_currentRoom], screen, x, y, font, size, color, format, ##__VA_ARGS__)
inline void DSGM_DrawTextToBackgroundFull(DSGM_Room *room, u8 screen, int x, int y, DSGM_Background *font, int size, u16 color, const char *format, ...);
