PB_VARINT = 0
PB_I64 = 1
PB_STRING = 2
PB_I32 = 5
PB_INVALID = 7
class protobuf:
    def __init__(self, buffer = None):
        self.isDecode = True
        if buffer == None:
            self.isDecode = False
            self.buffer = b''
        else:
            self.offset = 0
            self.buffer = buffer
            self.nextType = None
            self.nextIndex = 0

    def get_buffer(self):
        return self.buffer

    def ident(self):
        key = self.buffer[self.offset]
        self.offset += 1
        self.nextType = key & 7
        self.nextIndex = key >> 3
    
    def decode_varint(self):
        num = 0
        boff = 0
        while True:
            b = self.buffer[self.offset]
            num += (b & 0x7F) << boff
            boff += 7
            self.offset+=1
            if b < 128:
                return num

        
    
    def decode(self):
        if self.offset == len(self.buffer):
            return None
        self.ident()
        if self.nextType == PB_VARINT:
            num = self.decode_varint()
            return (self.nextIndex,self.nextType,num)
        if self.nextType == PB_STRING:
            length = self.decode_varint()
            out = b''
            for i in range(length):
                out += self.buffer[self.offset].to_bytes(1)
                self.offset += 1
            return (self.nextIndex,self.nextType,out)
            
    def encode_varint(self,num):
        while num > 127:
            t = 0x80 | (num & 0x7F)
            self.buffer += t.to_bytes(1)
            num >>= 7
        self.buffer += num.to_bytes(1)
        
    def encode(self,index,pb_type,data):
        key = index << 3 | pb_type
        self.buffer += key.to_bytes(1)
        if pb_type == PB_VARINT:
            self.encode_varint(data)
        if pb_type == PB_STRING:
            self.encode_varint(len(data))
            for b in data:
                self.buffer += b.to_bytes(1)
    
    def __iter__(self):
        self.offset = 0
        return self
        
    def __next__(self):
        try:
            rv = self.decode()
        except:
            rv = None
        if rv == None:
            raise StopIteration
        return rv
        
if __name__ == '__main__':
    import sys

    print(sys.argv)
    if len(sys.argv) > 1:
        file = sys.argv[1]
        with open(file,'rb') as inf:
            pbr = protobuf(inf.read())
            for index,ptype,item in pbr:
                print(index,ptype,item)

    else:
    
        pbt = protobuf()
        pbt.encode(1,PB_VARINT,150)
        pbt.encode(5,PB_VARINT,12345)
        pbt.encode(2,PB_STRING,b'asdf')
        pb = pbt.get_buffer()
        for b in pb:
            sys.stdout.write('{} '.format(hex(b)))
        sys.stdout.write('\n')
        pbr = protobuf(pb)
        for index,ptype,item in pbr:
            print(index,ptype,item)
