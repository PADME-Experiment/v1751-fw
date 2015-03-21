#ifndef _caen_analyse_burst_h_
#define _caen_analyse_burst_h_

#include"caen_raw.h"



#include<iostream>
#include<map>
#include<list>

//#include<TCanvas.h>
#include<TH1F.h>
#include<TH2F.h>
#include<TFile.h>
#include<TDirectory.h>
#include<TGraphErrors.h>




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


  class AnalyseBurst{
    public:
      class ChannelHists{
        public:
        typedef std::list<TF1*> gausFuncList_t;
        typedef std::list<TF1*>::iterator gausFuncIter_t;
          typedef struct{
            bool             hasChan;
            TH2F*            cumulativeSignalPlot;
            TH1F*            integralOfPeakRegion;
            gausFuncList_t       gausFunctions;
            //TH1F*            photoElectronPeaksMerged;
            //TH2F*            photoElectronPeaksMerged2;
            //TH2F*            gausMean_photoElectrons;
            //TH2F*            gausAmplitude_photoElectrons;
            //TH2F*            gausSigma_photoElectrons;
            //TGraphErrors*    photPeakMer;
            //TGraphErrors*    gausMeanPhotElec;
          }hist_per_chan_t;
          ChannelHists();
          ~ChannelHists();
          void              MakeChan(int  ch);
          hist_per_chan_t&  GetChan(int   ch);
          bool              HasChan(int   ch);

        private:
          hist_per_chan_t chanhist[v1751_const::gChanMax];
      };
      AnalyseBurst(Event& evt);
      ~AnalyseBurst();
      void           Init(Event&              evt){       }
      void           Process(Event&           evt);
      void           Finish();
      void           WriteToFile(std::string  filename);
      ChannelHists&  GetHists(){return        fHists;}
    private:
      ChannelHists   fHists;
  };
}
#endif
