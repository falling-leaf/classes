import time # 加入时间计量
# 对初始常量进行设置
h0 = 0x6a09e667
h1 = 0xbb67ae85
h2 = 0x3c6ef372
h3 = 0xa54ff53a
h4 = 0x510e527f
h5 = 0x9b05688c
h6 = 0x1f83d9ab
h7 = 0x5be0cd19

hash = [h0, h1, h2, h3, h4, h5, h6, h7]
all_version_count = 26

arg = hash.copy()

k = [0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2]

# def func0(data):
#     return ((data >> 7) ^ (data >> 18) ^ (data >> 3)) & (2 ** 32 - 1)

# def func1(data):
#     return ((data >> 17) ^ (data >> 19) ^ (data >> 10)) & (2 ** 32 - 1)

def func0(data):
    return (ROTR(data, 7) ^ ROTR(data, 18) ^ (data >> 3)) & 0xffffffff

def func1(data):
    return (ROTR(data, 17) ^ ROTR(data, 19) ^ (data >> 10)) & 0xffffffff

def ROTR(data, bit):
    return ((data >> bit) | (data << (32 - bit))) & 0xffffffff

def sum0(data):
    return ROTR(data, 2) ^ ROTR(data, 13) ^ ROTR(data, 22)

def sum1(data):
    return ROTR(data, 6) ^ ROTR(data, 11) ^ ROTR(data, 25)

def Ch(arg1, arg2, arg3):
    return (arg1 & arg2) ^ (~arg1 & arg3)
    
def Maj(arg1, arg2, arg3):
    return (arg1 & arg2) ^ (arg1 & arg3) ^ (arg2 & arg3)

input = "Blockchain@ZhejiangUniversity" # 这是要求的字符串
nonce = 490000000 # 这是附着在后面的量

start_time = time.time() - 3951.148

while 1:
    nonce_str = str(nonce)
    input = "Blockchain@ZhejiangUniversity" + nonce_str 
    data = ""
    for i in range(len(input)):
        data += bin(ord(input[i]))[2:].zfill(8) # 将数据处理为二进制数，并补齐空位
        #add_str = "0" * (8 - len(bin(ord(input[i]))[2:]))
        #data += add_str + bin(ord(input[i]))[2:]

    length = len(input) * 8
    add_str = "1" + "0" * (448 - len(data) - 1) # 扩展字符串至448位
    data += add_str

    add_str = "0" * (64 - len(bin(length)[2:])) # 最后64位表示字符串大小
    data += add_str + bin(length)[2:]

    # print(len(data))

    w = [0 for _ in range(64)]
    hash = [h0, h1, h2, h3, h4, h5, h6, h7]
    arg = hash.copy()

    for i in range(16):
        w[i] = int(data[i * 32: i * 32 + 32], 2)

    for i in range(16, 64):
        w[i] = (func1(w[i - 2]) + w[i - 7] + func0(w[i - 15]) + w[i - 16]) & 0xffffffff


    for i in range(64):
        T1 = arg[7] + (ROTR(arg[4], 6) ^ ROTR(arg[4], 11) ^ ROTR(arg[4], 25)) + ((arg[4] & arg[5]) ^ (~arg[4] & arg[6])) + k[i] + w[i]
        #T1 = (arg[7] + sum1(arg[4]) + Ch(arg[4], arg[5], arg[6]) + k[i] + w[i])
        T2 = (ROTR(arg[0], 2) ^ ROTR(arg[0], 13) ^ ROTR(arg[0], 22)) + ((arg[0] & arg[1]) ^ (arg[0] & arg[2]) ^ (arg[1] & arg[2]))
        #T2 = (sum0(arg[0]) + Maj(arg[0], arg[1], arg[2]))
        for j in range(6, -1, -1):
            arg[j + 1] = arg[j]
        arg[4] = (arg[4] + T1) & 0xffffffff
        arg[0] = (T1 + T2) & 0xffffffff
    
    for j in range(8):
        hash[j] = (hash[j] + arg[j]) & 0xffffffff
    # for i in range(8):
    #     print(hex(hash[i]))
    #     print("")
    checklist = bin(hash[0])[2:].zfill(32)
    counter = 0
    for i in range(32):
        if checklist[i] != '0':
            counter = i
            break
    if hash[0] == 0:
        counter = 32
    if counter - 1 >= 0:
        if counter > all_version_count:
            print("the first {:} bits are 0!; the nonce is: {:}".format(counter, nonce))
            present_time = time.time()
            print("executing time: {:}s".format(present_time - start_time))
            all_version_count = counter

    # if hash[0] == 3 or hash[0] == 2:
    #     print("the first 30th bits are 0!")
    #     print("result: {:}".format(nonce))

    # if hash[0] == 1:
    #     print("the first 31th bits are 0!")
    #     print("result: {:}".format(nonce))

    # if hash[0] == 0:
    #     print("the first 32nd bits are 0!")
    #     print("result: {:}".format(nonce))
    #     break
    if nonce % 1000000 == 0:
        print("nonce: {:}".format(nonce))
        present_time = time.time()
        print("executing time: {:}s".format(present_time - start_time))
    nonce -= 1