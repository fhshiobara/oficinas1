#include <ESP32Servo.h>
Servo tampa;
Servo racao1;
Servo racao2;
Servo racao3;

const int PINO_TAMPA = 13;
const int PINO_RACAO1 = 18;
const int PINO_RACAO2 = 25;
const int PINO_RACAO3 = 15;

short int qualRacao(){
}

void Liberar_racao1(int grams){
    tampa.write(90);
    delay(1000);

    // Abre o compartimento de ração
    racao1.write(0);
    delay(800); //aqui depende da vazao de cada tipo de racao que sera utilizada, dependendo de possiveis testes, se nao fudeu
    racao1.write(90); // para

    delay(1000);

    // Fecha o compartimento de ração
    racao1.write(180); //aqui volrta,nao sei quanto tempo precisamos, mentira ele ja ta voltando no loop de cima,chapei
    delay(800);
    racao1.write(90); // para

    delay(1000);

    // Fecha a tampa
    tampa.write(0);

}

void Liberar_racao2(int grams ){
        tampa.write(90);
    delay(1000);

    // Abre o compartimento de ração
    racao2.write(0);
    delay(800); //aqui depende da vazao de cada tipo de racao que sera utilizada, dependendo de possiveis testes, se nao fudeu
    racao2.write(90); // para

    delay(1000);

    // Fecha o compartimento de ração
    racao2.write(180);
    delay(800);
    racao2.write(90); // para

    delay(1000);

    // Fecha a tampa
    tampa.write(0);

    
}
void Liberar_racao3(int grams){
    tampa.write(90);
    delay(1000);

    // Abre o compartimento de ração
    racao3.write(0);
    delay(800); //aqui depende da vazao de cada tipo de racao que sera utilizada, dependendo de possiveis testes, se nao fudeu
    racao3.write(90); // para

    delay(1000);

    // Fecha o compartimento de ração
    racao3.write(180); 
    delay(800);
    racao3.write(90); // para

    delay(1000);

    // Fecha a tampa
    tampa.write(0);
}


void setup() {
    tampa.attach(PINO_TAMPA);
    racao1.attach(PINO_RACAO1);
    racao2.attach(PINO_RACAO2);
    racao3.attach(PINO_RACAO3);

    tampa.write(0);
    racao1.write(90); // parado
    racao2.write(90);
    racao3.write(90); //inclusao dos motores

    delay(1000);
}

void loop() {
    //aqui deve fiar a logica de acionamento de acordo com cada achorro que foi detectado
    delay(15000);
    Liberar_racao2(0);

    delay(15000);

    /* 
    Liberar_racao2(0);
    delay(15000);
    Liberar_racao3(0);
    delay(15000);
    */
}
