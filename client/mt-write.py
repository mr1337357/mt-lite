import sys
import select
import serial
import time
import datetime
import traceback

import mt_radio_serial
from mt_packet import mt_packet

import pypb
from mt_lite import mt_lite
from sql_wrapper import sqlwrapper
    

if __name__ == '__main__':
    log = open('test.log','w')
    
    users = sqlwrapper()
    radio = mt_radio_serial.mt_radio_serial(sys.argv[1])
    mesht = mt_lite(radio)
    sendpkt = mt_packet()
    sendmsg = pypb.protobuf()
    sendmsg.encode(1,pypb.PB_VARINT,1)
    sendmsg.encode(2,pypb.PB_STRING,'hello world')
    sendpkt.payload = sendmsg.buffer
    mesht.send(sendpkt)
    while True:
        mesht.update()
        pkt = mesht.get()
        if pkt != None:
            pkt.dump()
        time.sleep(.01)
