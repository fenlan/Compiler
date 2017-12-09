#include "semantics.h"

extern double
		Parameter,
		Origin_x, Origin_y,
		Scale_x, Scale_y,
		Rot_angle;

double GetExprValue(ExprNode *root);
void DrawPixel(unsigned long x, unsigned long y);
void DrawLoop(double start, double end, double step, ExprNode *HorPtr, ExprNode *VerPtr);
void DelExprTree(ExprNode *root);

static void Errmsg(char *string);
static void CalcCoord(ExprNode *Hor_Exp, ExprNode *Ver_Exp, double &Hor_x, double &Ver_y);

void Errmsg(char *string){ exit(1); }
static void CalcCoord(ExprNode *Hor_Exp, ExprNode *Ver_Exp, double &Hor_x, double &Ver_y)
{
	double HorCord, VerCord, Hor_tmp;
	HorCord = GetExprValue(Hor_Exp);
	VerCord = GetExprValue(Ver_Exp);
	HorCord *= Scale_x;
	VerCord *= Scale_y;
	Hor_tmp = HorCord * cos(Rot_angle) + VerCord * sin(Rot_angle);
	VerCord = VerCord * cos(Rot_angle) - HorCord * sin(Rot_angle);
	HorCord = Hor_tmp;
	HorCord += Origin_x;
	VerCord += Origin_y;
	Hor_x = HorCord;
	Ver_y = VerCord;
}

void DrawLoop(double start, double end, double step, ExprNode *HorPtr, ExprNode *VerPtr)
{
	extern double Parameter;
	double x, y;
	for(Parameter = start; Parameter <= end; Parameter += step)
	{
		CalcCoord(HorPtr, VerPtr, x, y);
		DrawPixel((unsigned long)x, (unsigned long)y);
	}
}

double GetExprValue(ExprNode *root)
{
	if(root == NULL) 		return 0.0;
	switch(root->node_type)
	{
		case PLUS:
			return GetExprValue(root->Content.OperatorNode.left) +
					GetExprValue(root->Content.OperatorNode.right);
		case MINUS:
			return GetExprValue(root->Content.OperatorNode.left) -
					GetExprValue(root->Content.OperatorNode.right);
		case MUL:
			return GetExprValue(root->Content.OperatorNode.left) *
					GetExprValue(root->Content.OperatorNode.right);
		case DIV:
			return GetExprValue(root->Content.OperatorNode.left) /
					GetExprValue(root->Content.OperatorNode.right);
		case POWER:
			return pow(GetExprValue(root->Content.OperatorNode.left),
					GetExprValue(root->Content.OperatorNode.right));
		case FUNC:
			return (*root->Content.FuncNode.mathFuncPtr)
					(GetExprValue(root->Content.FuncNode.child));
		case CONST_ID:
			return root->Content.ConstNode;
		case T:
			return *(root->Content.ParmPtrNode);
		default :
			return 0.0;
	}
}

void DelExprTree(ExprNode *root)
{
	if(root == NULL)	return;
	switch(root->node_type)
	{
		case PLUS:	case MINUS:		case MUL:	case DIV:
		case POWER:
			DelExprTree(root->Content.OperatorNode.left);
			DelExprTree(root->Content.OperatorNode.right);
			break;
		case FUNC:
			DelExprTree(root->Content.FuncNode.child);
			break;
		default :
			break;
	}
	delete(root);
}

#ifdef _BC_COMPILER
	int InGraphMode = 0;
	int InitGraph()
	{
		int gd = DETECT, gm;
		if(InGraphMode)		return 0;
		registerbgidriver(EGAVGA_driver);
		initgraph(&gd, &gm, "");
		setcolor(-1);
		InGraphMode = 1;
		return 1;
	}
	void CloseGraph()
	{
		if(!InGraphMode)	return;
		getch();
		closegraph();
		InGraphMode = 0;
	}
#endif

void DrawPixel(unsigned long x, unsigned long y)
{
	#ifdef _VC_COMPILER
		SetPixel(hDC, x, y, black);
	#endif

	#ifdef _BC_COMPILER
		putpixel(x, y, white);
	#endif
}