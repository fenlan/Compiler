#include "scanner.h"

int main(int argc, char *argv[])
{ 
	Token token;
	if(argc < 2)
	{
		printf("please input Source File !\n");		return 0;
	}
	if(!InitScanner(argv[1]))
	{
		printf("Open Source File Error!\n");
	}
	printf("Token_type    String    Const    FuncP \n");
	printf("_______________________________________________\n");
	while(true)
	{
		token = GetToken();
		if(token.type != NONTOKEN)
			printf("%4d, %12s, %12f, %12x\n", 
				token.type, token.lexeme, token.value, token.FuncPtr);
		else    break;
	};
	printf("_______________________________________________\n");
	CloseScanner();

	return 0;
}