
class mt_packet:
    
    def from_buffer(self):
        self.dest = int.from_bytes(self.buffer[0:4],byteorder='little')
        self.src = int.from_bytes(self.buffer[4:8],byteorder='little')
        self.seq = int.from_bytes(self.buffer[8:12],byteorder='little')
        self.flags = int.from_bytes(self.buffer[12:13])
        self.hash = int.from_bytes(self.buffer[13:14])
        self.nexthop = int.from_bytes(self.buffer[14:15])
        self.relay = int.from_bytes(self.buffer[15:16])
        self.hopcount = self.flags & 7
        self.wantack = (self.flags & 8) >> 3
        self.mqtt = (self.flags & 16) >> 4
        self.maxhop = (self.flags & 224) >> 5
        self.payload = self.buffer[16:]

    def __init__(self,buffer=None):
        self.decrypted = False
        if buffer != None:
            self.buffer=buffer
            self.from_buffer()

    def dump(self):
        print('dest: {}'.format(hex(self.dest)))
        print('src: {}'.format(hex(self.src)))
        print('seq: {}'.format(hex(self.seq)))
        print('hopcount: {}'.format(self.hopcount))
        print('maxhop: {}'.format(self.maxhop))
        print('hash: {}'.format(self.hash))
        print('nexthop: {}'.format(self.nexthop))
        print('relay: {}'.format(self.relay))
        print(self.payload)
