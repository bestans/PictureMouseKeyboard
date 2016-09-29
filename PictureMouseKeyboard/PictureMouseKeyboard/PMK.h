#pragma once
#define DllExport __declspec(dllexport)
#include<Windows.h>
#include<ctime>

//Public API

class DllExport COORDINATE;

class DllExport RGB;

DllExport UINT CALLBACK ClickPic(HBITMAP hbmSmall, INT nAddX = 0, INT nAddY = 0, UINT nStep = 1, UINT nErrorLimit = 0, BOOL bDoubleClick = FALSE, UINT nL0M1R2Button = 0, DWORD dwSpan = 0);

DllExport UINT CALLBACK ClickPic(LPCWSTR lpWindowName, HBITMAP hbmSmall, INT nAddX = 0, INT nAddY = 0, UINT nStep = 1, UINT nErrorLimit = 0, BOOL bDoubleClick = FALSE, UINT nL0M1R2Button = 0, DWORD dwSpan = 0);

DllExport UINT CALLBACK ClickPic(HWND hWindow, HBITMAP hbmSmall, INT nAddX = 0, INT nAddY = 0, UINT nStep = 1, UINT nErrorLimit = 0, BOOL bDoubleClick = FALSE, UINT nL0M1R2Button = 0, DWORD dwSpan = 0);

DllExport COORDINATE CALLBACK FindPic(HBITMAP hbmSmall, HBITMAP hbmLarge, UINT nStep = 1);

DllExport HBITMAP CALLBACK GetPic();

DllExport HBITMAP CALLBACK GetPic(LPCWSTR lpWindowName);

DllExport HBITMAP CALLBACK GetPic(HWND hWindow);

DllExport UINT CALLBACK KeyboardClick(WORD wKey, UINT nC0D1U2 = 0);

DllExport UINT CALLBACK KeyboardClick(LPCWSTR lpWindowName, WORD wKey, UINT nC0D1U2 = 0);

DllExport UINT CALLBACK KeyboardClick(HWND hWindow, WORD wKey, UINT nC0D1U2 = 0);

DllExport UINT CALLBACK MouseClick(INT nX, INT nY, UINT nErrorLimit = 0, BOOL bDoubleClick = FALSE, UINT nL0M1R2Button = 0, DWORD dwSpan = 0);

DllExport UINT CALLBACK MouseClick(LPCWSTR lpWindowName, INT nX, INT nY, UINT nErrorLimit = 0, BOOL bDoubleClick = FALSE, UINT nL0M1R2Button = 0, DWORD dwSpan = 0);

DllExport UINT CALLBACK MouseClick(HWND hWindow, INT nX, INT nY, UINT nErrorLimit = 0, BOOL bDoubleClick = FALSE, UINT nL0M1R2Button = 0, DWORD dwSpan = 0);

DllExport UINT CALLBACK SavePic(LPCWSTR lpFileName);

DllExport UINT CALLBACK SavePic(LPCWSTR lpWindowName, LPCWSTR lpFileName);

DllExport UINT CALLBACK SavePic(HWND hWindow, LPCWSTR lpFileName);

DllExport UINT CALLBACK SavePic(HBITMAP hBitmap, LPCWSTR lpFileName);

//Private API

inline INT Private_GetnError(UINT nErrorLimit = 0);

inline RGB Private_GetRGB(BYTE *pbyPic, BYTE byPic, LONG lWidthBytes, LONG lX, LONG lY);

inline UINT Private_GetnDownUp(UINT &nDown, UINT &nUp, UINT nL0M1R2Button = 0, BOOL bDoubleClick = FALSE);