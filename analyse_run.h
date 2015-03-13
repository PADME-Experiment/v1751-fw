#ifndef _caen_analyse_run_h_
#define _caen_analyse_run_h_

#include"caen_raw.h"



//#include<TCanvas.h>
#include<TH1F.h>
#include<TH2F.h>
#include<TFile.h>
#include<TDirectory.h>





namespace caen{
  class AnalyseRun{
    public:
      void Init();
      void Process();
      void Finish();
    private:
  };
};
#endif
