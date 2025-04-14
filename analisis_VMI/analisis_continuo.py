import time
import ROOT

#timeStamp (ms)	FlukeHighFlow (0.1 l/min)	flujo VMI (l/min)	 apertura vf1 (%)	 sp1	 sp2 	vp1 (psi)
archivo_root = ROOT.TFile('prueba_continua.root', 'RECREATE' )
mylist = list()
#Marca de hora(ms) presion presion_filtrada SP_3 flujo_filtrado vol FiO2_measured frecuencia relacionIE vol_tidal FiO2 vol_max 
file  = open("CalibracionpruebaFlujo_22-06-01.txt", "r")
continuo = ROOT.TNtuple("continuo","fluke","tiempo:HF:flujo:vf1:sp1:sp2:vp1")
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
       flujo = float(mylist[i][2])
       vf1 = float(mylist[i][3])
       sp1 = float(mylist[i][4])
       sp2 = float(mylist[i][5])
       vp1 = float(mylist[i][6])
       continuo.Fill(tiempo,HF*0.1,flujo,vf1,sp1,sp2,vp1)
       i += 1
continuo.Write()


#time.sleep(10**9)