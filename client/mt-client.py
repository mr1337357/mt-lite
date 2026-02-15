import sys
import select
import serial
import time
import datetime
import traceback

import pypb
import mt_packet
import mt_crypto

class mt():
    def __init__(self,connection,logfile = None):
        self.connection = connection
        self.inbuff = []
        self.inqueue = []
        self.inlen = 0
        
    def update(self):
        while self.connection.in_waiting > 0:
            rxlen = len(self.inbuff)
            b = ord(self.connection.read())

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
        
    def get(self):
        if len(self.inqueue) > 0:
            msg = self.inqueue[0]
            self.inqueue = self.inqueue[1:]
            return msg
        return None
    
#def 
        
if __name__ == '__main__':
    log = open('test.log','w')
    dev = serial.Serial(sys.argv[1],115200)
    mesht = mt(dev)
    while True:
        mesht.update()
        pkt = mesht.get()
        if pkt != None:
            print('packet')
            pb = pypb.protobuf(pkt).to_map()
            try:
                
                mtp = mt_packet.mt_packet(pb[2])
                log.write(pb[2].hex()+'\n')
                log.flush()
                mtp.dump()
                asdf = mt_crypto.encrypt_packet(mtp)
                dec = pypb.protobuf(mtp.payload).to_map()
                print(dec)
            except Exception as e:
                print('except')
                print(e)
                print(traceback.format_exc())
            
        time.sleep(.1)
