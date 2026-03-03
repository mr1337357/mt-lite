from mt_radio import mt_radio
import serial

class mt_radio_serial(mt_radio):
    def __init__(self,port):
        self.ser = serial.Serial(sys.argv[1],115200)
        
    def update(self):
        while self.ser.in_waiting > 0:
            rxlen = len(self.inbuff)
            b = ord(self.ser.read())

            if self.inlen > 0:
                self.inbuff.append(b)
                self.inlen -= 1
                if self.inlen == 0:
                    self.inqueue.append(self.inbuff[4:])
                    self.inbuff = []
            elif rxlen == 0:
                if b == 0x94:
                    self.inbuff.append(b)
                else:
                    sys.stdout.write(chr(b))
            elif rxlen == 1:
                if b == 0xC3:
                    self.inbuff.append(b)
                else:
                    self.inbuff = []
            elif rxlen == 2:
                self.inbuff.append(b)
            elif rxlen == 3:
                self.inbuff.append(b)
                self.inlen = self.inbuff[2] * 256 + self.inbuff[3]
            else:
                sys.stdout.write(chr(b)) 
