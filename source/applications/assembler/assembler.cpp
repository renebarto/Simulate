#include "Stdio.h"
#include <iostream>
#include "Parser.h"
#include "Scanner.h"
#include <sys/timeb.h>
#include <wchar.h>

using namespace std;
using namespace Assembler;

int main (int argc, char *argv[])
{
	if (argc == 2)
    {
		wchar_t *fileName = coco_string_create(argv[1]);
		Scanner *scanner = new Scanner(fileName);
		Parser *parser = new Parser(scanner);
		parser->Parse();
		if (parser->errors->count == 0)
        {
			cout << "OK" << endl;
		}
        else
        {
			cout << "failure" << endl;
        }

		coco_string_delete(fileName);
		delete parser;
		delete scanner;
	} 
    else
		cout << "-- No source file specified" << endl;

	return 0;

}
