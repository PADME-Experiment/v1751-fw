#include"analyse_run.h"
#include"analyse_burst.h"

namespace caen{
  AnalyseRun::AnalyseRun(){
    for(int channel_i=0;channel_i<gChanMax;++channel_i){
      std::stringstream channelssname;
      channelssname<<channel_i;

      std::string photoElectronsName="photoElectronsPeaksMerged_ch_"+channelssname.str();
      std::string photoElectronsTitle="photo Electron Peaks Merged channel "+channelssname.str();
      hists[channel_i].photoElectronPeaksMerged2=new TH2F(photoElectronsName.c_str(), photoElectronsTitle.c_str(), 40, 0, 20,300,0,5000);

      std::string gausMean_photoElectronsName="gausMean_photoElectrons_ch_"+channelssname.str();
      std::string gausMean_photoElectronsTitle="gaus mean photo Electrons channel "+channelssname.str();
      hists[channel_i].gausMean_photoElectrons=new TH2F(gausMean_photoElectronsName.c_str(), gausMean_photoElectronsTitle.c_str(), 40, 0, 20,300,0,5000);
hists[channel_i].gausMean_photoElectrons->SetMarkerStyle(6);
hists[channel_i].photoElectronPeaksMerged2->SetMarkerStyle(6);

    }
  }
  AnalyseRun::~AnalyseRun(){
    
    }


  void AnalyseRun::Init(){ }
  void AnalyseRun::Process(ChannelHists& bursthists){
    for(int channel_i=0;channel_i<gChanMax;++channel_i){
      if(!  bursthists.HasChan(channel_i) )return;

      hists[channel_i].photoElectronPeaksMerged2->Add(bursthists.GetChan(channel_i).photoElectronPeaksMerged2);
      hists[channel_i].gausMean_photoElectrons->Add(bursthists.GetChan(channel_i).gausMean_photoElectrons);
    }
  }
  void AnalyseRun::Finish(){ }
  void AnalyseRun::WriteToFile(std::string filename){
    TFile* fRootFileP=new TFile(filename.c_str(),"recreate");
    for(int i=0;i<gChanMax;++i){
      fRootFileP->CurrentDirectory()->WriteTObject(hists[i].photoElectronPeaksMerged2);
      fRootFileP->CurrentDirectory()->WriteTObject(hists[i].gausMean_photoElectrons);
    }
    fRootFileP->Write();
    fRootFileP->Close();
    delete fRootFileP;
  }
}
