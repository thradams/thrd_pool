/*
  cl build.c && build

  gcc build.c -o build && ./build

*/

#include <stdlib.h>

#ifdef CAKE
#define MAIN_FILE " mainC89.c "
#else
#define MAIN_FILE " main.c "
#endif

#define SRC \
   " tinycthread.c " \
   " task_queue.c " \
   " thrd_pool.c "\
   MAIN_FILE

int main()
{
  #ifdef CAKE
    system("cake mainC2Y.c -o mainC89.c");
#endif


#ifdef _MSC_VER    
    system("cl /std:clatest  /utf-8 -o main " SRC);
#endif
#ifdef __linux__    
    system("gcc -g -o main -std=c2x" SRC);
#endif
}