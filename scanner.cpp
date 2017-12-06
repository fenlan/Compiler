#include "scanner.h"

#define TOKEN_LEN 100
unsigned int LineNo;
static FILE *InFile;
static char TokenBuffer[TOKEN_LEN];

extern bool InitScanner(const char *FileName) 
{
	InFile = fopen(FileName, "r");
	if (InFile == NULL)		return false;
	else {
		LineNo = 1;
		return true;
	}
}

extern void CloseScanner() 
{
	if (InFile != NULL)		fclose(InFile);
}

static char GetChar() 
{
	char _char = fgetc(InFile);
	return _char;
}

static void BackChar(char _char) 
{
	if (_char != EOF)	ungetc(_char, InFile);
}

static void AddCharTokenString(char _char)
{
	int TokenLength = strlen(TokenBuffer);
	if (TokenLength+1 >= sizeof(TokenBuffer))	return;
	TokenBuffer[TokenLength] = _char;
	TokenBuffer[TokenLength+1] = '\0'; 
}

static void EmptyTokenString()
{
	memset(TokenBuffer, 0, TOKEN_LEN);
}

static Token JudgeKeyToken(const char *IDString)
{
	for (int i = 0; i < sizeof(TokenTab)/sizeof(TokenTab[0]); ++i)
	{
		if (strcmp(TokenTab[i].lexeme, IDString) == 0)	return TokenTab[i];
		Token errorToken;
		memset(&errorToken, 0, sizeof(Token));
		errorToken.type = ERRTOKEN;
		return errorToken;
	}
}

extern Token GetToken() 
{
	char _char;
	Token token;

	memset(&token, 0, sizeof(Token));
	EmptyTokenString();
	token.lexeme = TokenBuffer;
	while(true) 
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
	if (isalpha(_char))
	{
		while(true) 
		{
			_char = GetChar();
			if (isalnum(_char))		AddCharTokenString(_char);
			else					break;
		}
		BackChar(_char);
		token = JudgeKeyToken(TokenBuffer);
		token.lexeme = TokenBuffer;
		return token;
	}
	else if (isdigit(_char))
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
	else {
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