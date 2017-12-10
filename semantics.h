#ifndef SEMANTICS_H
#define SEMANTICS_H

#include <windows.h>
#include <wingdi.h>
#include "parser.h"
#define red RGB(255, 0, 0)
#define black RGB(0, 0, 0)

extern HDC hDC;
extern void DrawPixel(unsigned long x, unsigned long y);
extern double GetExprValue(ExprNode *root);
extern void DrawLoop(double start, double end, double step, ExprNode *HorPtr, ExprNode *VerPtr);
extern void DelExprTree(ExprNode *root);

#endif