// https://faire-ca-soi-meme.fr/domotique/2016/09/12/module-teleinformation-tic/

/******* local macro *****/
#define UART_BAUDRATE (1200)
#define UART_CONFIG (SERIAL_7E1)
/*delay between TIC information in ms*/
#define DELAY_BETWEEN_TIC (10)
//Serial1 TX pin is TX1
#define UART_SERIAL Serial1

#define ASCII_ETX (0x03)
#define ASCII_STX (0x02)

#define MAX_PAPP_VAL 6000

/******* local typedef *****/
typedef struct
{
  int HCHC;//Heures Creuses 9 char in Wh
  int HCHP;//Heures Pleines 9 char in Wh
  int PTEC;//Période Tarifaire en cours
  int IINST;//Intensité Instantanée 3 char in A
  int PAPP;//Puissance apparente 5 char in VA
}tsrtTicInfo_t;

/******* local functions prototypes *****/
void vidPrepareTicInfo(void);

/******* local variables *****/
char acUartTxBuff[256];
tsrtTicInfo_t LOC_strTicInfo;

/******* functions *****/

void setup() {
  // put your setup code here, to run once:
  UART_SERIAL.begin(UART_BAUDRATE, UART_CONFIG);
  memset(&LOC_strTicInfo, 0, sizeof(LOC_strTicInfo));
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(DELAY_BETWEEN_TIC);
  vidPrepareTicInfo();
  UART_SERIAL.write(acUartTxBuff);
  UART_SERIAL.flush();//wait for end of transmission
}

/******* local functions definition *****/
void vidPrepareTicInfo(void)
{
  char tmpBuff[5];
  
  LOC_strTicInfo.HCHC = LOC_strTicInfo.HCHC+random(10);
  LOC_strTicInfo.HCHP = LOC_strTicInfo.HCHP+random(10);
  LOC_strTicInfo.PAPP = random(MAX_PAPP_VAL);
  LOC_strTicInfo.IINST = random(10,20);
  LOC_strTicInfo.PTEC = random(2); /*0 or 1*/

  if(LOC_strTicInfo.PTEC){
    sprintf(tmpBuff,"HP.."); 
  }else{
    sprintf(tmpBuff,"HC..");
  }
  sprintf(acUartTxBuff, \
           "ADCO 031961353121 <\r\nOPTARIF HC.. <\r\nISOUSC 60 <\r\nHCHC %09d #\r\nHCHP %09d :\r\nPTEC %s  \r\nIINST %03d X\r\nIMAX 090 H\r\nPAPP %05d +\r\nHHPHC A ,\r\nMOTDETAT 000000 B%c%c", \
           LOC_strTicInfo.HCHC, \
           LOC_strTicInfo.HCHP, \
           tmpBuff,\
           LOC_strTicInfo.IINST, \
           LOC_strTicInfo.PAPP, \
           ASCII_ETX, ASCII_STX); 
   
}
