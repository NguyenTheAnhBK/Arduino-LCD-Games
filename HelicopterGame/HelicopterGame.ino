#include<LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

byte spriteNull[8] = {
  0x00, //. . . . . 
  0x00, //. . . . . 
  0x00, //. . . . . 
  0x00, //. . . . . 
  0x00, //. . . . . 
  0x00, //. . . . . 
  0x00, //. . . . .
  0x00, //. . . . . 
};
byte spriteTail0[8] = {
  0x00, //. . . . . 
  0x00, //. . . . . 
  0x11, //o . . . o 
  0x0f, //. o o o o 
  0x05, //. . o . o 
  0x00, //. . . . . 
  0x01, //. . . . o 
  0x00, //. . . . . 
};
byte spriteTail1[8] = {
  0x00, //. . . . . 
  0x00, //. . . . . 
  0x05, //. . o . o 
  0x0f, //. o o o o 
  0x11, //o . . . o 
  0x00, //. . . . . 
  0x01, //. . . . o 
  0x00, //. . . . . 
};
byte spriteBody0[8] = {
  0x04, //. . o . . 
  0x04, //. . o . . 
  0x1e, //o o o o . 
  0x15, //o . o . o 
  0x1e, //o o o o . 
  0x10, //o . . . . 
  0x1f, //o o o o o 
  0x00, //. . . . . 
};
byte spriteBody1[8] = {
  0x1f, //o o o o o 
  0x04, //. . o . . 
  0x1e, //o o o o . 
  0x15, //o . o . o 
  0x1e, //o o o o . 
  0x10, //o . . . . 
  0x1f, //o o o o o 
  0x00, //. . . . . 
};
byte spriteWallT[8] = {
  0x00, //. . . . . 
  0x04, //. . o . . 
  0x09, //. o . . o 
  0x1e, //o o o o . 
  0x09, //. o . . o 
  0x04, //. . o . . 
  0x00, //. . . . . 
  0x00, //. . . . . 
};
byte spriteWallB[8] = {
  0x00, //. . . . . 
  0x04, //. . o . . 
  0x04, //. . o . . 
  0x0a, //. o . o . 
  0x0e, //. o o o . 
  0x0a, //. o . o . 
  0x0e, //. o o o . 
  0x0a, //. o . o . 
};
byte spriteExplosive[8] = {
  0x04, //. . o . . 
  0x14, //o . o . . 
  0x0d, //. o o . o 
  0x0a, //. o . o . 
  0x0a, //. o . o . 
  0x16, //o . o o . 
  0x05, //. . o . o 
  0x04, //. . o . . 
};

#define btnUp 0
#define btnDown 1

int key = 0;
int animTail[] = {1, 2};
int animBody[] = {3, 4};

int mat[2][24] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};
int fps = 0;
int stateHelicopter = 0;
int distance2Obs = 1;
bool gameStarted = false, isGameover = false;
int score = 0, highScore = 0;

void setup() {
  Serial.begin(9600);

  lcd.createChar(0, spriteNull);
  lcd.createChar(1, spriteTail0);
  lcd.createChar(2, spriteTail1);
  lcd.createChar(3, spriteBody0);
  lcd.createChar(4, spriteBody1);
  lcd.createChar(5, spriteWallT);
  lcd.createChar(6, spriteWallB);
  lcd.createChar(7, spriteExplosive);
  lcd.begin(16, 2);
  lcd.clear();
}

void loop() {
  if(isGameover)
    gameover();
    
  key = getKey();

  if(!gameStarted){
    clearMat();
    if(!key){
      gameStarted = true;
      score = 0;
    }
  }
  else{
    if(fps % 2)
      stateHelicopter = 1 - stateHelicopter;
    if(fps % 4){
      distance2Obs--;
      for(int i = 0; i < 2; i++)
        for(int j = 0; j < 23; j++)
          mat[i][j] = mat[i][j+1];
  
      mat[0][23] = 0;
      mat[1][23] = 0;
    }
    if(!distance2Obs)
      generateObstacles();

    //Collision
    if(!key && mat[0][2] == 5)
      isGameover = true;
    if(key && mat[1][2] == 6)
      isGameover = true;
    if(mat[0][0] || mat[1][0])
      score++;
    
    fps++;
  }
  
  redraw();
  //draw helicopter
  lcd.setCursor(1, key);
  lcd.write(animTail[stateHelicopter]);
  lcd.write(animBody[stateHelicopter]);
  
  delay(200);
}

void generateObstacles(){
  int types[2][2] = {{5, 0}, {0, 6}};
  int type = random(0, 2);
  int obsCount = random(1, 4); //số chướng ngại vật liên tiếp
  for(int i = 0; i < obsCount; i++){
    mat[0][16 + i] = types[type][0];
    mat[1][16 + i] = types[type][1];
  }
  int spaceCount = random(2, 5); //khoảng cách giữa 2 chướng ngại vật
  distance2Obs = spaceCount + obsCount;
}

void gameover(){
  if(score > highScore)
    highScore = score;
  lcd.setCursor(1, key);
  lcd.write(7);
  lcd.write(7);
  gameStarted = false;
  isGameover = false;
  delay(1000);
}

void clearMat(){
  for(int i = 0; i < 2; i++)
    for(int j = 0; j < 24; j++)
      mat[i][j] = 0;
}

void redraw(){
  for(int i = 0; i < 2; i++){
    for(int j = 23; j >= 0; j--){
      lcd.setCursor(j, i); //col, row
      lcd.write(mat[i][j]);
    }
  }

  if(!gameStarted){
    lcd.setCursor(3, 0);
    lcd.print("Helicopter");
    String s = "S:" + String(score);
    String hS = "HS:" + String(highScore);
    lcd.setCursor(2, 1);
    lcd.print(s);
    lcd.setCursor(8, 1);
    lcd.print(hS);
  }
}

int getKey(){
  int r = analogRead(A0);
  Serial.println(r);
  if (r < 135) return btnUp;
  //if (r < 310) return btnDown;
  return btnDown;
}
