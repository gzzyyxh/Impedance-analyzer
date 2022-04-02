from machine import SPI, Pin, Timer, UART
import ubinascii
import utime
import _thread
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

Time_list = []
Data_list = []
Reset()

print('#')
print("ID: ", Access('r','ID', bytes = 2))

Access('w', 'SETUPCON0', value = 0x2013)
print("SETUPCON0: ", Access('r', 'SETUPCON0', bytes = 2))

offset0 = Access('r', "OFFSET0", bytes = 3)
print("OFFSET0: ", offset0)

Access('w', 'GAIN0', value = 0x555555)
gain0 = Access('r', 'GAIN0', bytes = 3)
print("GAIN0: ", gain0)

# Access('w', 'IFMODE', value = 0x0000)
# Access('r', 'IFMODE', bytes = 2)

# data = Access('r', 'DATA', bytes = 3)
# vin_24 = VIN_24(data, gain0, offset0)
# print("VIN_24: ", vin_24)

Access('w', 'ADCMODE', value = 0x0080)
print("ADCMODE: ", Access('r', 'ADCMODE', bytes = 2))

Access('w', 'IFMODE', value = 0x0200)  # 连续转换模式
# Access('w', 'IFMODE', value = 0x0201)  # 连续读取模式
print("IFMODE: ", Access('r', 'IFMODE', bytes = 2))

# Access('w', 'FILTCON0', value = 0x0e05)    # 100SPS
Access('w', 'FILTCON0', value = 0x0705)    # 10000SPS
# Access('w', 'FILTCON0', value = 0x0A05)    # 1000SPS
print("FILTCON0: ", Access('r', 'FILTCON0', bytes = 2))
print('@')

# data = Access('r', 'DATA', bytes = 4)
# vin_32 = VIN_32(data, gain0, offset0)
# print("VIN_32: ", vin_32)
# print(offset0 + gain0 + data)

# Access('w', 'IFMODE', value = 0x0201)
# print("IFMODE: ", Access('r', 'IFMODE', bytes = 2))

CSn.value(0)
buf = bytearray()
buf.append(0x44)

for i in range(0, 10):
    while True:
        if not MISO.value():
            break
    spi.write(buf)
    v = spi.read(4)
    Time_list.append(utime.ticks_us())
    Data_list.append(v)

CSn.value(1)

for line in Time_list:
    print(str(line) + "\t" + str(VIN_32(int.from_bytes(Data_list[Time_list.index(line)], 'big'), gain0, offset0)))

Time_list.clear()
Data_list.clear()
print("%")
