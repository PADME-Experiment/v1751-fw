#ifndef _caen_analyse_run_h_
#define _caen_analyse_run_h_

#include"caen_raw.h"
#include"2016-02-10-ledpmt-analyse_burst.h"

//#include<TCanvas.h>
#include<TH1F.h>
#include<TH2F.h>
#include<TFile.h>
#include<TDirectory.h>

#include<string>
#include<deque>

namespace caen{
  class AnalyseRun{
    public:
      AnalyseRun(){HistInit();fBurst_i=0;}
      ~AnalyseRun(){HistDelete();}

      void Init();
      void Process(ChannelDat *);
      void Finish();
      void WriteToFile(std::string);
      static const int fNGausMax=20;
    private:
      void HistInit();
      void HistDelete();

      int fBurst_i;
      TGraphErrors*  nPhotoElectrons[v1751_const::gChanMax];
      TGraphErrors*  nEvents[v1751_const::gChanMax];
      TGraphErrors*  sigIntegral[v1751_const::gChanMax];
      TFile* fRootFileP;
      TGraphErrors* burstStatistics[v1751_const::gChanMax];
      TH1F *nPhotoelElectronSpe    [v1751_const::gChanMax];
      TH1F *nPhotoElectrSum        [v1751_const::gChanMax];



      TH1F
        *fHistRise              [v1751_const::gChanMax],
        *fHistFall              [v1751_const::gChanMax];
      TH2F
        *fHist2TotVsIntAmp      [v1751_const::gChanMax],
        *fHist2RiseVsFall       [v1751_const::gChanMax],
        *fHist2RiseVsIntAmp     [v1751_const::gChanMax],
        *fHist2FallVsIntAmp     [v1751_const::gChanMax];
      TGraphErrors
        *fMeanTimeStabInBurst   [v1751_const::gChanMax],
        *fMeanTimeStabVsBursti  [v1751_const::gChanMax],
        *fPeakTimeStabInBurst   [v1751_const::gChanMax],
        *fPeakTimeStabVsBursti  [v1751_const::gChanMax];




      std::deque<double>  fSumPeakTimes[v1751_const::gChanMax];
      std::deque<double> fSum2PeakTimes[v1751_const::gChanMax];
      std::deque<double>  fSumAmplitudes[v1751_const::gChanMax];
      std::deque<double> fSum2Amplitudes[v1751_const::gChanMax];
  };
};
#endif
