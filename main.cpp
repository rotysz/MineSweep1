#include <stdio.h>
#include <string>
#include<iostream>
#include <windows.h>

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
#define F_WHITE  FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED
#define B_WHITE  BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED
#define F_BLACK  0
#define B_BLACK  0

class CScreenChar {
    private:
        char scr_value = ' ';
        int  scr_fcolor = F_WHITE;
        int  scr_bcolor  = B_BLACK;
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

class CKeyOrMouseDetails {
  private:
    INPUT_RECORD input_event;
  public:
    enum TInputType {MOUSE,KEYBOARD,OTHER};
    enum TMouseEvent {LEFT_CLK, RIGHT_CLK, DOUBLE_CLK, CLK, MOUSE_MOVE, MOUSE_OTHER, MOUSE_NO_EVENT };
    enum TKeyEvent {KEY_PRES,KEY_REL,KEY_OTHER,KEY_NO_EVENT} ;

     CKeyOrMouseDetails () {

     };

     CKeyOrMouseDetails (INPUT_RECORD input_rec_) {
        input_event = input_rec_;
     }

     void SetInputEvent (INPUT_RECORD input_rec_) {
        input_event = input_rec_;
     }

     TInputType GetInputType () {

        TInputType ret_type;

        switch (input_event.EventType) {
          case KEY_EVENT: // keyboard input
              ret_type = KEYBOARD;
              break;

          case MOUSE_EVENT: // mouse input
               ret_type = MOUSE;
                break;
          default:
            ret_type = OTHER;
            break;
       };
       return ret_type;

     }

     TMouseEvent GetMouseEvent () {
        TMouseEvent ret_event;

        if (GetInputType() == MOUSE) {

           switch (input_event.Event.MouseEvent.dwEventFlags) {
             case 0:
                if (input_event.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
                    ret_event = LEFT_CLK;
                else if (input_event.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
                     ret_event = RIGHT_CLK;
                else
                    ret_event = CLK;
                break;
             case DOUBLE_CLICK:
                 ret_event = DOUBLE_CLK;
                 break;
             case MOUSE_MOVED:
                  ret_event = MOUSE_MOVE;
                  break;
             default:
                ret_event = MOUSE_OTHER;

           };

        } else {
          ret_event = MOUSE_NO_EVENT;
        };
        return ret_event;

      }

      TKeyEvent GetKeyEvent () {

        TKeyEvent ret_event;

        if (GetInputType() == KEYBOARD) {
            if (input_event.Event.KeyEvent.bKeyDown )
                ret_event = KEY_PRES;
            else
                ret_event = KEY_REL;
        } else {
          ret_event = KEY_NO_EVENT;
        }
        return ret_event;
      }

      int GetMouseXPos () {

         int ret_pos;

         if (GetInputType() == MOUSE)
            ret_pos = input_event.Event.MouseEvent.dwMousePosition.X;
         else
            ret_pos = -1;

         return ret_pos;
      }

     int GetMouseYPos () {

         int ret_pos;

         if (GetInputType() == MOUSE)
            ret_pos = input_event.Event.MouseEvent.dwMousePosition.Y;
         else
            ret_pos = -1;

         return ret_pos;
      }

      char GetPressedKey () {
        CHAR ret_char = '\x00';

        if(GetInputType() == KEYBOARD)
            ret_char = input_event.Event.KeyEvent.uChar.AsciiChar;

        return ret_char;
      }

};

class CConsoleScreen{
private:
    CScreenChar screen_table[MAX_SCREEN_X][MAX_SCREEN_Y];
    HANDLE h_std_in;
    HANDLE h_std_out;
    DWORD save_old_mode;


public:
    CConsoleScreen () {
       DWORD input_mode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
       h_std_in = GetStdHandle(STD_INPUT_HANDLE);
       if (h_std_in == INVALID_HANDLE_VALUE)
          cout << "Blad pobrania GetStdHandle input";
       if (! GetConsoleMode(h_std_in, &save_old_mode) )
           cout << "Blad pobrania GetConsoleMode";
       if (! SetConsoleMode(h_std_in, input_mode) )
          cout <<"Blad wywolania SetConsoleMode";
       h_std_out = GetStdHandle(STD_OUTPUT_HANDLE);
       if (h_std_out == INVALID_HANDLE_VALUE)
          cout << "Blad pobrania GetStdHandle output";

    }

    bool SetInputMode () {


        DWORD input_mode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
        if (! SetConsoleMode(h_std_in, input_mode) ) {
          cout <<"Blad wywolania SetConsoleMode";
          return false;
        } else {
            return true;

        };

    };

    void Cls () {
        for (int x= 0;  x < MAX_SCREEN_X;x++)
            for (int y = 0; y<MAX_SCREEN_Y;y++ )
                screen_table[x][y].SetValueAndColor(' ', F_WHITE, B_BLACK);
        system("cls");
        SetInputMode();
    }

    void HideCursor () {
       CONSOLE_CURSOR_INFO cursor_info;

       if (GetConsoleCursorInfo(h_std_out, &cursor_info)) {
        cursor_info.bVisible = false;
        SetConsoleCursorInfo(h_std_out,&cursor_info);
       }
    }

    void ShowCursor () {
       CONSOLE_CURSOR_INFO cursor_info;

       if (GetConsoleCursorInfo(h_std_out, &cursor_info)) {
        cursor_info.bVisible = true;
        SetConsoleCursorInfo(h_std_out,&cursor_info);
       }
    }


    void SetCursor (int x_, int y_) {
        COORD pos;

        pos.X = x_;
        pos.Y = y_;
        SetConsoleCursorPosition(h_std_out,pos);
    }

    void PrintChar (int x_, int y_, char scr_char_, int fcolor_ = F_WHITE, int bcolor_ = B_BLACK) {

        screen_table[x_][y_].SetValueAndColor(scr_char_, fcolor_, bcolor_);
        SetCursor(x_, y_);
        SetConsoleTextAttribute(h_std_out,fcolor_|bcolor_);
        cout << scr_char_;
        SetConsoleTextAttribute(h_std_out,F_WHITE|B_BLACK);

    };

    void PrintString (int x_, int y_, string in_string, int fcolor_ = F_WHITE, int bcolor_ = B_BLACK ) {

        for (int pos = 0; pos < in_string.length();pos++) {
            PrintChar(x_+ pos, y_, in_string.at(pos),fcolor_, bcolor_);
        }
    }

    void PrintInt(int x_, int y_, int number, int fcolor_ = F_WHITE, int bcolor_ = B_BLACK ){

        PrintString(x_, y_, to_string(number), fcolor_, bcolor_);
    }

    void PrintIntRJust(int x_, int y_, int number_, int field_len_ , int fcolor_ = F_WHITE, int bcolor_ = B_BLACK ){

        string buf = to_string(number_);

        ClearLine(x_,y_,field_len_,fcolor_,bcolor_);
        PrintString(x_+(field_len_ - buf.length()), y_, buf, fcolor_, bcolor_);
    }

    void ChangeColor (int x_, int y_, int fcolor_ = F_WHITE, int bcolor_ = B_BLACK ) {
        PrintChar(x_, y_, screen_table[x_][y_].GetValue(),fcolor_,bcolor_);
    }

    void ReverseColor (int x_, int y_ ) {
        PrintChar(x_, y_, screen_table[x_][y_].GetValue(),
                  screen_table[x_][y_].GetBackgroundColor(),
                  screen_table[x_][y_].GetForegroundColor()|16384);
    }

    void ClearLine ( int x_, int y_, int len_,int fcolor_ = F_WHITE, int bcolor_ = B_BLACK ) {

        for (int pos = 0; pos <len_ ; pos ++) {
            PrintChar(x_+pos, y_, ' ', fcolor_, bcolor_ );
        }

    }

    void Refresh(){
        system("cls");
        for (int x= 0;  x < MAX_SCREEN_X;x++)
          for (int y = 0; y<MAX_SCREEN_Y;y++ )
            PrintChar(x,y, screen_table[x][y].GetValue(), screen_table[x][y].GetForegroundColor(), screen_table[x][y].GetBackgroundColor());

    }

    bool MouseEventAction (MOUSE_EVENT_RECORD mer_, bool ret_move) {

    bool ret_val = false;
    switch(mer_.dwEventFlags) {
           case 0:

            if(mer_.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                ret_val = true;
            }
            else if(mer_.dwButtonState == RIGHTMOST_BUTTON_PRESSED)  {
                ret_val = true;
            }
            else {
                ret_val = true;
            };
            break;
        case DOUBLE_CLICK:
                ret_val = true;

            break;
        case MOUSE_MOVED:
            ret_val = ret_move;
            break;
        default:
            break;
    }

       return ret_val;

    }

    bool KeyEventAction (KEY_EVENT_RECORD ker) {
         return true;
    }



#define INPUT_BUF_SIZE 128
  CKeyOrMouseDetails ReadConInput(bool return_mouse_move) {

        DWORD num_read, i;
        INPUT_RECORD input_buf[INPUT_BUF_SIZE];
        CKeyOrMouseDetails key_or_mouse_details;
        enum TExit {CONT,KEY_EXIT,MOUSE_EXIT,READ_ERROR} exit_loop;


//        if (! SetInputMode() )
//          cout <<"Blad wywolania SetConsoleMode";


        exit_loop = CONT;
        while (exit_loop == CONT) {
           if (! ReadConsoleInput(
                h_std_in,      // input buffer handle
                input_buf,     // buffer to read into
                INPUT_BUF_SIZE,         // size of read buffer
                &num_read) ){
                    cout << "ERROR ReadConsoleINPUT";
                    exit_loop = READ_ERROR;
                    continue;
                  } ;
         for (i=0; i < num_read; i++) {
            switch(input_buf[i].EventType) {
                case KEY_EVENT: // keyboard input
                    if (KeyEventAction(input_buf[i].Event.KeyEvent))
                            exit_loop = KEY_EXIT;
                    break;

                case MOUSE_EVENT: // mouse input
                    if (MouseEventAction(input_buf[i].Event.MouseEvent,return_mouse_move))
                        exit_loop = MOUSE_EXIT;
                    break;

                case WINDOW_BUFFER_SIZE_EVENT: // scrn buf. resizing
                    break;

                case FOCUS_EVENT:  // disregard focus events

                case MENU_EVENT:   // disregard menu events
                    break;

                default:
                    break;

            }
           if (exit_loop != CONT) break;
         }
        }
       key_or_mouse_details.SetInputEvent(input_buf[i]);
       return key_or_mouse_details;
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
            if (idx_x_ == xsize-1)
                screen.PrintString(init_x+3, init_y,"|",frame_f_color_);
            else
                screen.PrintString(init_x+2, init_y," ");
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
            DisplayOneCell( x, y, Plansza[x][y].toChar(debug_),FOREGROUND_GREEN);

    }
    void InputHandler() {
       bool quit_program = false;
       CKeyOrMouseDetails key_or_mouse;

       screen.HideCursor();

    while (!quit_program) {
        key_or_mouse = screen.ReadConInput(true);
        if (key_or_mouse.GetInputType() == CKeyOrMouseDetails::MOUSE) {
            screen.ClearLine(1,22,60);
            screen.PrintString(1,22,"MOUSE:");
            if (key_or_mouse.GetMouseEvent() == CKeyOrMouseDetails::LEFT_CLK)
                screen.PrintString(8,22,"LEFT_CLICK");
            else if (key_or_mouse.GetMouseEvent() == CKeyOrMouseDetails::RIGHT_CLK)
                screen.PrintString(8,22,"RIGHT_CLICK");
            else if (key_or_mouse.GetMouseEvent() == CKeyOrMouseDetails::DOUBLE_CLK)
                screen.PrintString(8,22,"DOUBLE_CLICK");
            else if (key_or_mouse.GetMouseEvent() == CKeyOrMouseDetails::CLK)
                screen.PrintString(8,22,"OTHER_CLICK");
            else if (key_or_mouse.GetMouseEvent() == CKeyOrMouseDetails::MOUSE_MOVE)
                screen.ReverseColor(key_or_mouse.GetMouseXPos(),key_or_mouse.GetMouseYPos());
            screen.PrintIntRJust(20,22,key_or_mouse.GetMouseXPos(),4);
            screen.PrintIntRJust(26,22,key_or_mouse.GetMouseYPos(),4);
        } else {
            screen.ClearLine(1,22,60);
            screen.PrintString(1,22,"KEYB:");
            if (key_or_mouse.GetKeyEvent() == CKeyOrMouseDetails::KEY_PRES)
                screen.PrintString(8,22,"PRESSED");
            else
                screen.PrintString(8,22,"RELEASED");
            screen.PrintChar(30,22,key_or_mouse.GetPressedKey());
            if (key_or_mouse.GetPressedKey() == 'q')
                quit_program = true;
        };

    };
    screen.ShowCursor();
  }
}; //Class



int main()
{
  CPlansza Plansza(10,10);
  CConsoleScreen console_scr;

    console_scr.Cls();

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
  Plansza.ChangeCellFrameColor(5 , 5, FOREGROUND_RED,B_BLACK);
   console_scr.SetCursor(1, 28);
   Plansza.InputHandler();
//
    return 0;
}
