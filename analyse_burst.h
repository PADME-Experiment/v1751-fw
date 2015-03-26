#ifndef _caen_analyse_burst_h_
#define _caen_analyse_burst_h_

#include"caen_raw.h"

#include<TDirectory.h>
#include<TFile.h>
#include<TGraphErrors.h>
#include<TH1F.h>
#include<TH1I.h>
#include<TH2F.h>

#include<fstream>
#include<getopt.h>
#include<iomanip>
#include<iostream>
#include<list>
#include<map>
#include<sstream>
#include<stdio.h>
#include<stdlib.h>
#include<vector>


namespace caen{


  class AnalyseBurst{
    public:
      class ChannelHists{
        public:
        typedef std::list<TF1*> gausFuncList_t;
        typedef std::list<TF1*>::iterator gausFuncIter_t;
        typedef struct{
          bool hasChan;
          TH2F *cumulativeSignalPlot;
          TH1F *integralOfPeakRegion;
          TH1F *integralNoise;
          TH1F *netSignal;
          TGraphErrors *gausMean_photoElectrons;
          TGraphErrors *gausAmplitude_photoElectrons;
          TGraphErrors *gausSigma_photoElectrons;
          TH1F *numPhotoElectronsDistr;
          double gain[2/**0 value, 1 error*/];
          double offset[2];
          double nPhotoElectrons[2];
          double nPhotoElectronsNorm[2];
          int nEvt;
          gausFuncList_t  gausFunctions;
        }hists_per_chan_t;
        ChannelHists();
        ~ChannelHists();
        void              MakeChan(int  ch);
        hists_per_chan_t&  GetChan(int   ch);
        bool              HasChan(int   ch);

        private:
        hists_per_chan_t fChanHists[v1751_const::gChanMax];
        //bool fHasChan[v1751_const::gChanMax];
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
