#include "semantics.h"
#define MAX_CHARS 200

HDC hDC;
char FileName[MAX_CHARS];
static TCHAR* Name = TEXT("Compiler");

static bool PrepareWindow(HINSTANCE, HINSTANCE, int);
static bool CheckFile(LPSTR);
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	strcpy(FileName, lpCmdLine);
	if (PrepareWindow(hInstance, hPrevInstance, nCmdShow) != true)
	{
		MessageBox(NULL, TEXT("Window init failed !"), TEXT("ERROR"), MB_OK);
		return 1;
	}
	if (!CheckFile(lpCmdLine))		return 1;
	Parser(FileName);
	MSG Msg;
	while (GetMessage(&Msg, NULL, 0, 0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return 1;
}

bool PrepareWindow(HINSTANCE hInst, HINSTANCE hPrevInstance, int nCmdShow)
{
	HWND hWnd;
	WNDCLASS W;

	memset(&W, 0, sizeof(WNDCLASS));
	W.style = CS_HREDRAW | CS_VREDRAW;
	W.lpfnWndProc = WndProc;
	W.hInstance = hInst;
	W.hCursor = LoadCursor(NULL, IDC_ARROW);
	W.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	W.lpszClassName = Name;
	
	RegisterClass(&W);

	hWnd = CreateWindow(Name, Name,
		WS_OVERLAPPEDWINDOW, 10, 10, 740, 490, NULL, NULL, hInst, NULL);
	if (hWnd == NULL)		return false;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	SetCursor(LoadCursor(hInst, IDC_ARROW));

	hDC = GetDC(hWnd);
	return true;
}

bool CheckFile(LPSTR lpszCmdParam)
{
	FILE *file = NULL;

	if (strlen(lpszCmdParam) == 0)
	{
		MessageBox(NULL, TEXT("Not Specified File !"), TEXT("ERROR"), MB_OK);
		return false;
	}
	if ((file = fopen(lpszCmdParam, "r")) == NULL)
	{
		MessageBox(NULL, TEXT("Open File Failed !"), TEXT("ERROR"), MB_OK);
		// MessageBox(NULL, lpszCmdParam, TEXT("FileName"), MB_OK);
		return false;
	}
	else	fclose(file);
	return true;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_DESTROY:
		ReleaseDC(hWnd, hDC);
		PostQuitMessage(0);
		return 0;
		break;
	case WM_PAINT:
		PAINTSTRUCT pt;
		BeginPaint(hWnd, &pt);
		Parser(FileName);
		EndPaint(hWnd, &pt);
	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
	}
}