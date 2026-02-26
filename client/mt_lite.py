import traceback

import pypb
import mt_packet
import mt_crypto
import mt_channel
from mt_packet import mt_packet

class mt_lite:
    def __init__(self,connection,logfile = None):
        self.connection = connection
        self.inbuff = []
        self.inqueue = []
        self.inlen = 0
        mt_channel.add_channel('LongFast',1)
        
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
            #print('packet')
            #we're unpacking our local protobuf between radio and pc
            pb = pypb.protobuf(msg).to_map()
            if pb[1] == 1:
                mtp = mt_packet(pb[2])
                mtp.rssi = pb[3]-100
                try:
                    chan = mt_channel.get_channel_by_hash(mtp.hash)
                except:
                    return mtp
                mt_crypto.encrypt_packet(mtp,chan.key)
                dec = pypb.protobuf(mtp.payload).to_map()
                mtp.decrypted = True
                mtp.payload = dec
                return mtp

        return None
