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

void init_boards(){
    for (int i = 0; i < HSIZE; i++) /* set all boards to uninitialized */
    {
        htable[i].init = 0;
    }
}

int depth( Board board ){
    int markers = 0; /* mumber of pieces on board */
    for (int i = 0; i < 9; i++) /* check all 9 positions for pieces and increment markers when a piece is found*/
    {
        if ( board[ pos2idx[i] ] == 'X' || board[ pos2idx[i] ] == 'O' )
        {
            markers++; 
        }
    }
    return markers; /* return the result */
}

char winner( Board board ){
    char type;
    int matches;
    for (int i = 0; i < 2; i++) /* check board for a winner for either X or O */
    {
        if ( i == 0 ) /* check for X winner */
        {
            type = 'X';
        }
        else if (i == 1) /* check for O winner */
        {
            type = 'O';
        }
        for (int j = 0; j < 8; j++) /* check for the 8 win patterns */
        {
            matches = 0;
            for (int k = 0; k < 3; k++) /* if 3 matches are found then return the winner */
            {
                if ( board[ pos2idx[ WINS[j][k] ] ] == type )
                {
                    matches++;
                }
                if ( matches == 3 )
                {
                    return type;
                }
            }
        } 
    }
    if ( depth(board) == 9 ) /* check if board is full and result is a tie */
    {
        return '-';
    }
    else /* the game is still in progress */
    {
        return '?';
    }
}

char turn( Board board ){
    int deep = depth(board); /* depth of the board */
    if ( winner(board) == '?' ) /* check if game is still in progress */
    {
        if ( depth(board) % 2 == 0 || deep == 0 ) /* check if turn is even numbered (X's turn) */
        {
            return 'X';
        }
        else /* if its not X's turn then it is O's turn */
        {
            return 'O';
        }
    }
    else /* game is done with either a win or tie */
    {
        return '-';
    }
}

void init_board( Board board ){
    int hash = board_hash( board ); /* find hash value of board */
    htable[ hash ].init = 1; /* initialize node at hash value in hashtable*/
    htable[ hash ].turn = turn( board ); /* set turn value of board at hash value in hashtable */
    htable[ hash ].depth = depth( board ); /* set depth value of board at hash value in hashtable */
    strcpy( htable[ hash ].board, board ); /* set board string of board at hash value in hashtable */
    htable[ hash ].winner = winner( board ); /* set board value of board at hash value in hashtable */
}

void join_graph( Board board ){ /* create all board states */
    int hash = board_hash( board ); /* find hash value of board */
    int newHash; /* hash value for newly created boards */
    char newBoard[BSIZE]; /* temporary board string */
    for (int i = 0; i < 9; i++) /* move through child nodes */
    {
        if ( board[ pos2idx[i] ] == 'X' || board[ pos2idx[i] ] == 'O' ) /* invalid move so set to -1 */
        {
           htable[hash].move[i] = -1;
        }
        else /* state does not exist so create a new one */
        {
            strcpy(newBoard, board); /* copy current board */
            newBoard[ pos2idx[i] ] = turn( board ); /* add value for current players turn */
            newHash = board_hash( newBoard ); /* find hash of new board */
            htable[hash].move[i] = newHash; /* set current boards move array index to new hash value */
            if(htable[newHash].init == 0) /* value uninitialized so create new entry */
            {
                init_board( newBoard ); /* create the new board entry */
                if ( winner( newBoard ) == '?' ) /* check if game is still in progress */
                {
                    join_graph( newBoard ); /* add newboard to state table */
                }
                else /* game has ended */ 
                {
                    for (int j = 0; j < 9; j++) /* game has ended so fill remaining move array elements with -1 to indicate invalid next state */
                    {
                        htable[newHash].move[j] = -1;
                    }  
                }
            }
        } 
    } 
}

void compute_score(){
    int check; /* temp check variable */
    for (int i = 9; i > -1; i--) /* check depth of initialized elements in decending order */
    {
        for (int j= 0; j< HSIZE; j++) /* check each element of hashtable */
        {
            if ( htable[j].init == 1 && htable[j].depth == i ) /* check if hashvalue is initialized and is the desired depth */
            {
                if(  htable[j].winner == 'X' ) /* Winner is X so set score to 1 */
                {
                    htable[j].score = 1;
                }
                else if(  htable[j].winner == 'O' ) /* Winner is O so set score to -1 */
                {
                    htable[j].score = -1;
                }
                else if(  htable[j].winner == '-' ) /* Result of game is a tie so set score to 0 */
                {
                    htable[j].score = 0;
                }
                else if( htable[j].turn == 'X' ) /* X's turn */
                {   
                    check = 1; 
                    for (int k = 0; k < 9; k++) /* increment though all elements move children */
                    {
                        if ( htable[j].move[k] != -1 ) /* make sure move index value is valid */
                        {
                            if ( check == 1 ) /* first valid hash value so set score to it for future comparison */
                            {
                                htable[j].score = htable[ htable[j].move[k] ].score;
                                check = 0; /* first valid hash value found so stop checking for it */
                            }
                            else if ( htable[ htable[j].move[k] ].score > htable[j].score ) /* check if new score is greater than the current one and if so set the new value */
                            {
                                htable[j].score = htable[ htable[j].move[k] ].score;
                            }
                        }
                    }
                }
                else if( htable[j].turn == 'O' ) /* O's turn */
                {
                    check = 1;
                    for (int k = 0; k < 9; k++) /* increment through all elements move children */
                    {
                        if ( htable[j].move[k] != -1 ) /* make sure move index value is valid */
                        {
                            if ( check == 1 ) /* first valid hash value so set score to it for future comparison */
                            {
                                htable[j].score = htable[ htable[j].move[k] ].score;
                                check = 0; /* first valid hash value found so stop checing for it */
                            }
                            else if ( htable[ htable[j].move[k] ].score < htable[j].score ) /* check if new score is less than the current one and if so set the new value */
                            {
                                htable[j].score = htable[ htable[j].move[k] ].score;
                            } 
                        }
                    }
                }
            }
        }
    }
}

int best_move( int board ){
    int i;
    int next = 0; /* value of next index for comparison */
    int check = 1; /* temporary check value */
    int move; /* value of move best move position */
    if ( htable[board].turn == 'X' ) /* X's turn */
    {
        check = 1;
        for ( i = 0; i < 9; i++) /* check score of each move */
        {
            if ( htable[board].move[i] != -1 ) /* check for valid hash value */
            {
                if (check == 1) /* first valid hash index value */
                {
                    next = htable[board].move[i]; /* next value for comparison */
                    move = i; /* set move position */
                }
                else if ( htable[ htable[board].move[i] ].score > htable[ next ].score )
                {
                    next = htable[board].move[i]; /* next value for comparison */
                    move = i; /* set move position */
                }
                check = 0; /* first value has been found */
            }
        }
        return move; /* return best move position */
    }
    else
    {
        check = 1;
        for ( i = 0; i < 9; i++) /* check score of each move */
        {
            if ( htable[board].move[i] != -1 ) /* first valid hash index value */
            {
                if ( check == 1 ) /* first valid hash index value */
                {
                    next = htable[board].move[i]; /* next value for comparison */
                    move = i; /* set move position */
                }
                else if ( htable[ htable[board].move[i] ].score < htable[ next ].score )
                {
                    next = htable[board].move[i]; /* next value for comparison */
                    move = i; /* set move position */
                }
                check = 0; /* first value has been found */
            }
        }
        return move; /* return best move position */
    }
}


