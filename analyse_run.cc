#include"analyse_run.h"
#include"analyse_burst.h"
#include"main.h"

#include<TF1.h>
#include<TFile.h>
#include<TDirectory.h>
#include<TGraphErrors.h>

#include<list>
#include<string>

namespace caen{
  AnalyseRun::AnalyseRun(){/*{{{*/
    fBurst_i=0;
    std::string histName,histTitle;
    for(int chan_i=0;chan_i<v1751_const::gChanMax;chan_i++){
      std::stringstream chan_i_ss;
      chan_i_ss<<chan_i;

      nPhotoElectrons[chan_i]              =new TGraphErrors(gNBursts);
      gain_netSignal_MinToMax_hists[chan_i]=new TGraphErrors(gNBursts);
      nEvents[chan_i]                      =new TGraphErrors(gNBursts);

      histName="nPhotoElectrons_ch_"+chan_i_ss.str();
      histTitle="Number of photo elelctrons ch "+chan_i_ss.str();
      nPhotoElectrons[chan_i]->SetNameTitle(histName.c_str(),histTitle.c_str());

      histName="gain_netSignal_MinToMax_ch_"+chan_i_ss.str();
      histTitle="Gain based on difference between zeroth and first peak in netSignal ch "+chan_i_ss.str();
      gain_netSignal_MinToMax_hists[chan_i]->SetNameTitle(histName.c_str(),histTitle.c_str());


      histName="nEvents_ch_"+chan_i_ss.str();
      histTitle="Number of events ch "+chan_i_ss.str();
      nEvents[chan_i]->SetNameTitle(histName.c_str(),histTitle.c_str());

      for(int gaus_i=0;gaus_i<fNGausMax;gaus_i++){
        std::stringstream gaus_i_ss;
        gaus_i_ss<<gaus_i;
        gausPars_vs_burstId_hists[chan_i][gaus_i][0]=new TGraphErrors(gNBursts);
        gausPars_vs_burstId_hists[chan_i][gaus_i][1]=new TGraphErrors(gNBursts);
        gausPars_vs_burstId_hists[chan_i][gaus_i][2]=new TGraphErrors(gNBursts);

        histName="Agaus_"+gaus_i_ss.str()+"_ch_"+chan_i_ss.str()+"_vs_burstId";
        histTitle="gaus "+gaus_i_ss.str()+" A vs burstId chan "+chan_i_ss.str();
        gausPars_vs_burstId_hists[chan_i][gaus_i][0]->
          SetNameTitle(histName.c_str(),histTitle.c_str());
        histName="Xgaus_"+gaus_i_ss.str()+"_ch_"+chan_i_ss.str()+"_vs_burstId";
        histTitle="gaus X0 vs burstId chan "+chan_i_ss.str();
        gausPars_vs_burstId_hists[chan_i][gaus_i][1]->
          SetNameTitle(histName.c_str(),histTitle.c_str());
        histName="Wgaus_"+gaus_i_ss.str()+"_ch_"+chan_i_ss.str()+"_vs_burstId";
        histTitle="gaus W vs burstId chan "+chan_i_ss.str();
        gausPars_vs_burstId_hists[chan_i][gaus_i][2]->
          SetNameTitle(histName.c_str(),histTitle.c_str());
      }
    }
  }/*}}}*/
  AnalyseRun::~AnalyseRun(){/*{{{*/
    for(int chan_i=0;chan_i<v1751_const::gChanMax;++chan_i){
      delete gain_netSignal_MinToMax_hists[chan_i];
      delete nPhotoElectrons[chan_i];
      delete nEvents[chan_i];
      for(int gaus_i=0;gaus_i<fNGausMax;++gaus_i)
        for(int var_i=0;var_i<3;++var_i)
          //if(gausPars_vs_burstId_hists[chan_i][gaus_i][var_i])
          delete gausPars_vs_burstId_hists[chan_i][gaus_i][var_i];
    }
  }/*}}}*/

  void AnalyseRun::Init(){ }

  void AnalyseRun::Process(AnalyseBurst::ChannelHists& burstAllHists){/*{{{*/
    //fBurst_i++;
    for(int chan_i=0;chan_i<v1751_const::gChanMax;++chan_i){
      if(!  burstAllHists.HasChan(chan_i) )continue;
      AnalyseBurst::ChannelHists::hists_per_chan_t&
        burstChanHists = burstAllHists.GetChan(chan_i);

      gain_netSignal_MinToMax_hists[chan_i]->SetPoint     (fBurst_i,fBurst_i+1,burstChanHists.gain[0]);
      gain_netSignal_MinToMax_hists[chan_i]->SetPointError(fBurst_i,.5,burstChanHists.gain[1]);

      nPhotoElectrons[chan_i]->SetPoint     (fBurst_i,fBurst_i+1,burstChanHists.nPhotoElectronsNorm[0]);
      nPhotoElectrons[chan_i]->SetPointError(fBurst_i,.5,burstChanHists.nPhotoElectronsNorm[1]);

      nEvents[chan_i]->SetPoint     (fBurst_i,fBurst_i+1,burstChanHists.nEvt);
      nEvents[chan_i]->SetPointError(fBurst_i,0,0);

      int gaus_i=0;
      for( AnalyseBurst::ChannelHists::gausFuncIter_t
          gaus_it=burstChanHists.gausFunctions.begin();
          gaus_it!=burstChanHists.gausFunctions.end();
          ++gaus_it,++gaus_i){
        TF1* gausf=*gaus_it;

        gausPars_vs_burstId_hists[chan_i][gaus_i][0]->SetPoint     (fBurst_i,fBurst_i+1,gausf->GetParameter(0));
        gausPars_vs_burstId_hists[chan_i][gaus_i][0]->SetPointError(fBurst_i,.5,gausf->GetParError (0));
        gausPars_vs_burstId_hists[chan_i][gaus_i][1]->SetPoint     (fBurst_i,fBurst_i+1,gausf->GetParameter(1));
        gausPars_vs_burstId_hists[chan_i][gaus_i][1]->SetPointError(fBurst_i,.5,gausf->GetParError (1));
        gausPars_vs_burstId_hists[chan_i][gaus_i][2]->SetPoint     (fBurst_i,fBurst_i+1,gausf->GetParameter(2));
        gausPars_vs_burstId_hists[chan_i][gaus_i][2]->SetPointError(fBurst_i,.5,gausf->GetParError (2));

      }
    }
    fBurst_i++;
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
      nPhotoElectrons[chan_i]->Write();
      nEvents[chan_i]->Write();
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
