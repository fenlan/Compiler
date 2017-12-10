#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"

typedef double (*FuncPtr)(double);
typedef struct ExprNode
{
	Token_Type node_type;
	union
	{
		struct {ExprNode *left, *right;} OperatorNode;
		struct {ExprNode *child; FuncPtr mathFuncPtr;} FuncNode;
		double ConstNode;
		double *ParmPtrNode;
	} Content;
}ExprNode;

extern void Parser(char *FileName);

#endif