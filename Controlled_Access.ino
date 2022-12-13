// Controlled_Access

#include <SPI.h>                //Biblioteca padrão SPI 
#include <MFRC522.h>            //Biblioteca do RFID
#include <Wire.h>               //Biblioteca Do LCD + I2C
#include <LiquidCrystal_I2C.h>  //Biblioteca Do LCD + I2C


//Pinos definição do projeto 
#define SS_PIN 10               // Pino Chip Select 
#define RST_PIN 9               // Pino Reset 
#define BUZZER 8                // Pino Buzzer 
#define LED_VERDE 7             // Pino LED 
#define LED_VERMELHO 6          // Pino LED

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Instacia do objeto (RFID)


String cartoescadastrados[1] ={"93 5D E3 11"};   // Codigo UID das Tags com permissão.


LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); // Funcionamento do LCD + I2C 

 //Pinos do LED RGB
 int ledVermelho =2;
 int ledVerde =3;
 

void setup() 
{
  Serial.begin(9600);  //Inicia a Serial Arduino
  lcd.begin(16,2);     //Inicia LCD
  SPI.begin();         //Inicia conexão SPI. 
  mfrc522.PCD_Init();  // Inicia MFRC522  
  Serial.println();
  pinMode(LED_VERDE, OUTPUT);     // Declara o pino do led verde como saída
  pinMode(LED_VERMELHO, OUTPUT);  // Declara o pino do led vermelho como saída
  pinMode(BUZZER, OUTPUT);        // Declara o pino do buzzer como saída
  
  mensagemhome();    //Metodo do Menu 
  
} 

void loop() 
{

  if ( ! mfrc522.PICC_IsNewCardPresent())  //Espera a proximidade da tag
  {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial())   //Seleciona a tag
  {
    return;
  }
  Serial.print("Codigo do aluno :");   //Exibe o conteudo da tag no serial monitor
  String banco= "";                 //String que armazena a UID do cartão
  byte letra;                          //Captura cada dado em formato de Byte

  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     banco.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     banco.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  
  Serial.println();
  boolean tagVerificada = false;            //Tag autorizada ou não
  
  lcd.clear();
  lcd.setCursor(0,0);
  
  
  Serial.print("Leitura : ");
  

  banco.toUpperCase();    //coloca o valor do conteudo em maisculo
  
 for(int indice =0; indice < sizeof(cartoescadastrados); indice++){       // Nesse FOR verifica se a tag lida esta cadastrada
   if (banco.substring(1) == cartoescadastrados[0]) 
  {
    Serial.println("Ola Taylor!");
    Serial.println("Acesso Liberado!");
    
    lcd.print("Ola Taylor!");
    lcd.setCursor(0,1);
    lcd.println("Acesso Liberado!");
    
    tagVerificada = true;    
    efeitoPermitido();
    delay(800); 
    mensagemhome();
    banco= "";         //Limpa a String conteudo para  nova leitura
    
  }
  
  if((tagVerificada == false)&&(banco != "")){     //Informa que o o usuario que não tem cadastro
    

    Serial.println("Usuario Não Cadastrado! ");       //Se sim exibe as seguintes mensagem no serial
    Serial.println("Acesso Não Permitido!");
    
    lcd.print("Usuario Invalido!");                   //Se sim exibe as seguintes mensagem no lcd
    lcd.setCursor(0,1);
    lcd.println(" Acesso Negado! ");
    
    efeitoNegado();                              // Acende led vermelho
    delay(100);  

    mensagemhome();

    banco= "";
  }
}
  
} 


void mensagemhome()   //Menu inicial que informa ao usuário como prosseguir
{
  Serial.println();
  Serial.println("Aproxime o seu cartao do leitor :");
  
  lcd.clear();                 //Limpa o LCD 
  lcd.setCursor(0,0);
  lcd.print(" Aproxime a sua");     //Printa no LCD 
  lcd.setCursor(0,1);
  lcd.print(" tag p/ leitura!");
  
  digitalWrite(ledVermelho, HIGH);
  digitalWrite(ledVerde, HIGH);
  
  Serial.println();
 
}


void tagValida()             // Metodo quando tag valida 
{
  digitalWrite(ledVerde, LOW);
}

 
void tagInvalida()             //Metodo  quando a tag invalida 
{ 
  digitalWrite(ledVermelho, LOW);
  
}

void efeitoPermitido(){  
    int quantidadebips = 2;           //Bips Buzzer
    for(int j=0; j<quantidadebips; j++){
    
    tone(BUZZER,1300);                     ///Ligando o buzzer e o led verde. 
    digitalWrite(LED_VERDE, HIGH);   
    delay(800);   
        
    noTone(BUZZER);                      //Desligando o buzzer e led verde. 
    digitalWrite(LED_VERDE, LOW);
    delay(800);
  }  
}

void efeitoNegado(){  
    int quantidadebips = 1;  //Bips Buzzer
    for(int j=0; j<quantidadebips; j++){   
    //Ligando o buzzer com uma frequência de 500 hz e ligando o led vermelho.
    tone(BUZZER,500);
    digitalWrite(LED_VERMELHO, HIGH);   
    delay(800); 
    
    noTone(BUZZER);                            //Desligando o buzzer e led verde.          
    digitalWrite(LED_VERMELHO, LOW);
    delay(800);
  } 
} 
