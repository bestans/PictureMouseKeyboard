#include"PMK.h"

INT WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, PVOID pvReserved)
{
	return TRUE;
}

class DllExport COORDINATE
{
public:
	LONG lX = 0;
	LONG lY = 0;
	BOOL bValid = FALSE;
};

class DllExport RGB
{
public:
	INT nR = 0;
	INT nG = 0;
	INT nB = 0;
	BOOL bValid = FALSE;

	inline BOOL operator ==(RGB &rgb2)
	{
		if (
			nR == rgb2.nR &&
			nG == rgb2.nG &&
			nB == rgb2.nB &&
			bValid == rgb2.bValid
			)
		{
			return TRUE;
		}

		return FALSE;
	}

	inline BOOL operator !=(RGB &rgb2)
	{
		if (
			nR == rgb2.nR &&
			nG == rgb2.nG &&
			nB == rgb2.nB &&
			bValid == rgb2.bValid
			)
		{
			return FALSE;
		}

		return TRUE;
	}
};

DllExport UINT CALLBACK ClickPic(HBITMAP hbmSmall, INT nAddX, INT nAddY, UINT nStep, UINT nErrorLimit, BOOL bDoubleClick, UINT nL0M1R2Button, DWORD dwSpan)
{
	HBITMAP hbmDesktop = GetPic();
	COORDINATE cPic = FindPic(hbmSmall, hbmDesktop, nStep);
	if (cPic.bValid == FALSE)
	{
		return 1;
	}

	MouseClick(cPic.lX + nAddX, cPic.lY + nAddY, nErrorLimit, bDoubleClick, nL0M1R2Button, dwSpan);
	return 0;
}

DllExport UINT CALLBACK ClickPic(LPCWSTR lpWindowName, HBITMAP hbmSmall, INT nAddX, INT nAddY, UINT nStep, UINT nErrorLimit, BOOL bDoubleClick, UINT nL0M1R2Button, DWORD dwSpan)
{
	ClickPic(FindWindow(NULL, lpWindowName), hbmSmall, nAddX, nAddY, nStep, nErrorLimit, bDoubleClick, nL0M1R2Button, dwSpan);
	return 0;
}

DllExport UINT CALLBACK ClickPic(HWND hWindow, HBITMAP hbmSmall, INT nAddX, INT nAddY, UINT nStep, UINT nErrorLimit, BOOL bDoubleClick, UINT nL0M1R2Button, DWORD dwSpan)
{
	HBITMAP hbmLarge = GetPic(hWindow);
	COORDINATE cPic = FindPic(hbmSmall, hbmLarge, nStep);
	if (cPic.bValid == FALSE)
	{
		return 1;
	}

	MouseClick(hWindow, cPic.lX + nAddX, cPic.lY + nAddY, nErrorLimit, bDoubleClick, nL0M1R2Button, dwSpan);
	return 0;
}

DllExport COORDINATE CALLBACK FindPic(HBITMAP hbmSmall, HBITMAP hbmLarge, UINT nStep)
{
	HDC hDCSmall = CreateCompatibleDC(NULL);
	HDC hDCLarge = CreateCompatibleDC(NULL);
	SelectObject(hDCSmall, hbmSmall);
	SelectObject(hDCLarge, hbmLarge);
	BITMAP bmSmall;
	BITMAP bmLarge;
	GetObject(hbmSmall, sizeof(BITMAP), &bmSmall);
	GetObject(hbmLarge, sizeof(BITMAP), &bmLarge);
	BYTE bySmall = bmSmall.bmBitsPixel / 8;
	BYTE byLarge = bmLarge.bmBitsPixel / 8;
	BITMAPINFO biSmall;
	BITMAPINFO biLarge;
	biSmall.bmiHeader.biSize = sizeof(biSmall.bmiHeader);
	biLarge.bmiHeader.biSize = sizeof(biLarge.bmiHeader);
	biSmall.bmiHeader.biWidth = bmSmall.bmWidth;
	biLarge.bmiHeader.biWidth = bmLarge.bmWidth;
	biSmall.bmiHeader.biHeight = -bmSmall.bmHeight;
	biLarge.bmiHeader.biHeight = -bmLarge.bmHeight;
	biSmall.bmiHeader.biPlanes = 1;
	biLarge.bmiHeader.biPlanes = 1;
	biSmall.bmiHeader.biBitCount = bmSmall.bmBitsPixel;
	biLarge.bmiHeader.biBitCount = bmLarge.bmBitsPixel;
	biSmall.bmiHeader.biCompression = BI_RGB;
	biLarge.bmiHeader.biCompression = BI_RGB;
	biSmall.bmiHeader.biSizeImage = (bmSmall.bmWidth * biSmall.bmiHeader.biBitCount + 31) / 32 * 4 * bmSmall.bmHeight;
	biLarge.bmiHeader.biSizeImage = (bmLarge.bmWidth * biLarge.bmiHeader.biBitCount + 31) / 32 * 4 * bmLarge.bmHeight;
	BYTE* pbySmall = new BYTE[biSmall.bmiHeader.biSizeImage];
	BYTE* pbyLarge = new BYTE[biLarge.bmiHeader.biSizeImage];
	ZeroMemory(pbySmall, biSmall.bmiHeader.biSizeImage);
	ZeroMemory(pbyLarge, biLarge.bmiHeader.biSizeImage);
	GetDIBits(hDCSmall, hbmSmall, 0, bmSmall.bmHeight, pbySmall, &biSmall, DIB_RGB_COLORS);
	GetDIBits(hDCLarge, hbmLarge, 0, bmLarge.bmHeight, pbyLarge, &biLarge, DIB_RGB_COLORS);

	LONG lSmallWidth = bmSmall.bmWidth - nStep;
	LONG lLargeWidth = bmLarge.bmWidth - lSmallWidth - 1;
	LONG lSmallHeight = bmSmall.bmHeight - nStep;
	LONG lLargeHeigh = bmLarge.bmHeight - lSmallHeight - 1;

	COORDINATE cResult;

	RGB rgbSmall = Private_GetRGB(pbySmall, bySmall, bmSmall.bmWidthBytes, 0, 0);

	for (LONG lLargeX = 0; lLargeX <= lLargeWidth; lLargeX++)
	{
		for (LONG lLargeY = 0; lLargeY <= lLargeHeigh; lLargeY++)
		{
			RGB rgbLarge = Private_GetRGB(pbyLarge, byLarge, bmLarge.bmWidthBytes, lLargeX, lLargeY);

			if (rgbSmall == rgbLarge)
			{
				for (LONG lSmallX2 = 1; lSmallX2 <= lSmallWidth; lSmallX2 += nStep)
				{
					for (LONG lSmallY2 = 1; lSmallY2 <= lSmallHeight; lSmallY2 += nStep)
					{
						RGB rgbSmall2 = Private_GetRGB(pbySmall, bySmall, bmSmall.bmWidthBytes, lSmallX2, lSmallY2);
						RGB rgbLarge2 = Private_GetRGB(pbyLarge, byLarge, bmLarge.bmWidthBytes, lLargeX + lSmallX2, lLargeY + lSmallY2);

						if (rgbSmall2 != rgbLarge2)
						{
							break;
						}
						else if (lSmallX2 + nStep > lSmallWidth && lSmallY2 + nStep > lSmallHeight)
						{
							cResult.lX = lLargeX;
							cResult.lY = lLargeY;
							cResult.bValid = TRUE;

							DeleteDC(hDCSmall);
							DeleteDC(hDCLarge);
							delete[] pbySmall;
							delete[] pbyLarge;
							pbySmall = NULL;
							pbyLarge = NULL;

							return cResult;
						}
					}
				}
			}
		}
	}

	DeleteDC(hDCSmall);
	DeleteDC(hDCLarge);
	delete[] pbySmall;
	delete[] pbyLarge;
	pbySmall = NULL;
	pbyLarge = NULL;

	return cResult;

	COORDINATE aaa;
	return aaa;
}

DllExport HBITMAP CALLBACK GetPic()
{
	//获取桌面窗口句柄
	HWND hWndDesktop = GetDesktopWindow();
	//获取桌面窗口DC
	HDC hDCDesktop = GetWindowDC(hWndDesktop);
	RECT rectWindow;
	GetClientRect(hWndDesktop, &rectWindow);

	//获取当前窗口的位图  
	HDC hDCMemory = CreateCompatibleDC(hDCDesktop);
	HBITMAP hBitmapPic22;
	hBitmapPic22 = CreateCompatibleBitmap(hDCDesktop, rectWindow.right, rectWindow.bottom);
	SelectObject(hDCMemory, hBitmapPic22);
	BitBlt(hDCMemory, 0, 0, rectWindow.right, rectWindow.bottom, hDCDesktop, 0, 0, SRCCOPY);

	ReleaseDC(NULL, hDCDesktop);
	ReleaseDC(NULL, hDCMemory);
	DeleteDC(hDCDesktop);
	DeleteDC(hDCMemory);

	return hBitmapPic22;
}

DllExport HBITMAP CALLBACK GetPic(LPCWSTR lpWindowName)
{
	GetPic(FindWindow(NULL, lpWindowName));
	return 0;
}

DllExport HBITMAP CALLBACK GetPic(HWND hWindow)
{
	HDC hDCWindow = GetWindowDC(hWindow);

	//获取窗口的客户区区域  
	RECT rectWindow;
	GetClientRect(hWindow, &rectWindow);

	//获取当前窗口的位图  
	HDC hDCMemory = CreateCompatibleDC(hDCWindow);
	HBITMAP hBitmapPic;
	hBitmapPic = CreateCompatibleBitmap(hDCWindow, rectWindow.right, rectWindow.bottom);
	SelectObject(hDCMemory, hBitmapPic);
	BitBlt(hDCMemory, 0, 0, rectWindow.right, rectWindow.bottom, hDCWindow, 0, 0, SRCCOPY);

	ReleaseDC(NULL, hDCWindow);
	ReleaseDC(NULL, hDCMemory);
	DeleteDC(hDCWindow);
	DeleteDC(hDCMemory);

	return hBitmapPic;
}

DllExport UINT CALLBACK KeyboardClick(WORD wKey, UINT nC0D1U2)
{
	if (nC0D1U2 == 0)
	{
		KeyboardClick(wKey, 1);
		KeyboardClick(wKey, 2);

		return 0;
	}

	INPUT ipKeyboard[1];
	ZeroMemory(&ipKeyboard, sizeof(INPUT));
	ipKeyboard[0].type = INPUT_KEYBOARD;
	ipKeyboard[0].ki.wVk = wKey;

	if (nC0D1U2 == 2)
	{
		ipKeyboard[0].ki.dwFlags = KEYEVENTF_KEYUP;
	}

	SendInput(1, ipKeyboard, sizeof(INPUT));

	return 0;
}

DllExport UINT CALLBACK KeyboardClick(LPCWSTR lpWindowName, WORD wKey, UINT nC0D1U2)
{
	KeyboardClick(FindWindow(NULL, lpWindowName), wKey, nC0D1U2);
	return 0;
}

DllExport UINT CALLBACK KeyboardClick(HWND hWindow, WORD wKey, UINT nC0D1U2)
{
	SwitchToThisWindow(hWindow, TRUE);
	KeyboardClick(wKey, nC0D1U2);
	return 0;
}

DllExport UINT CALLBACK MouseClick(INT nX, INT nY, UINT nErrorLimit, BOOL bDoubleClick, UINT nL0M1R2Button, DWORD dwSpan)
{
	UINT nDown, nUp;
	Private_GetnDownUp(nDown, nUp, nL0M1R2Button, bDoubleClick);
	INT nError = Private_GetnError(nErrorLimit);

	SetCursorPos(nX + nError, nY + nError);
	mouse_event(nDown, 0, 0, 0, 0);
	Sleep(dwSpan);
	mouse_event(nUp, 0, 0, 0, 0);
	return 0;
}

DllExport UINT CALLBACK MouseClick(LPCWSTR lpWindowName, INT nX, INT nY, UINT nErrorLimit, BOOL bDoubleClick, UINT nL0M1R2Button, DWORD dwSpan)
{
	MouseClick(FindWindow(NULL, lpWindowName), nX, nY, nErrorLimit, dwSpan, nL0M1R2Button, bDoubleClick);
	return 0;
}

DllExport UINT CALLBACK MouseClick(HWND hWindow, INT nX, INT nY, UINT nErrorLimit, BOOL bDoubleClick, UINT nL0M1R2Button, DWORD dwSpan)
{
	UINT nDown, nUp;
	Private_GetnDownUp(nDown, nUp, nL0M1R2Button, bDoubleClick);
	INT nError = Private_GetnError(nErrorLimit);

	SendMessage(hWindow, nDown, NULL, MAKELPARAM(nX + nError, nY + nError));
	Sleep(dwSpan);
	SendMessage(hWindow, nUp, NULL, MAKELPARAM(nX + nError, nY + nError));
	return 0;
}

DllExport UINT CALLBACK SavePic(LPCWSTR lpFileName)
{
	SavePic(GetPic(), lpFileName);
	return 0;
}

DllExport UINT CALLBACK SavePic(LPCWSTR lpWindowName, LPCWSTR lpFileName)
{
	SavePic(GetPic(lpWindowName), lpFileName);
	return 0;
}

DllExport UINT CALLBACK SavePic(HWND hWindow, LPCWSTR lpFileName)
{
	SavePic(GetPic(hWindow), lpFileName);
	return 0;
}

DllExport UINT CALLBACK SavePic(HBITMAP hBitmap, LPCWSTR lpFileName)
{
	HDC hDC; //设备描述表
	INT iBits;//当前显示分辨率下每个像素所占字节数
	WORD wBitCount;//位图中每个像素所占字节数
	DWORD dwPaletteSize = 0;//定义调色板大小	
	DWORD dwBmBitsSize;//位图中像素字节大小	
	DWORD dwDIBSize;// 位图文件大小	
	DWORD dwWritten;//写入文件字节数	
	BITMAP Bitmap;//位图结构	
	BITMAPFILEHEADER bmfHdr;   //位图属性结构	
	BITMAPINFOHEADER bi;       //位图文件头结构	
	LPBITMAPINFOHEADER lpbi;     //位图信息头结构     指向位图信息头结构	
	HANDLE fh;//定义文件句柄	
	HANDLE hDib;//分配内存句柄	
	HANDLE hPal;//分配内存句柄	
	HANDLE hOldPal = NULL;//调色板句柄 

						  //计算位图文件每个像素所占字节数  		
	hDC = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);

	if (iBits <= 1)
	{
		wBitCount = 1;
	}
	else if (iBits <= 4)
	{
		wBitCount = 4;
	}
	else if (iBits <= 8)
	{
		wBitCount = 8;
	}
	else if (iBits <= 24)
	{
		wBitCount = 24;
	}
	else if (iBits <= 32)
	{
		wBitCount = 24;
	}

	//计算调色板大小  
	if (wBitCount <= 8)
	{
		dwPaletteSize = (1 << wBitCount) * sizeof(RGBQUAD);
	}

	//设置位图信息头结构  
	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;
	dwBmBitsSize = ((Bitmap.bmWidth *wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;

	//为位图内容分配内存  
	hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	if (lpbi == NULL)
	{
		return 1;
	}

	*lpbi = bi;

	// 处理调色板
	hPal = GetStockObject(DEFAULT_PALETTE);

	if (hPal)
	{
		hDC = GetDC(NULL);
		hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE);
		RealizePalette(hDC);
	}

	// 获取该调色板下新的像素值
	GetDIBits
	(
		hDC,
		hBitmap,
		0,
		(UINT)Bitmap.bmHeight,
		(LPSTR)lpbi + sizeof(BITMAPINFOHEADER) + dwPaletteSize,
		(LPBITMAPINFO)lpbi,
		DIB_RGB_COLORS
	);

	//恢复调色板
	if (hOldPal)
	{
		SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
		RealizePalette(hDC);
		ReleaseDC(NULL, hDC);
	}

	//创建位图文件
	fh = CreateFile
	(
		lpFileName,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL
	);

	if (fh == INVALID_HANDLE_VALUE)
	{
		return 1;
	}


	// 设置位图文件头
	bmfHdr.bfType = 0x4D42;  // "BM"  
	dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;

	// 写入位图文件头  
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);

	// 写入位图文件其余内容
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);

	//清除
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);

	return 0;
}

//Private API

inline INT Private_GetnError(UINT nErrorLimit)
{
	srand(time(NULL));
	return rand() % (nErrorLimit * 2 + 1) - nErrorLimit;
}

inline RGB Private_GetRGB(BYTE *pbyPic, BYTE byPic, LONG lWidthBytes, LONG lX, LONG lY)
{
	RGB rgbPic;
	rgbPic.nR = pbyPic[lX * byPic + lY * lWidthBytes + 2];
	rgbPic.nG = pbyPic[lX * byPic + lY * lWidthBytes + 1];
	rgbPic.nB = pbyPic[lX * byPic + lY * lWidthBytes + 0];
	rgbPic.bValid = TRUE;

	return rgbPic;
}

inline UINT Private_GetnDownUp(UINT &nDown, UINT &nUp, UINT nL0M1R2Button, BOOL bDoubleClick)
{
	if (nL0M1R2Button == 0)
	{
		if (bDoubleClick == TRUE)
		{
			nDown = WM_LBUTTONDBLCLK;
		}
		else if (bDoubleClick == FALSE)
		{
			nDown = WM_LBUTTONDOWN;
		}
		nUp = WM_LBUTTONUP;
	}
	else if (nL0M1R2Button == 1)
	{
		if (bDoubleClick == TRUE)
		{
			nDown = WM_MBUTTONDBLCLK;
		}
		else if (bDoubleClick == FALSE)
		{
			nDown = WM_MBUTTONDOWN;
		}
		nUp = WM_MBUTTONUP;
	}
	else if (nL0M1R2Button == 2)
	{
		if (bDoubleClick == TRUE)
		{
			nDown = WM_RBUTTONDBLCLK;
		}
		else if (bDoubleClick == FALSE)
		{
			nDown = WM_RBUTTONDOWN;
		}
		nUp = WM_RBUTTONUP;
	}

	return 0;
}