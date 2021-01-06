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
    init_boards(); /* initialize boards to 0 */
    init_board(START_BOARD); /* create starting board */
    join_graph(START_BOARD); /* create all states */
    compute_score(); /* initialize score of each node */
    for (int i = 1; i < argc; i++) /* print node at desired hash value from command line */
    {
        print_node( htable[atoi(argv[i])] );
    }
}