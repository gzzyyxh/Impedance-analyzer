from machine import SPI, Pin, Timer, UART
import ubinascii
import utime
import math
import sys

REG = {
    'COMMS': 0x00,
    'STATUS': 0x00,
    'ADCMODE': 0x01,
    'IFMODE': 0x02,
    'REGCHECK': 0x03,
    'DATA': 0x04,
    'GPIOCON': 0x06,
    'ID': 0x07,

    'CH0': 0x10,
    'CH1': 0x11,
    'CH2': 0x12,
    'CH3': 0x13,

    'SETUPCON0': 0x20,
    'SETUPCON1': 0x21,
    'SETUPCON2': 0x22,
    'SETUPCON3': 0x23,

    'FILTCON0': 0x28,
    'FILTCON1': 0x29,
    'FILTCON2': 0x2A,
    'FILTCON3': 0x2B,

    'OFFSET0': 0x30,
    'OFFSET1': 0x31,
    'OFFSET2': 0x32,
    'OFFSET3': 0x33,

    'GAIN0': 0x38,
    'GAIN1': 0x39,
    'GAIN2': 0x3A,
    'GAIN3': 0x3B
}

reglist = {
    'COMMS': 'Communication register',
    'STATUS': 'status register',
    'ADCMODE': 'ADCmode register',
    'IFMODE': 'Interface mode register',
    'REGCHECK': 'reg-check register',
    'DATA': 'data register',
    'GPIOCON': 'gpio configuration register',
    'ID': 'id register',

    'CH0': 'channel0 register',
    'CH1': 'channel1 register',
    'CH2': 'channel2 register',
    'CH3': 'channel3 register',

    'SETUPCON0': 'setup0 configuration register',
    'SETUPCON1': 'setup1 configuration register',
    'SETUPCON2': 'setup2 configuration register',
    'SETUPCON3': 'setup3 configuration register',

    'FILTCON0': 'filter0 configuration register',
    'FILTCON1': 'filter1 configuration register',
    'FILTCON2': 'filter2 configuration register',
    'FILTCON3': 'filter3 configuration register',

    'OFFSET0': 'offset0 register',
    'OFFSET1': 'offset1 register',
    'OFFSET2': 'offset2 register',
    'OFFSET3': 'offset3 register',

    'GAIN0': 'gain0 register',
    'GAIN1': 'gain1 register',
    'GAIN2': 'gain2 register',
    'GAIN3': 'gain3 register'
}

OP = {'w': 0x00, 'r': 0x40}

MISO = Pin(0, Pin.IN)
CSn = Pin(1, Pin.OUT)
SCLK = Pin(2, Pin.OUT)
MOSI = Pin(3, Pin.OUT)
spi = SPI(0, baudrate = 10000000, polarity = 1, phase = 1, bits = 8, firstbit = SPI.MSB, sck = Pin(2), mosi = Pin(3), miso = Pin(0))

def Reset():    #复位
    i = 64
    buf = bytearray()
    buf.append(0x00)
    MOSI.value(1)
    CSn.value(0)
    while i > 0:
        spi.write(buf)
        i = i - 1
        utime.sleep_us(10)
    CSn.value(1)
    MOSI.value(0)
    utime.sleep_us(500)
    print("Reset.")

def Access(op, reg, value = 0x00, bytes = 3):
    buf = bytearray()
    buf.append(OP[op] + REG[reg])
    CSn.value(0)
    spi.write(buf)

    if op == 'r':
        # v = ubinascii.hexlify(spi.read(3))
        v = hex(int.from_bytes(spi.read(bytes), 'big'))
        CSn.value(1)
        # print(reglist[reg], ": ", v)
        # uart1.write((reglist[reg] + ": " + str(v) + '\n').encode("gbk"))
        return v

    if op == 'w':
        buf = bytearray()
        while value > 0:
            buf.append(value)
            value = value >> 8
        spi.write(buf)
        CSn.value(1)

def VIN_24(data, gain, offset, vref = 2.5):
    return ((int(data) / 2 * 0x400000 / int(gain)) + (int(offset) - 0x800000)) / pow(2, 23) * vref / 0.75 * 4.3

def VIN_32(data, gain, offset, vref = 2.5):
    #return (((int(data) /2) * 0x400000 / int(gain)) + (int(offset) - 0x800000)) / pow(2, 31) * vref / 0.75 * 4.3
    return (((data - 0x80000000) * 0x400000 / int(gain)) + (int(offset) - 0x800000)) / pow(2, 31) * vref / 0.75 * 4.3

def mean(data):
    sum = 0
    for x in data:
        sum = sum + VIN_32(x,gain0,offset0)
    return sum/len(data)
    

Time_list_I = []
Time_list_Q = []
Data_list_I = []
Data_list_Q = []
count_I = 0
count_Q = 0
PI = 3.14159265358
Reset()

print('#')
print("ID: ", Access('r','ID', bytes = 2))
Access('w', 'CH0', value = 0x0280)
# print('CH0',Access('r','CH0',bytes = 2))
Access('w', 'CH1', value = 0x2280)
# print('CH1',Access('r','CH1',bytes = 2))

Access('w', 'SETUPCON0', value = 0x2013)
Access('w', 'SETUPCON1', value = 0x2013)
print("SETUPCON0: ", Access('r','SETUPCON0', bytes = 2))
print("SETUPCON1: ", Access('r', 'SETUPCON1', bytes = 2))

#Access('w', 'GAIN0', value = 0x555555)
# Access('w', 'GAIN1', value = 0x555555)
gain0 = Access('r', 'GAIN0', bytes = 3)
gain1 = Access('r', 'GAIN1', bytes = 3)
offset0 = Access('r', 'OFFSET0', bytes = 3)
offset1 = Access('r', 'OFFSET1', bytes = 3)
# print("GAIN0: ", gain0)
# print("GAIN1: ", gain1)
# print("OFFSET0: ", offset0)
# print("OFFSET1: ", offset1)

# Access('w', 'IFMODE', value = 0x0000)
# print("IFMODE:",Access('r', 'IFMODE', bytes = 2))

# data = Access('r', 'DATA', bytes = 3)
# vin_24 = VIN_24(data, gain0, offset0)
# print("VIN_24: ", vin_24)

Access('w', 'ADCMODE', value = 0x0080)
# print("ADCMODE: ", Access('r', 'ADCMODE', bytes = 2))

Access('w', 'IFMODE', value = 0x4200)  # 连续转换模式
# Access('w', 'IFMODE', value = 0x0201)  # 连续读取模式
print("IFMODE: ", Access('r', 'IFMODE', bytes = 2))

# Access('w', 'FILTCON0', value = 0x0e05)    # 100SPS
Access('w', 'FILTCON0', value = 0x0705)    # 10000SPS
Access('w', 'FILTCON1', value = 0x0705)    # 10000SPS
# Access('w', 'FILTCON0', value = 0x0A05)    # 1000SPS
# print("FILTCON0: ", Access('r', 'FILTCON0', bytes = 2))
# print("FILTCON1: ", Access('r', 'FILTCON1', bytes = 2))
print('@')

# data = Access('r', 'DATA', bytes = 4)
# vin_32 = VIN_32(data, gain0, offset0)
# print("VIN_32: ", vin_32)
# print(offset0 + gain0 + data)

# Access('w', 'IFMODE', value = 0x0201)
# print("IFMODE: ", Access('r', 'IFMODE', bytes = 2))

CSn.value(0)
buf_data = bytearray()
buf_data.append(0x44)
buf_status = bytearray()
buf_status.append(0x40)

#for i in range(0, 20):
while True:
    while True:
        if not MISO.value():
            break
    #spi.write(buf_status)
    #s = spi.read(2)
    #print("STATUS",Access('r','STATUS',bytes = 1))
    #Channle_list.append(hex(int.from_bytes(s,'big')))
    spi.write(buf_data)
    v = int.from_bytes(spi.read(4),'big')
    #print(hex(v))
    if (v&0xf == 0x1):
        Time_list_I.append(utime.ticks_us())
        Data_list_I.append(v)
        count_I = count_I + 1
    if (v&0xf == 0x0):
        Time_list_Q.append(utime.ticks_us())
        Data_list_Q.append(v)
        count_Q = count_Q + 1
    if(count_I >= 50 & count_Q >= 50 ):
        break

CSn.value(1)

# print("I:")
# for line in Time_list_I:
#     print(str(line) + "\t"
#         + str(VIN_32(Data_list_I[Time_list_I.index(line)], gain0, offset0)) + '\t'
#             + hex(Data_list_I[Time_list_I.index(line)]))

# print("Q:")
# for line in Time_list_Q:
#     print(str(line) + "\t"
#         + str(VIN_32(Data_list_Q[Time_list_Q.index(line)], gain0, offset0)) + '\t'
#             + hex(Data_list_Q[Time_list_Q.index(line)]))

mean_I = mean(Data_list_I)
mean_Q = mean(Data_list_Q)
print("mean for I:",mean_I)
print("mean for Q:",mean_Q)
v_f = 0.496/2
v_i = magnitude = math.sqrt(mean_I**2 + mean_Q**2)*PI/2/10/2
tan_phi = -mean_I/mean_Q
phase = math.atan(tan_phi)/PI*180
r = 550
X = r/(v_i/v_f*math.sqrt((mean_Q/mean_I)**2+1))
R = r*(1/(v_i/v_f*math.sqrt((mean_I/mean_Q)**2+1))-1)
f = 1000000
print("mean_I/mean_Q",mean_Q/mean_I)
print("magnitude:",magnitude)
print("phase:",phase)
print("R:",R)
print("X:",X)
print("R+jX:",R,"+j",X)
print("C:",1/2/PI/f/X*1000000,"uF")
print("L:",X/2/PI/f*1000000,"uH")
#print("


Time_list_I.clear()
Data_list_I.clear()
Time_list_Q.clear()
Data_list_Q.clear()
print("%")
