import time
import ROOT
#Marca de hora(ms) Respiracion.presion Respiracion.presion_filtrada Respiracion.SP_3 Respiracion.flujo_filtrado Respiracion.vol Blender.FiO2_measured Respiracion.frecuencia Respiracion.relacionIE Respiracion.vol_tidal global.FiO2 Respiracion.vol_max 

archivo_root = ROOT.TFile('prueba_variable.root', 'RECREATE' )
mylist = list()
#Marca de hora(ms) presion presion_filtrada SP_3 flujo_filtrado vol FiO2_measured frecuencia relacionIE vol_tidal FiO2 vol_max 
file  = open("Final_Prueba_VolTidalVariable_volumenControl.txt", "r")
ultimos = ROOT.TNtuple("ultimos","codesys","tiempo:P:P_F:SP3:flujo:vol:FiO2_m:fr:IE:v_tidal:FiO2:V_max")
i = 0
while True:
    a = file.readline().split()
    if a is None or len(a) == 0 or a is EOFError:
        break
    else:
       mylist.append(a)
       tiempo = float(mylist[i][0])
       P = float(mylist[i][1])
       P_F = float(mylist[i][2])
       SP3 = float(mylist[i][3])
       flujo = float(mylist[i][4])
       vol = float(mylist[i][5])
       FiO2_m = float(mylist[i][6])
       fr = float(mylist[i][7])
       IE = float(mylist[i][8])
       v_tidal = float(mylist[i][9])
       FiO2 = float(mylist[i][10])
       V_max = float(mylist[i][11])
       ultimos.Fill(tiempo,P,P_F,SP3,flujo,vol,FiO2_m,fr,IE,v_tidal,FiO2,V_max)
       i += 1
ultimos.Write()


#time.sleep(10**9)