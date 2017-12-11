#include "parser.h"

#define PARSER_DEBUG

#ifndef PARSER_DEBUG
	#include "semantics.h"
#endif

#ifdef PARSER_DEBUG
	#define enter(x) printf("enter in  "); printf(x); printf("\n")
#else
	#define enter(x)
#endif

#ifdef PARSER_DEBUG
	#define back(x) printf("exit from  "); printf(x); printf("\n")
#else
	#define back(x)
#endif

#ifdef PARSER_DEBUG
	#define call_match(x) printf("matchtoken    "); printf(x); printf("\n");
#else
	#define call_match(x)
#endif

#ifdef PARSER_DEBUG
	#define Tree_trace(x)	PrintSyntaxTree(x, 1);
#else
	#define Tree_trace
#endif

#ifdef PARSER_DEBUG
	double	Parameter=0;
#else
	double	Parameter=0,
			Origin_x=0,Origin_y=0,
			Scale_x=1,	Scale_y=1,
			Rot_angle=0;
#endif

static Token token;

static void FetchToken();
static void MatchToken(Token_Type AToken);
static void SyntaxError(int case_of);
static void ErrMsg(unsigned LineNo, const char *descrip, const char *string);
static void PrintSyntaxTree(ExprNode *root, int indent);

static void Program();
static void Statement();
static void OriginStatement();
static void RotStatement();
static void ScaleStatement();
static void ForStatement();
static ExprNode *Expression();
static ExprNode *Term();
static ExprNode *Factor();
static ExprNode *Component();
static ExprNode *Atom();

extern void Parser(char *FileName);
static ExprNode *MakeExprNode(Token_Type node_type, ...);

static void FetchToken()
{
	token = GetToken();
	if(token.type == ERRTOKEN)		SyntaxError(1);
}

static void MatchToken(Token_Type The_Token)
{
	if(token.type != The_Token)		SyntaxError(2);
	FetchToken();
}

static void SyntaxError(int case_of)
{
	switch(case_of)
	{
		case 1:		ErrMsg(LineNo, " Error Token ", token.lexeme);break;
		case 2:		ErrMsg(LineNo, " Not the expected token ", token.lexeme);break;
	}
}

void ErrMsg(unsigned LineNo, const char *descrip, const char *string)
{
	#ifdef PARSER_DEBUG
		printf("Line No %5d:%s %s !\n", LineNo, descrip, string);
	#else
		char msg[256];
		memset(msg, 0, 256);
		sprintf(msg, "Line No %5d:%s %s !", LineNo, descrip, string);
	#endif

	#ifdef _VC_COMPILER
		MessageBox(NULL, msg, "error!", MB_OK);
	#endif

	#ifdef _BC_COMPILER
		printf("%s\n", msg);
	#endif

		CloseScanner();
		exit(0);
}

void PrintSyntaxTree(ExprNode *root, int indent)
{
	int temp;
	for(temp = 1; temp <= indent; temp++)	printf("\t");
	switch(root->node_type)
	{
		case PLUS:		printf("%s\n", "+");break;
		case MINUS:		printf("%s\n", "-");break;
		case MUL:		printf("%s\n", "*");break;
		case DIV:		printf("%s\n", "/");break;
		case POWER:		printf("%s\n", "**");break;
		case FUNC:		printf("%x\n", root->Content.FuncNode.mathFuncPtr);break;
		case CONST_ID:	printf("%f\n", root->Content.ConstNode);break;
		case T:			printf("%s\n", "T");break;
		default:		printf("Error Tree Node !\n");exit(0);
	}
	if(root->node_type ==CONST_ID || root->node_type == T)		return;
	if(root->node_type == FUNC)
		PrintSyntaxTree(root->Content.FuncNode.child, indent+1);
	else {
		PrintSyntaxTree(root->Content.OperatorNode.left, indent+1);
		PrintSyntaxTree(root->Content.OperatorNode.right, indent+1);
	}
}

void Parser(char *FileName)
{
	enter("Parser");
	if(!InitScanner(FileName))
	{
		printf("Open Source File Error !\n");
		return;
	}
	FetchToken();
	Program();
	CloseScanner();
	back("Parser");
	return;
}

static void Program()
{
	enter("Program");
	while(token.type != NONTOKEN)
	{
		Statement();
		MatchToken(SEMICO);
	}
	back("Program");
}

static void Statement()
{
	enter("Statement");
	switch(token.type)
	{
		case ORIGIN:	OriginStatement();break;
		case SCALE:		ScaleStatement();break;
		case ROT:		RotStatement();break;
		case FOR:		ForStatement();break;
		default :		SyntaxError(2);
	}
	back("Statement");
}

static void OriginStatement()
{
	ExprNode *tmp;
	enter("OriginStatement");
	MatchToken(ORIGIN);
	MatchToken(IS);
	MatchToken(L_BRACKET);
	tmp = Expression();
	#ifndef PARSER_DEBUG
		Origin_x = GetExprValue(tmp);
		DelExprTree(tmp);
	#endif
		MatchToken(COMMA);
		tmp = Expression();
	#ifndef PARSER_DEBUG
		Origin_y = GetExprValue(tmp);
		DelExprTree(tmp);
	#endif
		MatchToken(R_BRACKET);
		back("OriginStatement");
}

static void ScaleStatement()
{
	ExprNode *tmp;
	enter("ScaleStatement");
	MatchToken(SCALE);
	MatchToken(IS);
	MatchToken(L_BRACKET);
	tmp = Expression();
	#ifndef PARSER_DEBUG
		Scale_x = GetExprValue(tmp);
		DelExprTree(tmp);
	#endif
	MatchToken(COMMA);
	tmp = Expression();
	#ifndef PARSER_DEBUG
		Scale_y = GetExprValue(tmp);
		DelExprTree(tmp);
	#endif
	MatchToken(R_BRACKET);
	back("ScaleStatement");
}

static void RotStatement()
{
	ExprNode *tmp;
	enter("RotStatement");
	MatchToken(ROT);
	MatchToken(IS);
	tmp = Expression();
	#ifndef PARSER_DEBUG
		Rot_angle = GetExprValue(tmp);
		DelExprTree(tmp);
	#endif
	back("RotStatement");
}

static void ForStatement()
{
	#ifndef PARSER_DEBUG
		double start, end, step;
	#endif
	ExprNode *start_ptr, *end_ptr, *step_ptr, *x_ptr, *y_ptr;
	enter("ForStatement");
	MatchToken(FOR);	call_match("FOR");
	MatchToken(T);		call_match("T");
	MatchToken(FROM);	call_match("FROM");
	start_ptr = Expression();
	#ifndef PARSER_DEBUG
		start = GetExprValue(start_ptr);
		DelExprTree(start_ptr);
	#endif
	MatchToken(TO);		call_match("TO");
	end_ptr = Expression();
	#ifndef PARSER_DEBUG
		end = GetExprValue(end_ptr);
		DelExprTree(end_ptr);
	#endif
	MatchToken(STEP);	call_match("STEP");
	step_ptr = Expression();
	#ifndef PARSER_DEBUG
		step = GetExprValue(step_ptr);
		DelExprTree(step_ptr);
	#endif
	MatchToken(DRAW);	call_match("DRAW");
	MatchToken(L_BRACKET);	call_match("(");
	x_ptr = Expression();
	MatchToken(COMMA);	call_match(",");
	y_ptr = Expression();
	MatchToken(R_BRACKET);	call_match(")");
	#ifndef PARSER_DEBUG
		DrawLoop(start, end, step, x_ptr, y_ptr);
		DelExprTree(x_ptr);
		DelExprTree(y_ptr);
	#endif
	back("ForStatement");
}

static ExprNode *Expression()
{
	ExprNode *left, *right;
	Token_Type token_tmp;
	enter("Expression");
	left = Term();
	while(token.type == PLUS || token.type == MINUS)
	{
		token_tmp = token.type;
		MatchToken(token_tmp);
		right = Term();
		left = MakeExprNode(token_tmp, left, right);
	}
	Tree_trace(left);
	back("Expression");
	return left;
}

static ExprNode *Term()
{
	ExprNode *left, *right;
	Token_Type token_tmp;
	left = Factor();
	while(token.type == MUL || token.type == DIV)
	{
		token_tmp = token.type;
		MatchToken(token_tmp);
		right = Factor();
		left = MakeExprNode(token_tmp, left, right);
	}
	return left;
}

static ExprNode *Factor()
{
	ExprNode *left, *right;
	if(token.type == PLUS)
	{
		MatchToken(PLUS);
		right=Factor();
	}
	else if(token.type == MINUS)
	{
		MatchToken(MINUS);
		right = Factor();
		left = new ExprNode;
		left->node_type = CONST_ID;
		left->Content.ConstNode = 0.0;
		right = MakeExprNode(MINUS, left, right);
	}
	else right = Component();
}

static ExprNode *Component()
{
	ExprNode *left, *right;
	left = Atom();
	if(token.type == POWER)
	{
		MatchToken(POWER);
		right = Component();
		left = MakeExprNode(POWER, left, right);
	}
	return left;
}

static ExprNode *Atom()
{
	Token t = token;
	ExprNode *address, *tmp;
	switch(token.type)
	{
		case CONST_ID:
			MatchToken(CONST_ID);
			address = MakeExprNode(CONST_ID, t.value);
			break;
		case T:
			MatchToken(T);
			address = MakeExprNode(T);
			break;
		case FUNC:
			MatchToken(FUNC);
			MatchToken(L_BRACKET);
			tmp = Expression();
			address = MakeExprNode(FUNC, t.FuncPtr, tmp);
			MatchToken(R_BRACKET);
			break;
		case L_BRACKET:
			MatchToken(L_BRACKET);
			address = Expression();
			MatchToken(R_BRACKET);
			break;
		default:
			SyntaxError(2);
	}
	return address;
}

static ExprNode *MakeExprNode(Token_Type Node_type, ...)
{
	ExprNode *ExprPtr = new ExprNode;
	ExprPtr->node_type = Node_type;
	va_list ArgPtr;
	va_start(ArgPtr, Node_type);
	switch(Node_type)
	{
		case CONST_ID:
			ExprPtr->Content.ConstNode = (double)va_arg(ArgPtr, double);
			break;
		case T:
			ExprPtr->Content.ParmPtrNode = &Parameter;
			break;
		case FUNC:
			ExprPtr->Content.FuncNode.mathFuncPtr 
				= (FuncPtr)va_arg(ArgPtr, FuncPtr);
			ExprPtr->Content.FuncNode.child
				= (ExprNode *)va_arg(ArgPtr, ExprNode *);
			break;
		default:
			ExprPtr->Content.OperatorNode.left
				= (ExprNode *)va_arg(ArgPtr, ExprNode *);
			ExprPtr->Content.OperatorNode.right
				= (ExprNode *)va_arg(ArgPtr, ExprNode *);
			break;
	}
	va_end(ArgPtr);

	return ExprPtr;
}