#include "EEMemoryLeak.h"

#include <crtdbg.h>

int before_main()
{
	//_CrtSetBreakAlloc(336);
	return 0;
}
int after_main()
{
	_CrtDumpMemoryLeaks();
	return 0;
}
