/*
  cl build.c && build

  gcc build.c -o build && ./build

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
#ifdef __linux__    
    system("gcc -o main -std=c2x" SRC);
#endif
}