/*
 *This program is written in Energia for TI TM4C123 board. Also works fine in Arudino.
 *This program uses ESP8266 Wifi Module to communicate with the chat-server program.
 *This program is a chat-client program.
*/



//---------CONFIG-------------//
#define SSID        "XT1033"
#define PASS        "Ilovecoding"
#define CONN_TYPE   "TCP"
#define IP_ADDR     "192.168.1.44"
#define DEST_HOST   "192.168.1.44"
#define IP_PORT     "1111"
#define TIMEOUT     10000

int stepB=1;
char irL=0;
char irF=0;
char irR=0;

int leftA=4;
int leftB=5;
int rightA=6;
int rightB=7;

//-------------botMove_method-------------------//
void botMove(char instr)
{
   if(instr=='m') {
     digitalWrite(leftA, HIGH);
     digitalWrite(leftB, LOW);
     digitalWrite(rightA, HIGH);
     digitalWrite(rightB, LOW);
     delay(1000);
   }
   
   
   if(instr=='l') {
     digitalWrite(leftA, LOW);
     digitalWrite(leftB, HIGH);
     digitalWrite(rightA, HIGH);
     digitalWrite(rightB, LOW);
     delay(500);
   }
   
   
   if(instr=='r') {
     digitalWrite(leftA, HIGH);
     digitalWrite(leftB, LOW);
     digitalWrite(rightA, LOW);
     digitalWrite(rightB, HIGH);
     delay(500);
   }
   
   digitalWrite(leftA, LOW);
   digitalWrite(leftB, LOW);
   digitalWrite(rightA, LOW);
   digitalWrite(rightB, LOW);
   
}





//------------WIFI_CHAT_METHOD------------------//
//-->Chat function which sends and receives from the server; Core of the program;
//-->AT Commands used and the procedure to send data:
//-->    1. AT+CIPSEND=<length> - After the TCP connection is established, to send any data, you've to specify the length of the msg to the ESP, so that it prepares the TCP buffer.
//-->    2. After the ESP responds with '>' to 'AT+CIPSEND=<len>' command, you've to send the data to the ESP, then the ESP will send the data to the TCP server. 
//-->    3. If the data is successfully sent, the response from ESP would be 'SENT OK'.
char WifiChat()
{

  char send[30];
  char irdat[3];
  char recv[30];
  int i=0,j;
  send[0]='\0';
  recv[0]='\0';
  

 
  irL=digitalRead(26)+48;
  irF=digitalRead(24)+48;
  irR=digitalRead(28)+48;
  
  
  send[0]=32;
  send[1]='[';
  send[2]=(stepB+48); stepB++; if(stepB==10) { stepB=0; } //
  send[3]=44;
  send[4]=39;

  send[5]=irL;      //Storing IR Values
  send[6]=irF;
  send[7]=irR;
  
  send[8]=39; 
  send[9]=']';
  send[10]='\0';
  
  Serial.println(send);
resend:  
for(i=0;send[i]!='\0';i++){}          //Calculating the length of send[]
  
  Serial2.println("AT+CIPSEND="+(String)i); //Sending the length of the data to ESP 
  if(echoFind(">"))                         //Checking for the response from ESP(>)
  {
    Serial.print("ESP: ");
    Serial.println(send);
    Serial2.println(send);
    if(echoFind("OK"))                      //Checking for the response from ESP(SENT OK)
    { 
      
      Serial.print("Server: ");
      
     while(!Serial2.available()){}         //Waiting for receiving data..
     
     delay(1000);                        //---->Decreased to 2s from 10s. Bcoz, 10s is too slow<----///
     
     while(Serial2.available()>0 && Serial2.read()!=':') {} // to strip unwanted char until ':' in the received msg
     
     i=0;
     while(Serial2.available()>0)  {    // saving the data
       recv[i] = Serial2.read();
        i++;
     }
     recv[i]='\0';
     Serial.println(recv);
    }
  
    else                                  //(SENT OK) response is not received.
    {
      goto resend;
    }
    
  }
  
  else                           // ( > ) not received
    {
      goto resend;
    }
    
    
   Serial.println("---------Debug----------");
   Serial.print("Left-IR :"); Serial.println(irL);
   Serial.print("Front-IR :"); Serial.println(irF);
   Serial.print("Right-IR :"); Serial.println(irR);
   Serial.print("nextMove :"); Serial.println(recv[0]); 
    
    
   return recv[0];
   
}



//---------ECHO_FIND_METHOD------------//
//--->To find whether the given string is present within the msg received from ESP
//--->Returns TRUE if the string exists within the msg received from ESP, else it returns FALSE 

boolean echoFind(String keyword)
{
  byte current_char   = 0;
  byte keyword_length = keyword.length();

  long deadline = millis() + TIMEOUT; // To use it as a timeout counter
  
  while(millis() < deadline)
  {
    if (Serial2.available())
    {
      char ch = Serial2.read();
      if (ch == keyword[current_char])
        if (++current_char == keyword_length) //match found
        {
          return true;
        }
    }
  }
  return false;  // Timed out
}
 


//-----------WIFI_RESET_METHOD----------//
//-->To reset wifi using 'AT+RST' command
//-->the command was put in a do-while loop, bcoz until the repsone from ESP is OK, it should keep sending the command.
void WifiReset() {
   do  {
    Serial2.println("AT+RST");
  } while(!echoFind("OK"));

  Serial.println("Wifi Module Reset - Successfull");
}



//-----------WIFI_MODE_SET_METHOD----------//
//-->AT command: "AT+CWMODE=<mode>"
//-->To set the wifi mode to '1'(one), to make the wifi module act as a device which connects to an Access Point.
//-->Mode 1 - Station Mode(to connect to available Access Point);    Mode 2 - Access Point;    Mode 3 - Both;
void WifiModeSet(String mode)  {
  do  {
    Serial2.println("AT+CWMODE="+mode);
  } while(!echoFind("OK"));

  Serial.println("Wifi Mode"+mode+" Set - Successfull");
}



//-----------WIFI_CONNECT_AP_METHOD----------//
//-->To connect to an Access Point
//-->AT command: "AT+CWJAP=<SSID>,<PASSWD>"
void WifiConnectAP(String ssid, String pass)  {
  do  {
    Serial2.println("AT+CWJAP="+ssid+","+pass);
  } while(!echoFind("OK"));

  Serial.println("Connected to AP-"+ssid+", Pass-"+pass);
}



//-----------WIFI_'dis'CONNECT_AP_METHOD----------//
//-->To disconnect from an Access Point
//-->AT command: "AT+CWQAP"
void WifiDisconnectAP()
{
  Serial2.println("AT+CWQAP");
  Serial.println("Disconnected from the AP");
}




//-----------WIFI_CONNECT_'server'_METHOD----------//
//-->To establish TCP connection
//-->AT command: "AT+CIPSTART='<TCP/UDP>','<IP_ADDR>',<PORT>"
void WifiConnectServer(String type, String ip_addr, String port)  {
  Serial.println("Trying to connect "+ip_addr+" ....");
  do  {
    Serial2.println("AT+CIPSTART=\""+type+"\",\""+ip_addr+"\","+port); 
  } while(!echoFind("OK"));

  Serial.println("Connected to the Server: IP-"+ip_addr+", Port-"+port);
}



//-----------WIFI_'dis'CONNECT_'server'_METHOD----------//
//-->To close a TCP connection
//-->AT command: "AT+CIPCLOSE"
void WifiDisconnectServer()  {
  String msg="";
  do  {
    Serial2.println("AT+CIPCLOSE");
    delay(500);
  } while(!echoFind("OK"));

  msg+="Connection with ";
  msg+=IP_ADDR;
  msg+=" has been closed";
  
  Serial.println(msg);
}




void setup()
{
  Serial.begin(115200);  
  Serial2.begin(115200); //ESP module's UART 

  pinMode(28,INPUT);  //IR inputs
  pinMode(24,INPUT);
  pinMode(26,INPUT);
  
  pinMode(leftA, OUTPUT);    //motor outputs
  pinMode(leftB, OUTPUT);
  pinMode(rightA, OUTPUT);
  pinMode(rightB, OUTPUT);
  
  WifiReset();  
  WifiModeSet("1");
  WifiConnectAP(SSID,PASS);   
}

void loop()
{
  char instr;
  WifiConnectServer(CONN_TYPE,IP_ADDR,IP_PORT);
  while(true) {
    instr = WifiChat();
    Serial.println("---->Moving the bot.. | Wait..");
    botMove(instr);
    delay(500);
  }
  WifiDisconnectServer();  
  WifiDisconnectAP();  
  while(true){}
}

