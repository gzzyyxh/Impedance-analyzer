from machine import Pin
import utime

SDIO = Pin(16,Pin.OUT)  #A0
#SDO = Pin(1,Pin.OUT)    #A1
IORST = Pin(17,Pin.OUT)  #A2
RD = Pin(18,Pin.OUT)
WR = Pin(19,Pin.OUT)
UDCLK = Pin(20,Pin.OUT)  
RESET = Pin(21,Pin.OUT)
SP = Pin(22,Pin.OUT)

CLK_Set = 10
Freq_mult_ulong = 938250
Freq_mult_doulle = 938249.9224
FreqWord = [0,0,0,0,0,0]

PHASE1 = 0x00	    #phase adjust register #1
PHASE2 = 0x01		#phase adjust register #2
FREQ1 = 0x02		#frequency tuning word 1
FREQ2 = 0x03		#/frequency tuning word 2
DELFQ = 0x04		#delta frequency word
UPDCK = 0x05		#update clock
RAMPF = 0x06		#ramp rate clock
CONTR = 0x07		#control register
SHAPEI = 0x08		#output shape key I mult
SHAPEQ = 0x09		#output shape key Q mult 
RAMPO = 0x0A		#output shape key ramp rate
CDAC = 0x0B		    #QDAC

def AD9854_WR_Byte(data):
    i = 8
    while(i > 0):
        if(data&0x80):
            SDIO.value(1)
        else:
            SDIO.value(0)
        data = data << 1
        WR.value(0)
        WR.value(1)
        i = i - 1

def AD9854_Init():
    SP.value(0)
    WR.value(0)
    UDCLK.value(0)
    RESET.value(1)
    utime.sleep_us(10)
    RESET.value(0)
    IORST.value(0)
    RD.value(0)
    
    AD9854_WR_Byte(CONTR)
    # AD9854_WR_Byte(0x10)
    AD9854_WR_Byte(0x00)
    
    AD9854_WR_Byte(CLK_Set)
    AD9854_WR_Byte(0x00)
    AD9854_WR_Byte(0x60)
    
    UDCLK.value(1)
    UDCLK.value(0)

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

def AD9854_SetSine(fre,shape):
    i = 0
    count = 6
    Freq_convert(fre)
    
    while(count > 0):
        if(i == 0):
            AD9854_WR_Byte(FREQ1)
        count = count - 1
        AD9854_WR_Byte(FreqWord[count])
        i = i + 1
        
    AD9854_WR_Byte(SHAPEI)
    AD9854_WR_Byte(shape>>8)
    AD9854_WR_Byte(shape&0xFF)
    
    AD9854_WR_Byte(SHAPEQ)
    AD9854_WR_Byte(shape>>8)
    AD9854_WR_Byte(shape&0xFF)

    UDCLK.value(1)
    UDCLK.value(0)

utime.sleep_ms(100)

AD9854_Init()

freq = 1000

if(freq > 99999999):
    freq = 99999999
AD9854_SetSine(freq,4095)
while(1):
    pass



