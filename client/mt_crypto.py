from Crypto.Cipher import AES
from Crypto.Util import Counter


defaultpsk = [0xd4, 0xf1, 0xbb, 0x3a, 0x20, 0x29, 0x07, 0x59, 0xf0, 0xbc, 0xff, 0xab, 0xcf, 0x4e, 0x69, 0x01]

class aesctr:
    def __init__(self,iv,key):
        self.iv=iv
        self.key=key
        

def encrypt_packet(packet,psk = defaultpsk):
    nonce = [0] * 16
    nonce[0:4] = packet.seq.to_bytes(4)
    nonce[8:12] = packet.src.to_bytes(4)
    crypto = AES.new(bytes(psk), AES.MODE_ECB)
    i = 0
    out = [0] * len(packet.payload)
    while i < len(packet.payload):
        bnonce = bytes(nonce)
        cypher = crypto.encrypt(bnonce)
        for j  in range(16):
            try:
                out[i + j] = cypher[j] ^ packet.payload[i + j]
            except:
                pass
        print(cypher)
        nonce[12] += 1
        i += 16
    bpayload = bytes(out)
    print(bpayload.hex())
    packet.payload = bpayload
