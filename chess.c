
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

//Strings compare-macro
#define STR_COMP(pargs) strcmp pargs

//Pawns move statement-macro
#define M_PIECE_CHECK_MOVE(ITERATOR_ROWS, ADDITIONAL_ROWS, ITERATOR_COLS, ADDITIONAL_COLS, FIELD_NUM, BOOLEAN_IS_MOVE)\
	if(STR_COMP((board_marks[ITERATOR_ROWS + ADDITIONAL_ROWS][ITERATOR_COLS + ADDITIONAL_COLS], buffr)) == 0 && board_num[ITERATOR_ROWS + ADDITIONAL_ROWS][ITERATOR_COLS + ADDITIONAL_COLS] == FIELD_NUM)\
	{\
		board_num[ITERATOR_ROWS + ADDITIONAL_ROWS][ITERATOR_COLS + ADDITIONAL_COLS] = board_num[j][i];\
		board_num[j][i] = board_fields[j][i];\
		bIsMove = BOOLEAN_IS_MOVE;\
	}
/*M_PAWN_CHECK_MOVE*/

//Pawn hit statement-macro
#define M_BLACK_PAWN_CHECK_HIT(ITERATOR, ADDITIONAL_COLS, BOOLEAN_IS_MOVE)\
		if(STR_COMP((board_marks[j][ITERATOR + ADDITIONAL_COLS], buffr)) == 0 && board_num[j][ITERATOR + ADDITIONAL_COLS] != -1 && board_num[j][ITERATOR + ADDITIONAL_COLS] != 0 && board_num[j][ITERATOR + ADDITIONAL_COLS] != 1 && board_num[j][ITERATOR + ADDITIONAL_COLS] != 4 && board_num[j][ITERATOR + ADDITIONAL_COLS] != 5)\
				{ board_num[j][ITERATOR + ADDITIONAL_COLS] = board_num[j][i];\
				board_num[j][i] = board_fields[j][i];\
				bIsMove = BOOLEAN_IS_MOVE;\
				}

#define M_WHITE_PAWN_CHECK_HIT(ITERATOR, ADDITIONAL_COLS, BOOLEAN_IS_MOVE) \
		if(STR_COMP((board_marks[j][ITERATOR + ADDITIONAL_COLS], buffr)) == 0 && board_num[j][ITERATOR + ADDITIONAL_COLS] != -1 && board_num[j][ITERATOR + ADDITIONAL_COLS] != 0 && board_num[j][ITERATOR + ADDITIONAL_COLS] != 7 && board_num[j][ITERATOR + ADDITIONAL_COLS] != 6 && board_num[j][ITERATOR + ADDITIONAL_COLS] != 8) \
				{ board_num[j][ITERATOR + ADDITIONAL_COLS] = board_num[j][i]; \
				board_num[j][i] = board_fields[j][i]; \
				bIsMove = BOOLEAN_IS_MOVE; \
				}\
/*M_BLACK_PAWN_CHECK_HIT*/


//input variable duplicate description other file
static int stdin_copy = 0;

static int bIsMove = 0;
static int nHowMuchIsNotField = 0;

#ifndef _MY_CONTAINER_MAP
#define _MY_CONTAINER_MAP
struct unordered_map;

typedef struct
{ 
    const char * _key;
    int32_t _value; 
    unsigned int _id;

    struct unordered_map *me;

}unordered_map;



void umap_ctor(unordered_map * _me, const char * key, int32_t value)
{
     _me->_key = key;
     _me->_value = value;

     _me->me  = calloc(1, sizeof(*_me));
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




//Chess fields
UTF_8 WhiteField = " ■";
UTF_8 BlackField = " □";

//Chess pieces
UTF_8 WhitePawn = " ♟"; 
UTF_8 BlackPawn = " 🨸";

UTF_8 WhiteKnight = "♞";
UTF_8 BlackKnight = "♘";

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


int32_t board_num[WIDTH][HEIGHT] = 	{-1, 0,-1, 0,-1, 0,-1, 0,
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

#ifndef _PIECE_PROP
#define _PIECE_PROP

//Chess Piece Proporties
typedef struct
{
	int _x, _y;
	int _nFieldNumber;
	int _nHowMuchFieldMovedX;
	int _nHowMuchFieldMovedY;
}sPiece;

void piece_ctor(sPiece * const me, int x, int y, int nFieldNumber, int nHowMuchFieldMovedX, int nHowMuchFieldMovedY)
{
	me->_x = x;
	me->_y = y;

	me->_nFieldNumber = nFieldNumber;
	board_num[x][y] = me->_nFieldNumber;

	me->_nHowMuchFieldMovedX = nHowMuchFieldMovedX;
	me->_nHowMuchFieldMovedY = nHowMuchFieldMovedY;
	
}

#endif

//simple movement inline pattern function for knights
static void inline f_knight_move(int j, int i, int y, int x, int who_move, const char* buffr, sPiece * piece);

//black knight move mechanic
void black_knight_movement(int j, int i, const char* buffr, sPiece * piece);

//white knight move mechanic
void white_knight_movement(int j, int i, const char* buffr, sPiece * piece);

//general
void chess_piece(unordered_map * map, int j, int i, int range, const char* f_buffr, sPiece * piece);

//draw board
void drawMap();
//...


//update pawns
void pawn_switch(unordered_map * map, char* mark, int j, int i, const char* buffr)
{

	switch(map[j * i]._value)
	{

		case 1: //Black Pawns

		    if(bIsMove)
		    {		
			    	//Complexity O(n^1)+
				for(int it_forward = i; it_forward < 16; it_forward += WIDTH)
				{
				
					//A2 - H2 Fields only can move 2 fields	
					if(STR_COMP((mark, board_marks[1][it_forward - WIDTH])) == 0)
					{
						//2 fields forward ++ if didn't start yet	
						M_PIECE_CHECK_MOVE(j, 0, it_forward, 8, -1, 0); 
						M_PIECE_CHECK_MOVE(j, 0, it_forward, 8, 0, 0); 
					}	


					//Pawn move per 1 field
					M_PIECE_CHECK_MOVE(j, 0, it_forward, 0, -1, 0);
					M_PIECE_CHECK_MOVE(j, 0, it_forward, 0, 0, 0);

					//check hitting; link <1,6> teammates number 'off hitting friendly'
					M_BLACK_PAWN_CHECK_HIT(it_forward, -1, 0)
					M_BLACK_PAWN_CHECK_HIT(it_forward, 1, 0)
	
				}

		    }

		break;


		case 7: //White Pawns

		    if(!bIsMove)
		    {
			
				for(int it_backward = i - 16; it_backward < i; i -= WIDTH)
				{

					//A2 - H2 Fields only can move 2 fields	
					if(STR_COMP((mark, board_marks[6][i])) == 0)
					{
						//2 fields forward ++ if didn't start yet
						M_PIECE_CHECK_MOVE(j, 0, it_backward, 0, -1, 1);
						M_PIECE_CHECK_MOVE(j, 0, it_backward, 0, 0, 1);
					}	


					//Per 1 field - These statements checking 'if fields are oposite' not 'across' for pawns
					M_PIECE_CHECK_MOVE(j, 0, it_backward, 8, -1, 1);
					M_PIECE_CHECK_MOVE(j, 0, it_backward, 8, 0, 1);

					//check hitting; <7,12> 'no teammates hitting' included.	
					M_WHITE_PAWN_CHECK_HIT(i, -9, 1);
					M_WHITE_PAWN_CHECK_HIT(i, -7, 1);

				}

	            }
		
		    
		break;

	}

}


//inline update functions
static void inline moveStraightUp(int j, int i, int howOften, int who_move, const char* buffr, sPiece * piece)
{
		//reset variables
		nHowMuchIsNotField = 0;

		for(int it_backward = j; (j - piece->_nHowMuchFieldMovedY) <= it_backward; it_backward -= howOften)
		{
			
			//if we were out of bounds.
			if(board_num[it_backward][i] == 7)
			{
				nHowMuchIsNotField++;
				//printf("\nile nie jest polami: %d\n", nHowMuchIsNotField);
			}

			if(STR_COMP((board_marks[it_backward][i], buffr)) == 0)
			{

		            if(board_num[it_backward][i] == -1 || board_num[it_backward][i] == 0)
			    {
					if(nHowMuchIsNotField > 0)
					{
						break;
					}

                            }


			    //hitting
				if(nHowMuchIsNotField > 1)
				{
					break;
				}


				//swapping
				board_num[it_backward][i] = board_num[j][i];
				board_num[j][i] = board_fields[j][i];

				//reset variables
				nHowMuchIsNotField = 0;

				bIsMove = who_move;

				//count 'y' fields
				//need for calculate how much 'we can move back'
				piece->_nHowMuchFieldMovedY = it_backward;
			}

		}

}

static void inline moveStraightDown(int j, int i, int nRange, int howOften, int who_move, const char* buffr, sPiece * piece)
{
	   //reset variables
	   nHowMuchIsNotField = 0;

		for(int it_forward = j; it_forward < nRange; it_forward += howOften)
		{
		
			//if we were out of bounds.
			if(board_num[it_forward][i] == 7)
			{
				nHowMuchIsNotField++;
				//printf("\nile nie jest polami: %d\n", nHowMuchIsNotField);
			}

			//only down	
			if(STR_COMP((board_marks[it_forward][i], buffr)) == 0)
			{

			    if(board_num[it_forward][i] == -1 || board_num[it_forward][i] == 0)
			      {
					if(nHowMuchIsNotField > 0)
					{
						break;
					}
			      }
			
				
			    //hitting
				if(nHowMuchIsNotField > 1)
				{
					break;
				}


				board_num[it_forward][i] = board_num[j][i];
				board_num[j][i] = board_fields[j][i];

				//reset variables
				nHowMuchIsNotField = 0;

				bIsMove = who_move;
	
				piece->_nHowMuchFieldMovedY = it_forward;
			}

		}


}

static void inline moveStraightRight(int j, int i, int nRange, int howOften, int who_move, const char* buffr, sPiece * piece)
{

	   //reset variables
	   nHowMuchIsNotField = 0;

		for(int it_forward = i; it_forward < nRange; it_forward += howOften)
		{

			//if we were out of bounds.
			if(board_num[j][it_forward] == 7)
			{
				nHowMuchIsNotField++;
				//printf("\nile nie jest polami: %d\n", nHowMuchIsNotField);
			}
			

			//only right				
			if(STR_COMP((board_marks[j][it_forward], buffr)) == 0)
			{

			    if(board_num[j][it_forward] == -1 || board_num[j][it_forward] == 0)
			    {
					if(nHowMuchIsNotField > 0)
					{
						break;
					}

			    }


			    	//hitting 
				 if(nHowMuchIsNotField > 1)
				 {
					break;
				 }
			    
			    
				board_num[j][it_forward] = board_num[j][i];
				board_num[j][i] = board_fields[j][i];

				//reset variables
				nHowMuchIsNotField = 0;

				bIsMove = who_move;	

				piece->_nHowMuchFieldMovedX = it_forward;
			}

		}


}


static void inline moveStraightLeft(int j, int i, int howOften, int who_move, const char* buffr, sPiece * piece)
{

	   //reset variables
	   nHowMuchIsNotField = 0;

		for(int it_backward = i; (i - piece->_nHowMuchFieldMovedX) <= it_backward; it_backward -= howOften)
		{
		
			//if we were out of bounds.
			if(board_num[j][it_backward] == 7)
			{
				nHowMuchIsNotField++;
				//printf("\nile nie jest polami: %d\n", nHowMuchIsNotField);
			}
			

			if(STR_COMP((board_marks[j][it_backward], buffr)) == 0)
			{
			    
			   if(board_num[j][it_backward] == -1 || board_num[j][it_backward] == 0)
		           {
					if(nHowMuchIsNotField > 0)
					{
						break;
					}

                           }

			   //hitting
				if(nHowMuchIsNotField > 1)
				{
					break;
				}


				board_num[j][it_backward] = board_num[j][i];
				board_num[j][i] = board_fields[j][i];

				//reset variables
				nHowMuchIsNotField = 0;
					
				bIsMove = who_move;

				piece->_nHowMuchFieldMovedX = it_backward;

			}

		}

}

static void inline clearInput()
{
	
	//duplicate file's description
	stdin_copy = dup(STDIN_FILENO);
	tcdrain(stdin_copy);
	tcflush(stdin_copy, TCIFLUSH);

	close(stdin_copy);

}

void rook_black_update(unordered_map * map, int j, int i, int range, const char* buffr, sPiece * piece)
{
		//Rook movement and hitting rules	
		if(bIsMove)
		{	
			moveStraightDown(j, i, range, 1, 0, buffr, piece);	
			moveStraightRight(j, i, range, 1, 0, buffr, piece);
			

			//printf("\nLiczba pol X: %d\n", piece->_nHowMuchFieldMovedX);
			//printf("Liczba pol Y: %d\n", piece->_nHowMuchFieldMovedY);
		}

		if(bIsMove && piece->_nHowMuchFieldMovedY > 0)
		{
			//check how much free place left until end of array (memory)
			moveStraightUp(j, i, 1, 0, buffr, piece);
			
		}

		
		if(bIsMove && piece->_nHowMuchFieldMovedX > 0)
		{	
			//check how much free place left until end of array (memory)
			moveStraightLeft(j, i, 1, 0, buffr, piece);


		}

}


void rook_white_update(unordered_map * map, int j, int i, int range, const char* buffr, sPiece * piece)
{
		//Rook movement and hitting rules	
		if(!bIsMove)
		{	
			moveStraightDown(j, i, range, 1, 1, buffr, piece);	
			moveStraightRight(j, i, range, 1, 1, buffr, piece);
			

			//printf("\nLiczba pol X: %d\n", piece->_nHowMuchFieldMovedX);
			//printf("Liczba pol Y: %d\n", piece->_nHowMuchFieldMovedY);
		}

		if(!bIsMove && piece->_nHowMuchFieldMovedY > 0)
		{
			//check how much free place left until end of array (memory)
			moveStraightUp(j, i, 1, 1, buffr, piece);
			
		}

		
		if(!bIsMove && piece->_nHowMuchFieldMovedX > 0)
		{	
			//check how much free place left until end of array (memory)
			moveStraightLeft(j, i, 1, 1, buffr, piece);


		}

}



int main(void)
{
   unordered_map umap_chess[WIDTH * HEIGHT];  

   //allocate mem for struct black and white rooks.
   sPiece* _rook = calloc(4, sizeof(sPiece));
   sPiece* _knight = calloc(4, sizeof(sPiece));

   int rc = 0;
   char buffr_mark[3] = "";
   char buffr_field[3] = "";	

   //init pieces

   //Black Rooks         j  i  f  x  y 
   piece_ctor(&_rook[0], 0, 0, 4, 1, 0);
   piece_ctor(&_rook[1], 0, 7, 5, 7, 0);

   //White Rooks 
   piece_ctor(&_rook[2], 7, 0, 6, 0, 7);
   piece_ctor(&_rook[3], 7, 7, 8, 7, 7);

   //Black Knight
   piece_ctor(&_knight[0], 0, 1, 2, 1, 0);
   piece_ctor(&_knight[1], 0, 6, 3, 6, 0); 

   //White Knight
   piece_ctor(&_knight[2], 7, 1, 9, 1, 7);
   piece_ctor(&_knight[3], 7, 6, 10, 6, 7); 

label:
	
   //reset variables
   nHowMuchIsNotField = 0;

    //drawing
    drawMap();
	
    //remove garbage from stdin - clean description other file
    clearInput();

   printf("\nChoose a piece by typing field's mark: ");
   gets(buffr_mark, sizeof(buffr_mark), stdin_copy); //for ints no files invalid in C99
   
   //Complexity (n^2)+ 
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
		
				//field move process
				clearInput();
				drawMap();

				printf("What's field you move to?: ");
				gets(buffr_field, sizeof(buffr_field), stdin_copy);

				//pawns both update
				{
				 	pawn_switch(umap_chess, buffr_mark, j, i, buffr_field);
				}

				//BlackRook update
				{	
					chess_piece(umap_chess, j, i, 8, buffr_field, &_rook[0]);	
					chess_piece(umap_chess, j, i, 8, buffr_field, &_rook[1]);
				}
				
							
				//WhiteRook update	
				{	
					chess_piece(umap_chess, j, i, 8, buffr_field, &_rook[2]);
					chess_piece(umap_chess, j, i, 8, buffr_field, &_rook[3]);
				}
				
				//BlackKnight update
				{ 
					chess_piece(umap_chess, j, i, 0, buffr_field, &_knight[0]);
					chess_piece(umap_chess, j, i, 0, buffr_field, &_knight[1]);
				}

				//WhiteKnight update
				{
					chess_piece(umap_chess, j, i, 0, buffr_field, &_knight[2]);
					chess_piece(umap_chess, j, i, 0, buffr_field, &_knight[3]);
				}

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
	//free mem from pieces	
  	 free(_rook);	 
	 free(_knight);

   return EXIT_SUCCESS;
}

static void inline f_knight_move(int j, int i, int y, int x, int who_move, const char* buffr, sPiece * piece)
{	
	if(strcmp(board_marks[j + y][i + x], buffr) == 0)
	{
		board_num[j + y][i + x] = board_num[j][i];
		board_num[j][i] = board_fields[j][i];

		piece->_nHowMuchFieldMovedX = i + x;
		piece->_nHowMuchFieldMovedY = j + y;

		bIsMove = who_move;

	}
}


//chess piece update here
void chess_piece(unordered_map * map, int j, int i, int range, const char* f_buffr, sPiece * piece)
{
	switch(map[j * i]._value)
	{
		//black rooks update
		case 4:
			rook_black_update(map, j, i, range, f_buffr, piece);
		break;

		case 5:
			rook_black_update(map, j, i, range, f_buffr, piece);
		break;
		//////////////////////
		
		//white rooks update
		case 6:
			rook_white_update(map, j, i, range, f_buffr, piece);
	 	break;
		
		case 8: 
			rook_white_update(map, j, i, range, f_buffr, piece);
		break;
		/////////////////////
		
		//black knights update	
		case 2:
		  if(bIsMove)
		  {
			  for(int x = i; x < i + 17; x+=15)
			  {
					  //down-left
					  if(STR_COMP((board_marks[j][x], f_buffr)) == 0)
					  {
							  /* printf("\nIt is the field!\n"); */
							 board_num[j][x] = board_num[j][i];
							 board_num[j][i] = board_fields[j][i];

							 bIsMove = 0;

							 piece->_nHowMuchFieldMovedX = x;
					  }
	
					  //down-right
					  if(STR_COMP((board_marks[j][x + 2], f_buffr)) == 0)
					  {
							  /* printf("\nIt is the field!\n"); */
							 board_num[j][x + 2] = board_num[j][i];
							 board_num[j][i] = board_fields[j][i];

							 bIsMove = 0;
							 piece->_nHowMuchFieldMovedX = x;
					  }

					  //right-down 
					  if(STR_COMP((board_marks[j][x - 5], f_buffr)) == 0)
					  {
							  /* printf("\nIt is the field!\n"); */
							 board_num[j][x - 5] = board_num[j][i];
							 board_num[j][i] = board_fields[j][i];

							 bIsMove = 0;
							 piece->_nHowMuchFieldMovedX = x;
					  }

					  //left-down
					  if(STR_COMP((board_marks[j][x - 9], f_buffr)) == 0)
					  {
							 board_num[j][x - 9] = board_num[j][i];
							 board_num[j][i] = board_fields[j][i];

							 bIsMove = 0;
							 piece->_nHowMuchFieldMovedX = x;
					  }

			  }

			  if(piece->_nHowMuchFieldMovedX >= 15)
			   {
				  for(int x = i; i - 17 <= x; x-=17)
				  {
					  //top-right
					   if(STR_COMP((board_marks[j][x], f_buffr)) == 0)
					   {
								  board_num[j][x] = board_num[j][i];
								  board_num[j][i] = board_fields[j][i];
								  bIsMove = 0;
					   }

					   if(STR_COMP((board_marks[j][x + 2], f_buffr)) == 0)
					   {
								  board_num[j][x + 2] = board_num[j][i];
								  board_num[j][i] = board_fields[j][i];
								  bIsMove = 0;
					   }
				  }

			   }
	
		  }

		break;
		
		case 3:	

		break;
		/////////////////////

		//white knights update
		case 9:
			if(!bIsMove)
			{
				  //upside loop
					  for(int x = i; i - 17 <= x; x-=17)
					  {
						  //top-right
						   if(STR_COMP((board_marks[j][x], f_buffr)) == 0)
						   {
									  board_num[j][x] = board_num[j][i];
									  board_num[j][i] = board_fields[j][i];
									  bIsMove = 1;
						   }

						   if(STR_COMP((board_marks[j][x + 2], f_buffr)) == 0)
						   {
									  board_num[j][x + 2] = board_num[j][i];
									  board_num[j][i] = board_fields[j][i];
									  bIsMove = 1;
						   }
					  }


			}

		break;

		case 10:
				
		break;
		/////////////////////
	}
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

		  //Fields
		  case -1: //WHITE
		      printf("%s", WhiteField);
	          break;
		  
		  case 0: //BLACK
		      printf("%s", BlackField);
		  break;
		  ///////////////////////////////


		  //Pawns
		  case 7:
		      printf("%s", WhitePawn);
		  break;

		  case 1:
		      printf("%s", BlackPawn);
		  break;
		  ///////////////////////////////


		  //Knights
		  //Top-Left BlackKnight
		  case 2:
		      printf(" %s", BlackKnight);
		  break;

		  //Top-Right BlackKnight
		  case 3:
		      printf(" %s", BlackKnight);
		  break;
		  
		  //Bottom-Left WhiteKnight
		  case 9:
		      printf(" %s", WhiteKnight);
		  break;
		  
		  //Bottom-Right WhiteKnight
		  case 10:
		      printf(" %s", WhiteKnight);
		  break;
		  //////////////////////////////


		  //Rooks
		  //Top-Left BlackRook 
		  case 4:
		      printf(" %s", BlackRook);
		  break;
		
		  //Top-Right BlackRook
		  case 5:
		      printf(" %s", BlackRook);
		  break;

		  //Bottom-Left WhiteRook
		  case 6:
		      printf(" %s", WhiteRook);
		  break;
		  
		  //Bottom-Right WhiteRook
		  case 8:
		      printf(" %s", WhiteRook);
		  break;
		  //////////////////////////////	

		  /*
		  case 5:
		      printf("%s", BlackQueen);
		  break;

		  case 6:
		      printf("%s", BlackKing);
		  break;
		 */

		
		  /*
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
		  */

/*		  
		  default:
		      printf("Not Found a piece!\n");
		  break;
*/		  

	      }


	 }
	 printf(" %d",num_rows++);
	 printf("\n");
    }	    
}


