from xmlrpc.client import MAXINT
import pandas as pd
import sendinglocationviawhatsapp as snd

df=pd.read_csv('hospital_database.csv')
df=df.dropna(axis=1,how="all")
df=df.dropna(how="all")

a,b,c,d,e,lat,lon,t=snd.readData()

lat,lon = 21.12775,81.7641

num=df.iloc[:,2]
loc=df.iloc[:,3].astype(str)

latl=[]
lonl=[]

for i in loc:
    l=i.split(",")
    latl.append(l[0])
    lonl.append(l[-1])

ind=-1
diff=MAXINT

for i in range (0,len(latl)):
    d=(float(lat)-float(latl[i]))
    if(d<0):
        d=d*-1
    if(d<diff):
        diff=d
        ind=i

phnum=num[10]
phnum="+91"+phnum