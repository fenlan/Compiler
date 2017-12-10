#ifdef _VC_COMPILER

#include "semantics.h"
#define MAX_CHARS 200

HDC hDC;
char FileName[MAX_CHARS];
static char Name[] = "Compiler";

static bool PrepareWindow(HINSTANCE, HINSTANCE, int);
static bool CheckFile(LPSTR);
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int APIENTRY WinMain(HINSTANCE hInstance, 
					HINSTANCE hPrevInstance, 
					LPSTR lpCmdLine,
					int nCmdShow)
{
	strcpy(FileName, lpCmdLine);
	if(PrepareWindow(hInstance, hPrevInstance, nCmdShow) != true)
	{
		MessageBox(NULL, "Window init failed !", "ERROR", MB_OK);
		return 1;
	}
	if(!CheckFile(lpCmdLine))		return 1;
	Parser(FileName);
	MSG Msg;
	while(GetMessage(&Msg, NULL, 0, 0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}

bool PrepareWindow(HINSTANCE hInst, HINSTANCE hPrevInstance, int nCmdShow)
{
	HWND hWnd;
	WNDCLASS w;

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
	if(hWnd == NULL)		return false;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	SetCursor(LoadCursor(hInst, IDC_ARROW));

	hDC = GetDC(hWnd);
	return true;
}

bool CheckFile(LPSTR lpszCmdParam)
{
	FILE *file = NULL;

	if(strlen(lpszCmdParam) == 0)
	{
		MessageBox(NULL, "Not Specified File !", "ERROR", MB_OK);
		return false;
	}
	if((file = fopen(lpszCmdParam, "r")) == NULL)
	{
		MessageBox(NULL, "Open File Failed !", "ERROR", MB_OK);
		MessageBox(NULL, lpszCmdParam, "FileName", MB_OK);
		return false;
	}
	else	fclose(file);
	return true;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch(Message)
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
		default :
			return DefWindowProc(hWnd, Message, wParam, lParam);
	}
}
#endif

#ifdef _BC_COMPILER
#include "semantics.h"
int main(int argc, char **argv)
{
	int Result;

	if(!InitGraph())		return -1;
	if(argc != 2)			return -1;
	Parser(argv[1]);
	CloseGraph();
	printf("Hello World\n");
	return 0;
}
#endif