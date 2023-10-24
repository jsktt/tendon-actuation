import spidev
from time import sleep

# From spidev{BUS}.{DEV}
BUS = 0
DEV = 0

spi = spidev.SpiDev()

to_send = [0xaa, 0xaa]

def main():
    spi.open()

    # spi.bits_per_word = 8
    spi.cshigh = False
    # spi.loop = False
    # spi.lsbfirst = False
    spi.max_speed_hz = 4000000
    # spi.mode = 0x00
    # spi.threewire = False

    while True:
        res = spi.xfer3([0xaa, 0xaa])
        print(f'Received: {res}')
        sleep(0.1)

if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        spi.close()
        exit()