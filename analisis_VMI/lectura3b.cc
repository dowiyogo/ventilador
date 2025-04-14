#include <fstream>
#include <string>
#include <iostream>
#include <TH1F.h>
#include <cmath>

//archivo es el nombre del archivo

int lectura3b(char *archivo){
	//ifstream file(archivo);
	//char buffer[100];
	

	TNtuple *adc = new TNtuple("adc","arduino","fn:vp:vt:pp:pva:sf1f:sf2f:vp1:vf1:sp1:sp2:pm");
	/*Double_t fn,vp,vt,sf1,sf1f,sf2,sf2f,pm;
	while(! file.eof()) {
	
	file.getline(buffer,100);
	std::sprintf(buffer,"%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f", fn,vp,vt,sf1,sf1f,sf2,sf2f,pm);
	adc->Fill(fn,vp,vt,sf1,sf1f,sf2,sf2f,pm);


  }*/
	Double_t x[5]; //vol tidal
	Double_t ex[5]; // error vol tidal
	
	Double_t yc1[5]; // cero ins
	Double_t eyc1[5]; // error cero ins
	
	Double_t yc2[5]; // cero esp
	Double_t eyc2[5]; // error cero esp

	Double_t yf1[5]; // flujo ins
	Double_t eyf1[5]; // error flujo ins
	
	Double_t vinsh[5]; // V ins hist
	Double_t evinsh[5]; // error V ins hist
	
	Double_t vinsrho[5]; // V ins rho
	Double_t evinsrho[5]; // error V ins rho
	
	Double_t vesph[5]; // V esp hist
	Double_t evesph[5]; // error V esp hist
	
	Double_t vesprho[5]; // V esp rho
	Double_t evesprho[5]; // error V esp rho
	
	Float_t rho_aire = 1.29;
    Float_t densidad_ins = 1.350; // estaba 1800
	
  	adc->ReadFile(archivo);
	TCanvas *canvas1 = new TCanvas("canvas1","Histogramas");
	canvas1->Divide(2,5);
    canvas1->cd(1);
    
    adc->Draw("sf1f>>hist1ins(1000,-10,50)","vt<610&&vt>580"); 
    	gPad->SetLogy();
	TH1F* h1ins;
	h1ins = (TH1F *)gROOT->FindObject("hist1ins"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
    
    TSpectrum *specins= new TSpectrum(10);	
	 //finding Spectra Peaks
	Int_t npeaksins = specins->Search(h1ins,1,"",0.005); // Al objeto de clase TSpectrum le hago buscar peaks en el histograma apuntado por el puntero h
	//Finding peaks and fitting gaussians
	Double_t *peaksins = specins->GetPositionX(); // Obtengo las posiciones de los peaks, guardadas en un arreglo de mayor a menor en amplitud
	Double_t *peaksYins= specins->GetPositionY();
	Double_t sigmains = 0.5;
	if(npeaksins > 1)
		std::cout<<"Se encontraron los peaks de flujo, donde peaks[0] es "<<peaksins[0]<<"y peaks[1] es "<<peaksins[1]<<std::endl;
	else
	{
		std::cout<<"Se encontraron menos de 2 peaks"<<std::endl;
		return 1;
	}
	TF1 *funcins = new TF1("funcins","gaus(0)",peaksins[0]-2*sigmains,peaksins[0]+2*sigmains); //hago un fit de la gausiana, con el primer parametro de nombre 0
	funcins->SetLineColor(kRed);
	funcins->SetParameters(peaksYins[0],peaksins[0],sigmains);
	funcins->SetParLimits(0,0,20000);
	funcins->SetParLimits(1,peaksins[0]-sigmains/2.,peaksins[0]+sigmains/2.);
	funcins->SetParLimits(2,0,3*sigmains);
	h1ins->Fit("funcins","RB+"); //  El "+" agrega la función a la lista de funciones del histograma y el "B" es para considerar los limites.
	yc1[0]= funcins->GetParameter(1); 
	eyc1[0]= funcins->GetParameter(2); 

	
	TF1 *func2ins = new TF1("func2ins","gaus(0)+pol0(3)",peaksins[1]-0.5,peaksins[1]+0.5); //hago un fit de la gausiana, con el primer parametro de nombre 0
	func2ins->SetLineColor(kGreen);
	func2ins->SetParameters(peaksYins[1],peaksins[1],2,0);
	func2ins->SetParLimits(0,0,20000);
	func2ins->SetParLimits(1,peaksins[1]-2,peaksins[1]+2);
	func2ins->SetParLimits(2,0,3);
	func2ins->SetParLimits(3,0,10000);
	h1ins->Fit("func2ins","RB+"); //  El "+" agrega la función a la lista de funciones del histograma y el "B" es para considerar los limites.
	
	yf1[0]= func2ins->GetParameter(1); 
	eyf1[0]= func2ins->GetParameter(2); 
    
    canvas1->cd(2);
    
    adc->Draw("sf2f>>hist1esp(1000,-10,50)","vt<610&&vt>580"); 
    	gPad->SetLogy();
	TH1F* h1esp;
	h1esp = (TH1F *)gROOT->FindObject("hist1esp"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
    

	TSpectrum *specesp= new TSpectrum(10);	
	 //finding Spectra Peaks
	Int_t npeaksesp = specesp->Search(h1esp,1,"",0.005); // Al objeto de clase TSpectrum le hago buscar peaks en el histograma apuntado por el puntero h
	//Finding peaks and fitting gaussians
	Double_t *peaksesp = specesp->GetPositionX(); // Obtengo las posiciones de los peaks, guardadas en un arreglo de mayor a menor en amplitud
	Double_t *peaksYesp= specesp->GetPositionY();
	Double_t sigmaesp = 0.5;

	std::cout<<"Se encontraron los peaks de flujo esp, donde peaks[0] es "<<peaksesp[0]<<std::endl;
	
	TF1 *funcesp = new TF1("funcesp","gaus(0)",peaksesp[0]-2*sigmaesp,peaksesp[0]+2*sigmaesp); //hago un fit de la gausiana, con el primer parametro de nombre 0
	funcesp->SetLineColor(kRed);
	funcesp->SetParameters(peaksYesp[0],peaksesp[0],sigmaesp);
	funcesp->SetParLimits(0,0,20000);
	funcesp->SetParLimits(1,peaksesp[0]-sigmaesp*2,peaksesp[0]+sigmaesp*2);
	funcesp->SetParLimits(2,0,3*sigmaesp);
	h1esp->Fit("funcesp","RB+"); //  El "+" agrega la función a la lista de funciones del histograma y el "B" es para considerar los limites.
	yc2[0]= funcesp->GetParameter(1); 
	eyc2[0]= funcesp->GetParameter(2); 
	

	TCanvas *canvas1b = new TCanvas("canvas1b","Histogramas");
	canvas1b->Divide(1,5);
	canvas1b->cd(1);
	adc->Draw("vt>>hist1b(10000,0,700)","vt<610&&vt>580"); 
		gPad->SetLogy();
	TH1F* h1b;
	h1b = (TH1F *)gROOT->FindObject("hist1b"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	
	TCanvas *canvaspablo = new TCanvas("canvaspablo","Histogramas para Pablito");
	h1ins->Draw();
	h1ins->SetMaximum(8000);
	
		TCanvas *canvaspablo2 = new TCanvas("canvaspablo2","Histogramas para Pablito dos");
	h1esp->Draw();
	h1esp->SetMaximum(3500);
	
	x[0]= h1b->GetMean();
	ex[0]=h1b->GetStdDev();
	
	std::cout<<"El volumen tidal medido por el ventilador es "<< x[0]<<"y el volumen inspirado "<<h1ins->GetMean()/20.<<" escalado con densidad "<< h1ins->GetMean()/20.*(rho_aire/densidad_ins) <<std::endl;
	std::cout<<"El volumen tidal medido por el ventilador es "<< x[0]<<"y el volumen espirado "<<h1esp->GetMean()/20.<<" escalado con densidad "<< h1esp->GetMean()/20.*(rho_aire/densidad_ins) <<std::endl;
	
	//****************************************************************************************************************************//
	
	//TCanvas *canvas2 = new TCanvas("canvas2","Histogramas");
	//canvas2->Divide(2,1);
    //canvas2->cd(1);
	 canvas1->cd(3);
	 
	adc->Draw("sf1f>>hist2ins(1000,-10,50)","vt<510&&vt>480"); 
		gPad->SetLogy();
	TH1F* h2ins;
	h2ins = (TH1F *)gROOT->FindObject("hist2ins"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	

	TSpectrum *spec2ins= new TSpectrum(10);	
	 //finding Spectra Peaks
	Int_t npeaks2ins = spec2ins->Search(h2ins,1,"",0.005); // Al objeto de clase TSpectrum le hago buscar peaks en el histograma apuntado por el puntero h
	//Finding peaks and fitting gaussians
	Double_t *peaks2ins = spec2ins->GetPositionX(); // Obtengo las posiciones de los peaks, guardadas en un arreglo de mayor a menor en amplitud
	Double_t *peaksY2ins= spec2ins->GetPositionY();
	Double_t sigma2ins = 0.5;
	if(npeaks2ins > 1)
		std::cout<<"Se encontraron los peaks de flujo, donde peaks[0] es "<<peaks2ins[0]<<"y peaks[1] es "<<peaks2ins[1]<<std::endl;
	else
	{
		std::cout<<"Se encontraron menos de 2 peaks"<<std::endl;
		return 1;
	}
	TF1 *func3ins = new TF1("func3ins","gaus(0)",peaks2ins[0]-2*sigma2ins,peaks2ins[0]+2*sigma2ins); //hago un fit de la gausiana, con el primer parametro de nombre 0
	func3ins->SetLineColor(kRed);
	func3ins->SetParameters(peaksY2ins[0],peaks2ins[0],sigma2ins*0.5);
	func3ins->SetParLimits(0,0,20000);
	func3ins->SetParLimits(1,peaks2ins[0]-sigma2ins,peaks2ins[0]+sigma2ins);
	func3ins->SetParLimits(2,0,3*sigma2ins);
	h2ins->Fit("func3ins","RB+"); //  El "+" agrega la función a la lista de funciones del histograma y el "B" es para considerar los limites.
	yc1[1]= func3ins->GetParameter(1); 
	eyc1[1]= func3ins->GetParameter(2); 
	
	TF1 *func4ins = new TF1("func4ins","gaus(0)+pol0(3)",peaks2ins[1]-0.5,peaks2ins[1]+0.5); //hago un fit de la gausiana, con el primer parametro de nombre 0
	func4ins->SetLineColor(kGreen);
	func4ins->SetParameters(peaksY2ins[1],peaks2ins[1],0.5,0);
	func4ins->SetParLimits(0,0,2000);
	func4ins->SetParLimits(1,peaks2ins[1]-1,peaks2ins[1]+1);
	func4ins->SetParLimits(2,0,3);
	func4ins->SetParLimits(3,0,10000);
	h2ins->Fit("func4ins","RB+"); //  El "+" agrega la función a la lista de funciones del histograma y el "B" es para considerar los limites.

	yf1[1]= func4ins->GetParameter(1); 
	eyf1[1]= func4ins->GetParameter(2); 
	
	//canvas2->cd(2);
	canvas1->cd(4);
	
	adc->Draw("sf2f>>hist2esp(1000,-10,50)","vt<510&&vt>480"); 
		gPad->SetLogy();
	TH1F* h2esp;
	h2esp = (TH1F *)gROOT->FindObject("hist2esp"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	

	TSpectrum *spec2esp= new TSpectrum(10);	
	//finding Spectra Peaks
	Int_t npeaks2esp = spec2esp->Search(h2esp,1,"",0.005); // Al objeto de clase TSpectrum le hago buscar peaks en el histograma apuntado por el puntero h
	//Finding peaks and fitting gaussians
	Double_t *peaks2esp = spec2esp->GetPositionX(); // Obtengo las posiciones de los peaks, guardadas en un arreglo de mayor a menor en amplitud
	Double_t *peaksY2esp = spec2esp->GetPositionY();
	Double_t sigma2esp = 0.5;

	std::cout<<"Se encontraron los peaks de flujo esp, donde peaks[0] es "<<peaks2esp[0]<<std::endl;

	TF1 *func3esp = new TF1("func3esp","gaus(0)",peaks2esp[0]-2*sigma2esp,peaks2esp[0]+2*sigma2esp); //hago un fit de la gausiana, con el primer parametro de nombre 0
	func3esp->SetLineColor(kRed);
	func3esp->SetParameters(peaksY2esp[0],peaks2esp[0],sigma2esp*0.5);
	func3esp->SetParLimits(0,0,20000);
	func3esp->SetParLimits(1,peaks2esp[0]-sigma2esp*2,peaks2esp[0]+sigma2esp*2);
	func3esp->SetParLimits(2,0,3*sigma2esp);
	h2esp->Fit("func3esp","RB+"); //  El "+" agrega la función a la lista de funciones del histograma y el "B" es para considerar los limites.
	yc2[1]= func3esp->GetParameter(1); 
	eyc2[1]= func3esp->GetParameter(2); 

	canvas1b->cd(2);
	adc->Draw("vt>>hist2b(10000,0,700)","vt<510&&vt>480"); 
		gPad->SetLogy();
	TH1F* h2b;
	h2b = (TH1F *)gROOT->FindObject("hist2b"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	
	x[1]= h2b->GetMean();
	ex[1]=h2b->GetStdDev();
	
	std::cout<<"El volumen tidal medido por el ventilador es "<< x[1]<<"y el volumen inspirado "<<h2ins->GetMean()/20.<<" escalado con densidad "<< h2ins->GetMean()/20.*(rho_aire/densidad_ins) <<std::endl;
	std::cout<<"El volumen tidal medido por el ventilador es "<< x[1]<<"y el volumen espirado "<<h2esp->GetMean()/20.<<" escalado con densidad "<< h2esp->GetMean()/20.*(rho_aire/densidad_ins) <<std::endl;
	
	
	//****************************************************************************************************************************//

	//TCanvas *canvas3 = new TCanvas("canvas3","Histogramas");
	//canvas3->Divide(2,1);
	//canvas3->cd(1);
	
	canvas1->cd(5);
	adc->Draw("sf1f>>hist3ins(1000,-10,50)","vt<420&&vt>390"); 
		gPad->SetLogy();
	TH1F* h3ins;
	h3ins = (TH1F *)gROOT->FindObject("hist3ins"); // Busco el objeto y asigno un puntero para el histograma recien dibujado

	TSpectrum *spec3ins = new TSpectrum(10);	
	 //finding Spectra Peaks
	Int_t npeaks3ins = spec3ins->Search(h3ins,1,"",0.005); // Al objeto de clase TSpectrum le hago buscar peaks en el histograma apuntado por el puntero h
	//Finding peaks and fitting gaussians
	Double_t *peaks3ins = spec3ins->GetPositionX(); // Obtengo las posiciones de los peaks, guardadas en un arreglo de mayor a menor en amplitud
	Double_t *peaksY3ins= spec3ins->GetPositionY();
	Double_t sigma3ins = 1;
	if(npeaks3ins > 1)
		std::cout<<"Se encontraron los peaks de flujo, donde peaks[0] es "<<peaks3ins[0]<<"y peaks[1] es "<<peaks3ins[1]<<std::endl;
	else
	{
		std::cout<<"Se encontraron menos de 2 peaks"<<std::endl;
		return 1;
	}
	TF1 *func5ins = new TF1("func5ins","gaus(0)",peaks3ins[0]-2*sigma3ins,peaks3ins[0]+2*sigma3ins); //hago un fit de la gausiana, con el primer parametro de nombre 0
	func5ins->SetLineColor(kRed);
	func5ins->SetParameters(peaksY3ins[0],peaks3ins[0],sigma3ins*2);
	func5ins->SetParLimits(0,0,20000);
	func5ins->SetParLimits(1,peaks3ins[0]-sigma3ins,peaks3ins[0]+sigma3ins);
	func5ins->SetParLimits(2,0,sigma3ins);
	h3ins->Fit("func5ins","RB+"); //  El "+" agrega la función a la lista de funciones del histograma y el "B" es para considerar los limites.
		
	yc1[2]= func5ins->GetParameter(1); 
	eyc1[2]= func5ins->GetParameter(2); 

	
	TF1 *func6ins = new TF1("func6ins","gaus(0)+pol0(3)",peaks3ins[1]-0.5,peaks3ins[1]+0.5); //hago un fit de la gausiana, con el primer parametro de nombre 0
	func6ins->SetLineColor(kGreen);
	func6ins->SetParameters(peaksY3ins[1],peaks3ins[1],2,0);
	func6ins->SetParLimits(0,0,2000);
	func6ins->SetParLimits(1,peaks3ins[1]-2,peaks3ins[1]+2);
	func6ins->SetParLimits(2,0,3);
	func6ins->SetParLimits(3,0,1000000);
	h3ins->Fit("func6ins","RB+"); //  El "+" agrega la función a la lista de funciones del histograma y el "B" es para considerar los limites.

	yf1[2]= func6ins->GetParameter(1); 
	eyf1[2]= func6ins->GetParameter(2); 

	//canvas3->cd(2);
    canvas1->cd(6);
	adc->Draw("sf2f>>hist3esp(1000,-10,50)","vt<420&&vt>390"); 
		gPad->SetLogy();
	TH1F* h3esp;
	h3esp = (TH1F *)gROOT->FindObject("hist3esp"); // Busco el objeto y asigno un puntero para el histograma recien dibujado

	TSpectrum *spec3esp= new TSpectrum(10);	
	 //finding Spectra Peaks
	Int_t npeaks3esp = spec3esp->Search(h3esp,1,"",0.005); // Al objeto de clase TSpectrum le hago buscar peaks en el histograma apuntado por el puntero h
	//Finding peaks and fitting gaussians
	Double_t *peaks3esp = spec3esp->GetPositionX(); // Obtengo las posiciones de los peaks, guardadas en un arreglo de mayor a menor en amplitud
	Double_t *peaksY3esp= spec3esp->GetPositionY();
	Double_t sigma3esp = 1;

	std::cout<<"Se encontraron los peaks de flujo esp, donde peaks[0] es "<<peaks3esp[0]<<std::endl;

	TF1 *func5esp = new TF1("func5esp","gaus(0)",peaks3esp[0]-2*sigma3esp,peaks3esp[0]+2*sigma3esp); //hago un fit de la gausiana, con el primer parametro de nombre 0
	func5esp->SetLineColor(kRed);
	func5esp->SetParameters(peaksY3esp[0],peaks3esp[0],sigma3esp);
	func5esp->SetParLimits(0,0,20000);
	func5esp->SetParLimits(1,peaks3esp[0]-sigma3esp,peaks3esp[0]+sigma3esp);
	func5esp->SetParLimits(2,0,sigma3esp);
	h3esp->Fit("func5esp","RB+"); //  El "+" agrega la función a la lista de funciones del histograma y el "B" es para considerar los limites.
		
	yc2[2]= func5esp->GetParameter(1); 
	eyc2[2]= func5esp->GetParameter(2); 


	canvas1b->cd(3);
	adc->Draw("vt>>hist3b(10000,0,700)","vt<420&&vt>390"); 
		gPad->SetLogy();
	TH1F* h3b;
	h3b = (TH1F *)gROOT->FindObject("hist3b"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	
	x[2]= h3b->GetMean();
	ex[2]=h3b->GetStdDev();
	
		
	std::cout<<"El volumen tidal medido por el ventilador es "<< x[2]<<"y el volumen inspirado "<<h3ins->GetMean()/20.<<" escalado con densidad "<< h3ins->GetMean()/20.*(rho_aire/densidad_ins) <<std::endl;
	std::cout<<"El volumen tidal medido por el ventilador es "<< x[2]<<"y el volumen espirado "<<h3esp->GetMean()/20.<<" escalado con densidad "<< h3esp->GetMean()/20.*(rho_aire/densidad_ins) <<std::endl;
	
	
	//****************************************************************************************************************************//
	
	//TCanvas *canvas4 = new TCanvas("canvas4","Histogramas");
	//canvas4->Divide(2,1);
	//canvas4->cd(1);
	canvas1->cd(7);
	adc->Draw("sf1f>>hist4ins(1000,-10,50)","vt<310&&vt>280"); 
		gPad->SetLogy();
	TH1F* h4ins;
	h4ins = (TH1F *)gROOT->FindObject("hist4ins"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	

	
	TSpectrum *spec4ins= new TSpectrum(10);	
	 //finding Spectra Peaks
	Int_t npeaks4ins = spec4ins->Search(h4ins,1,"",0.005); // Al objeto de clase TSpectrum le hago buscar peaks en el histograma apuntado por el puntero h
	//Finding peaks and fitting gaussians
	Double_t *peaks4ins = spec4ins->GetPositionX(); // Obtengo las posiciones de los peaks, guardadas en un arreglo de mayor a menor en amplitud
	Double_t *peaksY4ins = spec4ins->GetPositionY();
	Double_t sigma4ins = 1;
	if(npeaks4ins > 1)
		std::cout<<"Se encontraron los peaks de flujo, donde peaks[0] es "<<peaks4ins[0]<<"y peaks[1] es "<<peaks4ins[1]<<std::endl;
	else
	{
		std::cout<<"Se encontraron menos de 2 peaks"<<std::endl;
		return 1;
	}
	TF1 *func7ins = new TF1("func7ins","gaus(0)",peaks4ins[0]-2*sigma4ins,peaks4ins[0]+2*sigma4ins); //hago un fit de la gausiana, con el primer parametro de nombre 0
	func7ins->SetLineColor(kRed);
	func7ins->SetParameters(peaksY4ins[0],peaks4ins[0],sigma4ins);
	func7ins->SetParLimits(0,0,20000);
	func7ins->SetParLimits(1,peaks4ins[0]-sigma4ins,peaks4ins[0]+sigma4ins);
	func7ins->SetParLimits(2,0,sigma4ins);
	h4ins->Fit("func7ins","RB+"); //  El "+" agrega la función a la lista de funciones del histograma y el "B" es para considerar los limites.
	
	yc1[3]= func7ins->GetParameter(1); 
	eyc1[3]= func7ins->GetParameter(2); 
	
	TF1 *func8ins = new TF1("func8ins","gaus(0)+pol0(3)",peaks4ins[1]-0.5,peaks4ins[1]+0.5); //hago un fit de la gausiana, con el primer parametro de nombre 0
	func8ins->SetLineColor(kGreen);
	func8ins->SetParameters(peaksY4ins[1],peaks4ins[1],2);
	func8ins->SetParLimits(0,0,2000);
	func8ins->SetParLimits(1,peaks4ins[1]-1,peaks4ins[1]+1);
	func8ins->SetParLimits(2,0,1);
	func8ins->SetParLimits(3,0,1000000);
	h4ins->Fit("func8ins","RB+"); //  El "+" agrega la función a la lista de funciones del histograma y el "B" es para considerar los limites.

	yf1[3]= func8ins->GetParameter(1); 
	eyf1[3]= func8ins->GetParameter(2); 
	
	//canvas4->cd(2);
	canvas1->cd(8);
	
	adc->Draw("sf2f>>hist4esp(1000,-10,50)","vt<310&&vt>280"); 
		gPad->SetLogy();
	TH1F* h4esp;
	h4esp = (TH1F *)gROOT->FindObject("hist4esp"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	
	TSpectrum *spec4esp= new TSpectrum(10);	
	 //finding Spectra Peaks
	Int_t npeaks4esp = spec4esp->Search(h4esp,1,"",0.005); // Al objeto de clase TSpectrum le hago buscar peaks en el histograma apuntado por el puntero h
	//Finding peaks and fitting gaussians
	Double_t *peaks4esp = spec4esp->GetPositionX(); // Obtengo las posiciones de los peaks, guardadas en un arreglo de mayor a menor en amplitud
	Double_t *peaksY4esp= spec4esp->GetPositionY();
	Double_t sigma4esp = 0.5;

	std::cout<<"Se encontraron los peaks de flujo esp, donde peaks[0] es "<<peaks4esp[0]<<std::endl;

	TF1 *func7esp = new TF1("func7esp","gaus(0)",peaks4esp[0]-2*sigma4esp,peaks4esp[0]+2*sigma4esp); //hago un fit de la gausiana, con el primer parametro de nombre 0
	func7esp->SetLineColor(kRed);
	func7esp->SetParameters(peaksY4esp[0],peaks4esp[0],sigma4esp);
	func7esp->SetParLimits(0,0,20000);
	func7esp->SetParLimits(1,peaks4esp[0]-2*sigma4esp,peaks4esp[0]+2*sigma4esp);
	func7esp->SetParLimits(2,0,sigma4esp);
	h4esp->Fit("func7esp","RB+"); //  El "+" agrega la función a la lista de funciones del histograma y el "B" es para considerar los limites.
	
	yc2[3]= func7esp->GetParameter(1); 
	eyc2[3]= func7esp->GetParameter(2); 
	

	canvas1b->cd(4);
	adc->Draw("vt>>hist4b(10000,0,700)","vt<310&&vt>280"); 
		gPad->SetLogy();
	TH1F* h4b;
	h4b = (TH1F *)gROOT->FindObject("hist4b"); // Busco el objeto y asigno un puntero para el histograma recien dibujado

	x[3]= h4b->GetMean();
	ex[3]=h4b->GetStdDev();
	
	std::cout<<"El volumen tidal medido por el ventilador es "<< x[3]<<"y el volumen inspirado "<<h4ins->GetMean()/20.<<" escalado con densidad "<< h4ins->GetMean()/20.*(rho_aire/densidad_ins) <<std::endl;
	std::cout<<"El volumen tidal medido por el ventilador es "<< x[3]<<"y el volumen espirado "<<h4esp->GetMean()/20.<<" escalado con densidad "<< h4esp->GetMean()/20.*(rho_aire/densidad_ins) <<std::endl;
	
	
	//****************************************************************************************************************************//
	
	
	//TCanvas *canvas5 = new TCanvas("canvas5","Histogramas");
	//canvas5->Divide(2,1);
	//canvas5->cd(1);
	canvas1->cd(9);
	adc->Draw("sf1f>>hist5ins(1000,-10,50)","vt<210&&vt>190"); 
		gPad->SetLogy();
	TH1F* h5ins;
	h5ins = (TH1F *)gROOT->FindObject("hist5ins"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	
	TSpectrum *spec5ins= new TSpectrum(10);	
	 //finding Spectra Peaks
	Int_t npeaks5ins = spec5ins->Search(h5ins,1,"",0.005); // Al objeto de clase TSpectrum le hago buscar peaks en el histograma apuntado por el puntero h
	//Finding peaks and fitting gaussians
	Double_t *peaks5ins = spec5ins->GetPositionX(); // Obtengo las posiciones de los peaks, guardadas en un arreglo de mayor a menor en amplitud
	Double_t *peaksY5ins= spec5ins->GetPositionY();
	Double_t sigma5ins = 0.25;
	if(npeaks5ins > 1)
		std::cout<<"Se encontraron los peaks de flujo, donde peaks[0] es "<<peaks5ins[0]<<"y peaks[1] es "<<peaks5ins[1]<<std::endl;
	else
	{
		std::cout<<"Se encontraron menos de 2 peaks"<<std::endl;
		return 1;
	}
	TF1 *func9ins = new TF1("func9ins","gaus(0)",peaks5ins[0]-2*sigma5ins,peaks5ins[0]+2*sigma5ins); //hago un fit de la gausiana, con el primer parametro de nombre 0
	func9ins->SetLineColor(kRed);
	func9ins->SetParameters(peaksY5ins[0],peaks5ins[0],sigma5ins);
	func9ins->SetParLimits(0,0,20000);
	func9ins->SetParLimits(1,peaks5ins[0]-sigma5ins,peaks5ins[0]+sigma5ins);
	func9ins->SetParLimits(2,0,2);
	h5ins->Fit("func9ins","RB+"); //  El "+" agrega la función a la lista de funciones del histograma y el "B" es para considerar los limites.
	yc1[4]= func9ins->GetParameter(1); 
	eyc1[4]= func9ins->GetParameter(2); 
	
	
	TF1 *func10ins = new TF1("func10ins","gaus(0)+pol0(3)",peaks5ins[1]-0.5,peaks5ins[1]+0.5); //hago un fit de la gausiana, con el primer parametro de nombre 0
	func10ins->SetLineColor(kGreen);
	func10ins->SetParameters(peaksY5ins[1],peaks5ins[1],0.4,0);
	func10ins->SetParLimits(0,0,2000);
	func10ins->SetParLimits(1,peaks5ins[1]-0.7,peaks5ins[1]+0.7);
	func10ins->SetParLimits(2,0,3);
	func10ins->SetParLimits(3,0,10000);
	h5ins->Fit("func10ins","RB+"); //  El "+" agrega la función a la lista de funciones del histograma y el "B" es para considerar los limites.
	yf1[4]= func10ins->GetParameter(1); 
	eyf1[4]= func10ins->GetParameter(2); 
	
	
	//canvas5->cd(2);
	canvas1->cd(10);
	
	adc->Draw("sf2f>>hist5esp(1000,-10,50)","vt<210&&vt>190"); 
		gPad->SetLogy();
	TH1F* h5esp;
	h5esp = (TH1F *)gROOT->FindObject("hist5esp"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	
	TSpectrum *spec5esp= new TSpectrum(1);	
	 //finding Spectra Peaks
	Int_t npeaks5esp = spec5esp->Search(h5esp,1,"",0.005); // Al objeto de clase TSpectrum le hago buscar peaks en el histograma apuntado por el puntero h
	//Finding peaks and fitting gaussians
	Double_t *peaks5esp = spec5esp->GetPositionX(); // Obtengo las posiciones de los peaks, guardadas en un arreglo de mayor a menor en amplitud
	Double_t *peaksY5esp= spec5esp->GetPositionY();
	Double_t sigma5esp = 0.25;

	std::cout<<"Se encontraron los peaks de flujo esp, donde peaks[0] es "<<peaks5esp[0]<<std::endl;

	TF1 *func9esp = new TF1("func9esp","gaus(0)",peaks5esp[0]-2*sigma5esp,peaks5esp[0]+2*sigma5esp); //hago un fit de la gausiana, con el primer parametro de nombre 0
	func9esp->SetLineColor(kRed);
	func9esp->SetParameters(peaksY5esp[0],peaks5esp[0],sigma5esp);
	func9esp->SetParLimits(0,0,20000);
	func9esp->SetParLimits(1,peaks5esp[0]-sigma5esp,peaks5esp[0]+sigma5esp);
	func9esp->SetParLimits(2,0,2);
	h5esp->Fit("func9esp","RB+"); //  El "+" agrega la función a la lista de funciones del histograma y el "B" es para considerar los limites.
	yc2[4]= func9esp->GetParameter(1); 
	eyc2[4]= func9esp->GetParameter(2); 
	
	
	canvas1b->cd(5);
	adc->Draw("vt>>hist5b(10000,0,700)","vt<210&&vt>190"); 
		gPad->SetLogy();
	TH1F* h5b;
	h5b = (TH1F *)gROOT->FindObject("hist5b"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	
	x[4]= h5b->GetMean();
	ex[4]=h5b->GetStdDev();
	
	std::cout<<"El volumen tidal medido por el ventilador es "<< x[4]<<"y el volumen inspirado "<<h5ins->GetMean()/20.<<" escalado con densidad "<< h5ins->GetMean()/20.*(rho_aire/densidad_ins) <<std::endl;
	std::cout<<"El volumen tidal medido por el ventilador es "<< x[4]<<"y el volumen espirado "<<h5esp->GetMean()/20.<<" escalado con densidad "<< h5esp->GetMean()/20.*(rho_aire/densidad_ins) <<std::endl;
	
	
	//******************************//
	std::cout<<"****************************************************************************"<<std::endl;
	TCanvas *canvas6 = new TCanvas("canvas6","Histogramas");
	canvas6->SetLogz();
	adc->Draw("sf1f:vt","","BOXCOLZ");
	
	
	TGraphErrors *grfins = new TGraphErrors(5,x,yf1,ex,eyf1);
	
	TGraphErrors *grcins = new TGraphErrors(5,x,yc1,ex,eyc1);

	grfins->SetMarkerColor(4);
	grfins->SetMarkerStyle(21);
	grfins->Fit("pol1");
   //Access the fit resuts
	TF1 *f1ins = grfins->GetFunction("pol1");
	f1ins->SetLineWidth(1);
	grfins->Draw("same");
	
	grcins->SetMarkerColor(4);
	grcins->SetMarkerStyle(21);
	grcins->Fit("pol0");
	//Access the fit resuts
	TF1 *f2ins = grcins->GetFunction("pol0");
	f2ins->SetLineWidth(1);
	grcins->Draw("same");
	
	
	Double_t scaleIns=f2ins->GetParameter(0);
	std::cout<<"el escalamiento del cero inspiración "<< scaleIns <<std::endl;
	
	TCanvas *canvas1c = new TCanvas("canvas1c","Histogramas");
	canvas1c->Divide(2,5);
	canvas1c->cd(1);
	adc->Draw(Form("sf1f - %f>>hist1cins(1000,-10,50)",scaleIns),"vt<610&&vt>580"); 
	TH1F* h1cins;
	h1cins = (TH1F *)gROOT->FindObject("hist1cins"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	h1cins->Sumw2();
	//usar standard error of the mean
	vinsh[0]=h1cins->GetMean()/20.*1000.;
	//	evinsh[0]=h1cins->GetSumw2()->GetSum()/20.*1000.;
	evinsh[0]=h1cins->GetStdDev()/std::sqrt(h1cins->GetEntries())/20.*1000.;
	vinsrho[0]=h1cins->GetMean()/20.*(rho_aire/densidad_ins)*1000.;
	// evinsrho[0]=h1cins->GetSumw2()->GetSum()/20.*(rho_aire/densidad_ins)*1000.;
	evinsrho[0]=h1cins->GetStdDev()/std::sqrt(h1cins->GetEntries())/20.*(rho_aire/densidad_ins)*1000.;
	
	std::cout<<"El volumen tidal medido por el ventilador es "<< x[0]<<"+/-"<< ex[0]<<" y del historgrama en inspiración es "<<vinsh[0]<<"+/-"<< evinsh[0]<<" escalado con rho "<<vinsrho[0]<<"+/-"<< evinsrho[0]<<std::endl;
	
	canvas1c->cd(3);
	adc->Draw(Form("sf1f - %f>>hist2cins(1000,-10,50)",scaleIns),"vt<510&&vt>480"); 
	TH1F* h2cins;
	h2cins = (TH1F *)gROOT->FindObject("hist2cins"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	h2cins->Sumw2();
	
	vinsh[1]=h2cins->GetMean()/20.*1000.;
	//evinsh[1]=h2cins->GetSumw2()->GetSum()/20.*1000.;
	evinsh[1]=h2cins->GetStdDev()/std::sqrt(h2cins->GetEntries())/20.*1000.;
	vinsrho[1]=h2cins->GetMean()/20.*(rho_aire/densidad_ins)*1000.;
	//evinsrho[1]=h2cins->GetSumw2()->GetSum()/20.*(rho_aire/densidad_ins)*1000.;
	evinsrho[1]=h2cins->GetStdDev()/std::sqrt(h2cins->GetEntries())/20.*(rho_aire/densidad_ins)*1000.;
	
	std::cout<<"El volumen tidal medido por el ventilador es "<< x[1]<<"+/-"<< ex[1]<<" y del historgrama en inspiración es "<<vinsh[1]<<"+/-"<< evinsh[1]<<" escalado con rho "<<vinsrho[1]<<"+/-"<< evinsrho[1]<<std::endl;
	
	canvas1c->cd(5);
	adc->Draw(Form("sf1f - %f>>hist3cins(1000,-10,50)",scaleIns),"vt<420&&vt>390"); 
	TH1F* h3cins;
	h3cins = (TH1F *)gROOT->FindObject("hist3cins"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	h3cins->Sumw2();
	
	vinsh[2]=h3cins->GetMean()/20.*1000.;
	//evinsh[2]=h3cins->GetSumw2()->GetSum()/20.*1000.;
	evinsh[2]=h3cins->GetStdDev()/std::sqrt(h3cins->GetEntries())/20.*1000.;
	vinsrho[2]=h3cins->GetMean()/20.*(rho_aire/densidad_ins)*1000.;
	//evinsrho[2]=h3cins->GetSumw2()->GetSum()/20.*(rho_aire/densidad_ins)*1000.;
	evinsrho[2]=h3cins->GetStdDev()/std::sqrt(h3cins->GetEntries())/20.*(rho_aire/densidad_ins)*1000.;

	
	std::cout<<"El volumen tidal medido por el ventilador es "<< x[2]<<"+/-"<< ex[2]<<" y del historgrama en inspiración es "<<vinsh[2]<<"+/-"<< evinsh[2]<<" escalado con rho "<<vinsrho[2]<<"+/-"<< evinsrho[2]<<std::endl;
	
	canvas1c->cd(7);
	adc->Draw(Form("sf1f - %f>>hist4cins(1000,-10,50)",scaleIns),"vt<310&&vt>280"); 
	TH1F* h4cins;
	h4cins = (TH1F *)gROOT->FindObject("hist4cins"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	h4cins->Sumw2();
	
	vinsh[3]=h4cins->GetMean()/20.*1000.;
	//evinsh[3]=h4cins->GetSumw2()->GetSum()/20.*1000.;
	evinsh[3]=h4cins->GetStdDev()/std::sqrt(h4cins->GetEntries())/20.*1000.;
	vinsrho[3]=h4cins->GetMean()/20.*(rho_aire/densidad_ins)*1000.;
	//evinsrho[3]=h4cins->GetSumw2()->GetSum()/20.*(rho_aire/densidad_ins)*1000.;
	evinsrho[3]=h4cins->GetStdDev()/std::sqrt(h4cins->GetEntries())/20.*(rho_aire/densidad_ins)*1000.;
	
	std::cout<<"El volumen tidal medido por el ventilador es "<< x[3]<<"+/-"<< ex[3]<<" y del historgrama en inspiración es "<<vinsh[3]<<"+/-"<< evinsh[3]<<" escalado con rho "<<vinsrho[3]<<"+/-"<< evinsrho[3]<<std::endl;
	
	canvas1c->cd(9);
	adc->Draw(Form("sf1f - %f>>hist5cins(1000,-10,50)",scaleIns),"vt<210&&vt>190"); 
	TH1F* h5cins;
	h5cins = (TH1F *)gROOT->FindObject("hist5cins"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	h5cins->Sumw2();
	
	vinsh[4]=h5cins->GetMean()/20.*1000.;
	//evinsh[4]=h5cins->GetSumw2()->GetSum()/20.*1000.;
	evinsh[4]=h5cins->GetStdDev()/std::sqrt(h5cins->GetEntries())/20.*1000.;
	vinsrho[4]=h5cins->GetMean()/20.*(rho_aire/densidad_ins)*1000.;
	//evinsrho[4]=h5cins->GetSumw2()->GetSum()/20.*(rho_aire/densidad_ins)*1000.;
	evinsrho[4]=h5cins->GetStdDev()/std::sqrt(h5cins->GetEntries())/20.*(rho_aire/densidad_ins)*1000.;
	
	std::cout<<"El volumen tidal medido por el ventilador es "<< x[4]<<"+/-"<< ex[4]<<" y del historgrama en inspiración es "<<vinsh[4]<<"+/-"<< evinsh[4]<<" escalado con rho "<<vinsrho[4]<<"+/-"<< evinsrho[4]<<std::endl;
	
	//***************************************************************//
	
	TCanvas *canvas6a = new TCanvas("canvas6a","Histogramas");
	canvas6a->SetLogz();
	adc->Draw("sf2f:vt","","BOXCOLZ");
		
	TGraphErrors *grcesp = new TGraphErrors(5,x,yc2,ex,eyc2);

	grcesp->SetMarkerColor(4);
	grcesp->SetMarkerStyle(21);
	grcesp->Fit("pol0");
   //Access the fit resuts
	TF1 *f2esp = grcesp->GetFunction("pol0");
	f2esp->SetLineWidth(1);
	grcesp->Draw("same");
	
	Double_t scaleEsp=f2esp->GetParameter(0);
	std::cout<<"el escalamiento del cero espiración "<< scaleEsp <<std::endl;
	
	canvas1c->cd(2);
	adc->Draw(Form("sf2f - %f>>hist1cesp(1000,-10,50)",scaleEsp),"vt<610&&vt>580"); 
	TH1F* h1cesp;
	h1cesp = (TH1F *)gROOT->FindObject("hist1cesp"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	h1cesp->Sumw2();
	
	vesph[0]=h1cesp->GetMean()/20.*1000.;
	//evesph[0]=h1cesp->GetSumw2()->GetSum()/20.*1000.;
	evesph[0]=h1cesp->GetStdDev()/std::sqrt(h1cesp->GetEntries())/20.*1000.;
	vesprho[0]=h1cesp->GetMean()/20.*(rho_aire/densidad_ins)*1000.;
	//evesprho[0]=h1cesp->GetSumw2()->GetSum()/20.*(rho_aire/densidad_ins)*1000.;
	evesprho[0]=h1cesp->GetStdDev()/std::sqrt(h1cesp->GetEntries())/20.*(rho_aire/densidad_ins)*1000.;
	
	std::cout<<"El volumen tidal es "<< x[0]<<"+/-"<< ex[0]<<" y el espirado en histograma es "<<vesph[0]<<"+/-"<<evesph[0] <<" escalado con densidad "<< vesprho[0] <<"+/-"<< evesprho[0]<<std::endl;
	
	canvas1c->cd(4);
	adc->Draw(Form("sf2f - %f>>hist2cesp(1000,-10,50)",scaleEsp),"vt<510&&vt>480"); 
	TH1F* h2cesp;
	h2cesp = (TH1F *)gROOT->FindObject("hist2cesp"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	h2cesp->Sumw2();
		
	vesph[1]=h2cesp->GetMean()/20.*1000.;
	//evesph[1]=h2cesp->GetSumw2()->GetSum()/20.*1000.;
	evesph[1]=h2cesp->GetStdDev()/std::sqrt(h2cesp->GetEntries())/20.*1000.;
	vesprho[1]=h2cesp->GetMean()/20.*(rho_aire/densidad_ins)*1000.;
	//evesprho[1]=h2cesp->GetSumw2()->GetSum()/20.*(rho_aire/densidad_ins)*1000.;
	evesprho[1]=h2cesp->GetStdDev()/std::sqrt(h2cesp->GetEntries())/20.*(rho_aire/densidad_ins)*1000.;
	
	std::cout<<"El volumen tidal es "<< x[1]<<"+/-"<< ex[1]<<" y el espirado en histograma es "<<vesph[1]<<"+/-"<<evesph[1] <<" escalado con densidad "<< vesprho[1] <<"+/-"<< evesprho[1]<<std::endl;
	
	canvas1c->cd(6);
	adc->Draw(Form("sf2f - %f>>hist3cesp(1000,-10,50)",scaleEsp),"vt<420&&vt>390"); 
	TH1F* h3cesp;
	h3cesp = (TH1F *)gROOT->FindObject("hist3cesp"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	h3cesp->Sumw2();
	
	vesph[2]=h3cesp->GetMean()/20.*1000.;
	//evesph[2]=h3cesp->GetSumw2()->GetSum()/20.*1000.;
	evesph[2]=h3cesp->GetStdDev()/std::sqrt(h3cesp->GetEntries())/20.*1000.;
	vesprho[2]=h3cesp->GetMean()/20.*(rho_aire/densidad_ins)*1000.;
	//evesprho[2]=h3cesp->GetSumw2()->GetSum()/20.*(rho_aire/densidad_ins)*1000.;
	evesprho[2]=h3cesp->GetStdDev()/std::sqrt(h3cesp->GetEntries())/20.*(rho_aire/densidad_ins)*1000.;
	
	std::cout<<"El volumen tidal es "<< x[2]<<"+/-"<< ex[2]<<" y el espirado en histograma es "<<vesph[2]<<"+/-"<<evesph[2] <<" escalado con densidad "<< vesprho[2] <<"+/-"<< evesprho[2]<<std::endl;
	
	canvas1c->cd(8);
	adc->Draw(Form("sf2f - %f>>hist4cesp(1000,-10,50)",scaleEsp),"vt<310&&vt>280"); 
	TH1F* h4cesp;
	h4cesp = (TH1F *)gROOT->FindObject("hist4cesp"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	h4cesp->Sumw2();
	
	vesph[3]=h4cesp->GetMean()/20.*1000.;
	//evesph[3]=h4cesp->GetSumw2()->GetSum()/20.*1000.;
	evesph[3]=h4cesp->GetStdDev()/std::sqrt(h4cesp->GetEntries())/20.*1000.;
	vesprho[3]=h4cesp->GetMean()/20.*(rho_aire/densidad_ins)*1000.;
	//evesprho[3]=h4cesp->GetSumw2()->GetSum()/20.*(rho_aire/densidad_ins)*1000.;
	evesprho[3]=h4cesp->GetStdDev()/std::sqrt(h4cesp->GetEntries())/20.*(rho_aire/densidad_ins)*1000.;
	
	std::cout<<"El volumen tidal es "<< x[3]<<"+/-"<< ex[3]<<" y el espirado en histograma es "<<vesph[3]<<"+/-"<<evesph[3] <<" escalado con densidad "<< vesprho[3] <<"+/-"<< evesprho[3]<<std::endl;
	
	canvas1c->cd(10);
	adc->Draw(Form("sf2f - %f>>hist5cesp(1000,-10,50)",scaleEsp),"vt<210&&vt>190"); 
	TH1F* h5cesp;
	h5cesp = (TH1F *)gROOT->FindObject("hist5cesp"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	h5cesp->Sumw2();	
	
	vesph[4]=h5cesp->GetMean()/20.*1000.;
	//evesph[4]=h5cesp->GetSumw2()->GetSum()/20.*1000.;
	evesph[4]=h5cesp->GetStdDev()/std::sqrt(h5cesp->GetEntries())/20.*1000.;
	vesprho[4]=h5cesp->GetMean()/20.*(rho_aire/densidad_ins)*1000.;
	//evesprho[4]=h5cesp->GetSumw2()->GetSum()/20.*(rho_aire/densidad_ins)*1000.;
	evesprho[4]=h5cesp->GetStdDev()/std::sqrt(h5cesp->GetEntries())/20.*(rho_aire/densidad_ins)*1000.;
	
	std::cout<<"El volumen tidal es "<< x[4]<<"+/-"<< ex[4]<<" y el espirado en histograma es "<<vesph[4]<<"+/-"<<evesph[4] <<" escalado con densidad "<< vesprho[4] <<"+/-"<< evesprho[4]<<std::endl;

	Double_t x_set[]={600.,500.,400.,300.,200.};
	Double_t ex_set[]={0.,0.,0.,0.,0.};
	
	/*
	
	Double_t x[5]; //vol tidal
	Double_t ex[5]; // error vol tidal

	Double_t vinsh[5]; // V ins hist
	Double_t evinsh[5]; // error V ins hist
	
	Double_t vinsrho[5]; // V ins rho
	Double_t evinsrho[5]; // error V ins rho
	
	Double_t vesph[5]; // V esp hist
	Double_t evesph[5]; // error V esp hist
	
	Double_t vesprho[5]; // V esp rho
	Double_t evesprho[5]; // error V esp rho
	*/
	
	TCanvas *canvas7 = new TCanvas("canvas7","Graficos");
		
	TGraphErrors *grafvinsh = new TGraphErrors(5,x_set,vinsh,ex_set,evinsh);

	grafvinsh ->SetMarkerColor(42);
	grafvinsh ->SetMarkerStyle(21);
	grafvinsh ->Fit("pol1");
	TF1 *fgrafvinsh  = grafvinsh->GetFunction("pol1");
	fgrafvinsh->SetLineWidth(4);
	fgrafvinsh->SetLineColor(kOrange);
	grafvinsh->Draw("ALP");
	
	TGraphErrors *grafvinsrho = new TGraphErrors(5,x_set,vinsrho,ex_set,evinsrho);

	grafvinsrho ->SetMarkerColor(8);
	grafvinsrho ->SetMarkerStyle(22);
	grafvinsrho ->Fit("pol1");
	TF1 *fgrafvinsrho  = grafvinsrho->GetFunction("pol1");
	fgrafvinsrho->SetLineWidth(4);
	fgrafvinsrho->SetLineColor(kGreen);
	grafvinsrho->Draw("same");
	
	TGraphErrors *grafvesph = new TGraphErrors(5,x_set,vesph,ex_set,evesph);

	grafvesph ->SetMarkerColor(4);
	grafvesph ->SetMarkerStyle(24);
	grafvesph ->Fit("pol1");
	TF1 *fgrafvesph  = grafvesph->GetFunction("pol1");
	fgrafvesph->SetLineWidth(4);
	fgrafvesph->SetLineColor(kBlue);
	grafvesph->Draw("same");
	
	TGraphErrors *grafvesprho = new TGraphErrors(5,x_set,vesprho,ex_set,evesprho);

	grafvesprho ->SetMarkerColor(5);
	grafvesprho ->SetMarkerStyle(28);
	grafvesprho ->Fit("pol1");
	TF1 *fgrafvesprho  = grafvesprho->GetFunction("pol1");
	fgrafvesprho->SetLineWidth(4);
	fgrafvesprho->SetLineColor(kYellow);
	grafvesprho->Draw("same");
	
	TGraphErrors *grafx = new TGraphErrors(5,x_set,x,ex_set,ex);

	grafx ->SetMarkerColor(6);
	grafx ->SetMarkerStyle(29);
	grafx ->Fit("pol1");
	TF1 *fgrafx  = grafx->GetFunction("pol1");
	fgrafx->SetLineWidth(4);
	fgrafx->SetLineColor(kMagenta);
	grafx->Draw("same");
	
	
	/*
	
	TCanvas *canvas7 = new TCanvas("canvas7","Graficos");
		
	TGraph *grafvinsh = new TGraph(5,x_set,vinsh);

	grafvinsh ->SetMarkerColor(42);
	grafvinsh ->SetMarkerStyle(21);
	grafvinsh ->Fit("pol1");
	TF1 *fgrafvinsh  = grafvinsh->GetFunction("pol1");
	fgrafvinsh->SetLineWidth(4);
	fgrafvinsh->SetLineColor(kOrange);
	grafvinsh->Draw("ALP");
	
	TGraph *grafvinsrho = new TGraph(5,x_set,vinsrho);

	grafvinsrho ->SetMarkerColor(8);
	grafvinsrho ->SetMarkerStyle(22);
	grafvinsrho ->Fit("pol1");
	TF1 *fgrafvinsrho  = grafvinsrho->GetFunction("pol1");
	fgrafvinsrho->SetLineWidth(4);
	fgrafvinsrho->SetLineColor(kGreen);
	grafvinsrho->Draw("same");
	
	TGraph *grafvesph = new TGraph(5,x_set,vesph);

	grafvesph ->SetMarkerColor(4);
	grafvesph ->SetMarkerStyle(24);
	grafvesph ->Fit("pol1");
	TF1 *fgrafvesph  = grafvesph->GetFunction("pol1");
	fgrafvesph->SetLineWidth(4);
	fgrafvesph->SetLineColor(kBlue);
	grafvesph->Draw("same");
	
	TGraph *grafvesprho = new TGraph(5,x_set,vesprho);

	grafvesprho ->SetMarkerColor(5);
	grafvesprho ->SetMarkerStyle(28);
	grafvesprho ->Fit("pol1");
	TF1 *fgrafvesprho  = grafvesprho->GetFunction("pol1");
	fgrafvesprho->SetLineWidth(4);
	fgrafvesprho->SetLineColor(kYellow);
	grafvesprho->Draw("same");
	
	TGraph *grafx = new TGraph(5,x_set,x);

	grafx ->SetMarkerColor(6);
	grafx ->SetMarkerStyle(29);
	grafx ->Fit("pol1");
	TF1 *fgrafx  = grafx->GetFunction("pol1");
	fgrafx->SetLineWidth(4);
	fgrafx->SetLineColor(kMagenta);
	grafx->Draw("same");
	*/
	
	//*****************************************************************************************//
	
	
	
	
	
	TCanvas *canvas8 = new TCanvas("canvas8","Histogramas");
	canvas6a->SetLogz();
	Int_t n1 = adc->Draw("sf1f:sf2f","","BOXCOLZ");
	
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
	
		TCanvas *canvasnuevo1 = new TCanvas("canvasnuevo1","Histogramas");
	adc->Draw("sf1f:vp1:vf1");
	
	return 0;
}
