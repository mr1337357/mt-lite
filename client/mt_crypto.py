from Crypto.Cipher import AES
from Crypto.Util import Counter


defaultpsk = [0xd4, 0xf1, 0xbb, 0x3a, 0x20, 0x29, 0x07, 0x59, 0xf0, 0xbc, 0xff, 0xab, 0xcf, 0x4e, 0x69, 0x01]

class aesctr:
    def __init__(self,iv,key):
        self.iv=iv
        self.key=key
        

def encrypt_packet(packet,psk = defaultpsk):
    nonce = [0] * 16
    nonce[0] = packet[8]
    nonce[1] = packet[9]
    nonce[2] = packet[10]
    nonce[3] = packet[11]