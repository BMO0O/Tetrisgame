#include <iostream>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define SPACE 32
#define p 112
#define P 80
#define ESC 27

#define false 0
#define true 1

#define ACTIVE_BLOCK -2
#define CEILLING -1
#define EMPTY 0
#define WALL 1
#define INACTIVE_BLOCK 2

#define MAIN_X 11
#define MAIN_Y 23
#define MAIN_X_ADJ 3
#define MAIN_Y_ADJ 1

#define STATUS_X_ADJ MAIN_X_ADJ+MAIN_X+1

using namespace std;


const int blocks[7][4][4][4] = {
   { { 0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0 },{ 0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0 },
   { 0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0 },{ 0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0 } },
   { { 0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0 },{ 0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0 }, 
   { 0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0 },{ 0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0 } },
   { { 0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0 },{ 0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0 },
   { 0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0 },{ 0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0 } },
   { { 0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0 },{ 0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0 },
   { 0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0 },{ 0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0 } },
   { { 0,0,0,0,0,0,1,0,1,1,1,0,0,0,0,0 },{ 0,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0 },
   { 0,0,0,0,0,0,0,0,1,1,1,0,1,0,0,0 },{ 0,0,0,0,0,1,0,0,0,1,0,0,0,1,1,0 } },
   { { 0,0,0,0,1,0,0,0,1,1,1,0,0,0,0,0 },{ 0,0,0,0,0,1,0,0,0,1,0,0,1,1,0,0 },
   { 0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,0 },{ 0,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0 } },
   { { 0,0,0,0,0,1,0,0,1,1,1,0,0,0,0,0 },{ 0,0,0,0,0,1,0,0,0,1,1,0,0,1,0,0 },
   { 0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0 },{ 0,0,0,0,0,1,0,0,1,1,0,0,0,1,0,0 } }
   }; //블록모양 저장 4*4공간에 블록을 표현 blcoks[b_type][b_rotation][i][j]로 사용

class Board {
public:
   Board() {
      gameboard = NULL;
   }
   Board(const int i){
      gameboard = new int*[MAIN_Y];
      for (int j = 0; j < MAIN_Y; ++j) {
         gameboard[j] = new int[MAIN_X];
         memset(gameboard[j], 0, sizeof(int)*MAIN_X);
      }

      if (i != 0) {
         for (int y = 0; y < MAIN_Y; y++) {
            for (int x = 0; x < MAIN_X; x++) {
               gameboard[y][x] = i;
               
            }
            
         }
         
      }
      
      STATUS_Y_GOAL = 4;
      STATUS_Y_LEVEL = 3;
      STATUS_Y_SCORE = 12;
   }
   Board(const Board &rhs) {
      
      gameboard = new int*[MAIN_Y];
      for (int j = 0; j < MAIN_Y; ++j) {
         gameboard[j] = new int[MAIN_X];
         memset(gameboard[j], 0, sizeof(int)*MAIN_X);
      }
      

      for (int y = 0; y < MAIN_Y; y++) {
         for (int x = 0; x < MAIN_X; x++) {
            gameboard[y][x] = rhs.gameboard[y][x];
            
         }
         
      }
      
   }

   ~Board() {
      for (int i = 0; i < MAIN_Y; ++i) {
         delete[] gameboard[i];
      }
      delete[] gameboard;

      gameboard = NULL;
   }

   Board &operator=(const Board &rhs) {
      if (this != &rhs) {
         for (int i = 0; i < MAIN_Y; ++i) {
            delete[] gameboard[i];
         }
         delete[] gameboard;//동적할당 해제
         gameboard = new int*[MAIN_Y];
         for (int j = 0; j < MAIN_Y; ++j) {
            gameboard[j] = new int[MAIN_X];
            memset(gameboard[j], 0, sizeof(int)*MAIN_X);
         }

         for (int y = 0; y < MAIN_Y; y++) {
            for (int x = 0; x < MAIN_X; x++) {
               gameboard[y][x] = rhs.gameboard[y][x];
               
            }
            
         }
         return *this;
      }
   }

   Board &operator=(const int &rhs) {
      for (int i = 0; i < MAIN_Y; ++i) {
         delete[] gameboard[i];
      }
      delete[] gameboard;//동적할당 해제
      gameboard = new int*[MAIN_Y];
      for (int j = 0; j < MAIN_Y; ++j) {
         gameboard[j] = new int[MAIN_X];
         memset(gameboard[j], 0, sizeof(int)*MAIN_X);
      }
      
      if (rhs != 0) {
         for (int y = 0; y < MAIN_Y; y++) {
            for (int x = 0; x < MAIN_X; x++) {
               gameboard[y][x] = rhs;
               
            }
            
         }
      }

      return *this;
   }
   
   Board &operator=(Board &&rhs) {//이건 좀 생각해보자,,
      bx = rhs.bx;
      by = rhs.by;
      b_rotation = rhs.b_rotation;
      b_crash = rhs.b_crash;
      b_type = rhs.b_type;
      b_type_next = rhs.b_type_next;
      key = rhs.key;
      level = rhs.level;
      score = rhs.score;
      cnt = rhs.cnt;
      speed = rhs.speed;
      //flag
      level_up_on = rhs.level_up_on;
      new_block_on = rhs.new_block_on;
      crush_on = rhs.crush_on;
      space_key_on = rhs.space_key_on;
      //board
      gameboard = rhs.gameboard;
      STATUS_Y_GOAL = rhs.STATUS_Y_GOAL;
      STATUS_Y_LEVEL = rhs.STATUS_Y_LEVEL;
      STATUS_Y_SCORE = rhs.STATUS_Y_SCORE;
      rhs.gameboard = NULL;
      return *this;
   }

   void reset(Board& cpy);
   void reset_main(Board& cpy);
   void change_bit(const int y, const int x, const int num);
   void draw_map();
   void draw_main(Board& cpy);
   void new_block();
   void check_key(Board& cpy);
   void drop_block(Board& cpy);
   void move_block(const int dir);
   void check_line(Board& cpy);
   void check_level_up(Board& cpy);
   void check_game_over(Board& cpy);
   void pause(Board& cpy);


   int check_crush(const int bx, const int by, const int rotation);

   int (*Getmainboard())[MAIN_X] {
      return main_board;
   }

   int Getmainboard_bit(int i, int j) {
      return gameboard[i][j];
   }


   int Getkey() const {
      return key;
   }

   int Getscore() const {
      return score;
   }
   
   int Getspeed() const {
      return speed;
   }

   int Getcrush_flag() const {
      return crush_on;
   }

   int Getbx() const {
      return bx;
   }

   int Getby() const {
      return by;
   }

   int Getb_rotation() const {
      return b_rotation;
   }

   int Getspace_flag() const {
      return space_key_on;
   }

   int Getnewblock_flag() const {
      return new_block_on;
   }

   void Changespace_flag(const int num) {
      space_key_on = num;
   }

    


private:
   int bx, by, b_rotation, b_crash, b_type, b_type_next;
   int key;
   int level;
   int level_goal;
   int score;
   int cnt;
   int speed;
   //flag
   int level_up_on;
   int new_block_on;
   int crush_on;
   int space_key_on;
   //board
   int **gameboard;
   int main_board[MAIN_Y][MAIN_X];
   int STATUS_Y_GOAL;
   int STATUS_Y_LEVEL;
   int STATUS_Y_SCORE;
};

void title();

void gotoxy(const int x, const int y) {  //커서 숨기는 함수
   COORD pos;
   pos.X = 2*x;
   pos.Y = y;
   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

typedef enum {NOCURSOR, SOLIDCURSOR, NORMALCURSOR} CURSOR_TYPE;
void setcursortype(CURSOR_TYPE c) {
   CONSOLE_CURSOR_INFO CurInfo;

   switch (c) {
   case NOCURSOR:
      CurInfo.dwSize = 1;
      CurInfo.bVisible = FALSE;
      break;
   case SOLIDCURSOR:
      CurInfo.dwSize = 100;
      CurInfo.bVisible = TRUE;
   case NORMALCURSOR:
      CurInfo.dwSize = 20;
      CurInfo.bVisible = TRUE;
      break;

   }

   SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

int main() {
   Board cpy(0);
   Board org(0);
   int i;
   
   //cpy.operator=(10);
   //org.operator=(1);
   int board[MAIN_Y][MAIN_X] = {0,};
   
   srand((unsigned)time(NULL));
   setcursortype(NOCURSOR);

   //change(org, cpy);
   org.reset(cpy);

   while (1) {
      for (i = 0; i < 5; i++) {
         org.check_key(cpy);
         org.draw_main(cpy);
         Sleep(org.Getspeed());

         if (org.Getcrush_flag() && org.check_crush(org.Getbx(), org.Getby() + 1, org.Getb_rotation()) == false) Sleep(100);
         if (org.Getspace_flag() == 1) {
            org.Changespace_flag(0);
            break;
         }
      }
      org.drop_block(cpy);
      //org.check_level_up(cpy);
      org.check_game_over(cpy);
      if (org.Getnewblock_flag() == 1) org.new_block();
   }

}

void Board::reset(Board& cpy) {
   level = 1;
   level_goal = 1000;
   score = 0;
   key = 0;
   crush_on = 0;
   cnt = 0;
   speed = 100;

   system("cls");
   reset_main(cpy);
   
   draw_map();
   draw_main(cpy);
   
   b_type_next = rand() % 7;
   new_block();
}

void Board::reset_main(Board& cpy) {
   int i, j;

   this->operator=(0);
   cpy.operator=(100);

   for (j = 1; j < MAIN_X; j++) {
      //this->gameboard[3][j] = CEILLING;
      this->gameboard[3][j] = CEILLING;
   }

   for (i = 1; i < MAIN_Y - 1; i++) {
      this->gameboard[i][0] = WALL;
      this->gameboard[i][MAIN_X - 1] = WALL;
   }

   for (j = 0; j < MAIN_X; j++) {
      this->gameboard[MAIN_Y - 1][j] = WALL;
   }
}

void Board::draw_map(void) {
   int y = 3;

   gotoxy(STATUS_X_ADJ, y); cout << "LEVEL : ";
   gotoxy(STATUS_X_ADJ, y + 1); cout << "GOAL : ";
   gotoxy(STATUS_X_ADJ, y + 2); cout << "+-  N E X T  -+";
   gotoxy(STATUS_X_ADJ, y + 3); cout << "|             |";
   gotoxy(STATUS_X_ADJ, y + 3); cout << "|             | ";
   gotoxy(STATUS_X_ADJ, y + 4); cout << "|             | ";
   gotoxy(STATUS_X_ADJ, y + 5); cout << "|             | ";
   gotoxy(STATUS_X_ADJ, y + 6); cout << "|             | ";
   gotoxy(STATUS_X_ADJ, y + 7); cout << "+-- -  -  - --+ ";
   gotoxy(STATUS_X_ADJ, y + 8); cout << " YOUR SCORE :";
   gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE = y + 9); cout << "              "<<score;
   /*
   gotoxy(STATUS_X_ADJ, y + 10); printf(" LAST SCORE :");
   gotoxy(STATUS_X_ADJ, y + 11); score:("        %6d", last_score);
   gotoxy(STATUS_X_ADJ, y + 12); printf(" BEST SCORE :");
   gotoxy(STATUS_X_ADJ, y + 13); printf("        %6d", best_score);*/
   gotoxy(STATUS_X_ADJ, y + 15); printf("  △   : Shift        SPACE : Hard Drop");
   gotoxy(STATUS_X_ADJ, y + 16); printf("◁  ▷ : Left / Right   P   : Pause");
   gotoxy(STATUS_X_ADJ, y + 17); printf("  ▽   : Soft Drop     ESC  : Quit");
}

void Board::draw_main(Board& cpy){
   int i, j;

   for (j = 1; j < MAIN_X - 1; j++) {
      if (this->gameboard[3][j] == EMPTY) this->gameboard[3][j] = CEILLING;
   }

   for (i = 0; i < MAIN_Y; i++) {
      for (j = 0; j < MAIN_X; j++) {
         if (cpy.gameboard[i][j] != this->gameboard[i][j]) {
            gotoxy(MAIN_X_ADJ + j, MAIN_Y_ADJ + i);
            switch (this->gameboard[i][j]) {
            case EMPTY:
               cout << "  ";
               break;
            case CEILLING:
               cout << ". ";
               break;
            case WALL:
               cout << "▩";
               break;
            case INACTIVE_BLOCK:
               cout << "□";
               break;
            case ACTIVE_BLOCK:
               cout << "■";
               break;
            }
         }
      }
   }

   for (i = 0; i < MAIN_Y; i++) {
      for (j = 0; j < MAIN_X; j++) {
         //cpy.change_bit(i, j, org.Getmainboard_bit(i, j));
         cpy.gameboard[i][j] = this->gameboard[i][j];

      }
   }
}

void Board::new_block() {
   int i, j;

   bx = (MAIN_X / 2) - 1;
   by = 0;
   b_type = b_type_next;
   b_type_next = rand() % 7;
   b_rotation = 0;

   new_block_on = 0;

   for (i = 0; i < 4; i++) {
      for (j = 0; j < 4; j++) {
         if (blocks[b_type][b_rotation][i][j] == 1) this->gameboard[by + i][bx + j] = ACTIVE_BLOCK;//org.change_bit(by+i, bx+j, ACTIVE_BLOCK);
      }
   }

   for (i = 1; i < 3; i++) {
      for (j = 0; j < 4; j++) {
         if (blocks[b_type_next][0][i][j] == 1) {
            gotoxy(STATUS_X_ADJ + 2 + j, i + 6);
            cout << "■";
         }
         else {
            gotoxy(STATUS_X_ADJ + 2 + j, i + 6);
            cout << "  ";

         }
      }
   }
}


void Board::check_key(Board& cpy) {
   key = 0;

   if (_kbhit()) {
      key = _getch();
      if (key == 224) {
         do { key = _getch(); } while (key == 224);
         switch (key) {
         case LEFT:
            if (check_crush(bx - 1, by, b_rotation) == true) move_block(LEFT);
            break;
         case RIGHT:
            if (check_crush(bx + 1, by, b_rotation) == true) move_block(RIGHT);
            break;
         case DOWN:
            if (check_crush(bx, by + 1, b_rotation) == true) move_block(DOWN);
            break;
         case UP:
            if (check_crush(bx, by, (b_rotation + 1) % 4) == true) move_block(UP);
            else if (crush_on == 1 && check_crush(bx, by - 1, (b_rotation + 1) % 4) == true) move_block(100);

         }
      }
      else {
         switch (key) {
         case SPACE:
            space_key_on = 1;
            while (crush_on == 0) {
               drop_block(cpy);
               score += level;
               
            }
            break;
         case P:
         case p:
            pause(cpy);
            break;
         case ESC:
            system("cls");
            exit(0);
            break;
         }
      }
   }
   while (_kbhit()) _getch();
}

void Board::drop_block(Board& cpy) {
   int i, j;

   if (crush_on&&check_crush(bx, by + 1, b_rotation) == true) crush_on = 0;
   if (crush_on&&check_crush(bx, by + 1, b_rotation) == false) {
      for (i = 0; i < MAIN_Y; i++) {
         for (j = 0; j < MAIN_X; j++) {
            if (gameboard[i][j] == ACTIVE_BLOCK) gameboard[i][j] = INACTIVE_BLOCK;
         }
      }
      crush_on = 0;
      check_line(cpy); //이거 오류 확인
      new_block_on = 1;
      return;
   }
   if (check_crush(bx, by + 1, b_rotation) == true) move_block(DOWN);
   if (check_crush(bx, by + 1, b_rotation) == false) crush_on++;

}

int Board::check_crush(const int bx, const int by, const int b_rotation) {
   int i, j;

   for (i = 0; i < 4; i++) {
      for (j = 0; j < 4; j++) {
         if (blocks[b_type][b_rotation][i][j] == 1 && this->gameboard[by + i][bx + j] > 0) return false;
      }
   }
   return true;
}

void Board::move_block(const int dir) {
   int i, j;

   switch (dir) {
   case LEFT:
      for (i = 0; i < 4; i++) {
         for (j = 0; j < 4; j++) {
            if (blocks[b_type][b_rotation][i][j] == 1) gameboard[by + i][bx + j] = EMPTY;
         }
      }
      for (i = 0; i < 4; i++) {
         for (j = 0; j < 4; j++) {
            if (blocks[b_type][b_rotation][i][j] == 1) gameboard[by + i][bx + j - 1] = ACTIVE_BLOCK;
         }
      }
      bx--;
      break;
   
   case RIGHT:
      for (i = 0; i < 4; i++) {
         for (j = 0; j < 4; j++) {
            if (blocks[b_type][b_rotation][i][j] == 1) gameboard[by + i][bx + j] = EMPTY;
         }
      }
      for (i = 0; i < 4; i++) {
         for (j = 0; j < 4; j++) {
            if (blocks[b_type][b_rotation][i][j] == 1) gameboard[by + i][bx + j + 1] = ACTIVE_BLOCK;
         }
      }
      bx++;
      break;
      
   case DOWN:
      for (i = 0; i<4; i++) {
         for (j = 0; j<4; j++) {
            if (blocks[b_type][b_rotation][i][j] == 1) gameboard[by + i][bx + j] = EMPTY;
         }
      }
      for (i = 0; i<4; i++) {
         for (j = 0; j<4; j++) {
            if (blocks[b_type][b_rotation][i][j] == 1) gameboard[by + i + 1][bx + j] = ACTIVE_BLOCK;
         }
      }
      by++;
      break;
   case UP: //키보드 위쪽 눌렀을때 회전시킴. 
      for (i = 0; i<4; i++) { //현재좌표의 블럭을 지움  
         for (j = 0; j<4; j++) {
            if (blocks[b_type][b_rotation][i][j] == 1) gameboard[by + i][bx + j] = EMPTY;
         }
      }
      b_rotation = (b_rotation + 1) % 4; //회전값을 1증가시킴(3에서 4가 되는 경우는 0으로 되돌림) 
      for (i = 0; i<4; i++) { //회전된 블록을 찍음 
         for (j = 0; j<4; j++) {
            if (blocks[b_type][b_rotation][i][j] == 1) gameboard[by + i][bx + j] = ACTIVE_BLOCK;
         }
      }
      break;

   case 100: //블록이 바닥, 혹은 다른 블록과 닿은 상태에서 한칸위로 올려 회전이 가능한 경우 
           //이를 동작시키는 특수동작 
      for (i = 0; i<4; i++) {
         for (j = 0; j<4; j++) {
            if (blocks[b_type][b_rotation][i][j] == 1) gameboard[by + i][bx + j] = EMPTY;
         }
      }
      b_rotation = (b_rotation + 1) % 4;
      for (i = 0; i<4; i++) {
         for (j = 0; j<4; j++) {
            if (blocks[b_type][b_rotation][i][j] == 1) gameboard[by + i - 1][bx + j] = ACTIVE_BLOCK;
         }
      }
      by--;
      break;
   }

}

void Board::check_line(Board& cpy) {
   int i, j, k, l;

   int block_amount;
   int combo = 0;

   for (i = MAIN_Y - 2; i > 3;) {
      block_amount = 0;
      for (j = 1; j < MAIN_X - 2; j++) {
         if (gameboard[i][j] > 0) block_amount++;
      }
      if (block_amount >= MAIN_X - 3) {
         /*
         if (level_up_on == 0) {
            score += 100 + level;
            cnt++;
            combo++;
         }*/
         for (k = i; k > 1; k--) {
            for (l = 1; l < MAIN_X - 1; l++) {
               if (this->gameboard[k - 1][l] != CEILLING) gameboard[k][l] = gameboard[k - 1][l];
               if (this->gameboard[k - 1][l] == CEILLING) gameboard[k][l] = EMPTY;
            }
         }
      }
      else i--;
   }
   /*
   if (combo) {
      if (combo > 1) {
         gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 1, MAIN_Y_ADJ + by - 2); cout << combo << " COMBO!";
         Sleep(500);
         score += (combo*level * 100);
         cpy.operator=(0);
      }
      gotoxy(STATUS_X_ADJ, STATUS_Y_GOAL); cout << " GOAL  :  " << ((cnt <= 10) ? 10 - cnt : 0);
      gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE); cout << "        " << score;
   }*/
}

void Board::check_level_up(Board& cpy) {
   int i, j;

   if (cnt >= 10) {
      draw_main(cpy);
      level_up_on = 1;
      level += 1;
      cnt = 0;

      for (i = 0; i < 4; i++) {
         gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 3, MAIN_Y_ADJ + 4);
         cout << "             ";
         gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 2, MAIN_Y_ADJ + 6);
         cout << "             ";
         Sleep(200);

         gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 3, MAIN_Y_ADJ + 4);
         cout << "*-*LEVEL UP!*-*";
         gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 2, MAIN_Y_ADJ + 6);
         cout << "☆SPEED UP!☆";
         Sleep(200);
      }
      cpy.operator=(0);

      for (i = MAIN_Y - 2; i > MAIN_Y - 2 - (level - 1); i--) {
         for (j = 1; j < MAIN_X - 1; j++) {
            this->gameboard[i][j] = INACTIVE_BLOCK;
            gotoxy(MAIN_X_ADJ + j, MAIN_Y_ADJ + i);
            cout << "★";
            Sleep(20);
         }
      }
      Sleep(100);
      check_line(cpy);

      switch (level) {
      case 2:
         speed = 50;
         break;
      case 3:
         speed = 25;
         break;
      case 4:
         speed = 10;
         break;
      case 5:
         speed = 5;
         break;

      }
      level_up_on = 0;

      gotoxy(STATUS_X_ADJ, STATUS_Y_LEVEL); cout << " LEVEL : " << level;
      gotoxy(STATUS_X_ADJ, STATUS_Y_GOAL); cout << " GOAL : " << 10 - cnt;
   }
}

void Board::check_game_over(Board& cpy) {
   int i;
   int x=5;
   int y=5;

   for (i = 1; i < MAIN_X - 2; i++) {
      if (this->gameboard[3][i]>0) { //천장(위에서 세번째 줄)에 inactive가 생성되면 게임 오버 
         gotoxy(x, y + 0); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤"); //게임오버 메세지 
         gotoxy(x, y + 1); printf("▤                              ▤");
         gotoxy(x, y + 2); printf("▤  +-----------------------+   ▤");
         gotoxy(x, y + 3); printf("▤  |  G A M E  O V E R..   |   ▤");
         gotoxy(x, y + 4); printf("▤  +-----------------------+   ▤");
         gotoxy(x, y + 5); printf("▤   YOUR SCORE: %6d         ▤", score);
         gotoxy(x, y + 6); printf("▤                              ▤");
         gotoxy(x, y + 7); printf("▤  Press any key to restart..  ▤");
         gotoxy(x, y + 8); printf("▤                              ▤");
         gotoxy(x, y + 9); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
         

         Sleep(1000);
         while (_kbhit()) _getch();
         key = _getch();
         reset(cpy);
      }
   }
}

void Board::pause(Board& cpy) {
   int i, j;
   int x = 5;
   int y = 5;

   for (i = 1; i < MAIN_X - 2; i++) {
      gotoxy(x, y + 0); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
      gotoxy(x, y + 1); printf("▤                              ▤");
      gotoxy(x, y + 2); printf("▤  +-----------------------+   ▤");
      gotoxy(x, y + 3); printf("▤  |       P A U S E       |   ▤");
      gotoxy(x, y + 4); printf("▤  +-----------------------+   ▤");
      gotoxy(x, y + 5); printf("▤  Press any key to resume..   ▤");
      gotoxy(x, y + 6); printf("▤                              ▤");
      gotoxy(x, y + 7); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
   }
   Sleep(200);
   while (_getch()==0);

   system("cls");
   cpy.operator=(0);
   draw_main(cpy);
   draw_map();

   for (i = 1; i < 3; i++) {
      for (j = 0; j < 4; j++) {
         if (blocks[b_type_next][0][i][j] == 1) {
            gotoxy(MAIN_X + MAIN_X_ADJ + 3 + j, i + 6);
            cout << "■";
         }
         else {
            gotoxy(MAIN_X + MAIN_X_ADJ + 3 + j, i + 6);
            cout << "  ";
         }
      }
   }
}