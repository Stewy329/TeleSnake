#include <ncurses.h>
#include <stdlib.h>


//Globals

int Direction_y;
int Direction_x;

typedef struct body {
	int x;
	int y;
	int last_x;
	int last_y;
	struct body *next;
}body_t;



//if a block is not a teleporter than it is just food everytime we get food we must generate a new random one
typedef struct block{
	int tele;
	int x;
	int y;
	struct block *pair;
}block_t;
//So basically we make teleporting pairs for the game
//We will be able to randomnly generate worlds later on just not right now

//int mvprintw(int y, int x, char *fmt, ...);


void updateHeadValues(int new_x, int new_y, body_t *head){
	head->last_x = head->x;
	head->last_y = head->y;
	head->x = new_x;
	head->y = new_y;
	return;
}

//Draws all blocks then snake only one frame then return
void drawSnakeAndBlocks(body_t *head, block_t *ArrayOfBlocks, int len){
	body_t *current = head;

	mvprintw(0,0,"Direction_x: %d Direction_y: %d		Press q to leave",Direction_x, Direction_y);

	for(int i=0;i<len;i++){
		if(ArrayOfBlocks[i].tele<1){	
			attron(COLOR_PAIR(2));
			mvprintw(ArrayOfBlocks[i].y, ArrayOfBlocks[i].x, "f");
			attroff(COLOR_PAIR(2));
		}
		else{
			attron(COLOR_PAIR(3));
			mvprintw(ArrayOfBlocks[i].y, ArrayOfBlocks[i].x,"t");
			attroff(COLOR_PAIR(3));
		}
	}

	//draw head first
	attron(COLOR_PAIR(4));
	mvprintw(head->y, head->x, "h");
	napms(1000);
	attroff(COLOR_PAIR(4));


	while(true){
		//print next part of body by peeping into the next block of data without changing		
		attron(COLOR_PAIR(1));
		mvprintw(current->y, current->x,"b");
		attroff(COLOR_PAIR(1));


		if(current->next == NULL){
			return;
		}
		else{
			updateHeadValues(current->x, current->y, current->next);
			current = current-> next;
		}
	}

return;
}



//return value is really just me being retarded dw about it
int ate(block_t *ArrayOfBlocks, int len, body_t* head,int pos_x, int pos_y, int max_x, int max_y){
//	perror("Allah");
	//Find if we hit a block
	body_t *current = head;
	for(int i=0;i<len;i++){
		if(pos_x == ArrayOfBlocks[i].x && pos_y == ArrayOfBlocks[i].y){
			//When we teleport we are going to make the values of the head change, and that value should be the teleporters pair values plus the direction once more so it appears the snake came out of the teleporter and doesnt like move over it
			if(ArrayOfBlocks[i].tele>0){
updateHeadValues((ArrayOfBlocks[i].pair->x)+Direction_x, (ArrayOfBlocks[i].pair->y)+Direction_y, head);	
				endwin();
				printf("game over");
				return 1;
			}
			else if(ArrayOfBlocks[i].tele<0){
				while(current->next != NULL){ //while the next node isnt null keep going until there is one that is null
                                current = current->next;
                        }
                        //once we find a next node that isnt null we will allocate a new node there
                        current->next = (body_t*)malloc(sizeof(body_t));
				return 1;
			}
			return 0;
		}
	}
}

int handleInputAndNextFrame(block_t *ArrayOfBlocks, int len, body_t* head, int max_x, int max_y){
	nodelay(stdscr, true);
	int ch;
	while(ch=getch()){
		napms(500);
		erase();
		if(ch==KEY_DOWN){
			//perror("presses down");
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
		else if(ch=='q'){
			endwin();
			return 1;
		}
		//Update the next position of the snake
		//head->x = (head->x + Direction_x);
		//head->y = (head->y + Direction_y);

		//Find out if we hit a wall and if so we die
		//Find out if we ate smth or teleported some shit then add that to the snake
ate(ArrayOfBlocks, len, head, (head->x+Direction_x), (head->y+Direction_y), max_x, max_y);		
		//Now update the snake starting with the head making it down the chain of the body
drawSnakeAndBlocks(head, ArrayOfBlocks, len);

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
	int scrHeight, scrWidth;


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
	
	init_pair(1, COLOR_BLACK, COLOR_WHITE);//body
	init_pair(2, COLOR_BLACK, COLOR_YELLOW);//Food
	init_pair(3, COLOR_BLACK, COLOR_BLUE);//teleporter
	init_pair(4, COLOR_RED, COLOR_BLACK);//head of snake

	keypad(stdscr, true);
	curs_set(0);
	printw("Press any button to play snake");
	refresh();
	noecho();
	cbreak();
	getch();
	getmaxyx(stdscr, scrHeight, scrWidth);
	
	handleInputAndNextFrame(ArrayOfBlocks, 4, &head, scrWidth, scrHeight);

	endwin();

	return 0;
}
