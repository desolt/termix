#ifndef _ERR_H
#define _ERR_H

void tx_print_errors(void);
int tx_geterr(void);
void tx_error_callback(int error, const char * description);

#endif
