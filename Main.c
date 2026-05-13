#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define MPU6050_ADDR 0x68

LiquidCrystal_I2C lcd(0x27, 16, 2);

int16_t Ax = 0, Ay = 0, Az = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  lcd.init();
  lcd.backlight();

  lcd.print("Starting...");
  delay(1000);
  lcd.clear();

  // Wake MPU6050
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();

  delay(200); // 🔴 increase delay for stability
}

void loop() {

  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x3B);

  if (Wire.endTransmission() != 0) {
    lcd.setCursor(0,0);
    lcd.print("I2C Error      ");
    Serial.println("I2C Error");
    delay(1000);
    return;
  }

  Wire.requestFrom(MPU6050_ADDR, 6);

  if (Wire.available() >= 6) {

    int16_t Ax = Wire.read() << 8 | Wire.read();
    int16_t Ay = Wire.read() << 8 | Wire.read();
    int16_t Az = Wire.read() << 8 | Wire.read();

    // SERIAL OUTPUT
    Serial.print("X: "); Serial.print(Ax);
    Serial.print(" Y: "); Serial.print(Ay);
    Serial.print(" Z: "); Serial.println(Az);

    // MOTION DETECTION
    bool motion = (abs(Ax) > 15000 || abs(Ay) > 15000 || abs(Az) > 15000);

    if (motion) {
      // SERIAL MESSAGE
      Serial.println(">>> Device Tilt Detected <<<");

      // LCD DISPLAY
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Till detected");
      lcd.setCursor(0,1);
      lcd.print("X:");
      lcd.print(Ax);

      delay(1000);  // show message clearly
    } 
    else {
      // NORMAL DISPLAY
      lcd.setCursor(0,0);
      lcd.print("X:");
      lcd.print(Ax);
      lcd.print("   ");

      lcd.setCursor(0,1);
      lcd.print("Y:");
      lcd.print(Ay);
      lcd.print("   ");
    }

  } else {
    lcd.setCursor(0,0);
    lcd.print("No Data        ");
    Serial.println("No Data");
  }

  delay(500);
}
