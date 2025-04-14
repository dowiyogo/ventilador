#include <fstream>
#include <string>
#include <iostream>
#include <TH1F.h>
#include <cmath>

//archivo es el nombre del archivo

int lectura5(char *archivo){


	TNtuple *adc = new TNtuple("adc","arduino","fn:vp:vt:pp:pva:sf1f:sf2f:vp1:vf1:sp1:sp2:sf1old:sf2old:pm");
	
  	adc->ReadFile(archivo);
  	TCanvas *canvas1b= new TCanvas("canvas1b","Histogramas");
  	adc->Draw("sf1f>>histsf1(20000,-10,60)",""); 
	TH1F* hsf1;
	hsf1 = (TH1F *)gROOT->FindObject("histsf1"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	
	TSpectrum *specsf1= new TSpectrum(10);	
	 //finding Spectra Peaks
	Int_t npeaksf1 = specsf1->Search(hsf1,1,"",0.005); // Al objeto de clase TSpectrum le hago buscar peaks en el histograma apuntado por el puntero h
	//Finding peaks and fitting gaussians
	Double_t *peaksf1 = specsf1->GetPositionX(); // Obtengo las posiciones de los peaks, guardadas en un arreglo de mayor a menor en amplitud
	Double_t *peaksYsf1= specsf1->GetPositionY();
	Double_t sigmasf1 = 0.05;
	
	TF1 *funcsf1 = new TF1("funcsf1","gaus(0)",peaksf1[0]-2*sigmasf1,peaksf1[0]+2*sigmasf1); //hago un fit de la gausiana, con el primer parametro de nombre 0
	funcsf1->SetLineColor(kRed);
	funcsf1->SetParameters(peaksYsf1[0],peaksf1[0],sigmasf1/2);
	funcsf1->SetParLimits(0,0,100000);
	funcsf1->SetParLimits(1,peaksf1[0]-sigmasf1/2.,peaksf1[0]+sigmasf1/2.);
	funcsf1->SetParLimits(2,0,3*sigmasf1);
	hsf1->Fit("funcsf1","RB+"); //  El "+" agrega la función a la lista de funciones del histograma y el "B" es para considerar los limites.Double_t bsfi2=fsf1sf2_2->GetParameter(0);
  
 	Double_t zerosf1=funcsf1->GetParameter(1);
  	Double_t sigmasf1g=funcsf1->GetParameter(2);
  	
	//TPaveText *ptsf1 = new TPaveText(0.,17000.,10.,17500.,"BR");
	TPaveText *ptsf1 = new TPaveText(.1,.8,.3,.9,"BR");
 
	TText *tsf1 = ptsf1->AddText(Form("zero sf1 = %f  #pm %f",zerosf1,sigmasf1g));
 	gPad->SetLogy();
	tsf1->SetTextColor(kRed);
 
	ptsf1->Draw();
	
	////////////////////////////////////////////////////////////////////
	
	TCanvas *canvas2b= new TCanvas("canvas2b","Histogramas");
  	adc->Draw("sf2f>>histsf2(20000,-10,60)",""); //"vp1<8 && vf1<15"); 
	TH1F* hsf2;
	hsf2 = (TH1F *)gROOT->FindObject("histsf2"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	
	TSpectrum *specsf2= new TSpectrum(10);	
	 //finding Spectra Peaks
	Int_t npeaksf2 = specsf2->Search(hsf2,1,"",0.005); // Al objeto de clase TSpectrum le hago buscar peaks en el histograma apuntado por el puntero h
	//Finding peaks and fitting gaussians
	Double_t *peaksf2 = specsf2->GetPositionX(); // Obtengo las posiciones de los peaks, guardadas en un arreglo de mayor a menor en amplitud
	Double_t *peaksYsf2= specsf2->GetPositionY();
	Double_t sigmasf2 = 0.05;
	
	TF1 *funcsf2 = new TF1("funcsf2","gaus(0)",peaksf2[0]-2*sigmasf2,peaksf2[0]+2*sigmasf2); //hago un fit de la gausiana, con el primer parametro de nombre 0
	funcsf2->SetLineColor(kRed);
	funcsf2->SetParameters(peaksYsf2[0],peaksf2[0],sigmasf2/2);
	funcsf2->SetParLimits(0,0,100000);
	funcsf2->SetParLimits(1,peaksf2[0]-sigmasf2/2.,peaksf2[0]+sigmasf2/2.);
	funcsf2->SetParLimits(2,0,3*sigmasf2);
	hsf2->Fit("funcsf2","RB+"); //  El "+" agrega la función a la lista de funciones del histograma y el "B" es para considerar los limites.Double_t bsfi2=fsf1sf2_2->GetParameter(0);
  
 	Double_t zerosf2=funcsf2->GetParameter(1);
  	Double_t sigmasf2g=funcsf2->GetParameter(2);
  	
	TPaveText *ptsf2 = new TPaveText(-10.,10500.,10.,11000.,"BR");
 
	TText *tsf2 = ptsf2->AddText(Form("zero sf2 = %f  #pm %f",zerosf2,sigmasf2g));
  	gPad->SetLogy();
	tsf2->SetTextColor(kRed);
 
	ptsf2->Draw();
	
	TCanvas *canvas2c= new TCanvas("canvas2c","Histogramas");
  	adc->Draw("sf2f>>histsf2c(1000,0.7,1)","sf2f<1"); //"vp1<8 && vf1<15"); 
		
  
  	
  	TCanvas *canvas1= new TCanvas("canvas1","Histogramas");
  	adc->Draw("sf2f:sf1f>>hprof","","prof");
  	
  	TProfile *hprof;
  	hprof = (TProfile *)gROOT->FindObject("hprof");
  	hprof->Fit("pol1");
  	TF1 *fsf1sf2  = hprof->GetFunction("pol1");
  	Double_t bsfi=fsf1sf2->GetParameter(0);
  	Double_t msfi=fsf1sf2->GetParameter(1);
  	
	TPaveText *pt = new TPaveText(5.,50.,25.,55.,"BR");
 
	TText *t1 = pt->AddText(Form("sf2=sf1#bullet %f  + %f",msfi,bsfi));
 
	t1->SetTextColor(kRed);
 
	pt->Draw();
  	
  	TCanvas *canvas2= new TCanvas("canvas2","Histogramas");

	adc->Draw("sf2f:sf1f>>histo(1000,0,65,1000,0,65)","","BOXCOLZ");
  	
  	TCanvas *canvas3= new TCanvas("canvas3","Histogramas");
  	adc->Draw("sf1f:vf1>>hprof3","sf1f > 2 && vp1 == 30. ","prof");
  	
  	TProfile *hprof3;
  	hprof3 = (TProfile *)gROOT->FindObject("hprof3");
  	
  	hprof3->Fit("pol3");
  	
  	//adc->Draw("sf1f:vf1>>hsf1vf1vp1()","sf1f > 2 && vp1 == 30. ","prof");
  	
  	
	/*adc->Draw("sf1f:vf1:vp1>>hsf1vf1vp1()","sf1f > 2");
	
	TH3F *hsf1_3d = (TH3F *)gROOT->FindObject("hsf1vf1vp1");
	
	TCanvas *canvas3b= new TCanvas("canvas3b","Histogramas");
	TProfile2D *hsf1_2d=hsf1_3d->Project3DProfile("zy");
	hsf1_2d->Draw();*/
	
	//TCanvas *canvas3c= new TCanvas("canvas3c","Histogramas");
  	
	
	
	//TCanvas *canvas4= new TCanvas("canvas4","Histogramas");
	//adc->Draw("sf2f:vf1:vp1>>hsf2vf1vp1()");
	
	
	TCanvas *canvas6= new TCanvas("canvas6","Histogramas");
  	adc->Draw(Form("sf1f-%f:sf2f-%f>>hprof2",zerosf1,zerosf2),"sf1f >2 && sf2f >2","prof");
  	
  	TProfile *hprof2;
  	hprof2 = (TProfile *)gROOT->FindObject("hprof2");
  	hprof2->Fit("pol1");
  	TF1 *fsf1sf2_2  = hprof2->GetFunction("pol1");
  	fsf1sf2_2->SetLineColor(kBlue);
  	Double_t bsfi2=fsf1sf2_2->GetParameter(0);
  	Double_t msfi2=fsf1sf2_2->GetParameter(1);
  	
	TPaveText *pt2 = new TPaveText(5.,50.,25.,55.,"BR");
 
	TText *t2 = pt2->AddText(Form("sf1=sf2 #bullet %f  + %f",msfi2,bsfi2));
 
	t2->SetTextColor(kBlue);
 
	pt2->Draw();
	
	TCanvas *canvas5= new TCanvas("canvas5","Histogramas");
	adc->Draw("sf1f:sf2f");
	Int_t n1=adc->GetEntries();
		
	TCanvas *canvas9= new TCanvas("canvas9","Histogramas");
	
	auto M = new TMultiGraph();

	printf("The arrays' dimension is %d\n",n1);
	Double_t *X1  = adc->TTree::GetVal(0);
	Double_t *Y1  = adc->TTree::GetVal(1);
	Double_t tiempo[n1];
	
	for (int i=0;i<n1;i++){
	tiempo[i]=(Double_t) i*0.1;	

	}

	TGraph *g1 = new TGraph(n1,tiempo,X1);
		g1->SetLineWidth(1);
		g1->SetLineColor(kMagenta);
	TGraph *g2 = new TGraph(n1,tiempo,Y1);

	M->Add(g1);
	M->Add(g2);
	M->Draw("AL");
	
	
	TCanvas *canvas5b= new TCanvas("canvas5b","Histogramas");
	adc->Draw(Form("sf1f - %f:(sf2f - %f)*%f",zerosf1,zerosf2,msfi2));
	Int_t n2=adc->GetEntries();
		
	TCanvas *canvas9b= new TCanvas("canvas9b","Histogramas");
	
	auto M2 = new TMultiGraph();

	printf("The arrays' dimension is %d\n",n2);
	Double_t *X1b  = adc->TTree::GetVal(0);
	Double_t *Y1b  = adc->TTree::GetVal(1);
	Double_t tiempob[n2];
	
	for (int i=0;i<n2;i++){
	tiempob[i]=(Double_t) i*0.1;	

	}

	TGraph *g1b = new TGraph(n2,tiempo,X1b);
		g1b->SetLineWidth(1);
		g1b->SetLineColor(kRed);
	TGraph *g2b = new TGraph(n2,tiempob,Y1b);
		g2b->SetLineColor(kGreen);
		
	M2->Add(g1b);
	M2->Add(g2b);
	M2->Draw("AL");
	
	return 0;
}
