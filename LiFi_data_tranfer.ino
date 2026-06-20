#include <LiquidCrystal.h>

#define SENSOR A0
int threshold = 500;

bool state = false;
String morse = "";   // DOT va DASH lar yig‘iladi
String text = "";    // matn chiqariladi
unsigned long lastTime = 0;

// LCD pinlari
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // RS, E, D4, D5, D6, D7

char decodeMorse(String code) {
  if(code == ".-") return 'A';
  if(code == "-...") return 'B';
  if(code == "-.-.") return 'C';
  if(code == "-..") return 'D';
  if(code == ".") return 'E';
  if(code == "..-.") return 'F';
  if(code == "--.") return 'G';
  if(code == "....") return 'H';
  if(code == "..") return 'I';
  if(code == ".---") return 'J';
  if(code == "-.-") return 'K';
  if(code == ".-..") return 'L';
  if(code == "--") return 'M';
  if(code == "-.") return 'N';
  if(code == "---") return 'O';
  if(code == ".--.") return 'P';
  if(code == "--.-") return 'Q';
  if(code == ".-.") return 'R';
  if(code == "...") return 'S';
  if(code == "-") return 'T';
  if(code == "..-") return 'U';
  if(code == "...-") return 'V';
  if(code == ".--") return 'W';
  if(code == "-..-") return 'X';
  if(code == "-.--") return 'Y';
  if(code == "--..") return 'Z';
  if(code == "-----") return '0';
  if(code == ".----") return '1';
  if(code == "..---") return '2';
  if(code == "...--") return '3';
  if(code == "....-") return '4';
  if(code == ".....") return '5';
  if(code == "-....") return '6';
  if(code == "--...") return '7';
  if(code == "---..") return '8';
  if(code == "----.") return '9';
  return '?';
}

void setup() {
  Serial.begin(115200);
  lastTime = millis();
  lcd.begin(16, 2);      // LCD 16x2
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Morse Decoder");
  delay(1000);
  lcd.clear();
}

void loop() {
  int val = analogRead(SENSOR);
  bool current = (val > threshold);
  unsigned long now = millis();

  if(current != state) {
    unsigned long duration = now - lastTime;

    // 🔴 OFF tugadi → signal (yorug‘lik bor edi)
    if(state != true) {
      if(duration > 40 && duration < 100) morse += ".";
      else if(duration > 140 && duration < 260) morse += "-";
    }

    // ⚫ ON tugadi → bo‘shliq
    else {
      if(duration > 140 && duration < 260) { // harf tugadi
        if(morse.length() > 0){
          char c = decodeMorse(morse);
          text += c;
          lcd.setCursor(text.length()%16, text.length()/16); // LCD cursor
          lcd.print(c);
          morse = "";
        }
      }
      else if(duration > 300) { // SPACE
        if(morse.length() > 0){
          char c = decodeMorse(morse);
          text += c;
          lcd.setCursor(text.length()%16, text.length()/16);
          lcd.print(c);
          morse = "";
        }
        text += " ";
        lcd.setCursor(text.length()%16, text.length()/16);
        lcd.print(" ");
      }
    }

    state = current;
    lastTime = now;
  }

  // 🔹 Timeout: oxirgi harfni decode qilish
  if(morse.length() > 0 && (millis() - lastTime) > 1000) {
    char c = decodeMorse(morse);
    text += c;
    lcd.setCursor(text.length()%16, text.length()/16);
    lcd.print(c);
    morse = "";
  }
}
