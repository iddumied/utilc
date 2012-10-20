#ifndef __UTILC_DEBUG_H__
#define __UTILC_DEBUG_H__

#include <stdio.h>

#define EXPLAIN_FUNC printf(":: "LIBRARYNAME": %s\n", __func__)
#define EXPLAIN_FUNC_WITH(infotype,info) printf(":: "LIBRARYNAME" : %s : "infotype"\n", __func__, info)

#endif
