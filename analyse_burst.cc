#include"analyse_burst.h"
#include"caen_raw.h"
#include<sstream>


namespace caen{
  void AnalyseBurst::Init(Event& evt){
    fRootFileP=new TFile(gOutputRootFile.c_str(),"recreate");
    for(int i=0;i<8;++i)
      if(evt.HasChannel(i)){
        std::stringstream channame;
        channame<<"_chan_"<<i;
        std::string namestr;
        namestr="allHist"+channame.str();
        all[i]=new TH1F(namestr.c_str(),"chan",1700,-200,1500);
        namestr="th2f_cumulative_plot"+channame.str();
        cumsignals[i]=new TH2F(namestr.c_str(),"chan",1000,0,1000,1700,-200,1500);
        namestr="integral_peak_region"+channame.str();
        integral_peak_region[i]=new TH1F(namestr.c_str(),"integral Peak",3000,0,20000);

      }
  }

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
        all[chanId]->Fill(*samp_it);
        cumsignals[chanId]->Fill(i,*samp_it);
        summ+=i>500&&i<750?*samp_it:0;
      }

      integral_peak_region[chanId]->Fill(summ);
    }
  }
  void AnalyseBurst::Finish(){


    fRootFileP->Write();
    fRootFileP->Close();
    delete fRootFileP;

  }


  unsigned int GetMaxSampPos(ChannelSamples& chan){
    unsigned int ret=0;
    for(ChannelSamples::iterator samps_it=chan.GetSamplesBegin();
        samps_it!=chan.GetSamplesEnd();
        ++samps_it){
      //if(*samps_it
    }


    return ret;
  }

};
