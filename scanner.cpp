#include "scanner.h"
 
#define TOKEN_LEN 100				// 记号最大长度
unsigned int LineNo;				// 跟踪源文件行号
static FILE *InFile;				
static char TokenBuffer[TOKEN_LEN];		// 记号字符缓冲

// -----------初始化词法分析器
extern bool InitScanner(const char *FileName) 
{
	InFile = fopen(FileName, "r");
	if (InFile == NULL)		return false;
	else {
		LineNo = 1;
		return true;
	}
}

// -----------关闭词法分析器
extern void CloseScanner() 
{
	if (InFile != NULL)		fclose(InFile);
}

// -----------从输入源文件中读取一个字符
static char GetChar() 
{
	char _char = fgetc(InFile);
	return _char;
}

// -----------将读取的字符退回到文件读取流中
static void BackChar(char _char) 
{
	if (_char != EOF)	ungetc(_char, InFile);
}

// -----------将一个字符添加进字符缓冲区,等待读取完成判断字符缓冲区中字符串是否是记号
static void AddCharTokenString(char _char)
{
	int TokenLength = strlen(TokenBuffer);
	if (TokenLength+1 >= sizeof(TokenBuffer))	return;
	TokenBuffer[TokenLength] = _char;
	TokenBuffer[TokenLength+1] = '\0'; 
}

// -----------判断完字符串，清空缓冲区
static void EmptyTokenString()
{
	memset(TokenBuffer, 0, TOKEN_LEN);
}

// -----------判断缓冲区中字符串是否是记号
static Token JudgeKeyToken(const char *IDString)
{
	for (int i = 0; i < sizeof(TokenTab)/sizeof(TokenTab[0]); ++i)
	{
		if (strcmp(TokenTab[i].lexeme, IDString) == 0)	return TokenTab[i];
	}
	Token errorToken;
	memset(&errorToken, 0, sizeof(Token));
	errorToken.type = ERRTOKEN;
	return errorToken;
}

// -----------通过DFA编写的字符记号读取，调用一次读取一个记号
extern Token GetToken() 
{
	char _char;
	Token token;

	memset(&token, 0, sizeof(Token));
	EmptyTokenString();
	token.lexeme = TokenBuffer;
	while(true) 			// 过滤掉上一次获取的记号到这一次要读取的记号之间的空格、Tab、换行等
	{
		_char = GetChar();
		if (_char == EOF)
		{
			token.type = NONTOKEN;
			return token;
		}
		if (_char == '\n')		LineNo++;
		if (!isspace(_char))	break;
	}
	AddCharTokenString(_char);
	if (isalpha(_char))		// 字符记号
	{
		while(true) 
		{
			_char = GetChar();
			if (isalnum(_char))		AddCharTokenString(_char);
			else					break;
		}
		BackChar(_char);
		token = JudgeKeyToken(TokenBuffer);
		token.lexeme = TokenBuffer;		// 防止没有识别记号时输出为null
		return token;
	}
	else if (isdigit(_char))		// 数字常数记号
	{
		while(true)
		{
			_char = GetChar();
			if (isdigit(_char))		AddCharTokenString(_char);
			else					break;
		}
		if (_char == '.')
		{
			AddCharTokenString(_char);
			while(true)
			{
				_char = GetChar();
				if(isdigit(_char))	AddCharTokenString(_char);
				else				break;
			}
		}
		BackChar(_char);
		token.type = CONST_ID;
		token.value = atof(TokenBuffer);
		return token;
	}
	else {				// 操作符号
		switch(_char)
		{
			case ';' : token.type = SEMICO		;break;
			case '(' : token.type = L_BRACKET	;break;
			case ')' : token.type = R_BRACKET	;break;
			case ',' : token.type = COMMA		;break;
			case '+' : token.type = PLUS		;break;
			case '-' : 
				_char = GetChar();
				if(_char == '-')
				{
					while(_char != '\n' && _char != EOF)	_char = GetChar();
					BackChar(_char);
					return GetToken();
				}
				else
				{
					BackChar(_char);
					token.type = MINUS;
					break;
				}
			case '/' :
				_char = GetChar();
				if (_char == '/')
				{
					while(_char != '\n' && _char != EOF)	_char = GetChar();
					BackChar(_char);
					return GetToken();
				}
				else
				{
					BackChar(_char);
					token.type = DIV;
					break;
				}
			case '*' :
				_char = GetChar();
				if (_char == '*')
				{
					token.type = POWER;
					break;
				}
				else
				{
					BackChar(_char);
					token.type = MUL;
					break;
				}
			default : token.type = ERRTOKEN 	;break;
		}
	}
	return token;
}
