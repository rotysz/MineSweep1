#include <stdio.h>
#include <string>
#include<iostream>

using namespace std;

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#define PLANSZA_MAX_X 20
#define PLANSZA_MAX_Y 20
typedef int TPlansza[PLANSZA_MAX_X][PLANSZA_MAX_Y];

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
};


class CPlansza {
    private:
      TPlansza Plansza;
      int xsize;
      int ysize;
    
    public:
    static const int BOMB_PRESENT = -9;
    static const int BOMB_NO_PRESENT = 0;
    static const int UNCOVER_OFFSET =10;
    
    CPlansza () {
        xsize=10;
        ysize=10;
    };
     
    CPlansza (int xsize_, int ysize_) {
        xsize = xsize_;
        ysize = ysize_;
    }
    
    void DspPlansza () {
      printf("-->\n");
       for (int i = 0; i <ysize;i++){
          for (int j = 0; j<xsize; j++)    
             printf ("%2i ",Plansza[j][i]);
          printf("\n");
        }  
    }
    
    int PutBomb(int x_, int y_) {
       if ( x_>=xsize ) 
          return -1;
       if (y_>=ysize) 
          return -1;
       Plansza[x_][y_] = BOMB_PRESENT;
       return 0;
    }
    
    int PutNoBomb(int x_, int y_) {
       if ( x_>=xsize ) 
          return -1;
       if (y_>=ysize) 
          return -1;
       Plansza[x_][y_] = BOMB_NO_PRESENT;
       return 0;
    }
    
    void CalcOneMine (int x_, int y_) {
        
      if (Plansza[x_][y_] == BOMB_PRESENT) return;
      if(Plansza[x_][y_] != BOMB_NO_PRESENT) return ;
         for (int i = max(0,x_-1); i<min(xsize,x_+2);i++)
            for (int j = max(0,y_-1); j<min(ysize,y_+2);j++)
               if(Plansza[i][j] == BOMB_PRESENT) 
                 Plansza[x_][y_]++;
           
              
    }

    void CalcAllMines () {
          
       for (int i = 0; i <xsize;i++)
         for (int j = 0; j<ysize; j++)
           CalcOneMine(i,j);
    }
    
    
    void Unhide( int x_, int y_) {
  
      if ((Plansza[x_][y_] == BOMB_PRESENT) || (Plansza[x_][y_] == BOMB_NO_PRESENT+UNCOVER_OFFSET) ) return;
        if ((Plansza[x_][y_] >BOMB_NO_PRESENT) && (Plansza[x_][y_]) <UNCOVER_OFFSET ) {
           Plansza[x_][y_] += UNCOVER_OFFSET;
           return;
        };
        
       if (Plansza[x_][y_]==BOMB_NO_PRESENT) {
          Plansza[x_][y_]=BOMB_NO_PRESENT+UNCOVER_OFFSET;
          for (int i = max(0,x_-1); i<min(xsize,x_+2);i++)
             for (int j = max(0,y_-1); j<min(ysize,y_+2);j++)
                Unhide(i,j);
        }
    }
    
    void GenerateTest () {
       for (int i = 0; i <xsize;i++)
         for (int j = 0; j<ysize; j++)
           if (i == j) Plansza[i][j] = BOMB_PRESENT;
           else Plansza[i][j]=BOMB_NO_PRESENT;
    }

    
    
}; //Class

class CPlansza2 {
    private:
      CPolePlanszy Plansza[PLANSZA_MAX_X][PLANSZA_MAX_Y];
      int xsize;
      int ysize;
    
    public:
   
    CPlansza2 () {
        xsize=10;
        ysize=10;
    };
     
    CPlansza2 (int xsize_, int ysize_) {
        xsize = xsize_;
        ysize = ysize_;
    }
    
    void DspPlansza () {
      printf("-->\n");
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
        
      if (Plansza[x_][y_].isMinned()) return;
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
  
      if (Plansza[x_][y_].isMinned() || (Plansza[x_][y_].isVisible() && !Plansza[x_][y_].isMinned()) ) return;
        if (Plansza[x_][y_].getNearMines() > 0) {
           Plansza[x_][y_].SetVisible();
           return;
        };
        
       if (!Plansza[x_][y_].isMinned()) {
          Plansza[x_][y_].SetVisible();
          for (int i = max(0,x_-1); i<min(xsize,x_+2);i++)
             for (int j = max(0,y_-1); j<min(ysize,y_+2);j++)
                Unhide(i,j);
        }
    }
    
    void GenerateTest () {
       for (int i = 0; i <xsize;i++)
         for (int j = 0; j<ysize; j++)
           if (i == j) Plansza[i][j].PutMine();
           else Plansza[i][j].RemoveMine();
    }

    
    
}; //Class
 


int main()
{
  CPlansza2 Plansza(20,20);
  
  Plansza.GenerateTest();
  Plansza.PutMine(4,5);
  Plansza.PutMine(4,7);
  Plansza.DspPlansza();
  Plansza.CalcAllMines();
  Plansza.DspPlansza();
  Plansza.Unhide(1,8);
  Plansza.DspPlansza();
  
    return 0;
}
