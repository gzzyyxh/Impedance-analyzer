from machine import Pin
import utime

#address port
A0 = Pin(0,Pin.OUT)
A1 = Pin(1,Pin.OUT)
A2 = Pin(2,Pin.OUT)
A3 = Pin(3,Pin.OUT)
A4 = Pin(4,Pin.OUT)
A5 = Pin(5,Pin.OUT)

#data port
D0 = Pin(8,Pin.OUT)
D1 = Pin(9,Pin.OUT)
D2 = Pin(10,Pin.OUT)
D3 = Pin(11,Pin.OUT)
D4 = Pin(12,Pin.OUT)
D5 = Pin(13,Pin.OUT)
D6 = Pin(14,Pin.OUT)
D7 = Pin(15,Pin.OUT)

#crtl port
SP = Pin(16,Pin.OUT)
RT = Pin(17,Pin.OUT)
UD = Pin(18,Pin.OUT)
OSK = Pin(19,Pin.OUT)
FD = Pin(20,Pin.OUT)
WR = Pin(21,Pin.OUT)
RD = Pin(22,Pin.OUT)
NC = Pin(23,Pin.OUT)

CLK_Set = 10
Freq_mult_ulong = 938250
Freq_mult_doulle = 938249.9224
FreqWord = [0,0,0,0,0,0]

def AD9854_Init():
    global WR,RD,UD,RT,SP
    WR.value(1)
    RD.value(1)
    UD.value(0)
    RT.value(1)
    utime.sleep_us(10)
    RT.value(0)
    SP.value(1)
    
    # AD9854_WR_Byte(0x1d,0x10)
    AD9854_WR_Byte(0x1d,0x00)
    AD9854_WR_Byte(0x1e,CLK_Set)
    AD9854_WR_Byte(0x1f,0x01)
    AD9854_WR_Byte(0x20,0x60)
    
    UD.value(1)
    UD.value(0)

def Freq_convert(fre):
    global Freq_mult_ulong,FreqWord
    temp = Freq_mult_ulong
    Array_Freq = [0,0,0,0]
    Array_Freq[0] = fre&0xff
    Array_Freq[1] = (fre>>8)&0xff
    Array_Freq[2] = (fre>>16)&0xff
    Array_Freq[3] = (fre>>24)&0xff
    
    FreqBuf = temp*Array_Freq[0]
    FreqWord[0] = FreqBuf
    FreqBuf = FreqBuf >> 8
    
    FreqBuf = FreqBuf + temp*Array_Freq[1]
    FreqWord[1] = FreqBuf
    FreqBuf = FreqBuf >> 8
    
    FreqBuf = FreqBuf + temp*Array_Freq[2]
    FreqWord[2] = FreqBuf
    FreqBuf = FreqBuf >> 8
    
    FreqBuf = FreqBuf + temp*Array_Freq[3]
    FreqWord[3] = FreqBuf
    FreqBuf = FreqBuf >> 8
    
    FreqWord[4] = FreqBuf
    FreqWord[5] = FreqBuf >> 8

def AD9854_WR_Byte(addr,dat):
    A0.value(addr&0x1)
    A1.value((addr>>1)&0x1)
    A2.value((addr>>2)&0x1)
    A3.value((addr>>3)&0x1)
    A4.value((addr>>4)&0x1)
    A5.value((addr>>5)&0x1)
    
    D0.value(dat&0x1)
    D1.value((dat>>1)&0x1)
    D2.value((dat>>2)&0x1)
    D3.value((dat>>3)&0x1)
    D4.value((dat>>4)&0x1)
    D5.value((dat>>5)&0x1)
    D6.value((dat>>6)&0x1)
    D7.value((dat>>7)&0x1)
    
    WR.value(0)
    WR.value(1)    

def AD9854_SetSine(fre,shape):
    adress = 0x04
    count = 6
    Freq_convert(fre)
    
    while(count):
        count = count - 1
        AD9854_WR_Byte(adress,FreqWord[count])
        adress = adress + 1
        
    AD9854_WR_Byte(0x21,(shape>>8)&0x00FF)
    AD9854_WR_Byte(0x22,(shape&0x00FF))
    
    AD9854_WR_Byte(0x23,(shape>>8)&0x00FF)
    AD9854_WR_Byte(0x24,(shape&0x0FF))
    UD.value(1)
    UD.value(0)
    
        

utime.sleep_ms(100);

AD9854_Init();

fre = 200

if(fre > 99999999):
    fre = 99999999

AD9854_SetSine(fre,4095)

while(1):
    pass