#ifndef __OLED_I2C_H
#define	__OLED_I2C_H

#define IMAGE_PATH "/home/zhaowei/dev/oled_badapple/badapple.bin"
#define OLED_ADDRESS	0x78 //通过调整0R电阻,屏可以0x78和0x7A两个地址 -- 默认0x78

#define VCC_PIN 26
#define SCL_PIN 27
#define SDA_PIN 28

/*置一*/
#define SCLH        setPin(SCL_PIN,1)
/*清零*/
#define SCLL        setPin(SCL_PIN,0)

#define SDAH        setPin(SDA_PIN,1)
#define SDAL        setPin(SDA_PIN,0)
/*读取输入数据*/
#define SCLread     digitalRead(SCL_PIN)
#define SDAread     digitalRead(SDA_PIN)
;
/********************************************************************************


	* Function List:
	*	1. void I2C_Configuration(void) -- 配置CPU的硬件I2C
	* 2. void I2C_WriteByte(uint8_t addr,uint8_t data) -- 向寄存器地址写一个byte的数据
	* 3. void WriteCmd(unsigned char I2C_Command) -- 写命令
	* 4. void WriteDat(unsigned char I2C_Data) -- 写数据
	* 5. void OLED_Init(void) -- OLED屏初始化
	* 6. void OLED_SetPos(unsigned char x, unsigned char y) -- 设置起始点坐标
	* 7. void OLED_Fill(unsigned char fill_Data) -- 全屏填充
	* 8. void OLED_CLS(void) -- 清屏
	* 9. void OLED_ON(void) -- 唤醒
	* 10. void OLED_OFF(void) -- 睡眠
	* 11. void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize) -- 显示字符串(字体大小有6*8和8*16两种)
	* 12. void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N) -- 显示中文(中文需要先取模，然后放到codetab.h中)
	* 13. void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]) -- BMP图片
	*
  *
  ******************************************************************************
  */
void I2C_Configuration(void);
int I2C_WriteByte(unsigned char WriteAddress,unsigned char SendByte);
void WriteCmd(unsigned char I2C_Command);
void WriteDat(unsigned char I2C_Data);
void OLED_Init(void);
void OLED_SetPos(unsigned char x, unsigned char y);
void OLED_Fill(unsigned char fill_Data);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char *CN);
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);

FILE *file_init(unsigned char *path);
int GetImage(FILE *fd, unsigned char* pBuffer,int frame);
void file_close(FILE *fd);
#endif
