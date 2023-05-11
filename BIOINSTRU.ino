#include <LiquidCrystal_I2C.h>   // Incluimos la biblioteca para el LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);   // Dirección del LCD y tamaño

int lm35Pin = A0;       // Pin analógico donde está conectado el LM35
int vcc = 11;       // Pin digital donde está conectado el relay
int relee = 13;
bool focoEncendido = false;  // Estado actual del foco

void setup() {
  Serial.begin(9600);   // Iniciamos la comunicación serie para depuración
  lcd.init();           // Iniciamos el LCD
  lcd.backlight();      // Encendemos la luz de fondo del LCD
  lcd.clear();
  pinMode(relee, OUTPUT);  // Configuramos el pin del relay como salida
  pinMode(vcc,OUTPUT);
}

void loop() {
  digitalWrite(vcc,HIGH);
  float temperatura = leerLM35();    // Leemos la temperatura del LM35
  mostrarTemperaturaLCD(temperatura);   // Mostramos la temperatura en el LCD
  controlarFoco(temperatura);           // Controlamos el foco según la temperatura
  delay(1000);  // Esperamos un segundo antes de volver a leer la temperatura
}

float leerLM35() {
  int valor = analogRead(lm35Pin);    // Leemos el valor del LM35
  float voltaje = valor * (5.0 / 1023.0);  // Convertimos el valor a voltaje
  float temperatura = voltaje * 100; // Convertimos el voltaje a temperatura en grados Celsius
  return temperatura;  // Devolvemos la temperatura
}

void mostrarTemperaturaLCD(float temperatura) {
  lcd.clear();                    // Borramos el contenido anterior del LCD
  lcd.setCursor(0, 0);            // Colocamos el cursor en la primera fila
  lcd.print("Temperatura:");
  lcd.setCursor(0,1);     // Escribimos el texto en el LCD
  lcd.print(temperatura);         // Escribimos la temperatura en el LCD
  lcd.print(" C");
}

void controlarFoco(float temperatura) {
  if(temperatura>=35 && focoEncendido) { // Si la temperatura es mayor que 35 grados y el foco está encendido
    digitalWrite(relee, LOW); //Apaga el foco
    focoEncendido = false;    // Cambiamos el estado del foco
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("LAMPARA OFF");
    delay(1500);
  }
  else if(temperatura<=30 && !focoEncendido) { // Si la temperatura es menor que 25 grados y el foco está apagado
    digitalWrite(relee, HIGH);//enciende el foco
    focoEncendido = true;     // Cambiamos el estado del foco
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("LAMPARA ON");
    delay(1500);
  }
}
