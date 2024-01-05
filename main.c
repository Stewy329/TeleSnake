#include <ncurses.h>

//Globals

int Direction_y;
int Direction_x;

typedef struct{
	int x;
	int y;
	body_t *next;
}body_t;


//if a block is not a teleporter than it is just food everytime we get food we must generate a new random one
typedef struct{
	int tele;
	int x;
	int y;
	block_t *pair;
}block_t;
//So basically we make teleporting pairs for the game
//We will be able to randomnly generate worlds later on just not right now

//int mvprintw(int y, int x, char *fmt, ...);

int drawSnake(body_t head){
	int temp_x, temp_y;
	//Draw head then the body
	temp_x = head.x;
	temp_y = head.y;
	


	while(true)
}



//return value is really just me being retarded dw about it
int ate(block_t *ArrayOfBlocks, int len, body_t* head,int pos_x, int pos_y){
	//Find if we hit a block
	for(int i=0;i<len;i++){
		if(pos_x == ArrayOfBlocks[i].x && pos_x == ArrayOfBlocks[i].y){
			//When we teleport we are going to make the values of the head change, and that value should be the teleporters pair values plus the direction once more so it appears the snake came out of the teleporter and doesnt like move over it
			if(ArrayOfBlocks[i].tele>0){
				head->x = (ArrayOfBlocks[i].pair->x)+Direction_x;
				head->y = (ArrayOfBlocks[i].pair->y)+Direction_y;
				return 0;
			}
			else{
				return 1;
			}
		}
	}
}

int handleInputAndNextFrame(block_t *ArrayOfBlocks, int len, body_t* head, int max_x, int max_y){
	noecho();
	cbreak();
	int ch;
	body_t *current = head;
	while(ch=getch()){
		erase();
		if(ch==KEY_DOWN){
			Direction_y = -1;
			Direction_x = 0;
		}
		else if(ch==KEY_UP){
			Direction_y = 1;
			Direction_x = 0;
		}
		else if(ch==KEY_RIGHT){
			Direction_y = 0;
			Direction_x = 1;
		}
		else if(ch==KEY_LEFT){
			Direction_y = 0;
			Direction_x = -1;
		}
		
		//Find out if we hit a wall and if so we die

		//Find out if we ate smth or teleported some shit then add that to the snake
		
		if(ate(ArrayOfBlocks, len, head, Direction_x, Direction_y)){
			while(current->next != NULL){ //while the next node isnt null keep going until there is one that is null
				current = current->next;
			}
			//once we find a next node that isnt null we will allocate a new node there
			current->next = (body_t*)malloc(sizeof(body_t));
		}	

		//Now update the snake starting with the head making it down the chain of the body
		drawSnake(head);

		//Now update new squares that are randomly placed around

	}
}

int drawScr(block_t *ArrayOfBlocks, int len, body_t head){ //give it the value of the array of blocks and just the head for now


}




int main(){
	//Snake head
	body_t head;
	head.x = 8;
	head.y = 6;
	Direction_x = 1;
	Direction_y = 0;

	//First Two blocks are a teleporting pair
	//Last two are food
	block_t ArrayOfBlocks[4];
	
	ArrayOfBlocks[0].x = 7;
	ArrayOfBlocks[0].y = 3;
	ArrayOfBlocks[0].tele = 1;
	ArrayOfBlocks[0].pair = &ArrayOfBlocks[1];
	
	ArrayOfBlocks[1].x = 14;
	ArrayOfBlocks[1].y = 9;
	ArrayOfBlocks[1].tele = 1;
	ArrayOfBlocks[1].pair = &ArrayOfBlocks[0];

	ArrayOfBlocks[2].x = 10;
	ArrayOfBlocks[2].y = 3;
	ArrayOfBlocks[2].tele = 0;
	ArrayOfBlocks[2].pair = NULL;

	ArrayOfBlocks[3].x = 4;
	ArrayOfBlocks[3].y = 11; 
	ArrayOfBlocks[3].tele = 0;
	ArrayOfBlocks[3].pair = NULL;


	initscr();
	
	init_pair(1, COLOR_WHITE, COLOR_BLACK);//body
	init_pair(2, COLOR_YELLOW, COLOR_BLACK);//Food
	init_pair(3, COLOR_BLUE, COLOR_BLACK);//teleporter
	init_pair(4, COLOR_RED, COLOR_BLACK);//head of snake

	keypad(stdscr, true);
	curs_set(0);
	printw("Press any button to play snake");
	refresh();
	getch();
	endwin();

	return 0;
}
