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
void I2C_SendByte(unsigned char SendByte) //????????//
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
	//I2C_SendByte(((WriteAddress & 0x0700) >>7) | DeviceAddress & 0xFFFE);//???????+????
	I2C_SendByte( OLED_ADDRESS & 0xFFFE);//?????
	if(!I2C_WaitAck())
	{
		I2C_Stop();
		LINE
		return 0;
	}
	I2C_SendByte(WriteAddress);   //???????
	I2C_WaitAck();
	I2C_SendByte(SendByte);          //???
	I2C_WaitAck();
	I2C_Stop();
	LINE
//    delay_ms(1);
	return 1;
}


 /**
  * @brief  WriteCmd閿涘苯鎮淥LED閸愭瑥鍙嗛崨鎴掓姢
  * @param  I2C_Command閿涙艾鎳℃禒銈勫敩閻
  * @retval 閺
  */
void WriteCmd(unsigned char I2C_Command)//閸愭瑥鎳℃禒
{
	I2C_WriteByte(0x00, I2C_Command);
}


 /**
  * @brief  WriteDat閿涘苯鎮淥LED閸愭瑥鍙嗛弫鐗堝祦
  * @param  I2C_Data閿涙碍鏆熼幑
  * @retval 閺
  */
void WriteDat(unsigned char I2C_Data)//閸愭瑦鏆熼幑
{
	I2C_WriteByte(0x40, I2C_Data);
}


 /**
  * @brief  OLED_Init閿涘苯鍨垫慨瀣OLED
  * @param  閺
  * @retval 閺
  */
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


 /**
  * @brief  OLED_SetPos閿涘矁顔曠純顔煎帨閺
  * @param  x,閸忓鐖娴ｅ秶鐤
	*					y閿涘苯鍘滈弽鍣涙担宥囩枂
  * @retval 閺
  */
void OLED_SetPos(unsigned char x, unsigned char y) //鐠佸墽鐤嗙挧宄邦潗閻愮懓娼楅弽
{
	WriteCmd(0xb0+y);
	WriteCmd(((x&0xf0)>>4)|0x10);
	WriteCmd((x&0x0f)|0x01);
}

 /**
  * @brief  OLED_Fill閿涘苯锝為崗鍛殻娑擃亜鐫嗛獮
  * @param  fill_Data:鐟曚礁锝為崗鍛畱閺佺増宓
	* @retval 閺
  */
void OLED_Fill(unsigned char fill_Data)//閸忋劌鐫嗘繅顐㈠帠
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

 /**
  * @brief  OLED_CLS閿涘本绔荤仦
  * @param  閺
	* @retval 閺
  */
void OLED_CLS(void)//濞撳懎鐫
{
	OLED_Fill(0x00);
}


 /**
  * @brief  OLED_ON閿涘苯鐨LED娴犲簼绱ら惇鐘辫厬閸炪倝鍟
  * @param  閺
	* @retval 閺
  */
void OLED_ON(void)
{
	WriteCmd(0X8D);  //鐠佸墽鐤嗛悽浣冨祹濞
	WriteCmd(0X14);  //瀵崥顖滄暩閼介攱杞
	WriteCmd(0XAF);  //OLED閸炪倝鍟
}


 /**
  * @brief  OLED_OFF閿涘矁顔OLED娴兼垹婀 -- 娴兼垹婀㈠Ο鈥崇础娑,OLED閸旂喕妞剧瑝閸10uA
  * @param  閺
	* @retval 閺
  */
void OLED_OFF(void)
{
	WriteCmd(0X8D);  //鐠佸墽鐤嗛悽浣冨祹濞
	WriteCmd(0X10);  //閸忔娊妫撮悽浣冨祹濞
	WriteCmd(0XAE);  //OLED娴兼垹婀
}


 /**
  * @brief  OLED_ShowStr閿涘本妯夌粈绡秓detab.h娑擃厾娈慉SCII鐎涙顑,閺6*8閸8*16閸欘垶澶嬪
  * @param  x,y : 鐠у嘲顫愰悙鐟版綏閺(x:0~127, y:0~7);
	*					ch[] :- 鐟曚焦妯夌粈铏规畱鐎涙顑佹稉;
	*					TextSize : 鐎涙顑佹径褍鐨(1:6*8 ; 2:8*16)
	* @retval 閺
  */
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
#define GBK_PATH "/home/zhaowei/dev/oled/Font.dat"

int GetGBKCode(unsigned char* pBuffer,unsigned char * c)
{
	unsigned char High8bit,Low8bit;
	unsigned int pos;
	FILE *fd;

	High8bit=*c;
	Low8bit=*(c+1);

	/*[(0xF7-0xB0)*94+(0xFE-0xA1)]*32*/
	pos = ((High8bit-0xb0)*94+Low8bit-0xa1)*32;
	// f_mount(0, &myfs[0]);
	// myres = f_open(&myfsrc , "0:/HZLIB.bin", FA_OPEN_EXISTING | FA_READ);
	fd = fopen(GBK_PATH, "rb");
	if(fd == NULL)
		return -1;
	fseek (fd, pos, SEEK_SET);
	fread(pBuffer, 32, 1, fd);
	// printf("\n\noffset:0x%x\n", pos);
	// printf("h:%x,L:%x\n", High8bit, Low8bit);
	// __hexdump(c,10);
	// __hexdump(pBuffer,32);
	fclose(fd);
	return 0;

}


static void str_show(int Xpos, int Ypos, unsigned char * __str1, unsigned char * __str2, int len)
{
	int i;
	if (len > 128)
	{
		len = 128;
	}
	OLED_SetPos(Xpos,Ypos);
	for(i = 0; i < len; i++)
		WriteDat(__str1[i]);

	OLED_SetPos(Xpos,Ypos+1);
	for(i = 0; i < len; i++)
		WriteDat(__str2[i]);
}
void cycle_show(int Xpos,int Ypos,unsigned char * __str)
{
	unsigned char str_buf_1[1024];
	unsigned char str_buf_2[1024];

	unsigned char *str=__str;
	unsigned char ch;
	unsigned char ch_buf[32];

	int str_len = strlen(str);
	int len,offset = 0;
	int i;

	memset(str_buf_1,0,1024);
	memset(str_buf_2,0,1024);

	while(*str != '\0')
	{
		if(*str < 125)
		{
			ch = *str - 32;
			memcpy(str_buf_1+offset, &F8X16[ch*16], 8);
			memcpy(str_buf_2+offset, &F8X16[ch*16+8], 8);
			offset +=8;
			str++;
		}
		else
		{
			if(*str == 163)
			{
				ch = ' ';
				memcpy(str_buf_1+offset, &F8X16[ch*16], 8);
				memcpy(str_buf_2+offset, &F8X16[ch*16+8], 8);
				offset += 8;
				str += 2;
				continue;
			}
			GetGBKCode(ch_buf, str);
			memcpy(str_buf_1+offset, ch_buf, 16);
			memcpy(str_buf_2+offset, ch_buf+16, 16);
			str += 2 ;
			offset += 16 ;
		}
	}

	if (offset > 128)
	{
		len = offset-110;
		while(1)
		{
			for (i = 0; i < len; i++)
			{
				str_show(Xpos,Ypos,str_buf_1+i,str_buf_2+i,offset-i);
				usleep(100*1000);
			}
		}
	}else
	{
		str_show(Xpos,Ypos,str_buf_1,str_buf_2,offset);
	}


}

void OLED_ShowChinese(int Xpos,int Ypos,unsigned char *ch_str)
{
	unsigned char ch_buff[35];
	GetGBKCode(ch_buff, ch_str);
	unsigned char i;


	OLED_SetPos(Xpos,Ypos);
	for(i=0;i<16;i++)
		WriteDat(*(ch_buff+i));
	OLED_SetPos(Xpos,Ypos+1);
	for(i=0;i<16;i++)
		WriteDat(*(ch_buff+i+16));


}

void OLED_ShowString(unsigned char Xpos, unsigned char Ypos, unsigned char *str)
{
	int Tmp_x, Tmp_y;
	unsigned char *tmp_str=str;
	Tmp_x = Xpos;
	Tmp_y = Ypos;
	while(*tmp_str != '\0')
	{
		if(*tmp_str<125)
		{
			OLED_ShowStr(Tmp_x, Tmp_y, tmp_str, 2);
			tmp_str++ ;
			Tmp_x += 8 ;
		}
		else
		{
			if(*tmp_str==163)																//163-172 鑴㈣劅鑴拌劏鑴﹁剾闇叉綖娼炶劀	 163-187鑴㈣劅鑴拌劏鑴﹁剾璺劙娼炶劀
			{
				OLED_ShowStr(Tmp_x, Tmp_y, " ", 2);
				tmp_str += 2 ;
				Tmp_x	+= 8 ;
				continue;
			}
			OLED_ShowChinese(Tmp_x, Tmp_y, tmp_str);
			tmp_str += 2 ;
			Tmp_x += 16 ;
		}
	}
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

  if(y1%8==0)
		y = y1/8;
  else
		y = y1/8 + 1;
	for(y=y0;y<y1;y++)
	{
		OLED_SetPos(x0,y);
	for(x=x0;x<x1;x++)
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
