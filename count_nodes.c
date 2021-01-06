#ifndef __stdio_h_
    #include <stdio.h>
#endif

#ifndef ___stdlib_h_
    #include <stdlib.h>
#endif

#ifndef __string_h_
    #include <string.h>
#endif

#ifndef __ttt_h_
    #include "ttt.h"
#endif

int main(int argc, char const *argv[]){
    int nel = 0; /* variable for number of elements/board states */
    init_boards(); /* initialize boards to 0 */
    init_board(START_BOARD); /* create starting board */
    join_graph(START_BOARD); /* create all states */
    for (int i = 0; i < HSIZE; i++) /* count all initialized elements */
    {
        if (htable[i].init == 1) /* when value in hashtable is initialized then increment number of nodes */
        {
            nel++;
        }
    }
    printf("%d\n", nel); /* print number of elements */   
}