//////////////////////////////////////////////////////////////////////////////////
//              GND  ��Դ��
//              VCC  5v��Դ
//              D0   A5��SCL��
//              D1   A4��SDA��
//******************************************************************************/
//SDA------------------------Digital 10
//SCK------------------------Digital 13
//MOSI----------------------Digital 11
//MISO----------------------Digital 12
//IRQ------------------------��������
//GND-----------------------GND
//RST------------------------Digital 9                    
//3.3V------------------------3.3V (ǧ��Ҫ���ӵ�5V�ӿڣ�����)

#include "oledfont.h"
#include "Wire.h"
#include "SoftwareSerial.h"

#include <SPI.h>
#include <MFRC522.h>

#define res A3//RES
//#define led 13 

#define ir1_pin A6 
#define ir2_pin A7
#define ir3_pin A0
#define beep 2 
#define led_l digitalWrite(led,LOW) 
#define led_h digitalWrite(led,HIGH) 
#define OLED_RES_Clr() digitalWrite(res,LOW)//RES
#define OLED_RES_Set() digitalWrite(res,HIGH)
//#define motor_close digitalWrite(motor,LOW) 
//#define motor_open  digitalWrite(motor,HIGH)
#define beep_off  digitalWrite(beep,LOW) 
#define beep_on   digitalWrite(beep,HIGH)
#define ir1   digitalRead(ir1_pin)
#define ir2   digitalRead(ir2_pin)
#define ir3   digitalRead(ir3_pin)

#define OLED_CMD  0  //д����
#define OLED_DATA 1 //д����
#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.
MFRC522::MIFARE_Key key;

static u8 Send_buf[10] = {0};
SoftwareSerial mySerial(3, 4);     // 3 -> RX, 4 -> TX
#define card_num_max 5   //���Ŀ�����

typedef struct _CARD_
{
    u32 id_buf[10];
    u8 count;    //ͬһ�ſ�ˢ������
    u8 card_type;
}_CARD;

_CARD card[card_num_max];

 
void setup()
{
  Serial.begin(9600); // �򿪴��ڣ��������ݴ�������Ϊ 9600bps
  mySerial.begin(9600);
  SPI.begin();  // Init SPI bus

 
  OLED_Init();
  OLED_ColorTurn(0);//0������ʾ 1��ɫ��ʾ
  OLED_DisplayTurn(0);//0������ʾ 1��ת180����ʾ
  OLED_ShowString(0, 0, "ID:  ",16);    

  pinMode(beep,OUTPUT);
  mfrc522.PCD_Init(); // Init MFRC522 card

    // Prepare the key (used both as key A and as key B)
    // using FFFFFFFFFFFFh which is the default at chip delivery from the factory
    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }

    Serial.println(F("Scan a MIFARE Classic PICC to demonstrate read and write."));
    Serial.print(F("Using key (for A and B):"));
    dump_byte_array(key.keyByte, MFRC522::MF_KEY_SIZE);
    Serial.println();

    Serial.println(F("BEWARE: Data will be written to the PICC, in sector #1"));

}
u8 card_cnt=0;    //����ˢ������  
u32 card_number=0;
u8 people_num=0; //���������
u8 flag1=0,flag2=0,flag3=0,flag4=0,flag5=0,flag6=0,remain=0;

void loop()
{
    char cStr [ 30 ];
    static u8 i = 0,j=0;
     static u8 a='A';
    u16 adc_val=0;
    if ( ! mfrc522.PICC_IsNewCardPresent())
        return;
//        
//    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial())   //����ע�͵�
        return;

    // Show some details of the PICC (that is: the tag/card)
    Serial.print(F("Card UID:"));
    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.println();
 
    sprintf (cStr, " %02X%02X%02X%02X", mfrc522.uid.uidByte[ 0 ], mfrc522.uid.uidByte[ 1 ], mfrc522.uid.uidByte[ 2 ], mfrc522.uid.uidByte[ 3 ] );
    
     OLED_ShowString(32, 0, cStr,16);      
       beep_on;
       delay(50);
        beep_off;
    delay(500);
}


void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
//        if(buffer[i]>='0') //�ų��ո�
//          card_number=(buffer[i]-48);
//        else 
//          card_number+=(buffer[i]-48)*10;
    }
}
//���Ժ���
void OLED_ColorTurn(u8 i)
{
  if (!i) OLED_WR_Byte(0xA6, OLED_CMD); //������ʾ
  else  OLED_WR_Byte(0xA7, OLED_CMD); //��ɫ��ʾ
}

//��Ļ��ת180��
void OLED_DisplayTurn(u8 i)
{
  if (i == 0)
  {
    OLED_WR_Byte(0xC8, OLED_CMD); //������ʾ
    OLED_WR_Byte(0xA1, OLED_CMD);
  }
  else
  {
    OLED_WR_Byte(0xC0, OLED_CMD); //��ת��ʾ
    OLED_WR_Byte(0xA0, OLED_CMD);
  }
}

//����һ���ֽ�
//��SSD1306д��һ���ֽڡ�
//mode:����/�����־ 0,��ʾ����;1,��ʾ����;
void OLED_WR_Byte(u8 dat, u8 mode)
{
  Wire.beginTransmission(0x3c);
  if (mode) {
    Wire.write(0x40);
  }
  else {
    Wire.write(0x00);
  }
  Wire.write(dat); // sends one byte
  Wire.endTransmission();    // stop transmitting
}

//��������

void OLED_Set_Pos(u8 x, u8 y)
{
  OLED_WR_Byte(0xb0 + y, OLED_CMD);
  OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
  OLED_WR_Byte((x & 0x0f), OLED_CMD);
}
//����OLED��ʾ
void OLED_Display_On(void)
{
  OLED_WR_Byte(0X8D, OLED_CMD); //SET DCDC����
  OLED_WR_Byte(0X14, OLED_CMD); //DCDC ON
  OLED_WR_Byte(0XAF, OLED_CMD); //DISPLAY ON
}
//�ر�OLED��ʾ
void OLED_Display_Off(void)
{
  OLED_WR_Byte(0X8D, OLED_CMD); //SET DCDC����
  OLED_WR_Byte(0X10, OLED_CMD); //DCDC OFF
  OLED_WR_Byte(0XAE, OLED_CMD); //DISPLAY OFF
}
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!
void OLED_Clear(void)
{
  u8 i, n;
  for (i = 0; i < 8; i++)
  {
    OLED_WR_Byte (0xb0 + i, OLED_CMD); //����ҳ��ַ��0~7��
    OLED_WR_Byte (0x00, OLED_CMD);     //������ʾλ�á��е͵�ַ
    OLED_WR_Byte (0x10, OLED_CMD);     //������ʾλ�á��иߵ�ַ
    for (n = 0; n < 128; n++)OLED_WR_Byte(0, OLED_DATA);
  } //������ʾ
}


//��ָ��λ����ʾһ���ַ�
//x:0~127
//y:0~63
//sizey:ѡ������ 6x8  8x16
void OLED_ShowChar(u8 x, u8 y, const u8 chr, u8 sizey)
{
  u8 c = 0, sizex = sizey / 2, temp;
  u16 i = 0, size1;
  if (sizey == 8)size1 = 6;
  else size1 = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * (sizey / 2);
  c = chr - ' '; //�õ�ƫ�ƺ��ֵ
  OLED_Set_Pos(x, y);
  for (i = 0; i < size1; i++)
  {
    if (i % sizex == 0 && sizey != 8) OLED_Set_Pos(x, y++);
    if (sizey == 8)
    {
      temp = pgm_read_byte(&asc2_0806[c][i]);
      OLED_WR_Byte(temp, OLED_DATA); //6X8�ֺ�
    }
    else if (sizey == 16)
    {
      temp = pgm_read_byte(&asc2_1608[c][i]);
      OLED_WR_Byte(temp, OLED_DATA); //8x16�ֺ�
    }
    else return;
  }
}
//m^n����
u32 oled_pow(u8 m, u8 n)
{
  u32 result = 1;
  while (n--)result *= m;
  return result;
}
//��ʾ����
//x,y :�������
//num:Ҫ��ʾ������
//len :���ֵ�λ��
//sizey:�����С
void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 sizey)
{
  u8 t, temp, m = 0;
  u8 enshow = 0;
  if (sizey == 8)m = 2;
  for (t = 0; t < len; t++)
  {
    temp = (num / oled_pow(10, len - t - 1)) % 10;
    if (enshow == 0 && t < (len - 1))
    {
      if (temp == 0)
      {
        OLED_ShowChar(x + (sizey / 2 + m)*t, y, ' ', sizey);
        continue;
      } else enshow = 1;
    }
    OLED_ShowChar(x + (sizey / 2 + m)*t, y, temp + '0', sizey);
  }
}
//��ʾһ���ַ��Ŵ�
void OLED_ShowString(u8 x, u8 y, const char *chr, u8 sizey)
{
  u8 j = 0;
  while (chr[j] != '\0')
  {
    OLED_ShowChar(x, y, chr[j++], sizey);
    if (sizey == 8)x += 6;
    else x += sizey / 2;
  }
}
//��ʾ����
void OLED_ShowChinese(u8 x, u8 y, const u8 no, u8 sizey)
{
  u16 i, size1 = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
  u8 temp;
  for (i = 0; i < size1; i++)
  {
    if (i % sizey == 0) OLED_Set_Pos(x, y++);
    if (sizey == 16)
    {
      temp = pgm_read_byte(&Hzk[no][i]);
      OLED_WR_Byte(temp, OLED_DATA); //16x16�ֺ�
    }
    //    else if(sizey==xx) OLED_WR_Byte(xxx[c][i],OLED_DATA);//�û�����ֺ�
    else return;
  }
}


//��ʾͼƬ
//x,y��ʾ����
//sizex,sizey,ͼƬ����
//BMP��Ҫ��ʾ��ͼƬ
void OLED_DrawBMP(u8 x, u8 y, u8 sizex, u8 sizey, const u8 BMP[])
{
  u16 j = 0;
  u8 i, m, temp;
  sizey = sizey / 8 + ((sizey % 8) ? 1 : 0);
  for (i = 0; i < sizey; i++)
  {
    OLED_Set_Pos(x, i + y);
    for (m = 0; m < sizex; m++)
    {
      temp = pgm_read_byte(&BMP[j++]);
      OLED_WR_Byte(temp, OLED_DATA);
    }
  }
} //OLED�ĳ�ʼ��
void OLED_Init(void)
{
  pinMode(res, OUTPUT); //RES
  Wire.begin(0x3c); // join i2c bus (address optional for master)
  OLED_RES_Clr();
  delay(200);
  OLED_RES_Set();

  OLED_WR_Byte(0xAE, OLED_CMD); //--turn off oled panel
  OLED_WR_Byte(0x00, OLED_CMD); //---set low column address
  OLED_WR_Byte(0x10, OLED_CMD); //---set high column address
  OLED_WR_Byte(0x40, OLED_CMD); //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
  OLED_WR_Byte(0x81, OLED_CMD); //--set contrast control register
  OLED_WR_Byte(0xCF, OLED_CMD); // Set SEG Output Current Brightness
  OLED_WR_Byte(0xA1, OLED_CMD); //--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
  OLED_WR_Byte(0xC8, OLED_CMD); //Set COM/Row Scan Direction   0xc0���·��� 0xc8����
  OLED_WR_Byte(0xA6, OLED_CMD); //--set normal display
  OLED_WR_Byte(0xA8, OLED_CMD); //--set multiplex ratio(1 to 64)
  OLED_WR_Byte(0x3f, OLED_CMD); //--1/64 duty
  OLED_WR_Byte(0xD3, OLED_CMD); //-set display offset Shift Mapping RAM Counter (0x00~0x3F)
  OLED_WR_Byte(0x00, OLED_CMD); //-not offset
  OLED_WR_Byte(0xd5, OLED_CMD); //--set display clock divide ratio/oscillator frequency
  OLED_WR_Byte(0x80, OLED_CMD); //--set divide ratio, Set Clock as 100 Frames/Sec
  OLED_WR_Byte(0xD9, OLED_CMD); //--set pre-charge period
  OLED_WR_Byte(0xF1, OLED_CMD); //Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
  OLED_WR_Byte(0xDA, OLED_CMD); //--set com pins hardware configuration
  OLED_WR_Byte(0x12, OLED_CMD);
  OLED_WR_Byte(0xDB, OLED_CMD); //--set vcomh
  OLED_WR_Byte(0x40, OLED_CMD); //Set VCOM Deselect Level
  OLED_WR_Byte(0x20, OLED_CMD); //-Set Page Addressing Mode (0x00/0x01/0x02)
  OLED_WR_Byte(0x02, OLED_CMD); //
  OLED_WR_Byte(0x8D, OLED_CMD); //--set Charge Pump enable/disable
  OLED_WR_Byte(0x14, OLED_CMD); //--set(0x10) disable
  OLED_WR_Byte(0xA4, OLED_CMD); // Disable Entire Display On (0xa4/0xa5)
  OLED_WR_Byte(0xA6, OLED_CMD); // Disable Inverse Display On (0xa6/a7)
  OLED_Clear();
  OLED_WR_Byte(0xAF, OLED_CMD); /*display ON*/
}


  byte bufferATQA[2];
  byte bufferSize = sizeof(bufferATQA);
//Serial.println("hello");    //abing
  // Reset baud rates
//  PCD_WriteRegister(TxModeReg, 0x00);
//  PCD_WriteRegister(RxModeReg, 0x00);
  // Reset ModWidthReg
//  PCD_WriteRegister(ModWidthReg, 0x26);
//
//  MFRC522::StatusCode result = PICC_RequestA(bufferATQA, &bufferSize);
//  return (result == STATUS_OK || result == STATUS_COLLISION);
// End PICC_IsNewCardPresent()
