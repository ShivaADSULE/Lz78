# ENCODE FUNCTION
def Encode(msg):
    HEX = ""
    for i in data:HEX+=hex(ord(i))[2:]
    def compress(msg,idx,dic):
        if msg in dic:
            return [(dic[msg],"")]
        i = 1
        n = len(msg)
        while i<=n:
            t = msg[:i]
            if t in dic:
                i+=1
            else:
                dic[msg[:i]]=idx
                if msg[:i-1] in dic:
                    return compress(msg[i:],idx+1,dic)+[(dic[msg[:i-1]],t[-1])]
                else:
                    return compress(msg[i:],idx+1,dic)+[(0,t[-1])]
        return []
    return compress(data,1,{})[::-1]

# DECODE FUNCTION
def Decode(msg):
    def decomress(msg,idx,dic):
        if len(msg)==0:
            return ""
        if msg[0][0]==0:
            dic[idx] = msg[0][1]
            idx+=1
            return msg[0][1]+decomress(msg[1:],idx,dic)
        tmp = dic[msg[0][0]]
        tmp+=msg[0][1]
        dic[idx]=tmp
        idx+=1
        return tmp+decomress(msg[1:],idx,dic)
    HEX = decomress(res,1,{})
    msg = ""
    for i in range(1,len(decode),2):msg+=chr(int(decode[i-1]+decode[i],16))
    return msg