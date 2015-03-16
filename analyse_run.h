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
      AnalyseRun();
      ~AnalyseRun();
      void Init();
      void Process(ChannelHists&);
      void Finish();
      void WriteToFile(std::string filename);

      typedef struct{
        TH2F* photoElectronPeaksMerged2;
        TH2F* gausMean_photoElectrons;
        TH2F* gausAmplitude_photoElectrons;
        TH2F* gausSigma_photoElectrons;
      }hist_per_chan_t;
      static const int gChanMax=8;
    private:
      hist_per_chan_t hists[gChanMax];
  };
};
#endif
