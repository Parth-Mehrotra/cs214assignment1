#ifndef MYFREE_H_
#define MYFREE_H_


#define free(x) myfree(x,__FILE__,__LINE__)

void myfree(void* ptr, char* errorLocation, int errorLine);

#endif
