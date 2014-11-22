/*
	CTurt's implementation of exceptions in C
	licensed under the MIT License (MIT)
	
	https://github.com/CTurt/Exception
*/

#include "exception.h"

struct jmp_table jmp_table = { NULL, 0 };

// Shouldn't ever need to be used since each pushed
// exception is automatically popped but it's here
// just in case
inline void free_exceptions(void) {
	free(jmp_table.buf);
	jmp_table.count = 0;
}
