#include"analyse_run.h"
#include"analyse_burst.h"
#include"main.h"

#include<TF1.h>
#include<TFile.h>
#include<TDirectory.h>

#include<list>
#include<string>

namespace caen{
  AnalyseRun::AnalyseRun(){/*{{{*/
    fBurst_i=0;
    std::string histName,histTitle;
    for(int chan_i=0;chan_i<v1751_const::gChanMax;chan_i++){
      std::stringstream chan_i_ss;
      chan_i_ss<<chan_i;
      std::string dirName="chan_"+chan_i_ss.str();
      histName="gain_netSignal_MinToMax_hists_ch_"+chan_i_ss.str();
      histTitle="Gain based on difference between zeroth and first peak in netSignal ch "+chan_i_ss.str();
      gain_netSignal_MinToMax_hists[chan_i]=new TH1F(
          histName.c_str(),histTitle.c_str(),(gNBursts+2)*2,0,gNBursts+2);

      for(int gaus_i=0;gaus_i<fNGausMax;gaus_i++){
        std::stringstream gaus_i_ss;
        gaus_i_ss<<gaus_i;

        histName="Agaus_"+gaus_i_ss.str()+"_ch_"+chan_i_ss.str()+"_vs_burstId";
        histTitle="gaus "+gaus_i_ss.str()+" A vs burstId chan "+chan_i_ss.str();
        gausPars_vs_burstId_hists[chan_i][gaus_i][0]=new TH1F(
            histName.c_str(),histTitle.c_str(),(gNBursts+2)*2,0,gNBursts+2);

        histName="Xgaus_"+gaus_i_ss.str()+"_ch_"+chan_i_ss.str()+"_vs_burstId";
        histTitle="gaus X0 vs burstId chan "+chan_i_ss.str();
        gausPars_vs_burstId_hists[chan_i][gaus_i][1]=new TH1F(
            histName.c_str(),histTitle.c_str(),(gNBursts+2)*2,0,gNBursts+2);

        histName="Wgaus_"+gaus_i_ss.str()+"_ch_"+chan_i_ss.str()+"_vs_burstId";
        histTitle="gaus W vs burstId chan "+chan_i_ss.str();
        gausPars_vs_burstId_hists[chan_i][gaus_i][2]=new TH1F(
            histName.c_str(),histTitle.c_str(),(gNBursts+2)*2,0,gNBursts+2);
      }
    }
  }/*}}}*/
  AnalyseRun::~AnalyseRun(){/*{{{*/
    for(int chan_i=0;chan_i<v1751_const::gChanMax;++chan_i){
      delete gain_netSignal_MinToMax_hists[chan_i];
      for(int gaus_i=0;gaus_i<fNGausMax;++gaus_i)
        for(int var_i=0;var_i<3;++var_i)
          //if(gausPars_vs_burstId_hists[chan_i][gaus_i][var_i])
          delete gausPars_vs_burstId_hists[chan_i][gaus_i][var_i];
    }
  }/*}}}*/

  void AnalyseRun::Init(){ }

  void AnalyseRun::Process(AnalyseBurst::ChannelHists& burstAllHists){/*{{{*/
    fBurst_i++;
    for(int chan_i=0;chan_i<v1751_const::gChanMax;++chan_i){
      if(!  burstAllHists.HasChan(chan_i) )continue;
      AnalyseBurst::ChannelHists::hists_per_chan_t&
        burstChanHists = burstAllHists.GetChan(chan_i);

      gain_netSignal_MinToMax_hists[chan_i]->SetBinContent(
          fBurst_i,
          burstChanHists.netSignal->GetBinCenter(
            (burstChanHists.netSignal->GetMaximumBin()-
             burstChanHists.netSignal->GetMinimumBin())));

      int gaus_i=0;
      for( AnalyseBurst::ChannelHists::gausFuncIter_t
          gaus_it=burstChanHists.gausFunctions.begin();
          gaus_it!=burstChanHists.gausFunctions.end();
          ++gaus_it,++gaus_i){
        TF1* gausf=*gaus_it;
        gausPars_vs_burstId_hists[chan_i][gaus_i][0]->SetBinContent(
            fBurst_i,gausf->GetParameter(0));
        gausPars_vs_burstId_hists[chan_i][gaus_i][0]->SetBinError  (
            fBurst_i,gausf->GetParError (0));
        gausPars_vs_burstId_hists[chan_i][gaus_i][1]->SetBinContent(
            fBurst_i,gausf->GetParameter(1));
        gausPars_vs_burstId_hists[chan_i][gaus_i][1]->SetBinError  (
            fBurst_i,gausf->GetParError (1));
        gausPars_vs_burstId_hists[chan_i][gaus_i][2]->SetBinContent(
            fBurst_i,gausf->GetParameter(2));
        gausPars_vs_burstId_hists[chan_i][gaus_i][2]->SetBinError  (
            fBurst_i,gausf->GetParError (2));
      }
    }
  } /*}}}*/ //AnalyseRun::Process

  void AnalyseRun::Finish(){
  }

  void AnalyseRun::WriteToFile(std::string outFileName){/*{{{*/
    fRootFileP=new TFile(outFileName.c_str(),"recreate");
    for(int chan_i=0;chan_i<v1751_const::gChanMax;++chan_i){
      std::stringstream chan_i_ss;
      chan_i_ss<<chan_i;
      std::string dirName="chan_"+chan_i_ss.str();
      TDirectory* fileDir=fRootFileP;
      TDirectory* dirchan=fileDir->mkdir(dirName.c_str());
      dirchan->cd();
      gain_netSignal_MinToMax_hists[chan_i]->Write();
      TDirectory* dirgauspars=dirchan->mkdir("gausPars_vs_burstId");
      dirgauspars->cd();
      for(int gaus_i=0;gaus_i<fNGausMax;++gaus_i){
        for(int var_i=0;var_i<3;++var_i){
          if(gausPars_vs_burstId_hists[chan_i][gaus_i][var_i]){
            gausPars_vs_burstId_hists[chan_i][gaus_i][var_i]->Write();
          }
        }
      }
    }
    fRootFileP->Write();
    fRootFileP->Close();
    delete fRootFileP;
  }/*}}}*/ //WriteToFile
}
