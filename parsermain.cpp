#include "parser.h"
 
extern void Parser(char *FileName);

int main(int argc, char *argv[])
{
	Token token;
	if(argc < 2)
	{
		printf("please input Source File !\n");		return 0;
	}
	Parser(argv[1]);
	return 0;
}