
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h> // use for define variables and function on Linux [tcdrain, tcflush]
#include <unistd.h> //use dup function for duplicate file descriptor


#ifndef __STDC_VERSION__201710L
   #define __STDC_VERSION__20170L
#endif

#define ARRAY_SIZE(array) (sizeof(array)/sizeof(*(array)))
#define WIDTH 8
#define HEIGHT 8 

//Macro goto-loop
#define _GAME_STATE_LOOP goto label

//Strings compare macro
#define STR_COMP(pargs) strcmp pargs

#ifndef _MY_CONTAINER_MAP
#define _MY_CONTAINER_MAP
struct unordered_map;

typedef struct
{ 
    const char * _key;
    int32_t _value; 

    struct unordered_map *me;

}unordered_map;



void umap_ctor(unordered_map * _me, const char * key, int32_t value)
{
     _me->_key = key;
     _me->_value = value;
    
     _me->me  = calloc(1, sizeof * _me);
}

void umap_dtor(unordered_map * _me)
{
    if(_me != NULL)
        free(_me->me);
}
#endif //EOC_MY_CONTAINER_

typedef char* UTF_8;

/* ASCII table characters
 *
 * 0 - empty field
 *
 *🨸  - 1
 *♘  - 2
 *♗  - 3 
 *♖  - 4 
 *♕  - 5
 *♔  - 6
 *
 *♟ - 7
 *♞ - 8 
 *♝ - 9 
 *♜ - 10
 *♛ - 11 
 *♚ - 12
 *
 * ■ □
 */



//input variable duplicate description other file
static int stdin_copy;


//Chess fields
UTF_8 WhiteField = " ■";
UTF_8 BlackField = " □";

//Chess pieces
UTF_8 WhitePawn = " ♟"; 
UTF_8 BlackPawn = " 🨸";

UTF_8 WhiteHorse = "♞";
UTF_8 BlackHorse = "♘";

UTF_8 WhiteBishop = "♝";
UTF_8 BlackBishop = "♗";

UTF_8 WhiteRook = "♜";
UTF_8 BlackRook = "♖";

UTF_8 WhiteQueen = "♛";
UTF_8 BlackQueen = "♕";

UTF_8 WhiteKing = "♚";
UTF_8 BlackKing = "♔";

int32_t board_fields[WIDTH][HEIGHT] =  {-1, 0,-1, 0,-1, 0,-1, 0,
           	                         0,-1, 0,-1, 0,-1, 0,-1,
                    	                -1, 0,-1, 0,-1, 0,-1, 0,
                            	         0,-1, 0,-1, 0,-1, 0,-1,
                                        -1, 0,-1, 0,-1, 0,-1, 0,        //empty fields 8 x 8
                                         0,-1, 0,-1, 0,-1, 0,-1,
                                        -1, 0,-1, 0,-1, 0,-1, 0,
			                 0,-1, 0,-1, 0,-1, 0, -1
	                                                        };


int32_t board_num[WIDTH][HEIGHT] =  {-1, 0,-1, 0,-1, 0,-1, 0,
           	                      1, 1, 1, 1, 1, 1, 1, 1,
                    	             -1, 0,-1, 0,-1, 0,-1, 0,
                            	      0,-1, 0,-1, 0,-1, 0,-1,
                                     -1, 0,-1, 0,-1, 0,-1, 0,        //with figures 8 x 8
                                      0,-1, 0,-1, 0,-1, 0,-1,
                                      7, 7, 7, 7, 7, 7, 7, 7,
			              0,-1, 0,-1, 0,-1, 0, -1
    	                                                        };


const char* board_marks[WIDTH][HEIGHT] =
				     {"A1","B1","C1","D1","E1","F1","G1","H1",
           	                      "A2","B2","C2","D2","E2","F2","G2","H2",
                    	              "A3","B3","C3","D3","E3","F3","G3","H3",
                            	      "A4","B4","C4","D4","E4","F4","G4","H4",  
                                      "A5","B5","C5","D5","E5","F5","G5","H5",     // 8 x 8
                                      "A6","B6","C6","D6","E6","F6","G6","H6",
                                      "A7","B7","C7","D7","E7","F7","G7","H7",
			              "A8","B8","C8","D8","E8","F8","G8","H8"
	                                                   		     };

//declare functions
void drawMap();
//...



//swap func
void swap_nums(int *x1, int *x2)
{
    int tmp = *x1;
    *x1 = *x2;    
    *x2 = tmp;
}


//logic piece func
void piece_switch(unordered_map * map, char* mark, int j, int i)
{
	char buffr[3] = "";	
	int nRangePawnIter = 0;
	int bPawnHasStarted = 0;
	int nHowMuchFieldsBetween = 0;

	const char* to_where_field = NULL;
	const char* from_where_field = NULL;

	//duplicate file's description
	stdin_copy = dup(STDIN_FILENO);
	tcdrain(stdin_copy);
	tcflush(stdin_copy, TCIFLUSH);

	close(stdin_copy);

	drawMap();
	printf("What's field you move to?: ");
	gets(buffr, sizeof(buffr), stdin_copy);

		
	if(STR_COMP((board_marks[j][i], mark)) == 0 && board_num[j][i] != 0 && board_num[j][i] != -1)
	{
		from_where_field = board_marks[j][i];
	}


	switch(map[j * i]._value)
	{
		case 7: //Black Pawns	
			for(int it_backward = i - WIDTH; it_backward < i; i -= WIDTH)
			{	
				//These statements checking 'if fields are oposite' not 'across' for pawns
				if(STR_COMP((board_marks[j][it_backward], buffr)) == 0 && board_num[j][it_backward] == -1)
				{	
					board_num[j][it_backward] = board_num[j][i];

					//replace piece pawn with field from both arrays.
					board_num[j][i] = board_fields[j][i];
				}	

				if(STR_COMP((board_marks[j][it_backward], buffr)) == 0 && board_num[j][it_backward] == 0)
				{	
					board_num[j][it_backward] = board_num[j][i];

					//replace piece pawn with field from both arrays.
					board_num[j][i] = board_fields[j][i];
				}

			}

		break;

		
		case 1: //White Pawns
			for(int it_forward = i + WIDTH; i < it_forward; i += WIDTH)
			{
				if(STR_COMP((board_marks[j][it_forward], buffr)) == 0 && board_num[j][it_forward] == -1)
				{
					board_num[j][it_forward] = board_num[j][i];

					board_num[j][i] = board_fields[j][i];
				}	

				if(STR_COMP((board_marks[j][it_forward], buffr)) == 0 && board_num[j][it_forward] == 0)
				{	
					board_num[j][it_forward] = board_num[j][i];

					board_num[j][i] = board_fields[j][i];
				}	
			

			}

		break;
		

	}

}

int main(void)
{
   unordered_map umap_chess[WIDTH * HEIGHT];   

   int rc = 0;

   char buffr_mark[3] = "";

   stdin_copy = dup(STDIN_FILENO);

label:

    //drawing
    drawMap();

    //remove garbage from stdin - clean description other file
    tcdrain(stdin_copy);
    tcflush(stdin_copy, TCIFLUSH);
    close(stdin_copy);
   
   printf("\nChoose a piece by typing field's mark: ");
   gets(buffr_mark, sizeof(buffr_mark), stdin_copy); //for ints no files invalid in C99
    
   for(int j = 0; j < WIDTH; j++)
   {
	   for(int i = 0; i < HEIGHT; i++) 
	   {
		//initialize unordered_map with marks and nums   
         	umap_ctor(&umap_chess[j * i], board_marks[j][i] , board_num[j][i]);

		//compare user stdin with board_marks strings
   	       	  rc = STR_COMP((umap_chess[j * i]._key, buffr_mark));


		
		 if(!rc)
		 {	     
			    if(umap_chess[j * i]._value != 0 && umap_chess[j * i]._value != -1)
			    {

				printf("You choosed piece on %s\n", buffr_mark);
			
				//piece logical
				piece_switch(umap_chess, buffr_mark, j, i);
				_GAME_STATE_LOOP;
			    }

			    else
			    { 
				_GAME_STATE_LOOP;
			    }
		   }
		  else if(rc < -7)
		  {   
			_GAME_STATE_LOOP;
		  }
		 
   
	      //dest unordered_map [marks,nums]
	      umap_dtor(&umap_chess[j * i]); 

	   }
   }

   return 0;
}

void drawMap()
{
  
   for(char lettr = 'A'; lettr <= 'H'; lettr++)
   {
         printf(" %c",lettr);
   }

   printf("\n");
   int num_rows = 1;

    for(int32_t rows = 0; rows < WIDTH; rows++)
    {
         for(int32_t col = 0; col < HEIGHT; col++)
	 {
	      switch(board_num[rows][col])
	      {
		  case -1: //WHITE
		      printf("%s", WhiteField);
	          break;
		  
		  case 0: //BLACK
		      printf("%s", BlackField);
		  break;

		  case 1:
		      printf("%s", BlackPawn);
		  break;

		  case 2:
		      printf("%s", BlackHorse);
		  break;

		  case 3:
		      printf("%s", BlackBishop);
		  break;

		  case 4:
		      printf("%s", BlackRook);
		  break;

		  case 5:
		      printf("%s", BlackQueen);
		  break;

		  case 6:
		      printf("%s", BlackKing);
		  break;

		  case 7:
		      printf("%s", WhitePawn);
		  break;
		
		  case 8:
		      printf("%s", WhiteHorse);
		  break;

		  case 9:
		      printf("%s", WhiteBishop);
		  break;

		  case 10:
		      printf("%s", WhiteRook);
		  break;

		  case 11:
		      printf("%s", WhiteQueen);
		  break;

		  case 12:
		      printf("%s", WhiteKing);
		  break;

		  default:
		      printf("Not Found a piece!\n");
		  break;

	      }
		
	 }
	 printf(" %d",num_rows++);
	 printf("\n");
    }	    
}

