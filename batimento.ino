#include "Lista.h"
#include "Avl.h"
#include <SPI.h>
#include "SdFat.h"

SdFat sdCard;
SdFile myFile;

Avl arv;
Lista myLista;

const int pinoSS = 10;
int pino_card = A5;
int auxdelay = 0;
float valor_card;
float maximo;
float minimo;
int cont = 0; // controla a quantidade dados de dados na lista.
unsigned ordem = 0; // a ordem em que foi inserido na lista.
unsigned ordemVer = 0; // variável controladora de registros na árvore.

int leitura = 1;
int escrita = 1;

void setup() {
    Serial.begin(9600);
    // Aguarda 1 seg antes de acessar as informações do sensor
    delay(1000);
    pinMode(pino_card, INPUT);
    pinMode(pinoSS, OUTPUT); // Declara pinoSS como saída
    pinMode(7, OUTPUT); //Led verde
    pinMode(4, OUTPUT); //Led amarelo
}

void loop() {
  if (leitura == 1){
    if (!sdCard.begin(pinoSS,SPI_HALF_SPEED))sdCard.initErrorHalt();//Inicializa o modulo SD
    if(myFile.open("batimentos.txt", O_CREAT | O_RDONLY))  {
        digitalWrite(4, HIGH); // Liga Led amarelo.
        delay(2000);
        while (myFile.available()){
          Serial.write(myFile.read());
        }
        Serial.println();
        Serial.println("Novos dados sendo capturados");
    }
    myFile.close();
    digitalWrite(4, LOW); // Desliga Led amarelo.
    leitura = 0;
    escrita = 1;
  }
    valor_card = analogRead(pino_card);
    if (cont > 10 and cont <= 40) { //controle de fluxo de dados, pois os 10 primeiras leituras são erro.
        myLista.insere(ordem, valor_card);
        ++ordem;
      }
     cont++;
    if (myLista.getTam() == 30) {
          cont = 0;
          if (ordemVer < 20) {
              Serial.print("Media: ");
              float media = myLista.media();
              Serial.println(media);
              delay(1000); 
              myLista.removeTodos();
              arv.insere(ordemVer, media);
              ordemVer++;
          } else if (ordemVer == 20) {
              Serial.println("Árvore de registro");
              arv.percorreEmOrdem();
              maximo = arv.maximoArv();
              minimo = arv.minimoArv();
              Serial.println();
              digitalWrite(7, HIGH);   // Liga Led verde
              delay(2000);
              Serial.print("Maior Valor: ");
              Serial.println(maximo);
              Serial.print("Menor Valor: ");
              Serial.println(minimo);
              Serial.print("Salvando o maior e menor valor...");
              if (escrita == 1){
                if (!myFile.open("batimentos.txt", O_WRONLY | O_CREAT | O_AT_END | O_TRUNC)) {
                  sdCard.errorHalt("Erro na abertura do arquivo Batimentos.txt"); 
                }
                myFile.println("Ultimos dados salvos");
                myFile.print("Maior: ");
                myFile.println(maximo);
                myFile.print("Minimo: ");
                myFile.println(minimo);
                // Fechamento de arquivo:
                myFile.close();
                digitalWrite(7, LOW); // Desliga led verde
                Serial.println("Salvo com sucesso.");
                leitura = 1;
                escrita = 0;
              }
              delay(4000);
              ordemVer = 0;
              arv.destroiArvore(); // Esvazia a árvore para novos valores.
          }
    }
  delay(100);
}
