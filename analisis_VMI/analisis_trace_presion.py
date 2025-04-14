import time
import ROOT
archivo_root = ROOT.TFile('trace_presion.root', 'RECREATE' )
mylist = list()
#Marca de hora(ms) presion presion_filtrada SP_3 flujo_filtrado vol presion_inspiracion frecuencia relacionIE vol_tidal vol_max 

mylist2 = list()
presion_file  = open("Prueba_SetpointPresionVariable_VolCte400.txt", "r")
prescon = ROOT.TNtuple("presion_control","codesys","tiempo:presion:presion_filt:flujo:flujo_filt:vol:presins:fr:i_e:v_tidal:v_max")
i = 0
while True:
    a = presion_file.readline().split()
    print(a)
    if a is None or len(a) == 0 or a is EOFError:
        break
    else:
       mylist2.append(a)
       tiempo = float(mylist2[i][0])
       presion = float(mylist2[i][1])
       presion_filt = float(mylist2[i][2])
       flujo = float(mylist2[i][3])
       flujo_filt = float(mylist2[i][4])
       vol = float(mylist2[i][5])
       presins = float(mylist2[i][6])
       fr = float(mylist2[i][7])
       i_e = float(mylist2[i][8])
       v_tidal = float(mylist2[i][9])
       v_max = float(mylist2[i][10])
       prescon.Fill(tiempo-1916117,presion,presion_filt,flujo,flujo_filt,vol,presins,fr,i_e,v_tidal,v_max)
       i += 1
prescon.Write()

#time.sleep(10**9)