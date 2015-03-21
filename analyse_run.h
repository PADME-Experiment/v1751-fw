#ifndef _caen_analyse_run_h_
#define _caen_analyse_run_h_

#include"caen_raw.h"
#include"analyse_burst.h"



//#include<TCanvas.h>
#include<TH1F.h>
#include<TH2F.h>
#include<TFile.h>
#include<TDirectory.h>





namespace caen{
  class AnalyseRun{
    public:
      static const int fNGausMax=20;
      AnalyseRun();
      ~AnalyseRun();
      void Init();
      void Process(AnalyseBurst::ChannelHists&);
      void Finish();
      void WriteToFile(std::string filename);

    private:
        double gaus_A_vs_burstId[2/** 0=val 1=err*/][fNGausMax];
        double gaus_X_vs_burstId[2/** 0=val 1=err*/][fNGausMax];
        double gaus_W_vs_burstId[2/** 0=val 1=err*/][fNGausMax];

      //hist_per_chan_t hists[v1751_const::gChanMax];
  };
};
#endif
