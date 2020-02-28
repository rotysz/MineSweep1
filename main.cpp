#include <stdio.h>
#include <string>
#include<iostream>

#include "rlutil.h"

using namespace std;

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#define PLANSZA_MAX_X 20
#define PLANSZA_MAX_Y 20
typedef int TPlansza[PLANSZA_MAX_X][PLANSZA_MAX_Y];


#define MAX_SCREEN_X 160
#define MAX_SCREEN_Y 100
class CScreenChar {
    private:
        char scr_value = ' ';
        int  scr_fcolor = rlutil::WHITE;
        int  scr_bcolor  = rlutil::BLACK;
    public:
    
    CScreenChar() {
        
    }
    
    void SetValueAndColor (char value_, int fcolor_, int bcolor_) {
        scr_value = value_;
        scr_fcolor = fcolor_;
        scr_bcolor = bcolor_;
    }
    
    void SetValue (char value_) {
        scr_value = value_;
    }
    void SetColor (char value_, int fcolor_, int bcolor_) {
        scr_fcolor = fcolor_;
        scr_bcolor = bcolor_;

    }
    
    char GetValue() {
        return scr_value;
    }
    
    int GetForegroundColor () {
        return scr_fcolor;
    
    }
    int GetBackgroundColor (){
        return scr_bcolor;
    }
    
};

class CConsoleScreen{
private:
    CScreenChar screen_table[MAX_SCREEN_X][MAX_SCREEN_Y];
    
public:
    void Cls () {
        for (int x= 0;  x < MAX_SCREEN_X;x++)
            for (int y = 0; y<MAX_SCREEN_Y;y++ )
                screen_table[x][y].SetValueAndColor(' ', rlutil::WHITE, rlutil::BLACK);
        rlutil::cls();
        rlutil::saveDefaultColor();
    }
    
    void SetCursor (int x_, int y_) {
        rlutil::locate(x_, y_);
    }
    
    void PrintChar (int x_, int y_, char scr_char_, int fcolor_ = rlutil::WHITE, int bcolor_ = rlutil::BLACK) {
        
        screen_table [x_][y_].SetValueAndColor(scr_char_, fcolor_, bcolor_);
        rlutil::locate(x_, y_);
        rlutil::setBackgroundColor(bcolor_);
        rlutil::setColor(fcolor_);
        rlutil::setChar(scr_char_);
        rlutil::resetColor();
        
    };
    
    void PrintString (int x_, int y_, string in_string, int fcolor_ = rlutil::WHITE, int bcolor_ = rlutil::BLACK ) {
       
        for (int pos = 0; pos < in_string.length();pos++) {
            PrintChar(x_+ pos, y_, in_string.at(pos),fcolor_, bcolor_);
        }
    }
    
    void PrintInt(int x_, int y_, int number, int fcolor_ = rlutil::WHITE, int bcolor_ = rlutil::BLACK ){
        
        PrintString(x_, y_, to_string(number), fcolor_, bcolor_);
    }
    
    void ChangeColor (int x_, int y_, int fcolor_ = rlutil::WHITE, int bcolor_ = rlutil::BLACK ) {
        PrintChar(x_, y_, screen_table[x_][y_].GetValue(),fcolor_,bcolor_);
    }
    
    void ReverseColor (int x_, int y_ ) {
        PrintChar(x_, y_, screen_table[x_][y_].GetValue(),screen_table[x_][y_].GetBackgroundColor(),screen_table[x_][y_].GetForegroundColor());
    }
    
    void ClearLine ( int x_, int y_, int len_,int fcolor_ = rlutil::WHITE, int bcolor_ = rlutil::BLACK ) {
        
        for (int pos = 0; pos <len_ ; pos ++) {
            PrintChar(x_, y_, ' ', fcolor_, bcolor_ );
        }
        
    }
    
    void Refresh(){
        rlutil::cls();
        for (int x= 0;  x < MAX_SCREEN_X;x++)
          for (int y = 0; y<MAX_SCREEN_Y;y++ )
            PrintChar(x,y, screen_table[x][y].GetValue(), screen_table[x][y].GetForegroundColor(), screen_table[x][y].GetBackgroundColor());
        
    }
        
};


class CPolePlanszy {
    private:
        bool mine_present = false;
        bool visible      = false;
        bool flaged      = false;
        int  near_mines   = 0;
        
    public:
      CPolePlanszy () {
          
      }
      
      CPolePlanszy (bool mine_present_) {
          mine_present = mine_present_;
      }
      
      void PutMine () {
          mine_present = true;
      }
      
      void RemoveMine () {
          mine_present = false;
      }
      
      bool isMinned () {
          return mine_present;
      }
      
      void SetVisible () {
          visible = true;
      }
      
      void Hide () {
          visible = false;
      }
      
      bool isVisible () {
          return visible;
      }
      
      void Flag () {
        flaged = true;
      }
      
      void UnFlag () {
          flaged = false;
      }
      
      void ToogleFlag () {
          flaged = !flaged;
      }
      
      bool isFlaged () {
          return (flaged);
      }
      
      void setNearMines (int near_mines_){
          near_mines = near_mines_;
      }
      
      int getNearMines () {
         return near_mines;
      }
      
      int IncrementNearMines() {
          return near_mines++;
      }
      
      int DecrementNearMines() {
          return near_mines--;
      }
      
      
      string toStr (bool debug) {
        
        string string_buffer= "  ";
        
        if (debug)
          if (visible)
            if (mine_present)
              string_buffer = "XX";
            else
              string_buffer ="0"+ to_string(near_mines);
          else
             if (mine_present)
                string_buffer = "XH";
             else
               string_buffer = to_string(near_mines) +"-";
         else
           if (visible)
              if (mine_present)
                 string_buffer = "MV";
              else
                 string_buffer ="0" + to_string(near_mines);
           else
              if (mine_present)
                 string_buffer = "HH";
              else
                string_buffer = "HH";
            
       
        return string_buffer;
     }
    
    char toChar (bool debug) {
       
       char ret_char= ' ';
       
       if (debug)
         if (visible)
           if (mine_present)
             ret_char = '*';
           else
             ret_char ='0'+ near_mines;
         else
            if (mine_present)
               ret_char = 'H';
            else
              ret_char = '0'+ near_mines;
        else
          if (visible)
             if (mine_present)
                ret_char = '*';
             else
                 ret_char ='0'+ near_mines;
          else
             if (mine_present)
                ret_char = 'X';
             else
               ret_char = 'X';
           
      
       return ret_char;
    }
};



class CPlansza {
    private:
      CPolePlanszy Plansza[PLANSZA_MAX_X][PLANSZA_MAX_Y];
      int xsize;
      int ysize;
      
      CConsoleScreen screen;
      enum TCellSize { SMALL, BIG} cell_size;
    int plansza_pos_x = 4;
    int plansza_pos_y = 4;
    
    
    public:
   
    CPlansza () {
        xsize=10;
        ysize=10;
    };
     
    CPlansza (int xsize_, int ysize_) {
        xsize = xsize_;
        ysize = ysize_;
        cell_size = SMALL;
    }
    
    void DspPlansza () {
      printf("-.\n");
       for (int i = 0; i <ysize;i++){
          for (int j = 0; j<xsize; j++)
             cout << (Plansza[j][i].toStr(true) + " ");
          printf("\n");
        }
    }
    
    int PutMine(int x_, int y_) {
       if ( x_>=xsize )
          return -1;
       if (y_>=ysize)
          return -1;
       Plansza[x_][y_].PutMine();
       return 0;
    }
    
    int RemoveMine(int x_, int y_) {
       if ( x_ >= xsize )
          return -1;
       if (y_ >= ysize)
          return -1;
       Plansza[x_][y_].RemoveMine();
       return 0;
    }
    
    void CalcOneMine (int x_, int y_) {
        
      if (!Plansza[x_][y_].isMinned())
          for (int i = max(0,x_-1); i<min(xsize,x_+2);i++)
            for (int j = max(0,y_-1); j<min(ysize,y_+2);j++)
               if(Plansza[i][j].isMinned())
                 Plansza[x_][y_].IncrementNearMines();
           
              
    }

    void CalcAllMines () {
          
       for (int i = 0; i <xsize;i++)
         for (int j = 0; j<ysize; j++)
           CalcOneMine(i,j);
    }
    
    
    void Unhide( int x_, int y_) {
        
        if (!(Plansza[x_][y_].isMinned() || (Plansza[x_][y_].isVisible() && !Plansza[x_][y_].isMinned())) ){
            if (Plansza[x_][y_].getNearMines() > 0) {
                Plansza[x_][y_].SetVisible();
            } else if (!Plansza[x_][y_].isMinned()) {
                Plansza[x_][y_].SetVisible();
                for (int i = max(0,x_-1); i<min(xsize,x_+2);i++)
                    for (int j = max(0,y_-1); j<min(ysize,y_+2);j++)
                        Unhide(i,j);
            }
        }
    }
    
    
    void GenerateTest () {
       for (int i = 0; i <xsize;i++)
         for (int j = 0; j<ysize; j++)
           if (i == j) Plansza[i][j].PutMine();
           else Plansza[i][j].RemoveMine();
    }
    
    void SetPlanszaPos(int x_, int y_) {
        plansza_pos_x = x_;
        plansza_pos_y = y_;
    }
    
    void SetCellSize (TCellSize cell_size_){
        cell_size = cell_size_;
    }

    void DisplayOneCell (int idx_x_,  int idx_y_, char inside_char, int frame_f_color_) {
        
        int init_x;
        int init_y;
        
        if (cell_size == BIG) {
            init_x = plansza_pos_x + idx_x_ *4 +2;
            init_y = plansza_pos_y + idx_y_ *2 +2;
            if (idx_x_ == 0) {
                screen.PrintChar(plansza_pos_x+1, init_y, '|',frame_f_color_);
                screen.PrintChar(plansza_pos_x+1, init_y+1, '-',frame_f_color_);
            }
            if (idx_y_ == 0) screen.PrintString(init_x, plansza_pos_y+1, "----",frame_f_color_);
            screen.PrintString(init_x, init_y,"   |",frame_f_color_);
            screen.PrintString(init_x, init_y+1,"----",frame_f_color_);
            screen.PrintChar(init_x+1, init_y,inside_char);
        } else {
            init_x = plansza_pos_x + idx_x_*2  +2;
            init_y = plansza_pos_y + idx_y_ +2;
            if (idx_x_ == 0) {
                screen.PrintChar(plansza_pos_x+1, init_y, '|',frame_f_color_);
                screen.PrintChar(plansza_pos_x+1, init_y+1, '-',frame_f_color_);
            }
            if (idx_y_ == 0) screen.PrintString(init_x, plansza_pos_y+1, "---",frame_f_color_);
            if (idx_y_ == ysize-1) screen.PrintString(init_x, init_y+1, "---",frame_f_color_);
            screen.PrintString(init_x, init_y,"   |",frame_f_color_);
            //screen.PrintString(init_x, init_y+1,"----",frame_f_color_);
            screen.PrintChar(init_x+1, init_y,inside_char);
        }
    }
    
    void ChangeCellFrameColor( int idx_x_,  int idx_y_,  int frame_f_color_, int frame_b_color_) {
        
        int init_x;
        int init_y;
        
        if (cell_size == BIG) {
            init_x = plansza_pos_x + idx_x_ *4 +3;
            init_y = plansza_pos_y + idx_y_ *2 +2;
            screen.ChangeColor(init_x -1, init_y-1,frame_f_color_,frame_b_color_);
            screen.ChangeColor(init_x +1, init_y-1,frame_f_color_,frame_b_color_);
            screen.ChangeColor(init_x, init_y-1,frame_f_color_,frame_b_color_);
            
            screen.ChangeColor(init_x -2, init_y,frame_f_color_,frame_b_color_);
            screen.ChangeColor(init_x +2, init_y,frame_f_color_,frame_b_color_);
            
            screen.ChangeColor(init_x -1, init_y+1,frame_f_color_,frame_b_color_);
            screen.ChangeColor(init_x -2, init_y+1,frame_f_color_,frame_b_color_);
            screen.ChangeColor(init_x , init_y+1,frame_f_color_,frame_b_color_);
            screen.ChangeColor(init_x +1, init_y+1,frame_f_color_,frame_b_color_);
            screen.ChangeColor(init_x +2, init_y+1,frame_f_color_,frame_b_color_);
        } else {
            init_x = plansza_pos_x + idx_x_ *2 +2;
            init_y = plansza_pos_y + idx_y_  +2;
            screen.ReverseColor(init_x+1, init_y);
            
        }
        
    }
    
    void DisplayPlansza (bool debug_) {
      for (int y =0; y<ysize;y++)
        for(int x = 0; x<xsize;x++)
            DisplayOneCell( x, y, Plansza[x][y].toChar(debug_),rlutil::GREEN);
               
    }
    
}; //Class
 


int main()
{
  CPlansza Plansza(10,10);
  CConsoleScreen console_scr;
  
//  std::cout << "\nTest 1: Colors" << std::endl;
//    for (int i = 0; i < 16; i++) {
//        rlutil::setColor(i);
//        std::cout << i << " ";
//    }
//  rlutil::resetColor();
  
//
    
    console_scr.Cls();
//   for (int y =0; y<10;y++)
//        for(int x = 0; x<15;x++)
//            Plansza.DisplayOneCell( x, y, '*',rlutil::GREEN);
    
    Plansza.GenerateTest();
    Plansza.DisplayPlansza(true);
    Plansza.ChangeCellFrameColor(5 , 5, rlutil::RED,rlutil::BLACK);
   
//  console_scr.PrintString(20,5,"Ala ma kota",rlutil::RED,rlutil::WHITE);
//  console_scr.PrintString(22,6,"Kot ma Ale");
//
  console_scr.SetCursor(1, 28);

  Plansza.GenerateTest();
  Plansza.PutMine(4,5);
  Plansza.PutMine(4,7);
  Plansza.DisplayPlansza(true);
//  Plansza.DspPlansza();
  Plansza.CalcAllMines();
  Plansza.DisplayPlansza(true);
//  Plansza.DspPlansza();
  Plansza.Unhide(1,8);
  Plansza.DisplayPlansza(false);
  Plansza.ChangeCellFrameColor(5 , 5, rlutil::RED,rlutil::BLACK);
    
   console_scr.SetCursor(1, 28);
//  
    return 0;
}
