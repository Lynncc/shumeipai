#include <stdio.h>
#include "string.h"
#include "oled.h"
#include "codetab.h"
#include <wiringPi.h>


#define LINE_PRINT 0

#if LINE_PRINT
	#define LINE printf("[%s] %s:%d\n", __FILE__, __func__, __LINE__);
#else
	#define LINE ;
#endif


static void __hexdump(unsigned char *p, unsigned int len);

void I2C_Configuration(void)
{
	while(wiringPiSetup() == -1)
		sleep(5);
	printf("wiringPiSetup success.\n");
	pinMode(VCC_PIN,OUTPUT);
	digitalWrite(VCC_PIN,1);
	pinMode(SCL_PIN,OUTPUT);
	pinMode(SDA_PIN,OUTPUT);
}

void setPin(int pin, int value)
{
	pinMode(pin,OUTPUT);
	digitalWrite (pin, value);
}

int readPin(int pin)
{
	// pinMode(pin,INPUT);
	// pullUpDnControl (pin, PUD_OFF);
	return digitalRead(pin);
}

void I2C_delay(void)
{
	unsigned char i=0;
	for (i = 0; i < 2; ++i)
	{
		// usleep(1);
		;
	}
	// usleep(1);
}
int I2C_Start(void)
{
	SDAH;
	SCLH;
	I2C_delay();
	if(!SDAread)
	{
		LINE
		return 0;
	}
	SDAL;
	I2C_delay();
	if(SDAread)
	{
		LINE
		return 0;
	}
	SDAL;
	I2C_delay();
	return 1;
}
void I2C_Stop(void)
{
	SCLL;
	I2C_delay();
	SDAL;
	I2C_delay();
	SCLH;
	I2C_delay();
	SDAH;
	I2C_delay();
}
void I2C_Ack(void)
{
	SCLL;
	I2C_delay();
	SDAL;
	I2C_delay();
	SCLH;
	I2C_delay();
	SCLL;
	I2C_delay();
}
void I2C_NoAck(void)
{
	SCLL;
	I2C_delay();
	SDAH;
	I2C_delay();
	SCLH;
	I2C_delay();
	SCLL;
	I2C_delay();
}
int I2C_WaitAck(void)
{
	SCLL;
	I2C_delay();
	SDAH;
	I2C_delay();
	SCLH;
	I2C_delay();
	if(SDAread)
	{
		SCLL;
		return 0;
	}
	SCLL;
	return 1;
}
void I2C_SendByte(unsigned char SendByte)
{
	unsigned char i=8;
	while(i--)
	{
		SCLL;
		I2C_delay();
		if(SendByte&0x80)
			SDAH;
		else
			SDAL;
		SendByte<<=1;
		I2C_delay();
		SCLH;
		I2C_delay();
	}
	SCLL;
}


int I2C_WriteByte(unsigned char WriteAddress,unsigned char SendByte)
{
	int j;
	if(!I2C_Start())
	{
		LINE;
		return 0;
	}
	I2C_SendByte( OLED_ADDRESS & 0xFFFE);
	if(!I2C_WaitAck())
	{
		I2C_Stop();
		LINE
		return 0;
	}
	I2C_SendByte(WriteAddress);
	I2C_WaitAck();
	I2C_SendByte(SendByte);
	I2C_WaitAck();
	I2C_Stop();
	LINE
//    delay_ms(1);
	return 1;
}


void WriteCmd(unsigned char I2C_Command)
{
	I2C_WriteByte(0x00, I2C_Command);
}



void WriteDat(unsigned char I2C_Data)
{
	I2C_WriteByte(0x40, I2C_Data);
}



void OLED_Init(void)
{
//	DelayMs(100); //鏉╂瑩鍣烽惃鍕閺冭泛绶㈤柌宥堫洣
	delay(100);

	WriteCmd(0xAE); //display off
	WriteCmd(0x20);	//Set Memory Addressing Mode
	WriteCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	WriteCmd(0xc8);	//Set COM Output Scan Direction
	WriteCmd(0x00); //---set low column address
	WriteCmd(0x10); //---set high column address
	WriteCmd(0x40); //--set start line address
	WriteCmd(0x81); //--set contrast control register
	WriteCmd(0xff); //娴滎喖瀹崇拫鍐Ν 0x00~0xff
	WriteCmd(0xa1); //--set segment re-map 0 to 127
	WriteCmd(0xa6); //--set normal display
	WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
	WriteCmd(0x3F); //
	WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	WriteCmd(0xd3); //-set display offset
	WriteCmd(0x00); //-not offset
	WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	WriteCmd(0xf0); //--set divide ratio
	WriteCmd(0xd9); //--set pre-charge period
	WriteCmd(0x22); //
	WriteCmd(0xda); //--set com pins hardware configuration
	WriteCmd(0x12);
	WriteCmd(0xdb); //--set vcomh
	WriteCmd(0x20); //0x20,0.77xVcc
	WriteCmd(0x8d); //--set DC-DC enable
	WriteCmd(0x14); //
	WriteCmd(0xaf); //--turn on oled panel
}


void OLED_SetPos(unsigned char x, unsigned char y)
{
	WriteCmd(0xb0+y);
	WriteCmd(((x&0xf0)>>4)|0x10);
	WriteCmd((x&0x0f)|0x01);
}


void OLED_Fill(unsigned char fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		WriteCmd(0xb0+m);		//page0-page1
		WriteCmd(0x00);		//low column start address
		WriteCmd(0x10);		//high column start address
		for(n=0;n<128;n++)
		{
			WriteDat(fill_Data);
		}
	}
}

void OLED_Line(unsigned char line, unsigned char fill_Data)
{
	unsigned char m,n;
	// for(m=0;m<8;m++)
	{
		WriteCmd(0xb0+line);		//page0-page1
		WriteCmd(0x00);		//low column start address
		WriteCmd(0x10);		//high column start address
		for(n=0;n<128;n++)
		{
			WriteDat(fill_Data);
		}
	}
}


void OLED_CLS(void)
{
	OLED_Fill(0x00);
}


void OLED_ON(void)
{
	WriteCmd(0X8D);
	WriteCmd(0X14);
	WriteCmd(0XAF);
}

void OLED_OFF(void)
{
	WriteCmd(0X8D);
	WriteCmd(0X10);
	WriteCmd(0XAE);
}



void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
{
	unsigned char c = 0,i = 0,j = 0;
	switch(TextSize)
	{
		case 1:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 126)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<6;i++)
					WriteDat(F6x8[c][i]);
				x += 6;
				j++;
			}
		}break;
		case 2:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 120)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i]);
				OLED_SetPos(x,y+1);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i+8]);
				x += 8;
				j++;
			}
		}break;
	}
}


FILE *file_init(unsigned char *path)
{
	return fopen(path, "rb");

}
int GetImage(FILE *fd, unsigned char* pBuffer,int frame)
{
	int pos;

	if (fd == NULL)
	{
		return -1;
	}
	pos = frame*1024;

	fseek (fd, pos, SEEK_SET);
	fread(pBuffer, 1024, 1, fd);


	return 0;

}

void file_close(FILE *fd)
{
	fclose(fd);
}




 /**
  * @brief  OLED_DrawBMP閿涘本妯夌粈绡塎P娴ｅ秴娴
  * @param  x0,y0 :鐠у嘲顫愰悙鐟版綏閺(x0:0~127, y0:0~7);
	*					x1,y1 : 鐠ч鍋ｇ电顫楃痪(缂佹挻娼悙)閻ㄥ嫬娼楅弽(x1:1~128,y1:1~8)
	* @retval 閺
  */
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
{
	unsigned int j=0;
	unsigned char x,y;
	int m,n;

 //  if(y1%8==0)
	// 	y = y1/8;
 //  else
	// 	y = y1/8 + 1;
	// for(y=y0;y<y1;y++)
	// {
	// 	OLED_SetPos(x0,y);
	// 	for(x=x0;x<x1;x++)
	// 	{
	// 		WriteDat(BMP[j++]);
	// 	}
	// }


	for(m=0;m<8;m++)
	{
		WriteCmd(0xb0+m);		//page0-page1
		WriteCmd(0x00);		//low column start address
		WriteCmd(0x10);		//high column start address
		for(n=0;n<128;n++)
		{
			WriteDat(BMP[j++]);
		}
	}
}

static void __hexdump(unsigned char *p, unsigned int len)
{
	unsigned char *line = p;
	unsigned int i, thisline, offset = 0;

	while (offset < len) {
		printf("%04x ", offset);
		thisline = len - offset;
		if (thisline > 16)
			thisline = 16;

		for (i = 0; i < thisline; i++)
			printf("%02x ", line[i]);

		for (; i < 16; i++)
			printf("   ");

		printf("|");
		for (i = 0; i < thisline; i++)
			printf("%c", (line[i] >= 0x20 && line[i] < 0x7f) ? line[i] : '.');
		for (; i < 16; i++)
			printf(" ");
		printf("|\n");

		offset += thisline;
		line += thisline;
	}
}
