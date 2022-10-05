/*!\brief main entry point for igcryptor utility
* \file main.cpp
* \date October 03, 2022
* \author Sergey Slyutov
*/

#include "igcryptor.h"

int main (int argc, char * argv[])
{
	CCryptoConfig cryptorcfg(argc, argv);

	if( !cryptorcfg.validate() )
		return 0;

	cryptorcfg.read();

	cryptorcfg.run();

	cryptorcfg.write();

	return 0;
};