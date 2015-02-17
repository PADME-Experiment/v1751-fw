#ifndef _caen_analyse_h_
#define _caen_analyse_h_

#include"caen-raw.h"



#include<iostream>
#include<map>

//#include<TCanvas.h>
#include<TH1F.h>
#include<TH2F.h>
#include<TFile.h>
#include<TDirectory.h>





#include<iostream>
#include<sstream>
#include<fstream>
#include<stdio.h>
#include<stdlib.h>
#include<getopt.h>
#include<vector>
#include<map>
#include<iomanip>
#include<fstream>


namespace caen{
  class Analyse{
    public:
      void Init(Event& evt);
      void Process(Event& evt);
      void Finish();
    private:
      TFile* fRootFileP;
      TH1F* hist;

      std::map<int,TH1F*> all;
      std::map<int,TH2F*> cumsignals;
      std::map<int,TH1F*> integral_peak_region;


  };
};
#endif
