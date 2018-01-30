#include"2016-02-10-ledpmt-analyse_burst.h"
#include"main.h"
#include"caen_raw.h"

#include<list>
#include<sstream>
#include<string>
#include<vector>


#include<TF1.h>
#include<TFitResult.h>
#include<TGraphErrors.h>
#include<TMath.h>



namespace caen{
  ChannelDat::ChannelDat(){
    //std::cout<<"constr channel "<<std::endl;
    fEventsNumber=0;
    fChannelID=-1;
    NullifyHistogramPointers();
  }

  void ChannelDat::RenameHistograms(){
  }

  void ChannelDat::CreateHistograms(){
    std::stringstream channame;channame<<fChannelID;
    std::string namestr,titlestr;

    const unsigned int nsamp=v1751_const::gNumSamples;
    namestr="cumulativeSignalPlot_ch_"+channame.str();titlestr=namestr;
    cumulativeSignalPlot = new TH2F(namestr.c_str(),titlestr.c_str(),nsamp,0,nsamp,1700,-1500,200);
    namestr="integralOfPeakRegion_ch_"+channame.str();titlestr=namestr;
    integralOfPeakRegion = new TH1D(namestr.c_str(),titlestr.c_str(),2000,0,20000);

    namestr="integralNoise_ch_"+channame.str();titlestr=namestr;
    integralNoise = new TH1F(namestr.c_str(),titlestr.c_str(),2000,0,20000);

    namestr="netSignal_ch_"+channame.str();titlestr=namestr;
    netSignal = new TH1F(namestr.c_str(),titlestr.c_str(),2000,0,20000);

    namestr="numPhotoElectronsDistr_ch_"+channame.str();titlestr=namestr;
    numPhotoElectronsDistr= new TH1F(namestr.c_str(),titlestr.c_str(),20,0,20);

    namestr="eventTimeDistr_ch_"+channame.str();titlestr=namestr;
    fH1EventTimeDistr = new TH1F(namestr.c_str(),titlestr.c_str(),3000,0,3000);

  }

  void ChannelDat::WriteHistograms(TDirectory* dir){
    dir->cd();
    if(cumulativeSignalPlot)cumulativeSignalPlot->Write();
    if(netSignal           )netSignal->Write();
    if(fH1EventTimeDistr   )fH1EventTimeDistr->Write();
  }

  void ChannelDat::DeleteHistograms(){
    if(numPhotoElectronsDistr){;delete numPhotoElectronsDistr;}
    if(cumulativeSignalPlot  ){;delete cumulativeSignalPlot;}
    if(integralOfPeakRegion  ){;delete integralOfPeakRegion;}
    if(integralNoise         ){;delete integralNoise;}
    if(netSignal             ){;delete netSignal;}
    if(fH1EventTimeDistr     ){;delete fH1EventTimeDistr;}
    NullifyHistogramPointers();
  }

  void ChannelDat::NullifyHistogramPointers(){
    numPhotoElectronsDistr =0;
    fH1EventTimeDistr      =0;
    cumulativeSignalPlot   =0;
    integralOfPeakRegion   =0;
    integralNoise          =0;
    netSignal              =0;
  }



  AnalyseBurst::AnalyseBurst(){/*{{{*/
fSamplestestfile.open("test-samples.dat",std::fstream::out);
  }/*}}}*/

  void AnalyseBurst::Init(RAW::Event&evt){
    //std::cout<<"analyseburst "<<std::endl;

    for(int ch=0;ch<8;++ch) {
      if(evt.HasChannel(ch)){
        fChannelsData_[ch].SetChannelID(ch);

        //fChannelsData[i].SetChannelID(i);
        //std::cout<<"make chan "<<ch<<std::endl;
      }
    }
  }

  void AnalyseBurst::Process(RAW::Event& evt){/*{{{*/

    //    for(channels_map_t::iterator chandat_it=fChannelsData.begin();
    //        chandat_it!=fChannelsData.end();
    //        ++chandat_it){


      int trig_reg[2]={2700,2900};
      double trigMinVal=-100;
      RAW::Samples::samples_vector_t trigsamp=evt.GetChannel(0);
      for(int samp_i=trig_reg[0];samp_i<trig_reg[1];++samp_i){
          //std::cout<<trigMinVal<<"  "<<trigsamp[samp_i]<<std::endl;
        if(trigMinVal<trigsamp[samp_i]){
          trigMinVal=trigsamp[samp_i];
        }
      }
      const double trigHalfMinVal=trigMinVal/3;
      int trigT0=1000;
      for(int samp_i=trig_reg[0];samp_i<trig_reg[1];++samp_i){
        if(trigHalfMinVal<trigsamp[samp_i]){
          trigT0=samp_i-2770;
          break;
        }
      }


    for(int chan_i=0;chan_i<8;++chan_i){
      if(!fChannelsData_[chan_i].IsEnabled())continue;

      const int chanID=chan_i;
      ChannelDat& chandat=fChannelsData_[chan_i];
      RAW::Samples::samples_vector_t samp=evt.GetChannel(chanID);

      if(!samp.size()>0)continue;

      int pedestal_reg[2]={0,500};
      //int signal_reg[2]={740,900};
      int signal_reg[2]={740,2300};
      //int signal_reg[2]={560,700};




      double ped=0;
      for(int samp_i=pedestal_reg[0];samp_i<pedestal_reg[1];++samp_i){
        ped+=samp[samp_i];
      }
      chandat.fPedestal=ped/(-pedestal_reg[0]+pedestal_reg[1]);
      for(unsigned int samp_i=0;samp_i<samp.size();++samp_i)
        samp[samp_i]-=chandat.fPedestal;

      if(chandat.fEventsNumber<10){
        for(unsigned int samp_i=0;samp_i<samp.size();++samp_i)
          fSamplestestfile<<samp[samp_i]<<std::endl;;
        fSamplestestfile<<std::endl;;
        fSamplestestfile<<std::endl;;
      }



      double maxY=100;
      double maxYTime=0;

      double signal_int=0;
      for(int samp_i=signal_reg[0]; samp_i<signal_reg[1]; ++samp_i){
        signal_int+=samp[samp_i];
        if(maxY>samp[samp_i]){
          maxY=samp[samp_i];
          maxYTime=samp_i;
        }
      }
      chandat.netSignal->Fill(-signal_int);
      chandat.fSignalIntegral=signal_int;
      chandat.fAmplitudesPerBlink.push_back(signal_int);
      chandat.fMeanTime.push_back(signal_int/(signal_reg[1]-signal_reg[0]));




      double front[10];
      double back[10];
      for(int samp_i=signal_reg[0];
          samp_i<maxYTime;
          ++samp_i){
        const int x1=samp_i;
        const int x2=samp_i+1;
        const double y1=samp[x1];
        const double y2=samp[x2];
        if(y2<.1*maxY&&.1*maxY<=y1){front[1]=-(.1*maxY-y1)/(y2-y1)+x1;}
        if(y2<.2*maxY&&.2*maxY<=y1){front[2]=-(.2*maxY-y1)/(y2-y1)+x1;}
        if(y2<.3*maxY&&.3*maxY<=y1){front[3]=-(.3*maxY-y1)/(y2-y1)+x1;}
        if(y2<.4*maxY&&.4*maxY<=y1){front[4]=-(.4*maxY-y1)/(y2-y1)+x1;}
        if(y2<.5*maxY&&.5*maxY<=y1){front[5]=-(.5*maxY-y1)/(y2-y1)+x1;}
        if(y2<.6*maxY&&.6*maxY<=y1){front[6]=-(.6*maxY-y1)/(y2-y1)+x1;}
        if(y2<.7*maxY&&.7*maxY<=y1){front[7]=-(.7*maxY-y1)/(y2-y1)+x1;}
        if(y2<.8*maxY&&.8*maxY<=y1){front[8]=-(.8*maxY-y1)/(y2-y1)+x1;}
        if(y2<.9*maxY&&.9*maxY<=y1){front[9]=-(.9*maxY-y1)/(y2-y1)+x1;}

        // std::cout
        // <<"front maxYtime "<<maxYTime
        // <<" samp_i "<<samp_i<<"  "
        // <<" maxY "<<maxY
        // <<" samp[i] "<<samp[samp_i]
        // <<std::endl;
        front[0]=samp_i;
      }
      for(int samp_i=signal_reg[1];
          samp_i>maxYTime;
          --samp_i){
        const int x1=samp_i;
        const int x2=samp_i-1;
        const double y1=samp[x1];
        const double y2=samp[x2];
        if(y2<.1*maxY&&.1*maxY<=y1){back[1]=-(.1*maxY-y1)/(y2-y1)+x1;}
        if(y2<.2*maxY&&.2*maxY<=y1){back[2]=-(.2*maxY-y1)/(y2-y1)+x1;}
        if(y2<.3*maxY&&.3*maxY<=y1){back[3]=-(.3*maxY-y1)/(y2-y1)+x1;}
        if(y2<.4*maxY&&.4*maxY<=y1){back[4]=-(.4*maxY-y1)/(y2-y1)+x1;}
        if(y2<.5*maxY&&.5*maxY<=y1){back[5]=-(.5*maxY-y1)/(y2-y1)+x1;}
        if(y2<.6*maxY&&.6*maxY<=y1){back[6]=-(.6*maxY-y1)/(y2-y1)+x1;}
        if(y2<.7*maxY&&.7*maxY<=y1){back[7]=-(.7*maxY-y1)/(y2-y1)+x1;}
        if(y2<.8*maxY&&.8*maxY<=y1){back[8]=-(.8*maxY-y1)/(y2-y1)+x1;}
        if(y2<.9*maxY&&.9*maxY<=y1){back[9]=-(.9*maxY-y1)/(y2-y1)+x1;}
        // std::cout
        // <<"back maxYtime "<<maxYTime
        // <<" samp_i "<<samp_i<<"  "
        // <<" maxY "<<maxY
        // <<" samp[i] "<<samp[samp_i]
        // <<std::endl;
        back[0]=samp_i;
      }
      for(int i=0;i<9;++i)
        chandat.fTOT[i].push_back(back[i]-front[i]);
      chandat.fRiseTime.push_back(front[9]-front[1]);
      chandat.fFallTime.push_back(back[1]- back[9]);
      chandat.fMaxY    .push_back(maxY);
      chandat.fMaxYTime.push_back(maxYTime);
      //std::cout
      //  <<"front "<<front[9]<<" "<<front[1]
      //  <<"back "<<back[1]<<" "<<back[9]
      //  <<std::endl;



      //double netAmplitude=0;

      //std::cout<<"bef"<<std::endl;
      for(unsigned int samp_i=0;samp_i<samp.size();++samp_i){
        chandat.cumulativeSignalPlot->Fill(
            samp_i,samp[samp_i]);
      }
      //std::cout<<"aft"<<std::endl;




      chandat.fEventsNumber++;
    }
  }/*}}}*/

  void AnalyseBurst::PostProcess(){/*{{{*/
    for(int channel_i=0;channel_i<v1751_const::gChanMax;++channel_i){
      //if(channel_i!=1)continue;

    }
  }/*}}}*/

  void AnalyseBurst::WriteToFile(std::string filename){/*{{{*/
    TFile* fRootFileP=new TFile(filename.c_str(),"recreate");
    for(int chan_i=0;chan_i<8;++chan_i){
      if(!fChannelsData_[chan_i].IsEnabled())continue;
      ChannelDat& chandat=fChannelsData_[chan_i];
      const int chanID=chan_i;


      TDirectory* fileDir=fRootFileP;
      std::stringstream chan_i_ss;
      chan_i_ss<<chanID;
      std::string dirName="chan_"+chan_i_ss.str();
      TDirectory* dirchan=fileDir->mkdir(dirName.c_str());
      chandat.WriteHistograms(dirchan);
    }
    fRootFileP->Write();
    fRootFileP->Close();
    delete fRootFileP;
  }/*}}}*/

  AnalyseBurst::~AnalyseBurst(){/*{{{*/

    fSamplestestfile.close();
    for(int chan_i=0;chan_i<v1751_const::gChanMax;++chan_i){
      //if(fHists.HasChan(chan_i)){
      //ChannelHists::hists_per_chan_t& fChanHists=fHists.GetChan(chan_i);
      //}
    }
  }/*}}}*/
  };
