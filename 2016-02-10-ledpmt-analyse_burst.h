#ifndef _caen_analyse_burst_h_
#define _caen_analyse_burst_h_

#include"caen_raw.h"

#include<TDirectory.h>
#include<TFile.h>
#include<TGraphErrors.h>
#include<TH1F.h>
#include<TH1D.h>
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
#include<valarray>
#include<deque>


namespace caen{



  class ChannelDat{
    public:
      ChannelDat();
      ~ChannelDat(){
        //std::cout<<"destr"<<std::endl;
        DeleteHistograms();
      }
      bool IsEnabled(){return fChannelID>=0;}
      int GetChannelID(){return fChannelID;}
      void SetChannelID(int ch){
        fChannelID=ch;
        CreateHistograms();
        RenameHistograms();
      }
    private:

    private:
      int fChannelID;
    public:
      std::deque<double> fTOT[10];
      std::deque<double> fRiseTime;
      std::deque<double> fFallTime;
      std::deque<double> fMaxY;
      std::deque<int   > fMaxYTime;
      std::deque<double> fMeanTime;



      void RenameHistograms();
      void CreateHistograms();
      void DeleteHistograms();
      void WriteHistograms(TDirectory* dir);
      void NullifyHistogramPointers();
      std::deque<double> fAmplitudesPerBlink;

      TH2F *cumulativeSignalPlot;
      double fPedestal;
      int fEventsNumber;
      double fSignalIntegral;

      TH1D *integralOfPeakRegion;
      TH1F *integralNoise;
      TH1F *netSignal;
      TH1F *numPhotoElectronsDistr;
      TH1F *fH1EventTimeDistr;
      double offset[2];
      double nPhotoElectrons[2];
      double nPhotoElectronsNorm[2];

      //int nEvt;

  };


  class AnalyseBurst{
    public:
      //ako se promeni da se vidi PostProcess
      typedef std::map<int,ChannelDat> channels_map_t;
      AnalyseBurst();
      ~AnalyseBurst();
      void           Init(RAW::Event&);
      void           Process(RAW::Event&           evt);
      void           PostProcess();
      void           WriteToFile(std::string  filename);
      //ChannelHists&  GetHists(){return        fHists;}
      //channels_map_t& GetChannelsData(){return fChannelsData;}
      ChannelDat* GetChannelsData(){return fChannelsData_;}
    private:
      std::fstream fSamplestestfile;
      //channels_map_t   fChannelsData;
      ChannelDat   fChannelsData_[8];
  };
}
#endif
