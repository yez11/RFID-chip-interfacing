#include <avr/io.h>
#include <util/delay.h>
#include <lcd.h>
#include <utils.h>
#include <spi.h>
#include <mfrc522.h>

uint8_t SelfTestBuffer[64];


void LCDWriteStringXY(uint16_t x,uint16_t y,char *str)
{
	lcd_gotoxy(x,y);          
	lcd_puts(str);	
}

void LCDHexDumpXY(uint8_t x, uint8_t y,uint8_t d)
{
	lcd_gotoxy(x,y);
	uint8_t byte = '0';
	(((d>>4)&0x0F)<=9) ? (byte='0'+((d>>4)&0x0F)) : (byte='A'+ ((d>>4)&0x0F)-0x0A);
	lcd_putc(byte);
	_delay_us(100);	
	((d&0x0F)<=9) ? (byte='0'+ (d&0x0F)) : (byte='A'+ (d&0x0F)-0x0A);
	lcd_putc(byte);
	_delay_us(100);
}


int main()
{
	uint8_t byte;
	uint8_t str[MAX_LEN];
	_delay_ms(50);

	lcd_init(LCD_DISP_ON);    // init lcd and turn on
	LCDWriteStringXY(2,0,"RFID Reader");
	LCDWriteStringXY(5,1,VERSION_STR);
	
	
	_delay_ms(1000);
	lcd_clrscr();
	
	//init reader
	spi_init();
	mfrc522_init();
	
	//check version of the reader
	byte = mfrc522_read(VersionReg);
	if(byte == 0x92)
	{
		LCDWriteStringXY(2,0,"MIFARE RC522v2");
		LCDWriteStringXY(4,1,"Detected");
	}else if(byte == 0x91 || byte==0x90)
	{
		LCDWriteStringXY(2,0,"MIFARE RC522v1");
		LCDWriteStringXY(4,1,"Detected");
	}else
	{
		LCDWriteStringXY(0,0,"No reader found");
	}
	byte = mfrc522_read(ComIEnReg);
	mfrc522_write(ComIEnReg,byte|0x20);
	byte = mfrc522_read(DivIEnReg);
	mfrc522_write(DivIEnReg,byte|0x80);
	
	_delay_ms(1500);
	lcd_clrscr();
	
	while(1){
		byte = mfrc522_request(PICC_REQALL,str);
		//LCDHexDumpXY(0,0,byte);
		
		if(byte == CARD_FOUND)
		{
			byte = mfrc522_get_card_serial(str);
			if(byte == CARD_FOUND)
			{
				for(byte=0;byte<8;byte++)
				LCDHexDumpXY(byte*2,0,str[byte]);
				
				_delay_ms(2500);
			}
			else
			{
				LCDWriteStringXY(0,1,"Error");
			}
		}
		
		_delay_ms(1000);
	}
}
