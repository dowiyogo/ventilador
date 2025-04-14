#include <fstream>
#include <string>
#include <iostream>
#include <TH1F.h>


//archivo es el nombre del archivo

int lectura2(char *archivo){
	//ifstream file(archivo);
	//char buffer[100];
	

	TNtuple *adc = new TNtuple("adc","arduino","fn:vp:vt:sf1:sf1f:sf2:sf2f:pm");
	/*Double_t fn,vp,vt,sf1,sf1f,sf2,sf2f,pm;
	while(! file.eof()) {
	
	file.getline(buffer,100);
	std::sprintf(buffer,"%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f", fn,vp,vt,sf1,sf1f,sf2,sf2f,pm);
	adc->Fill(fn,vp,vt,sf1,sf1f,sf2,sf2f,pm);


  }*/
	Double_t x[5];
	Double_t ex[5];
	
	Double_t yc[5];
	Double_t eyc[5];
	
	  Float_t rho_aire = 1.29;
  Float_t densidad_ins = 1.350; //estaba 1800
	
  	adc->ReadFile(archivo);
	TCanvas *canvas1 = new TCanvas("canvas1","Histogramas");

	adc->Draw("sf2f>>hist1(1000,-10,50)","vt<610&&vt>580"); 
	TH1F* h1;
	h1 = (TH1F *)gROOT->FindObject("hist1"); // Busco el objeto y asigno un puntero para el histograma recien dibujado


	TSpectrum *spec= new TSpectrum(2,0.1);	
	 //finding Spectra Peaks
	Int_t npeaks = spec->Search(h1); // Al objeto de clase TSpectrum le hago buscar peaks en el histograma apuntado por el puntero h
	//Finding peaks and fitting gaussians
	Double_t *peaks = spec->GetPositionX(); // Obtengo las posiciones de los peaks, guardadas en un arreglo de mayor a menor en amplitud
	Double_t *peaksY= spec->GetPositionY();
	Double_t sigma = 2;

		std::cout<<"Se encontraron los peaks de ruido, donde peaks[0] es "<<peaks[0]<<std::endl;
	
	TF1 *func = new TF1("func","gaus(0)",peaks[0]-2*sigma,peaks[0]+2*sigma); //hago un fit de la gausiana, con el primer parametro de nombre 0
	func->SetLineColor(kRed);
	func->SetParameters(peaksY[0],peaks[0],sigma*2);
	func->SetParLimits(0,0,20000);
	func->SetParLimits(1,peaks[0]-sigma,peaks[0]+sigma);
	func->SetParLimits(2,0,3*sigma);
	h1->Fit("func","RB+"); //  El "+" agrega la función a la lista de funciones del histograma y el "B" es para considerar los limites.
	yc[0]= func->GetParameter(1); 
	eyc[0]= func->GetParameter(2); 
	

	TCanvas *canvas1b = new TCanvas("canvas1b","Histogramas");

	adc->Draw("vt>>hist1b(10000,0,700)","vt<610&&vt>580"); 
	TH1F* h1b;
	h1b = (TH1F *)gROOT->FindObject("hist1b"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	
	x[0]= h1b->GetMean();
	ex[0]=h1b->GetStdDev();
	
		std::cout<<"El volumen tidal medido por el ventilador es "<< x[0]<<"y el calculado del histograma es "<<h1->GetMean()/20.<<"escalado "<< h1->GetMean()/20.*(rho_aire/densidad_ins) <<std::endl;
	
	TCanvas *canvas2 = new TCanvas("canvas2","Histogramas");
	adc->Draw("sf2f>>hist2(1000,-10,50)","vt<510&&vt>480"); 
	TH1F* h2;
	h2 = (TH1F *)gROOT->FindObject("hist2"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	

	TSpectrum *spec2= new TSpectrum(1);	
	 //finding Spectra Peaks
	Int_t npeaks2 = spec2->Search(h2,0.0001); // Al objeto de clase TSpectrum le hago buscar peaks en el histograma apuntado por el puntero h
	//Finding peaks and fitting gaussians
	Double_t *peaks2 = spec2->GetPositionX(); // Obtengo las posiciones de los peaks, guardadas en un arreglo de mayor a menor en amplitud
	Double_t *peaksY2= spec2->GetPositionY();
	Double_t sigma2 = 2;

	std::cout<<"Se encontraron los peaks de ruido, donde peaks[0] es "<<peaks2[0]<<std::endl;

	TF1 *func3 = new TF1("func3","gaus(0)",peaks2[0]-2*sigma2,peaks2[0]+2*sigma2); //hago un fit de la gausiana, con el primer parametro de nombre 0
	func3->SetLineColor(kRed);
	func3->SetParameters(peaksY2[0],peaks2[0],sigma2*2);
	func3->SetParLimits(0,0,20000);
	func3->SetParLimits(1,peaks2[0]-sigma2,peaks2[0]+sigma2);
	func3->SetParLimits(2,0,3*sigma2);
	h2->Fit("func3","RB+"); //  El "+" agrega la función a la lista de funciones del histograma y el "B" es para considerar los limites.
	yc[1]= func3->GetParameter(1); 
	eyc[1]= func3->GetParameter(2); 
	
	
	TCanvas *canvas2b = new TCanvas("canvas2b","Histogramas");
	adc->Draw("vt>>hist2b(10000,0,700)","vt<510&&vt>480"); 
	TH1F* h2b;
	h2b = (TH1F *)gROOT->FindObject("hist2b"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	
	x[1]= h2b->GetMean();
	ex[1]=h2b->GetStdDev();
	
		std::cout<<"El volumen tidal medido por el ventilador es "<< x[1]<<"y el calculado del histograma es "<<h2->GetMean()/20.<<"escalado "<< h2->GetMean()/20.*(rho_aire/densidad_ins) <<std::endl;
		
	TCanvas *canvas3 = new TCanvas("canvas3","Histogramas");
	adc->Draw("sf2f>>hist3(1000,-10,50)","vt<420&&vt>390"); 
	TH1F* h3;
	h3 = (TH1F *)gROOT->FindObject("hist3"); // Busco el objeto y asigno un puntero para el histograma recien dibujado

	TSpectrum *spec3= new TSpectrum(1);	
	 //finding Spectra Peaks
	Int_t npeaks3 = spec3->Search(h3,1); // Al objeto de clase TSpectrum le hago buscar peaks en el histograma apuntado por el puntero h
	//Finding peaks and fitting gaussians
	Double_t *peaks3 = spec3->GetPositionX(); // Obtengo las posiciones de los peaks, guardadas en un arreglo de mayor a menor en amplitud
	Double_t *peaksY3= spec3->GetPositionY();
	Double_t sigma3 = 1;

		std::cout<<"Se encontraron los peaks de ruido, donde peaks[0] es "<<peaks3[0]<<std::endl;

	TF1 *func5 = new TF1("func5","gaus(0)",peaks3[0]-2*sigma3,peaks3[0]+2*sigma3); //hago un fit de la gausiana, con el primer parametro de nombre 0
	func5->SetLineColor(kRed);
	func5->SetParameters(peaksY3[0],peaks3[0],sigma3*2);
	func5->SetParLimits(0,0,20000);
	func5->SetParLimits(1,peaks3[0]-sigma3,peaks3[0]+sigma3);
	func5->SetParLimits(2,0,sigma3);
	h3->Fit("func5","RB+"); //  El "+" agrega la función a la lista de funciones del histograma y el "B" es para considerar los limites.
		
	yc[2]= func5->GetParameter(1); 
	eyc[2]= func5->GetParameter(2); 


	TCanvas *canvas3b = new TCanvas("canvas3b","Histogramas");
	adc->Draw("vt>>hist3b(10000,0,700)","vt<420&&vt>390"); 
	TH1F* h3b;
	h3b = (TH1F *)gROOT->FindObject("hist3b"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	
	x[2]= h3b->GetMean();
	ex[2]=h3b->GetStdDev();
	
		std::cout<<"El volumen tidal medido por el ventilador es "<< x[2]<<"y el calculado del histograma es "<<h3->GetMean()/20.<<"escalado "<< h3->GetMean()/20.*(rho_aire/densidad_ins) <<std::endl;
	
	TCanvas *canvas4 = new TCanvas("canvas4","Histogramas");
	adc->Draw("sf2f>>hist4(1000,-10,50)","vt<310&&vt>280"); 
	TH1F* h4;
	h4 = (TH1F *)gROOT->FindObject("hist4"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	

	
	TSpectrum *spec4= new TSpectrum(1);	
	 //finding Spectra Peaks
	Int_t npeaks4 = spec4->Search(h4,1); // Al objeto de clase TSpectrum le hago buscar peaks en el histograma apuntado por el puntero h
	//Finding peaks and fitting gaussians
	Double_t *peaks4 = spec4->GetPositionX(); // Obtengo las posiciones de los peaks, guardadas en un arreglo de mayor a menor en amplitud
	Double_t *peaksY4= spec4->GetPositionY();
	Double_t sigma4 = 1;

		std::cout<<"Se encontraron los peaks de ruido, donde peaks[0] es "<<peaks4[0]<<std::endl;

	TF1 *func7 = new TF1("func7","gaus(0)",peaks4[0]-2*sigma4,peaks4[0]+2*sigma4); //hago un fit de la gausiana, con el primer parametro de nombre 0
	func7->SetLineColor(kRed);
	func7->SetParameters(peaksY4[0],peaks4[0],sigma4);
	func7->SetParLimits(0,0,20000);
	func7->SetParLimits(1,peaks4[0]-sigma4,peaks4[0]+sigma4);
	func7->SetParLimits(2,0,sigma4);
	h4->Fit("func7","RB+"); //  El "+" agrega la función a la lista de funciones del histograma y el "B" es para considerar los limites.
	
	yc[3]= func7->GetParameter(1); 
	eyc[3]= func7->GetParameter(2); 
	

	TCanvas *canvas4b = new TCanvas("canvas4b","Histogramas");
	adc->Draw("vt>>hist4b(10000,0,700)","vt<310&&vt>280"); 
	TH1F* h4b;
	h4b = (TH1F *)gROOT->FindObject("hist4b"); // Busco el objeto y asigno un puntero para el histograma recien dibujado

	x[3]= h4b->GetMean();
	ex[3]=h4b->GetStdDev();
	
		std::cout<<"El volumen tidal medido por el ventilador es "<< x[3]<<"y el calculado del histograma es "<<h4->GetMean()/20.<<"escalado "<< h4->GetMean()/20.*(rho_aire/densidad_ins) <<std::endl;
	
	TCanvas *canvas5 = new TCanvas("canvas5","Histogramas");
	adc->Draw("sf2f>>hist5(1000,-10,50)","vt<210&&vt>190"); 
	TH1F* h5;
	h5 = (TH1F *)gROOT->FindObject("hist5"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	
	TSpectrum *spec5= new TSpectrum(1);	
	 //finding Spectra Peaks
	Int_t npeaks5 = spec5->Search(h5,1); // Al objeto de clase TSpectrum le hago buscar peaks en el histograma apuntado por el puntero h
	//Finding peaks and fitting gaussians
	Double_t *peaks5 = spec5->GetPositionX(); // Obtengo las posiciones de los peaks, guardadas en un arreglo de mayor a menor en amplitud
	Double_t *peaksY5= spec5->GetPositionY();
	Double_t sigma5 = 0.25;

		std::cout<<"Se encontraron los peaks de ruido, donde peaks[0] es "<<peaks5[0]<<std::endl;

	TF1 *func9 = new TF1("func9","gaus(0)",peaks5[0]-2*sigma5,peaks5[0]+2*sigma5); //hago un fit de la gausiana, con el primer parametro de nombre 0
	func9->SetLineColor(kRed);
	func9->SetParameters(peaksY5[0],peaks5[0],sigma5);
	func9->SetParLimits(0,0,20000);
	func9->SetParLimits(1,peaks5[0]-sigma5,peaks5[0]+sigma5);
	func9->SetParLimits(2,0,2);
	h5->Fit("func9","RB+"); //  El "+" agrega la función a la lista de funciones del histograma y el "B" es para considerar los limites.
	yc[4]= func9->GetParameter(1); 
	eyc[4]= func9->GetParameter(2); 
	
	
	TCanvas *canvas5b = new TCanvas("canvas5b","Histogramas");
	adc->Draw("vt>>hist5b(10000,0,700)","vt<210&&vt>190"); 
	TH1F* h5b;
	h5b = (TH1F *)gROOT->FindObject("hist5b"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	
	x[4]= h5b->GetMean();
	ex[4]=h5b->GetStdDev();
	
		std::cout<<"El volumen tidal medido por el ventilador es "<< x[4]<<"y el calculado del histograma es "<<h5->GetMean()/20.<<"escalado "<< h5->GetMean()/20.*(rho_aire/densidad_ins) <<std::endl;
	
	TCanvas *canvas6 = new TCanvas("canvas6","Histogramas");
	adc->Draw("sf2f:vt");

	
	TGraphErrors *grc = new TGraphErrors(5,x,yc,ex,eyc);

	
  grc->SetMarkerColor(4);
   grc->SetMarkerStyle(21);
      grc->Fit("pol0");
   //Access the fit resuts
   TF1 *f2 = grc->GetFunction("pol0");
   f2->SetLineWidth(1);
	grc->Draw("same");
	
	Double_t scale=f2->GetParameter(0);
	std::cout<<"el factor de escala es "<< scale <<std::endl;
	
	TCanvas *canvas1c = new TCanvas("canvas1c","Histogramas");
	adc->Draw(Form("sf2f - %f>>hist1c(1000,-10,50)",scale),"vt<610&&vt>580"); 
	TH1F* h1c;
	h1c = (TH1F *)gROOT->FindObject("hist1c"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	std::cout<<"El volumen tidal medido por el ventilador es "<< x[0]<<"y el calculado del histograma es "<<h1c->GetMean()/20.*1000.<<"escalado "<< h1c->GetMean()/20.*(rho_aire/densidad_ins)*1000. <<std::endl;
	
	
	TCanvas *canvas2c = new TCanvas("canvas2c","Histogramas");
	adc->Draw(Form("sf2f - %f>>hist2c(1000,-10,50)",scale),"vt<510&&vt>480"); 
	TH1F* h2c;
	h2c = (TH1F *)gROOT->FindObject("hist2c"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	std::cout<<"El volumen tidal medido por el ventilador es "<< x[1]<<"y el calculado del histograma es "<<h2c->GetMean()/20.*1000.<<"escalado "<< h2c->GetMean()/20.*(rho_aire/densidad_ins)*1000. <<std::endl;
	
	
	TCanvas *canvas3c = new TCanvas("canvas3c","Histogramas");
	adc->Draw(Form("sf2f - %f>>hist3c(1000,-10,50)",scale),"vt<420&&vt>390"); 
	TH1F* h3c;
	h3c = (TH1F *)gROOT->FindObject("hist3c"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	std::cout<<"El volumen tidal medido por el ventilador es "<< x[2]<<"y el calculado del histograma es "<<h3c->GetMean()/20.*1000.<<"escalado "<< h3c->GetMean()/20.*(rho_aire/densidad_ins)*1000. <<std::endl;
	
	TCanvas *canvas4c = new TCanvas("canvas4c","Histogramas");
	adc->Draw(Form("sf2f - %f>>hist4c(1000,-10,50)",scale),"vt<310&&vt>280"); 
	TH1F* h4c;
	h4c = (TH1F *)gROOT->FindObject("hist4c"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	std::cout<<"El volumen tidal medido por el ventilador es "<< x[3]<<"y el calculado del histograma es "<<h4c->GetMean()/20.*1000.<<"escalado "<< h4c->GetMean()/20.*(rho_aire/densidad_ins)*1000. <<std::endl;
	
	TCanvas *canvas5c = new TCanvas("canvas5c","Histogramas");
	adc->Draw(Form("sf2f - %f>>hist5c(1000,-10,50)",scale),"vt<210&&vt>190"); 
	TH1F* h5c;
	h5c = (TH1F *)gROOT->FindObject("hist5c"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	std::cout<<"El volumen tidal medido por el ventilador es "<< x[4]<<"y el calculado del histograma es "<<h5c->GetMean()/20.*1000.<<"escalado "<< h5c->GetMean()/20.*(rho_aire/densidad_ins)*1000. <<std::endl;
	
	return 0;
}
