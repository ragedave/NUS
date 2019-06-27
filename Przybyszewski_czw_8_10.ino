#include <stdlib.h>
#include <LiquidCrystal.h>

#define A -6.56794315984512e-07
#define B 0.00115986903632839
#define C -0.754114433341475
#define D 265.530141159735
#define TEMP_0_PIN         13   // Analog Input
#define MOSFET_D_PIN     10
#define FAN1_PIN         8

#define X_MIN_PIN           3
#ifndef X_MAX_PIN
#define X_MAX_PIN         2
#endif
#define Y_MIN_PIN          14
#define Y_MAX_PIN          15
#define Z_MIN_PIN          18
#define Z_MAX_PIN          19

#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38
#ifndef X_CS_PIN
#define X_CS_PIN         53
#endif

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
#ifndef Y_CS_PIN
#define Y_CS_PIN         49
#endif

#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62
#ifndef Z_CS_PIN
#define Z_CS_PIN         40
#endif

#define E0_STEP_PIN        26
#define E0_DIR_PIN         28
#define E0_ENABLE_PIN      24
#ifndef E0_CS_PIN
#define E0_CS_PIN        42
#endif

#define E1_STEP_PIN        36
#define E1_DIR_PIN         34
#define E1_ENABLE_PIN      30
#ifndef E1_CS_PIN
#define E1_CS_PIN        44
#endif

// temp 
LiquidCrystal lcd(16,17,23,25,27,29);
int temp;
double x,y;
//ruch
int i = 0;
int kierunek = 0;
String komenda;
int a = 0, b = 0, c = 0;
int kiera = 0, kierb = 0, kierc = 0;
bool czy_mam_odp =0;


void setup() {
  lcd.begin(20,4);
  pinMode(MOSFET_D_PIN  ,OUTPUT);
  pinMode(FAN1_PIN , OUTPUT);
  
  Serial.begin(115200);
  pinMode(X_ENABLE_PIN, OUTPUT);
  digitalWrite(X_ENABLE_PIN, LOW);

  pinMode(Y_ENABLE_PIN, OUTPUT);
  digitalWrite(Y_ENABLE_PIN, LOW);

  pinMode(Z_ENABLE_PIN, OUTPUT);
  digitalWrite(Z_ENABLE_PIN, LOW);



  pinMode(X_DIR_PIN , OUTPUT);
  digitalWrite(X_DIR_PIN , LOW);

  pinMode(Y_DIR_PIN , OUTPUT);
  digitalWrite(Y_DIR_PIN , LOW);

  pinMode(Z_DIR_PIN , OUTPUT);
  digitalWrite(Z_DIR_PIN , LOW);

  pinMode(X_STEP_PIN, OUTPUT);
  pinMode(Y_STEP_PIN, OUTPUT);
  pinMode(Z_STEP_PIN, OUTPUT);

  pinMode(X_MAX_PIN, INPUT_PULLUP);
  pinMode(Y_MAX_PIN, INPUT_PULLUP);
  pinMode(Z_MAX_PIN, INPUT_PULLUP);
}

void loop() {
  //Serial.print("a\n");
  
  // wyswietlanie temperatury
  lcd.setCursor(1,1);
  temp = analogRead(TEMP_0_PIN );
  x=(double)temp;
  y=A*x*x*x+B*x*x+C*x+D;
  lcd.print(y);
  // podgrzewanie
  int Temp=30;
  if(y<(Temp-10)){
    digitalWrite(MOSFET_D_PIN , HIGH);
    analogWrite(FAN1_PIN, 90); 
    delay(50);
     }
    else{
     digitalWrite(MOSFET_D_PIN ,LOW);
     analogWrite(FAN1_PIN, 255); 
    }
  
  // kontrola ruchu
  digitalWrite(X_STEP_PIN, LOW);
  digitalWrite(Y_STEP_PIN, LOW);
  digitalWrite(Z_STEP_PIN, LOW);
  delayMicroseconds(50);
  if ((digitalRead(X_MAX_PIN) == 0) && a > 0)
  {
    digitalWrite(X_STEP_PIN, HIGH);
    a--;
  }
  if (digitalRead((Y_MAX_PIN) == 0) && b > 0)
  {
    digitalWrite(Y_STEP_PIN, HIGH);
    b--;
  }
  if (digitalRead((Z_MAX_PIN) == 0) && c > 0)
  {
    digitalWrite(Z_STEP_PIN, HIGH);
    c--;
  }
  delayMicroseconds(50);
  i++;
  /*if(i==600)
    {
    i=0;
    kierunek=!kierunek;
    digitalWrite(X_DIR_PIN, kierunek);
    digitalWrite(Y_DIR_PIN, kierunek);
    digitalWrite(Z_DIR_PIN, kierunek);
    }*/

  //Serial.println(digitalRead(X_MAX_PIN));

  if(a == 0 && b == 0 && c == 0 && czy_mam_odp == 1) /*komunikat zakonczenia*/
  {
    Serial.println("OK");
    czy_mam_odp = 0;
  }

  while (Serial.available() > 0)
  {
    komenda = Serial.readStringUntil('\n');
    sscanf(komenda.c_str(), "a %d b %d c %d", &a, &b, &c);
    czy_mam_odp = 1;
    kiera = a < 0;
    digitalWrite(X_DIR_PIN, kiera);
    a = abs(a);

    kierb = b < 0;
    digitalWrite(Y_DIR_PIN, kierb);
    b = abs(b);

    kierc = c < 0;
    digitalWrite(Z_DIR_PIN, kierc);
    c = abs(c);
  }
}


