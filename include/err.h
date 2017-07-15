#ifndef _ERR_H
#define _ERR_H

void tx_gl_print_errors(void);
int  tx_gl_geterr(void);
void tx_gl_error_callback(int error, const char * description);

#endif
