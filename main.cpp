#include <ESP32Servo.h>

Servo tampa;
Servo racao1;
Servo racao2;
Servo racao3;

short int dose_1 = 0;
short int dose_2 = 0;
short int dose_3 = 0;

const int PINO_TAMPA = 13;
const int PINO_RACAO1 = 18;
const int PINO_RACAO2 = 25;
const int PINO_RACAO3 = 15;

// Pinos da comunicação serial com a ESP32-S3 (câmera)
const int RXD1 = 16; // recebe da câmera (TX da câmera -> aqui)
const int TXD1 = 17; // livre, mas deixado disponível pra debug/ACK futuro

HardwareSerial CameraSerial(1); // UART1

short int qualRacao(){
    return 0;
}

void Liberar_racao1(int grams){
    if(dose_1 <= 2){
        dose_1++;

        tampa.write(90);
        delay(1000);

        racao1.write(0);
        delay(1000);
        racao1.write(90);

        delay(1000);

        racao1.write(180);
        delay(800);
        racao1.write(90);

        delay(1000);

        tampa.write(0);
    }
}

void Liberar_racao2(int grams){

    if(dose_2 <= 3){
        dose_2++;

        tampa.write(90);
        delay(1000);

        racao2.write(0);
        delay(300);
        racao2.write(90);

        delay(1000);

        racao2.write(180);
        delay(800);
        racao2.write(90);

        delay(1000);

        tampa.write(0);
    }
}

void Liberar_racao3(int grams){

    if(dose_3 <= 2){
        dose_3++;

        tampa.write(90);
        delay(1000);

        racao3.write(0);
        delay(1000);
        racao3.write(90);

        delay(1000);

        racao3.write(180);
        delay(800);
        racao3.write(90);

        delay(1000);

        tampa.write(0);
    }
}

void setup() {

    tampa.attach(PINO_TAMPA);
    racao1.attach(PINO_RACAO1);
    racao2.attach(PINO_RACAO2);
    racao3.attach(PINO_RACAO3);

    tampa.write(0);
    racao1.write(90);
    racao2.write(90);
    racao3.write(90);

    // Serial0 (USB) continua livre para debug via monitor serial
    Serial.begin(115200);

    // Serial1: comunicação dedicada com a ESP32-S3 (câmera)
    CameraSerial.begin(115200, SERIAL_8N1, RXD1, TXD1);

    delay(1000);

    Serial.println("WROOM-DA pronta. Aguardando sinais da camera...");
}

void loop() {

    if (CameraSerial.available()) {

        // Lê tudo até encontrar a vírgula
        String comando = CameraSerial.readStringUntil(',');

        // Limpa o resto do buffer (os gramas que vêm depois da vírgula)
        // até a quebra de linha, pra não sobrar lixo pro próximo loop
        if (CameraSerial.available()) {
            CameraSerial.readStringUntil('\n');
        }

        comando.trim(); // remove espaços/quebras de linha acidentais

        Serial.print("Comando recebido (ja sem os gramas): ");
        Serial.println(comando);

        if (comando == "1") {
            Liberar_racao1(0);
        } else if (comando == "2") {
            Liberar_racao2(0);
        } else if (comando == "3") {
            Liberar_racao3(0);
        }
    }
}
