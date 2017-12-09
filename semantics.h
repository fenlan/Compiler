#ifndef SEMANTICS_H
#define SEMANTICS_H

#ifdef _VC_COMPILER
	#include <windows.h>
	#include <wingdi.h>
	extern HDC hDC;
#endif
#ifdef _BC_COMPILER
	#include <graphics.h>
	#include <conio.h>
#endif

#include "parser.h"
#ifdef _VC_COMPILER
	#define red RGB(255, 0, 0)
	#define black RGB(0, 0, 0)
#endif

#ifdef _BC_COMPILER
	#define white 255
#endif

extern void DrawPixel(unsigned long x, unsigned long y);
extern double GetExprValue(ExprNode *root);
extern void DrawLoop(double start, double end, double step, ExprNode *HorPtr, ExprNode *VerPtr);
extern void DelExprTree(ExprNode *root);

#ifdef _BC_COMPILER
	extern int InGraphMode;
	extern int InitGraph();
	extern void CloseGraph();
#endif

#endif