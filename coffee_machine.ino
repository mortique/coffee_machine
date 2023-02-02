#include <Servo.h>

Servo kahvimitta;
Servo kahvihana;
Servo shaker;

boolean started=false;

int pos = 0, sensorPin = A0, dataPin1 = A5, dataPin2 = A4, dataPin3 = A3, dataPin4 = A2;
int waterFlow = 23; // senttilitraa / kalibrointiaika
int kuppikoko = 2; // desilitraa
int testTime = 10; // kalibrointiaika sekuntia

void setup() {
  Serial.begin(9600); 
  
  attachServo();
  kahvihana.write(0);
  kahvimitta.write(0);
  shaker.write(0);

  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT); 
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT); 
  
  digitalWrite(7, HIGH);  // kahvinkeitin pois
  digitalWrite(6, HIGH);  // vesipumppu pois
    
  digitalWrite(5, LOW);  // sensoriledi pois  
  digitalWrite(4, LOW); 
  
}

void loop() {   
  String incoming;
  int sensorValue = 0;  
    
 while(Serial.available()){     // luetaan komento
    incoming = Serial.readStringUntil('\n');
 }
  if(incoming.length() > 1){
    Serial.println(incoming);     
  }  

  if(incoming == "vesi")
    vesi();

  if(incoming == "shake"){  
    shake();    
  }
 if(incoming == "empty")
    empty();
    
  if(incoming == "auki"){
     kahvimitta.write(110);
  }
    if(incoming == "kiinni")
    kahvimitta.write(0);
 if(incoming == "kahvimitta")
    kahvimitallinen(); 
 if(incoming == "vesion")
    digitalWrite(6, LOW);
 if(incoming == "vesioff")
    digitalWrite(6, HIGH);
 if(incoming == "keitinon"){ 
    digitalWrite(7, LOW);
 }
 if(incoming == "keitinoff"){
    digitalWrite(7, HIGH);
 }
 if(incoming == "ledon")
    digitalWrite(5, HIGH);
 if(incoming == "ledoff")
    digitalWrite(5, LOW);


  
  if(incoming == "kahvi.5"){     // keitetään ½/1/2/3 kahvia    
    kahvi(9);
    Serial.println("kahvi½");
    delay(2000);
    Serial.println("kahvi in progress");   
  }   

  if(analogRead(dataPin1) > 800 || incoming == "kahvi1"){     // keitetään 1/2/3 kahvia    
    kahvi(1);
    Serial.println("kahvi1");
    delay(2000);
    Serial.println("kahvi in progress");   
  }   
  delay(20);
  if(analogRead(dataPin2) > 800 || incoming == "kahvi2"){      
    kahvi(2);
    Serial.println("kahvi2");
    delay(2000);    
    Serial.println("kahvi in progress");    
  }
  delay(20);
  if(analogRead(dataPin3) > 800 || incoming == "kahvi3"){ 
    kahvi(3);    
    Serial.println("kahvi3");
    delay(2000);   
    Serial.println("kahvi in progress");    
  }
  delay(20);
  if(analogRead(dataPin4) > 800){   // keitin pois päältä
    Serial.println("keitinoff_manual");
    digitalWrite(7, HIGH);
    delay(2000);
    Serial.println("keitinoff");
    
  }


  incoming == "";
}

void attachServo(){
   kahvimitta.attach(9);     // liitetään servot
   kahvihana.attach(8);
   shaker.attach(10);
}
void detachServo(){
   kahvimitta.detach();     // irrotetaan servot
   kahvihana.detach();
   shaker.detach();
}


void empty(){   // tyhjentää kahvisäiliön
     int max_hana = 115, min_hana = 40;
    int max_dosetti = 110, min_dosetti = 0;
        
    kahvimitta.write(max_dosetti);
    
    for (pos = min_hana; pos <= max_hana; pos += 1) { // hana auki
      kahvihana.write(pos);              
       delay(6);                       
    }
    
    for(int i = 0;i <= 10; i++)
      shake();

    for (pos = max_hana; pos >= min_hana; pos -= 1) { // hana kiinni
        kahvihana.write(pos);               
        delay(6);                        
    }  
    kahvimitta.write(min_dosetti);
 
}

void shake(){  
  pos = 130;
  shaker.write(pos);
  delay(700);
  shaker.write(0);
}

void rshake(){    
  shaker.write(90);
  delay(700);
}
void lshake(){    
  shaker.write(0);
  delay(700);  
}

void vesi(){
   int viive = 3300; //((waterFlow / kuppikoko) * testTime * 100);
    
   digitalWrite(6, LOW);  // lasketaan vesi
   delay(viive);
   digitalWrite(6, HIGH);
}

void kahvi(int lkm){    // keittää kahvia  
  int viive;
  
  if(lkm != 9){  // ½ kuppi?
    viive = 3600 * lkm; // ((waterFlow / kuppikoko) * testTime * 100) * lkm;
    for(int i = 0; i < lkm; i++){ kahvimitallinen(); kahvimitallinen();  }   // mitataan kahvi  
  }
   else{
     viive = 3600 * 0.5;
     kahvimitallinen();  
   }

   digitalWrite(6, LOW);  // lasketaan vesi
   delay(viive);
   digitalWrite(6, HIGH);
   digitalWrite(7, LOW);  // keitin päälle
   
}

void kahvimitallinen(){    // mittaa kahvimitallisen kahvia
  
    int max_hana = 122, min_hana = 60;
    int max_dosetti = 100, min_dosetti = 0;   
    int max_shaker = 130;
          
    for (pos = min_hana; pos <= max_hana; pos += 1) { 
      kahvihana.write(pos);              
       delay(2);                       
    }    
    rshake();           
    for (pos = max_hana; pos >= min_hana; pos -= 1) { 
        kahvihana.write(pos);              
        delay(6);                       
    }

   for (pos = min_hana; pos <= max_hana; pos += 1) { 
      kahvihana.write(pos);              
       delay(2);                       
    }    
    lshake();           
    for (pos = max_hana; pos >= min_hana; pos -= 1) { 
        kahvihana.write(pos);              
        delay(6);                       
    }

    for (pos = min_dosetti; pos <= max_dosetti; pos += 1) { 
        kahvimitta.write(pos);              
        delay(4);                       
    }    
    delay(500);

    for (pos = max_dosetti; pos >= min_dosetti; pos -= 1) { 
        kahvimitta.write(pos);              
        delay(4);                       
    }  
}
  
  digitalWrite(7, HIGH);
  Serial.print("Kalibroitu(s/5dl): ");
  Serial.println(i / 10);

  waterFlow = 50;
  testTime = i * 10;
}


int check(){  // kahvin määrän tarkistus

  int sensorValue;
  
  digitalWrite(5, HIGH);    // led päälle
  delay(50);
  
  sensorValue = analogRead(sensorPin);  // luetaan valovastus

  digitalWrite(5, LOW);
  if(sensorValue > 50)
    return 1;    
 else{
    digitalWrite(5, LOW);
    return 0;
 }         
}
