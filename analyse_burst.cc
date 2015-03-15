#include"analyse_burst.h"

#include"main.h"
#include"caen_raw.h"

#include<sstream>


#include<TF1.h>
#include<TMath.h>



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
      namestr="cumulativeSignalPlot_ch_"+channame.str();titlestr="cumulative Signal Plot channel"  +channame.str();   chanhist[ch].cumulativeSignalPlot = new TH2F(namestr.c_str(),titlestr.c_str(),3000,0,3000,1700,-200,1500);
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
        summ+=i>750&&i<850?*samp_it:0;
        //summ+=i>560&&i<620?*samp_it:0;
      }

      hists.GetChan(chanId).integralOfPeakRegion->Fill(summ);
    }
  }

  Double_t SinExp(Double_t *x,Double_t*par){
    return (sin(x[0]*par[0]+par[1])+1)*par[2]*TMath::Exp(-x[0]*par[3]);
  }


  void AnalyseBurst::Finish(){
    // TODO: fit with (sin+1)*gaus
    for(int i=0;i<ChannelHists::gChanMax;++i){
      if(hists.HasChan(i)){
        TF1* fitfunc=new TF1("fitfunc","[0]*exp(-0.5*((x-[1])/[2])**2)*(sin(x*[3]+[4])+1)",200,1400);
        fitfunc->SetParLimits(3,
        1.5*3.14/(hists.GetChan(i).integralOfPeakRegion->GetBinCenter(hists.GetChan(i).integralOfPeakRegion->GetNbinsX())),
        4*3.14/(hists.GetChan(i).integralOfPeakRegion->GetBinCenter(hists.GetChan(i).integralOfPeakRegion->GetMaximumBin()))
        );
        fitfunc->SetParameter(3,1.8*3.14/(hists.GetChan(i).integralOfPeakRegion->GetBinCenter(hists.GetChan(i).integralOfPeakRegion->GetMaximumBin())));
        fitfunc->SetParLimits(4,-3.14,3.15);
        fitfunc->SetParameter(4,3.15);
        fitfunc->SetParameter(0,hists.GetChan(i).integralOfPeakRegion->GetMaximum()/2.);
        //fitfunc->SetParameter(1,hists.GetChan(i).integralOfPeakRegion->GetMean());
        fitfunc->FixParameter(1,0);
        fitfunc->SetParLimits(2,hists.GetChan(i).integralOfPeakRegion->GetRMS(),10*hists.GetChan(i).integralOfPeakRegion->GetRMS());
        fitfunc->SetParameter(2,2*hists.GetChan(i).integralOfPeakRegion->GetRMS());
        hists.GetChan(i).integralOfPeakRegion->Fit("fitfunc","WW");
      }
    }
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
