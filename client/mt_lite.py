import sys
import traceback

import pypb
import mt_packet
import mt_crypto
import mt_channel
from mt_packet import mt_packet

class mt_lite:
    def __init__(self,radio,logfile = None):
        self.radio = radio

        mt_channel.add_channel('LongFast',1)
        
    def update(self):
        self.radio.update()

    def decode(self,buff):
        pb = pypb.protobuf(buff).to_map()
        if pb[1] == 1:
            raw = pb[2]
            mtp = None
            try:
                mtp = mt_packet(raw)
            except:
                return None
            mtp.rssi = pb[3]
            if mtp.rssi>= 2**31:
                mtp.rssi -= 2**32
            try:
                chan = mt_channel.get_channel_by_hash(mtp.hash)
                mt_crypto.encrypt_packet(mtp,chan.key)
                dec = pypb.protobuf(mtp.payload).to_map()
                mtp.decrypted = True
                mtp.payload = dec
            except:
                pass
            
        return mtp

    def get_raw(self):
        return self.radio.read()

    def get(self):
        msg = self.radio.read()
        if msg != None:
            dec = self.decode(msg)
            return dec
        return None
    
    def send(self,msg):
        if msg.decrypted == True:
            chan = mt_channel.get_channel_by_hash(msg.hash)
            mt_crypto.encrypt_packet(msg,chan.key)
        msg.decrypted = False
        wrapper = pypb.protobuf()
        wrapper.encode(1,pypb.PB_VARINT,1)
        wrapper.encode(2,pypb.PB_STRING,msg.to_buffer())
        print(wrapper.to_map())
        print(wrapper.get_buffer())
        buff = wrapper.get_buffer()
        self.radio.write(buff)
