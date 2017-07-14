#include "termix.h"

int main(int argc, char * argv[])
{
	if (tx_init(argc, argv) != 0)
	{
		return -1;
	}

	if (tx_run() != 0)
	{
		return -1;
	}

	if (tx_cleanup() != 0)
	{
		return -1;
	}

	return 0;
}
