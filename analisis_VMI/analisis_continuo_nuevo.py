import time
import ROOT

#timeStamp (ms);FlukeHighFlow (0.1 l/min);flujo VMIRaw (?); apertura vf1 (%); sp1; flujo VMI (l/min) ;vp1 (psi)
archivo_root = ROOT.TFile('prueba_nueva.root', 'RECREATE' )
mylist = list()
file  = open("Calibracionprueba_flujo-08-06-2022.txt", "r")
continuo = ROOT.TNtuple("continuo","fluke","tiempo:HF:flujoR:vf1:sp1:flujo:vp1")
i = 0
while True:
    a = file.readline().split()
    print(a)
    if a is None or len(a) == 0 or a is EOFError:
        break
    else:
       mylist.append(a)
       tiempo = float(mylist[i][0].replace('.',''))
       HF = float(mylist[i][1])
       flujoR = float(mylist[i][2])
       vf1 = float(mylist[i][3])
       sp1 = float(mylist[i][4])
       flujo = float(mylist[i][5])
       vp1 = float(mylist[i][6])
       continuo.Fill(tiempo-969652,HF*0.1,flujoR,vf1,sp1,flujo,vp1)
       i += 1
continuo.Write()


#time.sleep(10**9)