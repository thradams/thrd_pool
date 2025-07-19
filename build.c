/*
  cl build.c && build
*/

#include <stdlib.h>

#define SRC \
   " tinycthread.c " \
   " task_queue.c " \
   " thrd_pool.c "\
   " main.c "

int main()
{
#ifdef _MSC_VER    
    system("cl /std:clatest  /utf-8 -o main " SRC);
#endif
}