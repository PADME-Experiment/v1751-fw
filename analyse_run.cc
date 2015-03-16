#include"analyse_run.h"
#include"analyse_burst.h"

namespace caen{
  AnalyseRun::AnalyseRun(){
    for(int channel_i=0;channel_i<gChanMax;++channel_i){
      std::stringstream channelssname;
      channelssname<<channel_i;

      std::string photoElectronsName="photoElectronsPeaksMerged_ch_"+channelssname.str();
      std::string photoElectronsTitle="photo Electron Peaks Merged channel "+channelssname.str();
      hists[channel_i].photoElectronPeaksMerged2=new TH2F(photoElectronsName.c_str(), photoElectronsTitle.c_str(), 40, 0, 20,5000,0,5000);

      std::string gausMean_photoElectronsName="gausMean_photoElectrons_ch_"+channelssname.str();
      std::string gausMean_photoElectronsTitle="gaus mean photo Electrons channel "+channelssname.str();
      hists[channel_i].gausMean_photoElectrons=new TH2F(gausMean_photoElectronsName.c_str(), gausMean_photoElectronsTitle.c_str(), 40, 0, 20,500,0,5000);

      std::string gausAmpl_photoElectronsName="gausAmplitude_photoElectrons_ch_"+channelssname.str();
      std::string gausAmpl_photoElectronsTitle="gaus Amplitude photo Electrons channel "+channelssname.str();
      hists[channel_i].gausAmplitude_photoElectrons=new TH2F(gausAmpl_photoElectronsName.c_str(), gausAmpl_photoElectronsTitle.c_str(), 40, 0, 20,5000,0,5000);

      std::string gausSig_photoElectronsName="gausSigma_photoElectrons_ch_"+channelssname.str();
      std::string gausSig_photoElectronsTitle="gaus Sigma photo Electrons channel "+channelssname.str();
      hists[channel_i].gausSigma_photoElectrons=new TH2F(gausSig_photoElectronsName.c_str(), gausSig_photoElectronsTitle.c_str(), 40, 0, 20,500,0,500);


hists[channel_i].gausMean_photoElectrons->SetMarkerStyle(6);
hists[channel_i].gausAmplitude_photoElectrons->SetMarkerStyle(6);
hists[channel_i].gausSigma_photoElectrons->SetMarkerStyle(6);
hists[channel_i].photoElectronPeaksMerged2->SetMarkerStyle(6);

    }
  }
  AnalyseRun::~AnalyseRun(){
  }


  void AnalyseRun::Init(){ }
  void AnalyseRun::Process(ChannelHists& bursthists){
    for(int channel_i=0;channel_i<gChanMax;++channel_i){
      if(!  bursthists.HasChan(channel_i) )return;

if(bursthists.GetChan(channel_i).photoElectronPeaksMerged2)         hists[channel_i].photoElectronPeaksMerged2   ->  Add(bursthists.GetChan(channel_i).photoElectronPeaksMerged2);
if(bursthists.GetChan(channel_i).gausMean_photoElectrons)           hists[channel_i].gausMean_photoElectrons     ->  Add(bursthists.GetChan(channel_i).gausMean_photoElectrons);
if(bursthists.GetChan(channel_i).gausAmplitude_photoElectrons)      hists[channel_i].gausAmplitude_photoElectrons->  Add(bursthists.GetChan(channel_i).gausAmplitude_photoElectrons);
if(bursthists.GetChan(channel_i).gausSigma_photoElectrons)      hists[channel_i].gausSigma_photoElectrons->  Add(bursthists.GetChan(channel_i).gausSigma_photoElectrons);





    }
  }
  void AnalyseRun::Finish(){ }
  void AnalyseRun::WriteToFile(std::string filename){
    TFile* fRootFileP=new TFile(filename.c_str(),"recreate");
    for(int i=0;i<gChanMax;++i){
      if(hists[i].photoElectronPeaksMerged2    )fRootFileP->CurrentDirectory()->WriteTObject(hists[i].photoElectronPeaksMerged2);
      if(hists[i].gausMean_photoElectrons      )fRootFileP->CurrentDirectory()->WriteTObject(hists[i].gausMean_photoElectrons);
      if(hists[i].gausAmplitude_photoElectrons )fRootFileP->CurrentDirectory()->WriteTObject(hists[i].gausAmplitude_photoElectrons);
      if(hists[i].gausSigma_photoElectrons     )fRootFileP->CurrentDirectory()->WriteTObject(hists[i].gausSigma_photoElectrons);
    }
    fRootFileP->Write();
    fRootFileP->Close();
    delete fRootFileP;
  }
}
