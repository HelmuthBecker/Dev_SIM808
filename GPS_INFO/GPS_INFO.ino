#include <SoftwareSerial.h> //Importa SoftwareSerial
#define DEBUG true

SoftwareSerial sim808(8,9); //Rx-Tx

String response;                                      //Contém a informação bruta, enviada pelo módulo GPS, após o comando AT.
int tamanhoString;                                    //Informa a qtd de char da variável response; Acima de 110 char, indica que os dados fornecidos pelo GPS são utilizaveis.
String data[5];                                       //Vetor de string que, após separados os dados relevantes existentes na variável response, 
                                                      //irá armazenar os valores referentes a Lat, Long, Time e status separadamente.
        
void setup()
{
  Serial.begin(9600);
  sim808.begin(9600);
 delay (2000);
   getgps();                                          //Função com comandos para iniciar o GPS.
   Serial.println("Iniciando GPS...");
}

void loop()
{
        sendData( "AT+CGNSINF",1000,DEBUG);           //Envia comando ao módulo solicitando dados do GPS
        criarMensagem();                              //Faz o tratamento da string recebida pelo módulo com todos dados fornecidos
        if (data[1] != 0){                            //Se o status enviado pelo GPS for diferente de 0, indica que os dados recebidos estão OK
          while (data[1] != 0){                       //Enquanto o status for diferente de 0, permanece no loop
        sendData( "AT+CGNSINF",1000,DEBUG);  
        delay(1000);
        criarMensagem();
          }
        }
        else {                                      //Caso o módulo perca o sinal com o GPS, tenta reconectar
              Serial.println("Aguardando Sinal GPS...");
              while (data[1] == 0){
              delay(5000);
              getgps();
              }
             } 
   }
   
void criarMensagem (){                              //Função que faz o tratamento da string com os dados do GPS
      
      int i = 0;                                    //contador da string data
      tamanhoString = response.length();            //Informa a qtd de char da variável response; Acima de 110 char, indica que os dados fornecidos pelo GPS são utilizaveis.
      Serial.println();
           
      if (tamanhoString > 110){                     //indica que os dados fornecidos pelo GPS são utilizaveis.
          for (byte a = 1; a < tamanhoString; a++){ //Enquanto o contador a for menor que o tamanho total da string, executa o loop para tratamento de response
          char h = response[a];                     //char h, variavel que armazena o valor da string response byte a byte
      if (h != ',') {                               //enquanto h for diferente de "," preenche a string
         data[i] +=h;
         h = "";                                    //esvazia a string para o próximo loop
         delay(100);
      } else {                                      //caso h seja igual "," incrementa o contador i o que faz criar uma nova string para armazenar dados da variavel response
        i++;  
      }
      if (i == 5) {                                 //quando o contador i chega a 5, todas variáveis com os dados relevantes do GPS estão criadas.
        
        Serial.println("Status: "+data[1]);
        Serial.println("Time: "+data[2]);
        Serial.println("Latitude: "+data[3]);
        Serial.println("Longitude: "+data[4]);
        break;
          }
        }
    }    
  }
  
void getgps(void)                                   //Função que ativa o GPS
{
   sendData( "AT+CGNSPWR=1",1000,DEBUG);            //Liga o GPS em modo GNSS
   Serial.println(response); 
   sendData( "AT+CGNSSEQ=RMC",1000,DEBUG);          //Parâmetro de configuração
   Serial.println(response); 
}


String sendData(String command, const int timeout, boolean debug)
{
    data[0]="";                                     //Informações irrelevantes
    data[1]="";                                     //Status do GPS
    data[2]="";                                     //Time
    data[3]="";                                     //Latitude
    data[4]="";                                     //Longitude
    response = "";                                  //Informações completas do módulo GPS
    sim808.println(command); 
    long int time = millis();   
    while( (time+timeout) > millis())               //Enquanto a soma de time(millis()) + timeout for maior do que o valor atual de millis(), lê caracteres da Serial.
    {
      while(sim808.available())
      {       
        char c = sim808.read(); 
        response+=c;
      }  
    }    
    if(debug)                                       //Se debug for true, retorna o valor da variavel response
    {
      //Serial.print(response);
      return response;
}
}
