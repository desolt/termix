#ifndef _TERMIX_H
#define _TERMIX_H

#include "err.h"

tx_err tx_init(int argc, char * argv[]);
tx_err tx_run(void);
tx_err tx_cleanup(void);

#endif
