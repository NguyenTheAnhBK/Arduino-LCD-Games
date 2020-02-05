#include<LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define btnRight 0
#define btnUp 1
#define btnDown 2
#define btnLeft 3
#define btnSelect 4
#define btnNone 5

byte dino[8] = {
  0x02, //. . . o . 
  0x07, //. . o o o 
  0x07, //. . o o o 
  0x06, //. . o o . 
  0x07, //. . o o o 
  0x16, //o . o o . 
  0x1e, //o o o o . 
  0x0a, //. o . o . 
};
byte dino1[8] = {
  0x02, //. . . o . 
  0x07, //. . o o o 
  0x07, //. . o o o 
  0x06, //. . o o . 
  0x07, //. . o o o 
  0x16, //o . o o . 
  0x1e, //o o o o . 
  0x08, //. o . . . 
};
byte dino2[8] = {
  0x02, //. . . o . 
  0x07, //. . o o o 
  0x07, //. . o o o 
  0x06, //. . o o . 
  0x07, //. . o o o 
  0x16, //o . o o . 
  0x1e, //o o o o . 
  0x02, //. . . o . 
};
byte dinosit[8] = {
  0x00, //. . . . . 
  0x00, //. . . . . 
  0x02, //. . . o . 
  0x07, //. . o o o 
  0x07, //. . o o o 
  0x16, //o . o o . 
  0x1e, //o o o o . 
  0x0a, //. o . o . 
};
byte dinosit1[8] = {
  0x00, //. . . . . 
  0x00, //. . . . . 
  0x02, //. . . o . 
  0x07, //. . o o o 
  0x07, //. . o o o 
  0x16, //o . o o . 
  0x1e, //o o o o . 
  0x08, //. o . . . 
};
byte dinosit2[8] = {
  0x00, //. . . . . 
  0x00, //. . . . . 
  0x02, //. . . o . 
  0x07, //. . o o o 
  0x07, //. . o o o 
  0x16, //o . o o . 
  0x1e, //o o o o . 
  0x02, //. . . o . 
};
byte obs1[8] = {
  0x00, //. . . . . 
  0x00, //. . . . . 
  0x10, //o . . . . 
  0x1d, //o o o . o 
  0x1a, //o o . o . 
  0x0a, //. o . o . 
  0x0e, //. o o o . 
  0x0c, //. o o . . 
};
byte obs2[8] = {
  0x04, //. . o . . 
  0x04, //. . o . . 
  0x0e, //. o o o . 
  0x1f, //o o o o o 
  0x1f, //o o o o o 
  0x0e, //. o o o . 
  0x04, //. . o . . 
  0x04, //. . o . . 
};
byte obs3[8] = {
  0x1f, //o o o o o 
  0x0e, //. o o o . 
  0x04, //. . o . . 
  0x00, //. . . . . 
  0x00, //. . . . . 
  0x00, //. . . . . 
  0x00, //. . . . . 
  0x00, //. . . . . 
};
byte obs4[8] = {
  0x04, //. . o . . 
  0x15, //o . o . o 
  0x15, //o . o . o 
  0x1f, //o o o o o 
  0x15, //o . o . o 
  0x15, //o . o . o 
  0x1f, //o o o o o 
  0x04, //. . o . . 
};
byte freep[8] = {
  0x00, //. . . . . 
  0x00, //. . . . . 
  0x00, //. . . . . 
  0x00, //. . . . . 
  0x00, //. . . . . 
  0x00, //. . . . . 
  0x00, //. . . . . 
  0x00, //. . . . . 
};

int mat[2][24] = {
  {7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7},
  {7,0,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7}
};
int dinoAnim[] = {1, 0, 2, 3}; // {dino2, dino1, dinosit1, dinosit2}
int walk = 0;
int down = 0;
int up = 0;
int fps = 0;
int maxHigh = 600;
int pauseBetween = 1;
long jumpStarted = 0;
int isJumping = 1;
bool gameStarted = false;
int score = 0;
int highScore = 0;

void redraw(){
  for(int j = 15; j >= 0; j--)
    for(int i = 0; i < 2; i++){
      lcd.setCursor(j, i);
      lcd.write(mat[i][j]);
    }

  if(!gameStarted){
    String s = "Score: " + String(score/4);
    String hS = "High Score: " + String(highScore/4);
    lcd.setCursor(3, 0);
    lcd.print(s);
    lcd.setCursor(2, 1);
    lcd.print(hS);
  }
}

void setup() {
  Serial.begin(9600);
  
  lcd.begin(16, 2);
  
  lcd.createChar(0, dino1);
  lcd.createChar(1, dino2);
  lcd.createChar(2, dinosit1);
  lcd.createChar(3, dinosit2);
  lcd.createChar(4, obs1);
  lcd.createChar(5, obs3);
  lcd.createChar(6, obs4);
  lcd.createChar(7, freep);

  lcd.clear();
}

void loop() {
  if(getKey() == btnUp && !up) 
    jumpStarted = millis();
    
  down = (getKey() == btnDown);
  up = (getKey() == btnUp);

  if(!gameStarted){
    if(up || down){
      gameStarted = true;
      score = 0;
    }
  }

  if(gameStarted){
    //dino animator: 3 x delay (samples/ms)
    if(fps % 3 == 0)
       walk = 1 - walk;
    //fps: 4 x delay (frames/ms)
    if(fps % 4 == 0){
      pauseBetween--;
      for(int i = 0; i < 2; i++)
      for(int j = 0; j < 23; j++){
        mat[i][j] = mat[i][j+1];
      }
      mat[0][23] = 7;
      mat[1][23] = 7;
    }

    if(!pauseBetween) generateObstacles();
    isJumping = millis() - jumpStarted < maxHigh && up;
  
    if(isJumping && mat[0][1] == 6)
      gameover();
    if(isJumping && mat[0][1] == 5 && !down)
      gameover();
    if(!isJumping && mat[1][1] == 4)
      gameover();
    if(!isJumping && mat[1][1] == 5 && !down)
      gameover();
    //if(!down) score++;
    Serial.println(mat[0][1]);
    if(mat[0][1] != 7 || mat[1][1] != 7)
      score++;
    if(score > highScore) 
      highScore = score;
    fps++;
  }
  
  redraw(); 
  lcd.setCursor(1, !isJumping); 
  lcd.write(dinoAnim[down * 2 + walk]);
  
  delay(50);
}

void gameover(){
  cleanMatrix();
  gameStarted = false;
  pauseBetween = 1;
  delay(1000);
}

void cleanMatrix(){
  for(int i = 0; i < 2; i++)
    for(int j = 0; j < 16; j++)
      mat[i][j] = 7;
}

void generateObstacles(){
  int obsCount = random(1, 4);
  int types[4][2] = {{7, 4}, {6, 5}, {5, 4}, {6, 7}};
  int type = random(0, 4);
  for(int i = 0; i < type; i++){
    mat[0][16 + i] = types[type][0];
    mat[1][16 + i] = types[type][1];
  }
  int spaceCount = random(2, 5);
  pauseBetween = obsCount + spaceCount;
}

int getKey(){
  int r = analogRead(A0);
  //Serial.println(r);
  if(r > 1000) return btnNone;
  if(r < 1) return btnRight;
  if(r < 135) return btnUp;
  if(r < 310) return btnDown;
  if(r < 482) return btnLeft;
  if(r < 723) return btnSelect;
  return btnNone;
}
