#include <analysis.h>
#include <utility.h>
#include <formatio.h>
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "Wave.h"

static int wavePanel=0;
static int freqPanel=0; 

// Constants									   
#define SAMPLE_RATE		0
#define NPOINTS			1

// Global variables
int waveInfo[2]; //waveInfo[0] = sampleRate
				 //waveInfo[1] = number of elements
//frecventa de esantionare
double sampleRate = 0.0;
//numarul de esantioane
int npoints = 0;
//vectorul ce contine datele semnalului audio
double *waveData = 0;
//secunda de start
int sec_Start = 0;
//secunda de stop
int sec_Stop = 0;
//valoarea parametrului alpha
double alpha = 0.0;
//vectorul ce contine datele semnalului filtrat in domeniul timp
double *filter_Time_Data =0 ;
//tip filtru
int filterType; 
//dimensiunea ferestrei
int dim_Window = 0;

//tip fereastra
int windowType = 0;

int N=0;
double* anvelopa=0;
double *deriv = 0;





int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	
	if ((wavePanel= LoadPanel (0, "Wave.uir", PANEL)) < 0)
		return -1;
	if ((freqPanel = LoadPanel (0, "Wave.uir", FREQ_PANEL)) < 0)
		return -1;
	DisplayPanel (wavePanel);
	RunUserInterface ();
	DiscardPanel (wavePanel);
	return 0; 

}

int CVICALLBACK ON_WAVE_PANEL (int panel, int event, void *callbackData,
							   int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface (0);
			break;
	}
	return 0;
}

int CVICALLBACK OnLoadButtonCB (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{   
	int zero=0;
	double maxVal = 0.0;
	double minVal = 0.0;
	int maxIndex = 0;
	int minIndex = 0;
	double mean = 0.0;
	double median=0.0;
	double stdDev=0.0;
	int interv=10;
	double axis[100];
	int hist[100];

	switch (event)
	{
		case EVENT_COMMIT:
			//executa script python pentru conversia unui fisierului .wav in .txt
			LaunchExecutable("python main.py");
			
			//astept sa fie generate cele doua fisiere (modificati timpul daca este necesar
			Delay(4);
			
			//incarc informatiile privind rata de esantionare si numarul de valori
			FileToArray("wafeInfo.txt", waveInfo, VAL_INTEGER, 2, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			sampleRate = waveInfo[SAMPLE_RATE];
			npoints = waveInfo[NPOINTS];
			
			//alocare memorie pentru numarul de puncte
			waveData = (double *) calloc(npoints, sizeof(double));
			
			//incarcare din fisierul .txt in memorie (vector)
			FileToArray("waveData.txt", waveData, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			
			//afisare pe grapf
			DeleteGraphPlot(panel,PANEL_GRAPH_RAW_DATA,-1, VAL_IMMEDIATE_DRAW); 
			PlotY(panel,PANEL_GRAPH_RAW_DATA, waveData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			DeleteGraphPlot(freqPanel, FREQ_PANEL_GRAPH_RAW_DATA,-1, VAL_IMMEDIATE_DRAW);
			PlotY(freqPanel, FREQ_PANEL_GRAPH_RAW_DATA, waveData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			

			//calculare valoare minima si valoare maxima
			MaxMin1D(waveData,npoints,&maxVal,&maxIndex,&minVal,&minIndex);
			//calculare valoare medie
			Mean(waveData,npoints,&mean);
			//calculare valoare mediana
			Median(waveData,npoints,&median);
			//calculare dispersie
			StdDev(waveData,npoints,&mean,&stdDev); 
			//histograma
			Histogram(waveData,npoints,minVal-1,maxVal+1,hist,axis,interv);
			DeleteGraphPlot (panel,PANEL_GRAPH_HISTOGRAMA, -1, VAL_DELAYED_DRAW);
            PlotXY (panel,PANEL_GRAPH_HISTOGRAMA, axis,  hist, interv, VAL_DOUBLE, VAL_SSIZE_T, VAL_VERTICAL_BAR, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);
		
			
			//setare valori pe interfata
			SetCtrlVal(panel,PANEL_NUMERIC_MIN,minVal);
			SetCtrlVal(panel,PANEL_NUMERIC_MAX,maxVal);
			SetCtrlVal(panel,PANEL_NUMERIC_MIN_INDEX, (double)minIndex);
			SetCtrlVal(panel,PANEL_NUMERIC_MAX_INDEX, (double)maxIndex);  
			SetCtrlVal(panel,PANEL_NUMERIC_MEAN,mean);
			SetCtrlVal(panel,PANEL_NUMERIC_MEDIANA,median);
			SetCtrlVal(panel,PANEL_NUMERIC_DISPERSIE,stdDev);
			
			// frecventa de esantionare 
			SetCtrlVal(panel,PANEL_FRECV_ES, sampleRate); 
			// nr de esantioane
			SetCtrlVal(panel,PANEL_NR_ESAN, npoints); 
		
				//zero corossing
			for(int i=0;i<npoints-1;++i)
			{
				if( ( (waveData[i]<0) && (waveData[i+1]>0) ) || ( (waveData[i]>0) && (waveData[i+1]<0))  )
				{
					zero++;
				}
			}
			//afisam pe interfata valoarea pentru zero crossing
			SetCtrlVal(panel,PANEL_NUM_ZERO,zero);
			
			
			break;
	}
	return 0;
}

int CVICALLBACK On_Prev (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//apasarea butonului Prev
			int sec =(int) npoints/sampleRate;
			int start = 0;
			int stop= 0;
			
			GetCtrlVal(panel,PANEL_START, &start);
			GetCtrlVal(panel,PANEL_STOP, &stop);
			if(stop > 0 && start <= sec)
			{
				SetCtrlVal(panel,PANEL_START, start-1);
				SetCtrlVal(panel,PANEL_STOP, stop-1);
				On_Aplica(panel,PANEL_PREV,EVENT_COMMIT,0,0,0);
			}
		
			break;
	}
	return 0;
}

int CVICALLBACK On_Next (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//apasarea butonului Next
			int sec =(int) npoints/sampleRate;
			int start = 0;
			int stop = 0;
			
			GetCtrlVal(panel,PANEL_START, &start);
			GetCtrlVal(panel,PANEL_STOP, &stop);
			if(stop >= 0 && start < sec)
			{
				SetCtrlVal(panel,PANEL_START, start+1);
				SetCtrlVal(panel,PANEL_STOP, stop+1);
				On_Aplica(panel,PANEL_PREV,EVENT_COMMIT,0,0,0);
			}		
		
			break;
	}
	return 0;
}

int CVICALLBACK On_Aplica (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel, PANEL_FILTRE, &filterType);
			filter_Time_Data = (double *) calloc(npoints+5000, sizeof(double));

			if(filterType== 0) // filtru mediere
			{	
				double s = 0;
				
				GetCtrlVal(panel,PANEL_DIM_WINDOW, &dim_Window);
				
				for (int i = 0; i < npoints; i++)
				{
					for (int k = 0; k <= dim_Window - 1; k++)
					{
						if ((i - k) < 0)
						{
							s += 0;
						}
						else	
						{
							s += waveData[i-k];	
						}
					}
					filter_Time_Data[i] = s/dim_Window;
					s = 0;
					
				}
			}
			else if(filterType == 1) // filtru element de ordin 1
			{	
				GetCtrlVal(panel,PANEL_ALPHA, &alpha);
				filter_Time_Data[0] = waveData[0];
				
				for(int i = 1; i < npoints; i++)
				{
					filter_Time_Data[i] = (1-alpha)*filter_Time_Data[i-1] + alpha*waveData[i];
				}
			}
			// se extrag secundele 
			GetCtrlVal(panel, PANEL_STOP, &sec_Stop);
			GetCtrlVal(panel, PANEL_START, &sec_Start);
			double *p = filter_Time_Data + (int)(sec_Start * sampleRate);
			
			DeleteGraphPlot (panel,PANEL_GRAPH_FILTERED_DATA, -1, VAL_DELAYED_DRAW);
			PlotY(panel, PANEL_GRAPH_FILTERED_DATA,  p,(sec_Stop - sec_Start)*sampleRate, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			break;
	}
	return 0;
}

//selectare tipul de filtru: mediere/ordin 1
int CVICALLBACK FiltreType (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{

	switch (event)
	{
		case EVENT_COMMIT:
			
		
			GetCtrlVal (panel, control, &filterType);
			if(filterType==0)
			{
				SetCtrlAttribute(panel, PANEL_DIM_WINDOW , ATTR_VISIBLE, 1);
				SetCtrlAttribute(panel, PANEL_ALPHA, ATTR_VISIBLE, 0);
				GetCtrlVal(panel, PANEL_STOP, &sec_Stop);
				GetCtrlVal(panel, PANEL_START, &sec_Start);
			}
			else
			{
				SetCtrlAttribute(panel, PANEL_ALPHA, ATTR_VISIBLE, 1);
				SetCtrlAttribute(panel, PANEL_DIM_WINDOW , ATTR_VISIBLE, 0);
				GetCtrlVal(panel, PANEL_DIM_WINDOW, &N);
				GetCtrlVal(panel, PANEL_STOP, &sec_Stop);
				GetCtrlVal(panel, PANEL_START, &sec_Start);
			}  
			
			break;
			
	}
	return 0;
}

int CVICALLBACK Generare_Anvelopa (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{  
	
	switch (event)
	{
		case EVENT_COMMIT:
			LaunchExecutable("python anvelopa.py");
			
			//astept sa fie generate cele doua fisiere 
			Delay(0);
			
			anvelopa = (double *) calloc(npoints, sizeof(double));   
			
			//incarc informatiile privind rata de esantionare si numarul de valori
			FileToArray("anvelopa1.txt", anvelopa, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			PlotY(panel,PANEL_GRAPH_RAW_DATA, anvelopa, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLUE); 
			break;
	}
	return 0;
}


int CVICALLBACK Generate_Deriv (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			if (npoints == 0)
				return -1;
			
			deriv = (double *) calloc(npoints, sizeof(double));
			DifferenceEx (waveData, npoints, 1.0, waveData + 1, 1, waveData + npoints - 1, 1, DIFF_SECOND_ORDER_CENTRAL, deriv); 
			
			PlotY(panel, PANEL_GRAPH_FILTERED_DATA, deriv, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLACK);   
		
			break;
	}
	return 0;
}

int CVICALLBACK OnSwitchPanelCB (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
		switch (event)
	{
		case EVENT_COMMIT:
			//schimbarea panoului
			if(panel == wavePanel)
			{
				SetCtrlVal(freqPanel, FREQ_PANEL_IDC_SWITCH_PANEL, 1);
				DisplayPanel(freqPanel);
				HidePanel(panel);
			}
			else
			{		
				SetCtrlVal(wavePanel,PANEL_IDC_SWITCH_PANEL, 0);
				DisplayPanel(wavePanel);
				HidePanel(panel);
			}
			break;
	}
	return 0;
}


int CVICALLBACK OnFrequencyButtonCB (int panel, int control, int event,
									 void *callbackData, int eventData1, int eventData2)
{
	 
	//luam de pe interfata numarul de puncte pentru care se calculeaza spectrul
	GetCtrlVal(freqPanel,FREQ_PANEL_IDC_NR_PUNCTE, &N);
	WindowConst winConst;
	double *convertedSpectrum; //vectorul ce contine spectrul semnalului convertit in volti
	double *autoSpectrum; //spectrul de putere
	double df=0.0; //pasul in domeniul frecventei
	double freqPeak=0.0; //valoarea maxima din spectrul de putere
	double powerPeak=0.0; //frecventa estimata pentru spectrum de putere
	
	char unit[32]="V";  //voltage semnal
	convertedSpectrum=(double*)calloc(npoints/6,sizeof(double));
	autoSpectrum=(double*)calloc(npoints/6,sizeof(double));
	switch (event)
	{
	
			
		case EVENT_COMMIT:

		
		ScaledWindowEx (waveData,N, RECTANGLE_, 0, &winConst);
		//se calculeaza partea pozitiva a spectrului scalat de putere pentru un semnal esantionat
		AutoPowerSpectrum(waveData,npoints/6, 1.0/sampleRate, autoSpectrum, &df);
		//calculeaza puterea si frecventa corespunzatoare varfului din spectrul semnalului
		PowerFrequencyEstimate(autoSpectrum,npoints/6,-1.0,winConst,df,7,&freqPeak,&powerPeak);
		
		//afiseaza pe interfata valorile determinate
		SetCtrlVal(freqPanel,FREQ_PANEL_FREQ_PEAK,freqPeak);
		SetCtrlVal(freqPanel,FREQ_PANEL_POWER_PEAK,powerPeak);
		
		//se converteste spectrul de intrare în formate ce permit o reprezentare grafica mai convenabila
		SpectrumUnitConversion(autoSpectrum, N,0, SCALING_MODE_LINEAR, DISPLAY_UNIT_VRMS, df, winConst, convertedSpectrum, unit);
		//afisam spectrul pe grafic
		DeleteGraphPlot(panel,FREQ_PANEL_GRAPH_SPECTRU,-1,VAL_IMMEDIATE_DRAW);
		PlotWaveform(panel,  FREQ_PANEL_GRAPH_SPECTRU, convertedSpectrum, npoints/12 ,VAL_DOUBLE, 1.0, 0.0, 0.0, df,VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID,  VAL_CONNECTED_POINTS, VAL_RED);	
			
			
			
	
			break;
	}
	return 0;
}



int CVICALLBACK OnFilterButtonCB (int panel, int control, int event,
										void *callbackData, int eventData1, int eventData2)
{
	
	int secunda;
	double *raw;
	int winType;
	double final[npoints/6];
	double window[npoints/6];

	
	
	
	int fcut;
	int order;
	int fpass;
	int signalType;
	static WindowConst winConst;
	
	
	double *powerSpectrum;
	double *frequencyArray;
	
	char unit[32]="V";
	double df=0.0; //pasul in domeniul frecventei
	
	double coef[55];
		
	powerSpectrum=(double*)calloc(npoints/12,sizeof(double));
	frequencyArray=(double*)calloc(npoints/12,sizeof(double));
	
		switch (event)
	{
			
		case EVENT_COMMIT:
			
			//ferestruirea
			DeleteGraphPlot (freqPanel,FREQ_PANEL_GRAPH_WINDOW, -1, VAL_IMMEDIATE_DRAW);
			GetCtrlVal(freqPanel, FREQ_PANEL_IDC_NR_PUNCTE, &N);
			
			GetCtrlVal(freqPanel,FREQ_PANEL_IDC_SECUNDA,&secunda);
			
			GetCtrlVal(freqPanel,FREQ_PANEL_IDC_CUT_FREQ,&fcut);
			GetCtrlVal(freqPanel,FREQ_PANEL_IDC_ORDER,&order);
			GetCtrlVal(freqPanel,FREQ_PANEL_IDC_FPASS,&fpass);
			
			raw=(double*)calloc(npoints/6,sizeof(double));
			for(int i=0;i<npoints/6;i++)
			{
				raw[i]=waveData[secunda*npoints/6+i];
			}	 
			
			GetCtrlVal(freqPanel,FREQ_PANEL_WINDOW_TYPE,&winType);
			
			
			//afisam semnalul pe secunde
			DeleteGraphPlot (freqPanel, FREQ_PANEL_GRAPH_RAW_DATA, -1, VAL_IMMEDIATE_DRAW);
			PlotY (freqPanel, FREQ_PANEL_GRAPH_RAW_DATA, raw, npoints/6, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
			
			LinEv1D(raw,npoints/6,0,1,window);	   
			
			switch (winType)
			{
				case 0:
					HanWin(window,npoints/6);
					break;
				case 1:
					BlkHarrisWin(window,npoints/6);
					break;
			}
			DeleteGraphPlot (freqPanel,FREQ_PANEL_GRAPH_WINDOW, -1, VAL_IMMEDIATE_DRAW);
			PlotY (freqPanel,FREQ_PANEL_GRAPH_WINDOW, window, npoints/6, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
			
			Mul1D(raw,window,npoints/6,final);
			DeleteGraphPlot (freqPanel,FREQ_PANEL_GRAPH_RAW_WINDOW, -1, VAL_IMMEDIATE_DRAW);
			PlotY (freqPanel,FREQ_PANEL_GRAPH_RAW_WINDOW, final, npoints/6, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
			
			
			GetCtrlVal(freqPanel,FREQ_PANEL_FILTER_TYPE,&signalType);
			
			double *filteredSignal;
			filteredSignal = (double *) calloc(npoints, sizeof(double));
			
			switch(signalType)
			{
				case 0:
				
					Ksr_HPF (sampleRate, fcut, 55, coef,4.5);
					Convolve(final,npoints/6,coef,55,filteredSignal);
			
					break;
					
				case 1:
 				
					InvCh_HPF (final,npoints/6,npoints/6,fcut,40,order,filteredSignal);
				   	break;

						   
			}
			
			DeleteGraphPlot (freqPanel, FREQ_PANEL_GRAPH_FILTER, -1, VAL_IMMEDIATE_DRAW);
			PlotY (freqPanel, FREQ_PANEL_GRAPH_FILTER, final, npoints/6, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
				
			//afisam spectrul semnalului dupa ce am aplicat filtrul
			DeleteGraphPlot (freqPanel,FREQ_PANEL_GRAPH_SPECTRUM_FILTER, -1, VAL_IMMEDIATE_DRAW);
        	AutoPowerSpectrum (filteredSignal, npoints/6, 1.0/sampleRate, powerSpectrum, &df);
			SpectrumUnitConversion(powerSpectrum, npoints/12, 0, SCALING_MODE_LINEAR, DISPLAY_UNIT_VPK, df, winConst,frequencyArray, unit);
			PlotY (freqPanel,FREQ_PANEL_GRAPH_SPECTRUM_FILTER, frequencyArray, npoints/12, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
				
	
			
			
		
			
	}
	return 0;

			
	
}

int CVICALLBACK OnSaveCB (int panel, int control, int event,
						void *callbackData, int eventData1, int eventData2)
{
	int year = 0;
	int month = 0;
	int day = 0;
	int hour = 0;
	int minute = 0;
	int second = 0;
	int imgHandle;  
	char fileName[256] = {0},fileName1[256]={0},fileName2[256]={0},fileName3[256]={0},fileName4[256]={0},fileName5[256]={0},fileName6[256]={0};
	switch (event)
	{
		case EVENT_COMMIT:
			//luam data si ora
			GetSystemDate(&month, &day, &year);
			GetSystemTime(&hour, &minute, &second);
			
				//setam numele cu care salvam imaginea
			sprintf(fileName, "ROW_DATA_%4d.%2d.%2d_%2d-%2d-%2d.jpg", year, month, day, hour, minute, second);
			//salvam imaginea
			GetCtrlDisplayBitmap(panel,PANEL_GRAPH_RAW_DATA,1,&imgHandle);
			SaveBitmapToJPEGFile(imgHandle,fileName,JPEG_PROGRESSIVE,100);
			
			
			//setam numele cu care salvam imaginea
			sprintf(fileName1, "FILTERED_DATA__%4d.%2d.%2d_%2d-%2d-%2d.jpg", year, month, day, hour, minute, second);
			//salvam imagine
			GetCtrlDisplayBitmap(panel,PANEL_GRAPH_FILTERED_DATA,1,&imgHandle);
			SaveBitmapToJPEGFile(imgHandle,fileName1,JPEG_PROGRESSIVE,100);
			
			//setam numele cu care salvam imaginea
			sprintf(fileName2, "SPECTRUM_%4d.%2d.%2d_%2d-%2d-%2d.jpg", year, month, day, hour, minute, second);
			//salvam imaginea
			GetCtrlDisplayBitmap(freqPanel,FREQ_PANEL_GRAPH_SPECTRU,1,&imgHandle);
			SaveBitmapToJPEGFile(imgHandle,fileName2,JPEG_PROGRESSIVE,100);
			
			//setam numele cu care salvam imaginea
			sprintf(fileName3, "Window_%4d.%2d.%2d_%2d-%2d-%2d.jpg", year, month, day, hour, minute, second);
			//salvam imaginea
			GetCtrlDisplayBitmap(freqPanel,FREQ_PANEL_GRAPH_WINDOW,1,&imgHandle);
			SaveBitmapToJPEGFile(imgHandle,fileName3,JPEG_PROGRESSIVE,100);
			
				//setam numele cu care salvam imaginea
			sprintf(fileName4, "Row_Window%4d.%2d.%2d_%2d-%2d-%2d.jpg", year, month, day, hour, minute, second);
			//salvam imaginea
			GetCtrlDisplayBitmap(freqPanel,FREQ_PANEL_GRAPH_WINDOW,1,&imgHandle);
			SaveBitmapToJPEGFile(imgHandle,fileName4,JPEG_PROGRESSIVE,100);
			
				//setam numele cu care salvam imaginea
			sprintf(fileName5, "Filtered_Window%4d.%2d.%2d_%2d-%2d-%2d.jpg", year, month, day, hour, minute, second);
			//salvam imaginea
			GetCtrlDisplayBitmap(freqPanel,FREQ_PANEL_GRAPH_WINDOW,1,&imgHandle);
			SaveBitmapToJPEGFile(imgHandle,fileName5,JPEG_PROGRESSIVE,100);
			
					//setam numele cu care salvam imaginea
			sprintf(fileName6, "Spectrum_of_filter%4d.%2d.%2d_%2d-%2d-%2d.jpg", year, month, day, hour, minute, second);
			//salvam imaginea
			GetCtrlDisplayBitmap(freqPanel,FREQ_PANEL_GRAPH_WINDOW,1,&imgHandle);
			SaveBitmapToJPEGFile(imgHandle,fileName6,JPEG_PROGRESSIVE,100);
			
		
			DiscardBitmap(imgHandle);
			
			break;
	}
	return 0;
}
