// Broches de la matrice LED
#define ROW_1 5
#define ROW_2 8
#define ROW_3 3
#define ROW_4 9
#define ROW_5 13
#define ROW_6 2
#define ROW_7 A2
#define ROW_8 10
#define COL_1 A0
#define COL_2 A1
#define COL_3 12
#define COL_4 4
#define COL_5 11
#define COL_6 6
#define COL_7 7
#define COL_8 A3
const byte rows[8] = {ROW_1, ROW_2, ROW_3, ROW_4, ROW_5, ROW_6, ROW_7, ROW_8};
const byte cols[8] = {COL_1, COL_2, COL_3, COL_4, COL_5, COL_6, COL_7, COL_8};
// Coordonnées des LEDs à allumer pour effet "victoire"
const byte ledPoints[8][2] = {
  {2, 2}, {5, 2}, {6, 5}, {5, 6},
  {1, 5}, {2, 6}, {3, 6}, {4, 6}
};
// Joystick
#define JOY_X A4
#define JOY_Y A5
int playerX = 4;
int playerY = 4;
int fruitX;
int fruitY;
int score = 0;  // Compteur de pommes mangées
void setup() {
  Serial.begin(9600);
  Serial.println("Le jeu commence ! Manges 5 pommes pour gagner !");
  for (int i = 0; i < 8; i++) {
    pinMode(rows[i], OUTPUT);
    pinMode(cols[i], OUTPUT);
  }
clearMatrix();
// Générer une position de fruit différente du joueur
  do {
    fruitX = random(0, 8);
    fruitY = random(0, 8);
  } while (fruitX == playerX && fruitY == playerY);  // éviter même position
}
void loop() {
  // Lire joystick
  int xVal = analogRead(JOY_X);
  int yVal = analogRead(JOY_Y);
// Déplacement
  if (xVal < 400) playerX--;
  if (xVal > 600) playerX++;
  if (yVal < 400) playerY--;
  if (yVal > 600) playerY++;
// Empêcher de sortir de la matrice
  playerX = constrain(playerX, 0, 7);
  playerY = constrain(playerY, 0, 7);
// Collision : le joueur mange la pomme
  if (playerX == fruitX && playerY == fruitY) {
    score++;  // Augmenter le score
    Serial.print("Pommes mangées : ");
    Serial.println(score);
// Vérifier si le joueur a mangé 5 pommes
    if (score >= 5) {
      winEffect();
      score = 0;  // Réinitialiser le score pour recommencer une nouvelle partie
      Serial.println("Nouvelle partie !");
    }
    else {
      // Nouvelle position de pomme
      do {
        fruitX = random(0, 8);
        fruitY = random(0, 8);
      } while (fruitX == playerX && fruitY == playerY);
    }
  }
clearMatrix();
// Afficher la pomme
  lightUp(fruitX, fruitY);
// Afficher le joueur
  lightUp(playerX, playerY);
delay(200);
}
void lightUp(int x, int y) {
  digitalWrite(rows[y], LOW);
  digitalWrite(cols[x], HIGH);
}
void clearMatrix() {
  for (int i = 0; i < 8; i++) {
    digitalWrite(rows[i], HIGH);
    digitalWrite(cols[i], LOW);
  }
}
void winEffect() {
  unsigned long start = millis();
  while (millis() - start < 6000) {
    for (int i = 0; i < 8; i++) {
      clearMatrix();
      lightUp(ledPoints[i][0], ledPoints[i][1]);
      delay(3);  // Allumage rapide pour effet visuel
    }
  }
  clearMatrix();
}
