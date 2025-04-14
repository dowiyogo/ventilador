import time
import ROOT
#timeStamp (ms);flukeHighFlow (0.1 l/min);FlukePressureHF (0.01 mbar);FlukeVolumeHF (0.1 ml);FlukeVti (ml);FlukeVte (ml);Peep (0.1 mbar);HF (l/min);PressureHF (cmH2O);VolumeHF (ml);Vti (ml);Vte (ml);Peep (cmH2O)
archivo_root = ROOT.TFile('prueba_volumen.root', 'RECREATE' )
mylist = list()
#Marca de hora(ms) presion presion_filtrada SP_3 flujo_filtrado vol FiO2_measured frecuencia relacionIE vol_tidal FiO2 vol_max 
file  = open("CalpruebaVolumenControl_22-05-30.txt", "r")
volumen = ROOT.TNtuple("volumen","fluke","tiempo:HF01:PF001:VolF01:VtiF:VteF:PeepmB:HF:PM:VolHF:Vti:Vte:Peep")
i = 0
while True:
    a = file.readline().split()
    print(a)
    if a is None or len(a) == 0 or a is EOFError:
        break
    else:
       mylist.append(a)
       tiempo = float(mylist[i][0].replace('.',''))
       HF01 = float(mylist[i][1])
       PF001 = float(mylist[i][2])
       VolF01 = float(mylist[i][3])
       VtiF = float(mylist[i][4])
       VteF = float(mylist[i][5])
       PeepmB = float(mylist[i][6])
       HF = float(mylist[i][7])
       PM = float(mylist[i][8])
       VolHF = float(mylist[i][9])
       Vti = float(mylist[i][10])
       Vte = float(mylist[i][11])
       Peep = float(mylist[i][12])
       volumen.Fill(tiempo,HF01,PF001,VolF01,VtiF,VteF,PeepmB,HF,PM,VolHF,Vti,Vte,Peep)
       i += 1
volumen.Write()


#time.sleep(10**9)