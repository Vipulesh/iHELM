import pandas as pd
from urllib.request import urlopen
import json
import time
import pywhatkit 
import access_correct_hospital as ach
import model as mo

READ_API_KEY='2DRE0AY1MVYF01XI'
CHANNEL_ID= '1747522'

b,c,d,e,f,g,h="","","","","","",""
boo=0

def readData():
    TS = urlopen("http://api.thingspeak.com/channels/%s/feeds/last.json?api_key=%s"  % (CHANNEL_ID,READ_API_KEY))
    response = TS.read()
    data=json.loads(response)
    b = data['field1']
    c = data['field2']
    d = data['field3']
    e = data['field4']
    f = data['field5']
    g = data['field6']
    h = data['field7']
    g,h = 21.12775,81.7641
    return b,c,d,e,f,g,h,TS

def check():
    b,c,d,e,f,g,h,TS=readData()
    g,h = 21.12775,81.7641
    boo=mo.pred(float(b),float(c),float(d),float(e),float(f),float(g),float(h))
    return boo

def main():
    boo=int(check())
    while True:
        if(boo):
            b,c,d,e,f,g,h,TS=readData()
            number = ach.phnum
            print("\n")
            message ="Accident occured at\n"+ str(g)+", "+str(h)
            pywhatkit.sendwhatmsg_instantly(number,message)
            TS.close()
            time.sleep(1)   

if __name__ == "__main__":
    main()