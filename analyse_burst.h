#ifndef _caen_analyse_burst_h_
#define _caen_analyse_burst_h_

#include"caen_raw.h"



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
  class ChannelHists{
    public:
      typedef struct{
        bool hasChan;
        TH2F* cumulativeSignalPlot;
        TH1F* integralOfPeakRegion;
        TH1F* photoElectrons;
      }hist_per_chan_t;
      ChannelHists();
      ~ChannelHists();
      void MakeChan(int ch);
      hist_per_chan_t& GetChan(int ch);
      bool HasChan(int ch);

    static const int gChanMax=8;
    private:
      hist_per_chan_t chanhist[gChanMax];
  };


  class AnalyseBurst{
    public:
      AnalyseBurst(Event& evt);
      ~AnalyseBurst();
      void Init(Event& evt){ }
      void Process(Event& evt);
      void Finish();
      void WriteToFile(std::string filename);
    private:
      ChannelHists hists;
  };
}
#endif
