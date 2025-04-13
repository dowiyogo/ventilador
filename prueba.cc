#include <fstream>
#include <string>
#include <TH1F.h>

int prueba(char *archivo){

	//finding Spectra Peaks
	//TSpectrum *ruido= new TSpectrum(10);

	TCanvas *canvas1 = new TCanvas("canvas1","Histogramas");


	//Creating Ntuples to save data from CAMAC
	TNtuple *adc = new TNtuple("adc","arduino","datos");

	adc->ReadFile(archivo);

	adc->Draw("datos>>hist(20000,0,20000)"); //Dibujo la columna PMT1 del TNtuple como un Histograma
	TH1F* h;
	h = (TH1F *)gROOT->FindObject("hist"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	/*Int_t npeaks = ruido->Search(h,3); // Al objeto de clase TSpectrum le hago buscar peaks en el histograma apuntado por el puntero h 

	//Finding peaks and fitting gaussians
	Float_t *peaks = ruido->GetPositionX(); // Obtengo las posiciones de los peaks, guardadas en un arreglo de mayor a menor en amplitud
	Double_t sigma = (h->GetRMS())/6; // establezco un sigma para el fitting,unas 4 veces menor al RMS de todo el histograma

	if(npeaks > 1) 
		std::cout<<"Se encontraron los peaks de ruido, donde peaks[0] es "<<peaks[0]<<"y peaks[1] es "<<peaks[1]<<std::endl;
	else
	{
		std::cout<<"Se encontraron menos de 2 peaks en el ruido"<<std::endl;
		return 1;
	}

	TF1 *func = new TF1("func","gaus(0) + pol1(3)",peaks[1]-sigma,peaks[1]+sigma); //hago un fit de la gausiana, con el primer parametro de nombre 0
	func->SetLineColor(kRed);
	func->SetParameters(1000,peaks[1],sigma/2,50,-0.1);
	func->SetParLimits(0,0,3000);
	func->SetParLimits(1,peaks[1]-sigma,peaks[1]+sigma);
	func->SetParLimits(2,0,2*sigma);
	func->SetParLimits(3,0,200);
	func->SetParLimits(4,-1,0);

	h->Fit("func","RB+"); //  El "+" agrega la función a la lista de funciones del histograma y el "B" es para considerar los limites.
	Double_t sig = func->GetParameter(2); // asigno a sig el valor del sigma encontrado por el fitting*/


	return 0;
}
