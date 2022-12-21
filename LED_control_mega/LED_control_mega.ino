// testshapes demo for RGBmatrixPanel library.
// Demonstrates the drawing abilities of the RGBmatrixPanel library.
// For 32x64 RGB LED matrix.

// WILL NOT FIT on ARDUINO UNO -- requires a Mega, M0 or M4 board

#include <RGBmatrixPanel.h>

// Most of the signal pins are configurable, but the CLK pin has some
// special constraints.  On 8-bit AVR boards it must be on PORTB...
// Pin 11 works on the Arduino Mega.  On 32-bit SAMD boards it must be
// on the same PORT as the RGB data pins (D2-D7)...
// Pin 8 works on the Adafruit Metro M0 or Arduino Zero,
// Pin A4 works on the Adafruit Metro M4 (if using the Adafruit RGB
// Matrix Shield, cut trace between CLK pads and run a wire to A4).

#define CLK  11   // USE THIS ON ADAFRUIT METRO M0, etc.
//#define CLK A4 // USE THIS ON METRO M4 (not M0)
//#define CLK 11 // USE THIS ON ARDUINO MEGA
#define OE   9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3

// variable to store the value coming from the sensor
//value1,2 -> mode, 3-8 -> distance, 9-12 -> gesture 
int sensorValue1 = 0;  
int sensorValue2 = 0;
int sensorValue3 = 0;
int sensorValue4 = 0;
int sensorValue5 = 0;
int sensorValue6 = 0;
int sensorValue7 = 0;
int sensorValue8 = 0;
int sensorValue9 = 0;
int sensorValue10 = 0;
int sensorValue11 = 0;
int sensorValue12 = 0;
int SIGN_MOVE_UPDOWN = 0;
int SIGN_MOVE_LEFTRIGHT = 0;
int pre_x = 31;
int pre_y = 15;
int current_x = 0;
int current_y = 0;
int dx = 0;
int dy = 0;
int pre_mode = 0;
int step_num = 0;
int delta_x = 0;
int delta_y = 0;
int flag = 0;
int flag1 = 0;
  
RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);

void setup() {
  pinMode(42, INPUT);
  pinMode(41, INPUT);
  pinMode(40, INPUT);
  pinMode(43, INPUT);
  pinMode(44, INPUT);
  pinMode(45, INPUT);
  pinMode(46, INPUT);
  pinMode(47, INPUT);
  pinMode(48, INPUT);
  pinMode(49, INPUT);
  pinMode(50, INPUT);
  pinMode(51, INPUT);
  pinMode(52, INPUT);
  pinMode(36, INPUT);
  Serial.begin(9600);
  matrix.begin();
  // code below is for initialization
  // draw a pixel in solid white
  matrix.drawPixel(0, 0, matrix.Color333(7, 7, 7));
  delay(500);

  // fix the screen with green
  matrix.fillRect(0, 0, matrix.width(), matrix.height(), matrix.Color333(0, 7, 0));
  delay(500);

  // draw a box in yellow
  matrix.drawRect(0, 0, matrix.width(), matrix.height(), matrix.Color333(7, 7, 0));
  delay(500);

  // draw an 'X' in red
  matrix.drawLine(0, 0, matrix.width()-1, matrix.height()-1, matrix.Color333(7, 0, 0));
  matrix.drawLine(matrix.width()-1, 0, 0, matrix.height()-1, matrix.Color333(7, 0, 0));
  delay(500);

  // draw a blue circle
  matrix.drawCircle(10, 10, 10, matrix.Color333(0, 0, 7));
  delay(500);

  // fill a violet circle
  matrix.fillCircle(40, 21, 10, matrix.Color333(7, 0, 7));
  delay(500);

  // fill the screen with 'black'
  matrix.fillScreen(matrix.Color333(0, 0, 0));

  // draw some text!
  matrix.setTextSize(1);     // size 1 == 8 pixels high
  matrix.setTextWrap(false); // Don't wrap at end of line - will do ourselves

  matrix.setCursor(8, 0);    // start at top left, with 8 pixel of spacing
  uint8_t w = 0;
  char *str = "Stanford Sucks";
  for (w=0; w<8; w++) {
    matrix.setTextColor(matrix.Color333(4, 4, 4));
    //matrix.setTextColor(Wheel(w));
    matrix.print(str[w]);
  }
  matrix.setCursor(2, 8);    // next line
  for (w=8; w<18; w++) {
    matrix.setTextColor(Wheel(w));
    matrix.print(str[w]);
  }
  matrix.println();
  //matrix.setTextColor(matrix.Color333(4,4,4));
  //matrix.println("zhouwenkai");
  matrix.setTextColor(matrix.Color333(7,7,7));
  matrix.println("LED MATRIX!");

  // print each letter with a rainbow color
  matrix.setTextColor(matrix.Color333(7,0,0));
  matrix.print('3');
  matrix.setTextColor(matrix.Color333(7,4,0));
  matrix.print('2');
  matrix.setTextColor(matrix.Color333(7,7,0));
  matrix.print('x');
  matrix.setTextColor(matrix.Color333(4,7,0));
  matrix.print('6');
  matrix.setTextColor(matrix.Color333(0,7,0));
  matrix.print('4');
  matrix.setCursor(34, 24);
  matrix.setTextColor(matrix.Color333(0,7,7));
  matrix.print('*');
  matrix.setTextColor(matrix.Color333(0,4,7));
  matrix.print('R');
  matrix.setTextColor(matrix.Color333(0,0,7));
  matrix.print('G');
  matrix.setTextColor(matrix.Color333(4,0,7));
  matrix.print('B');
  matrix.setTextColor(matrix.Color333(7,0,4));
  matrix.print('*');

  // whew!
}

void loop() {
  sensorValue1 = digitalRead(40);
  sensorValue2 = digitalRead(41);
  sensorValue3 = digitalRead(42);
  sensorValue4 = digitalRead(43);
  sensorValue5 = digitalRead(44);
  sensorValue6 = digitalRead(45);
  sensorValue7 = digitalRead(46);
  sensorValue8 = digitalRead(47);
  sensorValue9 = digitalRead(48);
  sensorValue10 = digitalRead(49);
  sensorValue11 = digitalRead(50);
  sensorValue12 = digitalRead(51);
  SIGN_MOVE_UPDOWN = digitalRead(52);
  SIGN_MOVE_LEFTRIGHT = digitalRead(36);
  int mode = 2*sensorValue1 + sensorValue2;
  // translate the encoding digital signal into real number
  int mode_gesture = 8*sensorValue9 + 4*sensorValue10 + 2*sensorValue11 + sensorValue12;
  int mode_updown = 4*sensorValue3 + 2*sensorValue4 + sensorValue5;
  int mode_leftright = 4*sensorValue6 + 2*sensorValue7 + sensorValue8;
  
  Serial.print(mode);
  Serial.print(" ");
  Serial.print(mode_gesture);
  Serial.print(" ");
  Serial.print(SIGN_MOVE_UPDOWN);
  Serial.print(" ");
  Serial.print(SIGN_MOVE_LEFTRIGHT);
  Serial.print(" ");
  Serial.print(mode_updown);
  Serial.print(" ");
  Serial.println(mode_leftright);
  int sign_out = 0;
  //mode1 curser
  //sensorvalue3-5 updown, sensorvalue6-8 leftright
  if(mode == 1){   
    if(pre_mode != 1){
      matrix.fillScreen(matrix.Color333(0, 0, 0));
      matrix.setCursor(20, 0);
        char * str2 = "Mode1";
        for (int w=0; w<5; w++) {
          matrix.setTextColor(matrix.Color333(4, 4, 4));
          matrix.print(str2[w]);
          delay(100);
        }
        delay(400);
        matrix.fillScreen(matrix.Color333(0, 0, 0));
      }
//    int mode_updown = 4*sensorValue3 + 2*sensorValue4 + sensorValue5;
//    int mode_leftright = 4*sensorValue6 + 2*sensorValue7 + sensorValue8;
    //matrix.drawPixel(pre_x, pre_y, matrix.Color333(7, 7, 7));
    matrix.fillCircle(pre_x, pre_y, 1, matrix.Color333(7, 7, 7));
    if(SIGN_MOVE_UPDOWN == 0 && SIGN_MOVE_LEFTRIGHT == 0){
      dx = 0;
      dy = 0;
      step_num = 0;
      }
    else if(SIGN_MOVE_UPDOWN == 0 && SIGN_MOVE_LEFTRIGHT == 1){
      dy = 0;
      if (mode_leftright <= 3){
        // situation 0,1,2,3
        delta_x = -(mode_leftright+1)*2;
        current_x = pre_x + delta_x;
        // every step just move one
        dx = -1;
        step_num = delta_x  / dx;
      }
      else{
        // situation 4,5,6,7
        // get it to -7,-6,-5,-4
        // then -4,-3,-2,-1
        delta_x = -(-(mode_leftright) + 3) * 2;
        current_x = pre_x + delta_x;
        dx = 1;
        step_num = delta_x  / dx;
      }
    }
    else if(SIGN_MOVE_UPDOWN == 1 && SIGN_MOVE_LEFTRIGHT == 0){
      dx = 0;
      if (mode_updown <= 3){
        // situation 0,1,2,3
        delta_y = (mode_updown+1)*2;
        current_y = pre_y + delta_y;
        // every step just move one
        dy = 1;
        step_num = delta_y  / dy;
      }
      else{
        // situation 4,5,6,7
        // get it to -7,-6,-5,-4
        // then -4,-3,-2,-1
        delta_y = (-(mode_updown) + 3) * 2;
        current_y = pre_y + delta_y;
        dy = -1;
        step_num = delta_y  / dy;
      }
    }

    
    else {
      // then, we need to consider complicated move
      if(mode_leftright <= 3){
        // about situation 0,1,2,3
        // first, get the classfication of different left&right
        delta_x = -(mode_leftright+1)*2;
        if(mode_updown <= 3){
          // case 0,1,2,3
          delta_y = (mode_updown+1)*2;       
        }
        else{
          // case 4,5,6,7
          delta_y = (-(mode_updown) + 3) * 2;
        }
      }
      else{
        delta_x = -(-(mode_leftright) + 3) * 2;
        if(mode_updown <= 3){
          delta_y = (mode_updown+1)*2;       
        }
        else{
          delta_y = (-(mode_updown) + 3) * 2;
        }
      }
      current_x = pre_x + delta_y;
      current_y = pre_y + delta_y;
      step_num = 2;
      dx = delta_x / step_num;
      dy = delta_y / step_num;
    }

    for(int i = 0; i < step_num; i++){
      //matrix.drawPixel(pre_x, pre_y, matrix.Color333(0, 0, 0));
      matrix.fillCircle(pre_x, pre_y, 1, matrix.Color333(0, 0, 0));
      current_x = pre_x + (i+1)*dx;
      current_y = pre_y + (i+1)*dy;
      if(current_x > matrix.width()){
        current_x =  matrix.width();  
        sign_out = 1;
        }             
      if(current_x <0){
        current_x = 0;
        sign_out = 1; 
        }
      if(current_y >matrix.height()){
        current_y = matrix.height();
        sign_out = 1; 
        }
      if(current_y <0){
        current_y = 0;
        sign_out = 1; 
        }
      if(sign_out == 1){
        //matrix.drawPixel(current_x, current_y, matrix.Color333(7, 7, 7));
        matrix.fillCircle(current_x, current_y, 1, matrix.Color333(7, 7, 7));
        pre_x = current_x;
        pre_y = current_y;
        break;
        }
      else{
        //matrix.drawPixel(current_x, current_y, matrix.Color333(7, 7, 7));
        matrix.fillCircle(current_x, current_y, 1, matrix.Color333(7, 7, 7));
        pre_x = current_x;
        pre_y = current_y;
        }
      //delay(50);
      }
    }
   //mode2 writing
   //sensorvalue3-5 updown, sensorvalue6-8 leftright
   //sensorvalue9-12 gesture
  else if(mode == 2){
    if(pre_mode != 2){
      matrix.fillScreen(matrix.Color333(0, 0, 0));
      matrix.setCursor(20, 0);
        char * str2 = "Mode2";
        for (int w=0; w<5; w++) {
          matrix.setTextColor(matrix.Color333(4, 4, 4));
          matrix.print(str2[w]);
          delay(100);
        }
        delay(400);
        matrix.fillScreen(matrix.Color333(0, 0, 0));
      }
//    int mode_updown = 4*sensorValue3 + 2*sensorValue4 + sensorValue5;
//    int mode_leftright = 4*sensorValue6 + 2*sensorValue7 + sensorValue8;
//    int mode_gesture = 4*sensorValue9 + 2*sensorValue10 + sensorValue11;
    matrix.drawPixel(pre_x, pre_y, matrix.Color333(7, 7, 7));
    if(SIGN_MOVE_UPDOWN == 0 && SIGN_MOVE_LEFTRIGHT == 0){
      current_x = pre_x;
      current_y = pre_y;
      }
    else if(SIGN_MOVE_UPDOWN == 1 && SIGN_MOVE_LEFTRIGHT == 0){
      current_x = pre_x;
      if (mode_updown <= 3){
        // situation 0,1,2,3
        delta_y = (mode_updown+1)*2;
        current_y = pre_y + delta_y;
      }
      else{
        // situation 4,5,6,7
        // get it to -7,-6,-5,-4
        // then -4,-3,-2,-1
        delta_y = (-(mode_updown) + 3) * 2;
        current_y = pre_y + delta_y;
      }

    }
    else if(SIGN_MOVE_UPDOWN == 0 && SIGN_MOVE_LEFTRIGHT == 1){
      current_y = pre_y;
      if (mode_leftright <= 3){
        // situation 0,1,2,3
        delta_x = -(mode_leftright+1)*2;
        current_x = pre_x + delta_x;
      }
      else{
        // situation 4,5,6,7
        // get it to -7,-6,-5,-4
        // then -4,-3,-2,-1
        delta_x = -(-(mode_leftright) + 3) * 2;
        current_x = pre_x + delta_x;
      }
    }
    else if(SIGN_MOVE_UPDOWN == 1 && SIGN_MOVE_LEFTRIGHT == 1){
      if (mode_leftright <= 3){
        delta_x = -(mode_leftright+1)*2;
      }
      else if(mode_leftright > 3){
        delta_x = -(-(mode_leftright) + 3) * 2;
      }
      if (mode_updown <= 3){
        delta_y = (mode_updown+1)*2;
      }
      else if (mode_updown > 3){
        delta_y = (-(mode_updown) + 3) * 2;
      }
      current_x = pre_x + delta_x;
      current_y = pre_y + delta_y;
    }
    if(current_x > matrix.width()){
        current_x =  matrix.width()-1;  
        sign_out = 1;
      }             
    if(current_x <0){
        current_x = 0;
        sign_out = 1; 
      }
    if(current_y >matrix.height()){
        current_y = matrix.height()-1;
        sign_out = 1; 
      }
    if(current_y <0){
        current_y = 0;
        sign_out = 1; 
      }  
    
    if(sensorValue9 == 0 && sensorValue11 == 0){    
      matrix.drawCircle(pre_x, pre_y, 4, matrix.Color333(0, 0, 0));
      matrix.fillCircle(pre_x, pre_y, 4, matrix.Color333(0, 0, 0));
      matrix.drawCircle(current_x, current_y, 4, matrix.Color333(0, 0, 7));
      pre_x = current_x;
      pre_y = current_y;   
      flag1 = 1;
      
     }
     else{
      if(flag1 == 1){
        matrix.drawCircle(pre_x, pre_y, 4, matrix.Color333(0, 0, 0));
        matrix.fillCircle(pre_x, pre_y, 4, matrix.Color333(0, 0, 0)); 
        flag1 = 0;     
        }
      matrix.drawLine(pre_x, pre_y, current_x, current_y, matrix.Color333(7, 7, 7));     
      pre_x = current_x;
      pre_y = current_y;
     } 
  }
  //sensorvalue9-12 gesture
  else if(mode == 3){
      matrix.fillScreen(matrix.Color333(0, 0, 0));     
      matrix.setTextSize(1);     // size 1 == 8 pixels high
      matrix.setTextWrap(false); // Don't wrap at end of line - will do ourselves   
      if(pre_mode != 3){
        matrix.fillScreen(matrix.Color333(0, 0, 0));
        matrix.setCursor(20, 0);
        char * str2 = "Mode3";
        for (int w=0; w<5; w++) {
          matrix.setTextColor(matrix.Color333(4, 4, 4));
          matrix.print(str2[w]);
          delay(100);
        }
        delay(400);
        matrix.fillScreen(matrix.Color333(0, 0, 0));
      }  
      uint8_t w = 0;
      if(sensorValue9 == 1 && sensorValue11 ==0 ){
        matrix.setCursor(8, 0);    // start at top left, with 8 pixel of spacing
        char *str = "Stanford Sucks";
        for (w=0; w<8; w++) {
          matrix.setTextColor(matrix.Color333(4, 0, 0));
          //matrix.setTextColor(Wheel(w));
          matrix.print(str[w]);
        }
        matrix.setCursor(12, 8);    // next line
        for (w=8; w<14; w++) {
          matrix.setTextColor(matrix.Color333(4, 0, 0));
          matrix.print(str[w]);
        }
        matrix.println();
      }   
      else if(sensorValue9 == 0 && sensorValue10 == 1 && sensorValue11 == 1){
        matrix.setCursor(8, 0);    // start at top left, with 8 pixel of spacing
        char *str = "Let's Go Bears";
        for (w=0; w<5; w++) {
          matrix.setTextColor(matrix.Color333(4, 4, 4));
          //matrix.setTextColor(Wheel(w));
          matrix.print(str[w]);
        }
        matrix.setCursor(20, 8);    // next line
        for (w=6; w<8; w++) {
          matrix.setTextColor(Wheel(w));
          matrix.print(str[w]);
        }
        matrix.setCursor(16, 16);    // next line
        for (w=9; w<14; w++) {
          matrix.setTextColor(Wheel(w));
          matrix.print(str[w]);
        }
        matrix.println();       
     }
  }
  //matrix.fillRect(0, 0, matrix.width(), matrix.height(), matrix.Color333(0, 0, 0));
  // Do nothing -- image doesn't change
  pre_mode = mode;
  delay(350);
}


// Input a value 0 to 24 to get a color value.
// The colours are a transition r - g - b - back to r.
uint16_t Wheel(byte WheelPos) {
  if(WheelPos < 8) {
   return matrix.Color333(7 - WheelPos, WheelPos, 0);
  } else if(WheelPos < 16) {
   WheelPos -= 8;
   return matrix.Color333(0, 7-WheelPos, WheelPos);
  } else {
   WheelPos -= 16;
   return matrix.Color333(WheelPos, 0, 7 - WheelPos);
  }
}
