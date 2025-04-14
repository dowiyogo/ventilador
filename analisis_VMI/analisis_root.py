import time
import ROOT
archivo_root = ROOT.TFile('datos_pruebas.root', 'RECREATE' )
mylist = list()

#Marca de hora [ms]	SP1 [cmH2O]	SP2 [cmH2O]	flujo [L/min]	ap_VF1 [%]	VP1 [PSI]	PLC_PRG,VP1
sinfiltrar_file  = open("prueba_sin_filtrar.txt", "r")
sinfilt = ROOT.TNtuple("no_filtrados","codesys","tiempo:SP1:SP2:flujo:VF1:VP1:PLC")
i = 0
while True:
    a = sinfiltrar_file.readline().split()
    if a is None or len(a) == 0 or a is EOFError:
        break
    else:
       mylist.append(a)
       tiempo = float(mylist[i][0])
       SP1 = float(mylist[i][1])
       SP2 = float(mylist[i][2])
       flujo = float(mylist[i][3])
       VF1 = float(mylist[i][4])
       VP1 = float(mylist[i][5])
       PLC = float(mylist[i][6])
       sinfilt.Fill(tiempo,SP1,SP2,flujo,VF1,VP1,PLC)
       i += 1
sinfilt.Write()

mylist1 = list()
filtrados_file  = open("datos_filtrados.txt", "r")
filt = ROOT.TNtuple("filtrados","codesys","tiempo:SP1:SP2:flujo:VF1:VP1:PLC")
i = 0
while True:
    a = filtrados_file.readline().split()
    if a is None or len(a) == 0 or a is EOFError:
        break
    else:
       mylist1.append(a)
       tiempo = float(mylist1[i][0])
       SP1 = float(mylist1[i][1])
       SP2 = float(mylist1[i][2])
       flujo = float(mylist1[i][3])
       VF1 = float(mylist1[i][4])
       VP1 = float(mylist1[i][5])
       PLC = float(mylist1[i][6])
       filt.Fill(tiempo,SP1,SP2,flujo,VF1,VP1,PLC)
       i += 1
filt.Write()

#tiempo [ms]	presion [cmh2o]	presion_filt [cmh2o]	flujo [l/min]	flujo_filt [l/min]	volumen [mL]	FiO2 [%]	FR [bpm]	I:E [-]	Vol tidal [mL]	FiO2 conf [%]
mylist2 = list()
presion_file  = open("prueba_presion_control_30min.txt", "r")
prescon = ROOT.TNtuple("presion_control","codesys","tiempo:presion:presion_filt:flujo:flujo_filt:vol:fio2:fr:i_e:v_tidal:fio2conf")
i = 0
while True:
    a = presion_file.readline().split()
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
       fio2 = float(mylist2[i][6])
       fr = float(mylist2[i][7])
       i_e = float(mylist2[i][8])
       v_tidal = float(mylist2[i][9])
       fio2conf = float(mylist2[i][10])
       prescon.Fill(tiempo,presion,presion_filt,flujo,flujo_filt,vol,fio2,fr,i_e,v_tidal,fio2conf)
       i += 1
prescon.Write()

mylist3 = list()
volumen_file  = open("prueba_volumen_control_30min.txt", "r")
volcon = ROOT.TNtuple("volumen_control","codesys","tiempo:presion:presion_filt:flujo:flujo_filt:vol:fio2:fr:i_e:v_tidal:fio2conf")
i = 0
while True:
    a = volumen_file.readline().split()
    if a is None or len(a) == 0 or a is EOFError:
        break
    else:
       mylist3.append(a)
       tiempo = float(mylist3[i][0])
       presion = float(mylist3[i][1])
       presion_filt = float(mylist3[i][2])
       flujo = float(mylist3[i][3])
       flujo_filt = float(mylist3[i][4])
       vol = float(mylist3[i][5])
       fio2 = float(mylist3[i][6])
       fr = float(mylist3[i][7])
       i_e = float(mylist3[i][8])
       v_tidal = float(mylist3[i][9])
       fio2conf = float(mylist3[i][10])
       volcon.Fill(tiempo,presion,presion_filt,flujo,flujo_filt,vol,fio2,fr,i_e,v_tidal,fio2conf)
       i += 1
volcon.Write()

#time.sleep(10**9)