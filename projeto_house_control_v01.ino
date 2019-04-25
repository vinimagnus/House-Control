//OBS
//DTH PINOs -> D0, VIN e GND
//HORTA -> D1, 3V3, GND
//MOTOR AGUA -> D2, 3V3 e GND
//TEMPERATURA DA AGUA -> D3, 3V3, GND
//LEDS
  //Latch -> D5
  //Clock -> D4
  //Data -> D6
//Garagem -> D7, 3v3, GND

//STEP INICIAL
//WF wi-fi
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
const char* ssid = "MinhaRede";
const char* password = "12345678";
String site = "";
ESP8266WebServer server(80); // server: http://192.168.4.1

//DHT Sensor Temperatura e Umidade
#include <DHT.h>
#define DHTPIN D0 // pino que estamos conectado
#define DHTTYPE DHT11 // DHT 11
DHT dht(DHTPIN, DHTTYPE);


//Horta Sensor de Umidade do solo
#define HortaPIN D1 //Pino conectado


//Motor água
#define MotorAguaPIN D2
int statusMotorAgua = 0;

//Temperatura água (piscina)

#include <OneWire.h>
#include <DallasTemperature.h>
#define SLEEP_DELAY_IN_SECONDS  30
#define ONE_WIRE_BUS            D3      // DS18B20 pin
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);
char temperatureString[6];


//Leds
int latchPin = D5;
int clockPin = D4;
int dataPin = D6;

byte leds = 0;
int arrayleds[8];
int i;


// garagem
#include <Servo.h>
Servo motor;
#define motor_pin D7


//FUNÇÕES DO PROGRAMA

//funções wi-fi
void handleRoot() {
site = "<!DOCTYPE html>\n";
site += "<html lang='pt-br'>\n";
site += "<head>\n";
    site += "<meta charset='UTF-8'>\n";
    site += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>\n";
    site += "<meta http-equiv='X-UA-Compatible' content='ie=edge'>\n";
    site += "<title>House Control</title>\n";

    site += "<style> *,body{padding: 0; margin: 0; font-family: Arial, Helvetica, sans-serif;} header{ width: 100%; padding: 2em 0; background: #2ecc71; color: #fff; text-transform: uppercase; text-align: center; } #main{ background: #ecf0f1; text-align: center; } h2{color: #0da34b; text-transform: uppercase;} section{padding-top: 2em;} section .controles{ padding: 3em 0 1em 0; position: relative; text-align: center; margin:0; } .controles label{display: grid; position:absolute; top: 0; text-align: center; width: 100%; font-size: 1.3em; color: #333;} .controles a{ padding: 1em 2em; background: #2ecc71; color: #fff; text-decoration: none; text-transform: uppercase; } .controles span{ padding: 1em 2em; background: #ccc; color: #333; border-radius: 6px;} .controles.span{display:grid;} .controles.span label{margin-top:1em}.controles.span a, .controles.span span{width: 15em; margin: 0 auto;}</style>\n";
site += "</head>\n";
site += "<body>\n";
    site += "<header>\n";
        site += "<h1>House Control</h1>\n";
    site += "</header>\n";

    site += "<div id='main'>\n";
        site += "<section>\n";
            site += "<h2>Leds</h2>\n";
            site += "<div class='controles'>\n";
                site += "<label>Luz sala</label>\n";
                
                site += "<a href='op?f=l3&s=1'>Ligar</a>\n";
                site += "<a href='op?f=l3&s=0'>Desligar</a>\n";
            site += "</div>\n";
            site += "<div class='controles'>\n";
                site += "<label>Luz cozinha</label>\n";
                
                site += "<a href='op?f=l6&s=1'>Ligar</a>\n";
                site += "<a href='op?f=l6&s=0'>Desligar</a>\n";
            site += "</div>\n";

            site += "<div class='controles'>\n";
                site += "<label>Luz lavanderia</label>\n";
                
                site += "<a href='op?f=l5&s=1'>Ligar</a>\n";
                site += "<a href='op?f=l5&s=0'>Desligar</a>\n";
            site += "</div>\n";

            site += "<div class='controles'>\n";
                site += "<label>Luz Quarto 1</label>\n";
                
                site += "<a href='op?f=l7&s=1'>Ligar</a>\n";
                site += "<a href='op?f=l7&s=0'>Desligar</a>\n";
            site += "</div>\n";

            site += "<div class='controles'>\n";
                site += "<label>Luz Quarto 2</label>\n";
                
                site += "<a href='op?f=l4&s=1'>Ligar</a>\n";
                site += "<a href='op?f=l4&s=0'>Desligar</a>\n";
            site += "</div>\n";

            site += "<div class='controles'>\n";
                site += "<label>Luz Garagem</label>\n";
                
                site += "<a href='op?f=l1&s=1'>Ligar</a>\n";
                site += "<a href='op?f=l1&s=0'>Desligar</a>\n";
            site += "</div>\n";

            site += "<div class='controles'>\n";
                site += "<label>Luz banheiro</label>\n";
                
                site += "<a href='op?f=l2&s=1'>Ligar</a>\n";
                site += "<a href='op?f=l2&s=0'>Desligar</a>\n";
            site += "</div>\n";

            site += "<div class='controles'>\n";
                site += "<label>Luz quiosque</label>\n";
                
                site += "<a href='op?f=l8&s=1'>Ligar</a>\n";
                site += "<a href='op?f=l8&s=0'>Desligar</a>\n";
            site += "</div>\n";
        site += "</section>\n";

       site += "<section>\n";
            site += "<h2>Controles</h2>\n";
            site += "<div class='controles'>\n";
                site += "<label>Garagem</label>\n";
                
                site += "<a href='op?f=g1&s=1'>Abrir</a>\n";
                site += "<a href='op?f=g1&s=0'>Fechar</a>\n";
            site += "</div>\n";

        
            site += "<div class='controles span'>\n";
                site += "<label>Piscina</label>\n";
                site += "<a href='op?f=a1'>Ativar troca de água</a>\n";
                
                site += "<span>Temperatura da água: ";
site += temp_agua();
                site += "</span>\n";
            site += "</div>\n";
        site += "</section>\n";

        site += "<section>\n";
            site += "<div class='controles span'>\n";
                site += "<h2>Casa:</h2>\n";
                site += "<span>Temperatura: ";
site += temp_dht('t');
                site += "</span>\n";
                site += "<span>Umidade: ";
site += temp_dht('u');
                site += "</span>\n";              
            site += "</div>\n";
        site += "</section>\n";

        site += "<section>\n";
            site += "<div class='controles span'>\n";
                site += "<h2>Horta:</h2>\n";
                site += "<span>Umidade do solo: ";
site += horta();
                site += "</span>\n";
                site += "<span>Status: ";
site += resultado_solo(horta());
                site += "</span>\n";
            site += "</div>\n";
        site += "</section>\n";
    site += "</div>\n";
site += "</body>\n";
site += "</html>\n";

  server.send(200, "text/html", site);

  site = "";
}

//funcao para ler o url
void leituraUrl(){

  String funcao, funcaoStatus; 
  funcao = server.arg("f");
  funcaoStatus = server.arg("s");

  //Leds
  if(funcao.charAt(0) == 'l'){
    String led_selected = funcao;
    String led_function = funcaoStatus;
  
    led_selected = led_selected.charAt(1); //pega somente o numero - L0  
    setled(led_selected.toInt(), led_function.toInt());
  }


  // Motor d'água - piscina
  if(funcao == "a1"){
    if(statusMotorAgua == 0){
      motor_agua(0);
      statusMotorAgua = 1;
    } else if(statusMotorAgua == 1){
      motor_agua(1);
      statusMotorAgua = 0;
    }
  }

  if(funcao == "g1"){
    if(funcaoStatus == "1"){
      motor_garagem(1);
    } else if(funcaoStatus == "0"){
      motor_garagem(0);
    }
  }
   
  handleRoot();
}


//função para leitura DHT
float temp_dht(char x){
  if(x == 't'){
    float t = dht.readTemperature();
    return t;
  } else if(x == 'u'){
    float u = dht.readHumidity();
    return u;
  }
}


//função para leitura da umidade do solo (horta)
float horta(){
  //Lê o valor do pino
  float leitura_solo = analogRead(HortaPIN);
  return leitura_solo;
}

char* resultado_solo(float leitura_solo){
  if(leitura_solo > 0 && leitura_solo < 400){
    return "Solo umido";
  }
  if(leitura_solo > 400 && leitura_solo < 800){
    return "Umidade moderado";
  }
  if(leitura_solo > 800 && leitura_solo < 1024){
    return "Solo seco";
  }
}

//Motor d'água (piscina)
void motor_agua(int x){
  if(x == 1){
    digitalWrite(MotorAguaPIN, LOW); // Liga o motor
  } else if(x == 0){
    digitalWrite(MotorAguaPIN, HIGH); // Desliga o motor
  }
}


// Temperatura da água (piscina)
float getTemperature() {
  float temp;
  do {
    DS18B20.requestTemperatures(); 
    temp = DS18B20.getTempCByIndex(0);
    delay(100);
  } while (temp == 85.0 || temp == (-127.0));
  return temp;
}

char* temp_agua(){
   float temperature = getTemperature();
  // convert temperature to a string with two digits before the comma and 2 digits for precision
  dtostrf(temperature, 2, 2, temperatureString);
  // send temperature to the serial console

  return temperatureString;
  delay(100);
}


// função para acender os leds
void setled(int l, int s){
  arrayleds[l-1] = s;
  leds=0;
  for (i=0; i<8; i++){
     if(arrayleds[i]==1){
       bitSet(leds, i); 
     }
  }

  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, leds);
  digitalWrite(latchPin, HIGH);
}


//garagem
void motor_garagem(int f_garagem){
  if(f_garagem == 1){
    for(i=90;i>0;i--) {
        motor.write(i); 
        delay(10); 
      }
  } else if(f_garagem == 0){
      for(i=0;i<90;i++) {
        motor.write(i); 
        delay(10); 
      }
    }
}


void setup() {
  
  Serial.begin(9600);
  
  //wi-fi
  WiFi.mode(WIFI_AP); //aceita WIFI_AP / WIFI_AP_STA / WIFI_STA
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  server.on("/", handleRoot);
  server.on("/op", leituraUrl);
  server.begin();

  //DHT
   dht.begin();

  //Horta
  pinMode(HortaPIN, INPUT);

  //Motor da água (piscina)
  pinMode(MotorAguaPIN, OUTPUT);
  motor_agua(0); //Inicializa-o desligado

  //Temperatura da água (piscina)
  DS18B20.begin();

  //LEDS
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);

  //Garagem
  motor.attach(motor_pin);
}

void loop() {
  //wi-fi
  server.handleClient();
  float leitura_solo = analogRead(HortaPIN);
  Serial.println(leitura_solo);

}
