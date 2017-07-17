#include "termix.h"

int main(int argc, char * argv[])
{
	if (tx_init(argc, argv) != E_NONE)
		return -1;

	if (tx_run() != E_NONE)
		return -1;

	if (tx_cleanup() != E_NONE)
		return -1;

	return 0;
}
