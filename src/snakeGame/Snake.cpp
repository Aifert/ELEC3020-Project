// #include <TFT_eSPI.h>
// #include "../main.h"
// #include "back.h"
// #include "gameOver.h"
// #include "main.h"
// #include "newGame.h"
// #include "Snake.h"

// int size=1;
// int y[120]={0};
// int x[120]={0};

// unsigned long currentTime=0;
// int period=200;
// int deb,deb2=0;
// int dirX=1;
// int dirY=0;
// bool taken=0;
// unsigned short colors[2]={0x48ED,0x590F}; //terain colors
// unsigned short snakeColor[2]={0x9FD3,0x38C9};
// bool chosen=0;
// bool gOver=0;
// int moves=0;
// int playTime=0;
// int foodX=0;
// int foodY=0;
// int howHard=0;
// String diff[3]={"EASY","NORMAL","HARD"};
// bool ready=1;
// long readyTime=0;

// TFT_eSprite sprite(&tft);

// // Add these variables at the top of the file, after other global variables
// bool lastBigButtonState = true;
// bool currentBigButtonState = true;

// void getFood()//.....................getFood -get new position of food
// {
//     foodX=random(0,17);
//     foodY=random(0,17);
//     taken=0;
//     for(int i=0;i<size;i++)
//     if(foodX==x[i] && foodY==y[i])
//     taken=1;
//     if(taken==1)
//     getFood();
// }


// int change=0;

// void checkGameOver()//..,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,check game over
// {
// if(x[0]<0 || x[0]>=17 || y[0]<0 || y[0]>=17 )
// gOver=true;
// for(int i=1;i<size;i++)
// if(x[i]==x[0] && y[i]==y[0])
// gOver=true;
// }



// void run()//...............................run function
// {

//       for(int i=size;i>0;i--)
//        {
//       x[i]=x[i-1];
//       y[i]=y[i-1];
//        }

//      x[0]=x[0]+dirX;
//      y[0]=y[0]+dirY;

//     if(x[0]==foodX && y[0]==foodY)
//              {size++; getFood(); tft.drawString(String(size),44,250); period=period-1; tft.drawString(String(500-period),124,250);}

//      sprite.fillSprite(TFT_BLACK);
//      /*
//     for(int i=0;i<17;i++)
//       for(int j=0;j<17;j++)
//         {
//         sprite.fillRect(j*10,i*10,10,10,colors[chosen]);
//         chosen=!chosen;
//         }
//      chosen=0;*/
//      checkGameOver();
//       if(gOver==0){
//      sprite.drawRect(0,0,170,170,0x02F3);
//      for(int i=0;i<size;i++){
//      sprite.fillRoundRect(x[i]*10,y[i]*10,10,10,2,snakeColor[0]);
//      sprite.fillRoundRect(2+x[i]*10,2+y[i]*10,6,6,2,snakeColor[1]);
//      }
//      sprite.fillRoundRect(foodX*10+1,foodY*10+1,8,8,1,TFT_RED);
//      sprite.fillRoundRect(foodX*10+3,foodY*10+3,4,4,1,0xFE18);
//     }
// else

// {sprite.pushImage(0,0,170,170,gameOver);}
//  sprite.pushSprite(0,30);
// }

// void runSnakeGame() {
//     // Handle snake movement based on the period
//     if(millis() > currentTime + period) {
//         run();
//         currentTime = millis();
//     }

//     // Update button state
//     lastBigButtonState = currentBigButtonState;
//     currentBigButtonState = controller2.big;

//     // Check for button press (transition from HIGH to LOW)
//     if (currentBigButtonState == 0 && lastBigButtonState == 1) {
//         if (ready == 1) {
//             // Handle direction change
//             if (dirX == 1) {
//                 dirY = -1;
//                 dirX = 0;
//             } else if (dirX == -1) {
//                 dirY = 1;
//                 dirX = 0;
//             } else if (dirY == 1) {
//                 dirX = 1;
//                 dirY = 0;
//             } else if (dirY == -1) {
//                 dirX = -1;
//                 dirY = 0;
//             }

//             // Prevent immediate consecutive changes
//             ready = 0;
//             readyTime = millis();
//         }
//     }

//     // Reset readiness after a short delay to prevent rapid direction changes
//     if (millis() > readyTime + 100 && ready == 0) {
//         ready = 1;
//     }

//     // Rest of your existing code...
// }

// void setupSnakeGame() {  //.......................setup
//     tft.init();
//     tft.fillScreen(TFT_BLACK);
//     tft.setRotation(0);
//     tft.setSwapBytes(true);
//     tft.pushImage(0,0,170,320,back);
//     tft.pushImage(0,30,170,170,newGame);

//     size = 1;
//     period = 200;
//     howHard = 0;
//     gOver = false;

//     tft.setTextColor(TFT_PURPLE,0x7DFD);
//     tft.fillSmoothCircle(28,102+(howHard*24),5,TFT_RED,TFT_BLACK);

//     sprite.createSprite(170,170);
//     sprite.setSwapBytes(true);
//     while(controller2.big == 1)
//     {
//         if(controller2.b == 0)
//         {
//             if(deb2 == 0)
//             {
//                 deb2 = 1;
//                 tft.fillCircle(28, 102 + (howHard * 24), 6, TFT_BLACK);
//                 howHard++;   if(howHard == 3) howHard = 0;
//                 tft.fillSmoothCircle(28, 102 + (howHard * 24), 5, TFT_RED, TFT_BLACK);
//                 period = 200 - howHard * 20;
//                 delay(200);
//             }
//         }
//         else deb2 = 0;
//         delay(100);
//     }

//     y[0] = random(5, 15);
//     getFood();
//     tft.setTextSize(3);
//     tft.setTextDatum(4);
//     tft.drawString(String(size), 44, 250);
//     tft.drawString(String(500 - period), 124, 250);
//     delay(400);
//     dirX = 1;
//     dirY = 0;

//     while(true){
//         runSnakeGame();

//         if (gOver == 1) {
//             // Reset game variables
//             size = 1;
//             x[0] = 0;
//             y[0] = random(5, 15);
//             dirX = 1;
//             dirY = 0;
//             gOver = 0;

//             delay(3500);
//             break;
//         }
//     }
// }
