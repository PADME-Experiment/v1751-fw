#include"analyse_burst.h"

#include"main.h"
#include"caen_raw.h"

#include<sstream>


namespace caen{

  ChannelHists::ChannelHists(){
    for(int i=0;i<gChanMax;++i){
      chanhist[i].hasChan=false;
    }
  }

  ChannelHists::~ChannelHists(){
    for(int i=0;i<gChanMax;++i){
      if(chanhist[i].hasChan){
        //delete histos
      }
    }
  }

  void ChannelHists::MakeChan(int ch){
      if(chanhist[ch].hasChan)return;

      std::stringstream channame;channame<<ch;
      std::string namestr,titlestr;

      chanhist[ch].hasChan=true;
      namestr="cumulativeSignalPlot_ch_"+channame.str();titlestr="cumulative Signal Plot channel"  +channame.str();   chanhist[ch].cumulativeSignalPlot = new TH2F(namestr.c_str(),titlestr.c_str(),1000,0,1000,1700,-200,1500);
      namestr="integralOfPeakRegion_ch_"+channame.str();titlestr="integral Of Peak Region channel "+channame.str();   chanhist[ch].integralOfPeakRegion = new TH1F(namestr.c_str(),titlestr.c_str(),3000,0,20000);
  }

  bool ChannelHists::HasChan(int ch){
    return chanhist[ch].hasChan;
  }

  ChannelHists::hist_per_chan_t& ChannelHists::GetChan(int ch){
    if(!HasChan(ch)){std::cerr<<"chan "<<ch<<" has not been made"<<std::endl;exit;}
    return chanhist[ch];
  }



  AnalyseBurst::AnalyseBurst(Event& evt){
    for(int i=0;i<ChannelHists::gChanMax;++i)
      if(evt.HasChannel(i)){
        hists.MakeChan(i);
      }
  }
  AnalyseBurst::~AnalyseBurst(){ }

  void AnalyseBurst::Process(Event& evt){
    for(Event::chan_iterator chan_it=evt.GetChannelsBegin();
        chan_it!=evt.GetChannelsEnd();
        ++chan_it){
      ChannelSamples& chan=chan_it->second;
      int chanId=chan_it->first;
      int i=0;
      double summ=0;
      for(ChannelSamples::iterator samp_it=chan.GetSamplesBegin();
          samp_it!=chan.GetSamplesEnd();
          ++samp_it,++i){
        //all[chanId]->Fill(*samp_it);
        hists.GetChan(chanId).cumulativeSignalPlot->Fill(i,*samp_it);
        summ+=i>500&&i<750?*samp_it:0;
      }

      hists.GetChan(chanId).integralOfPeakRegion->Fill(summ);
    }
  }
  void AnalyseBurst::Finish(){
    // TODO: fit with (sin+1)*gaus
  }

  void AnalyseBurst::WriteToFile(std::string filename){
    TFile* fRootFileP=new TFile(filename.c_str(),"recreate");

    for(int i=0;i<ChannelHists::gChanMax;++i){
      if(hists.HasChan(i)){
        fRootFileP->CurrentDirectory()->WriteTObject(hists.GetChan(i).cumulativeSignalPlot);
        fRootFileP->CurrentDirectory()->WriteTObject(hists.GetChan(i).integralOfPeakRegion);
      }
    }

    fRootFileP->Write();
    fRootFileP->Close();
    delete fRootFileP;
  }

};
