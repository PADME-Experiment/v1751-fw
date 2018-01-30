#include"2016-02-10-ledpmt-analyse_run.h"
#include"2016-02-10-ledpmt-analyse_burst.h"
#include"main.h"

#include<TF1.h>
#include<TFile.h>
#include<TDirectory.h>
#include<TGraphErrors.h>

#include<list>
#include<string>

namespace caen{
  void AnalyseRun::HistDelete(){/*{{{*/
    for(int chan_i=0;chan_i<v1751_const::gChanMax;++chan_i){
      delete nPhotoElectrons[chan_i];
      delete nEvents[chan_i];
      delete sigIntegral[chan_i];
      delete burstStatistics[chan_i];
      delete nPhotoelElectronSpe[chan_i];

      delete nPhotoElectrSum        [chan_i];
      delete fHist2TotVsIntAmp      [chan_i];
      delete fHist2RiseVsFall       [chan_i];
      delete fHist2RiseVsIntAmp     [chan_i];
      delete fHist2FallVsIntAmp     [chan_i];
      delete fMeanTimeStabInBurst   [chan_i];
      delete fMeanTimeStabVsBursti  [chan_i];
      delete fPeakTimeStabInBurst   [chan_i];
      delete fPeakTimeStabVsBursti  [chan_i];
      delete fHistRise[chan_i];
      delete fHistFall[chan_i];
    }
  }/*}}}*/


  void AnalyseRun::HistInit(){
    std::string histName,histTitle;
    for(int chan_i=0;chan_i<v1751_const::gChanMax;chan_i++){
      std::stringstream chan_i_ss;
      chan_i_ss<<chan_i;

      histName="nPhotoElectronSpe_ch_"+chan_i_ss.str();
      histTitle="n photo electrons ch "+chan_i_ss.str();
      nPhotoelElectronSpe[chan_i] = new TH1F(histName.c_str(),histTitle.c_str(),
          1000,-20,1000);

histName="nPhotoelectrSum_ch_"+chan_i_ss.str();
histTitle=histName;
nPhotoElectrSum[chan_i]=new TH1F(histName.c_str(),histTitle.c_str(),60000,0,400000);

      histName="riseTime_ch_"+chan_i_ss.str();
      histTitle="Rising times ch "+chan_i_ss.str();
      fHistRise[chan_i] = new TH1F(histName.c_str(),histTitle.c_str(),
          400,-2,50);

      histName="fallTime_ch_"+chan_i_ss.str();
      histTitle="Falling times ch "+chan_i_ss.str();
      fHistFall[chan_i] = new TH1F(histName.c_str(),histTitle.c_str(),
          400,-2,50);



      histName="TOTvsIntAmp_ch_"+chan_i_ss.str();
      histTitle="TOT vs Nphe ch "+chan_i_ss.str();
      fHist2TotVsIntAmp      [chan_i]=new TH2F(histName.c_str(),histTitle.c_str(),
        400,-2,50,1000,-20,1000); histName="RiseVsFall_ch_"+chan_i_ss.str();
      histTitle="Riseing time vs Falling time ch "+chan_i_ss.str();
      fHist2RiseVsFall       [chan_i]=new TH2F(histName.c_str(),histTitle.c_str(),
          400,-2,50,400,-2,50);
      histName="RiseVsIntAmp_ch_"+chan_i_ss.str();
      histTitle="Rising time vs Nphe ch "+chan_i_ss.str();
      fHist2RiseVsIntAmp     [chan_i]=new TH2F(histName.c_str(),histTitle.c_str(),
          400,-2,50,200,-20,1000);
      histName="FallVsIntAmp_ch_"+chan_i_ss.str();
      histTitle="Falling time vs Nphe ch "+chan_i_ss.str();
      fHist2FallVsIntAmp     [chan_i]=new TH2F(histName.c_str(),histTitle.c_str(),
          400,-2,50,200,-20,1000);



      nPhotoElectrons[chan_i] =new TGraphErrors(gNBursts);
      nEvents[chan_i]         =new TGraphErrors(gNBursts);
      sigIntegral[chan_i]     =new TGraphErrors(gNBursts);
      burstStatistics[chan_i] =new TGraphErrors(caen::v1751_const::gNumSamples);


      fMeanTimeStabInBurst   [chan_i]=new TGraphErrors(gNBursts);
      fMeanTimeStabVsBursti  [chan_i]=new TGraphErrors(caen::v1751_const::gNumSamples);
      fPeakTimeStabInBurst   [chan_i]=new TGraphErrors(gNBursts);
      fPeakTimeStabVsBursti  [chan_i]=new TGraphErrors(caen::v1751_const::gNumSamples);




      histName="nPhotoElectrons_ch_"+chan_i_ss.str();
      histTitle="Number of photo elelctrons ch "+chan_i_ss.str();
      nPhotoElectrons[chan_i]->
        SetNameTitle(histName.c_str(),histTitle.c_str());

      histName="nEvents_ch_"+chan_i_ss.str();
      histTitle="Number of events ch "+chan_i_ss.str();
      nEvents[chan_i]->
        SetNameTitle(histName.c_str(),histTitle.c_str());

      histName="sigIntegral_ch_"+chan_i_ss.str();
      histTitle="Signal Integral ch "+chan_i_ss.str();
      sigIntegral[chan_i]->
        SetNameTitle(histName.c_str(),histTitle.c_str());

      histName="burstStatistics_ch_"+chan_i_ss.str();
      histTitle="Burst impulse statistics ch "+chan_i_ss.str();
      burstStatistics[chan_i]->
        SetNameTitle(histName.c_str(),histTitle.c_str());

      histName="MeanTimeStabInBurst_ch_"+chan_i_ss.str();
      histTitle="Mean time stability in the burst ch "+chan_i_ss.str();
      fMeanTimeStabInBurst [chan_i]->SetNameTitle(histName.c_str(),histTitle.c_str());
      histName="MeanTimeStabVsBursti_ch_"+chan_i_ss.str();
      histTitle="Mean time stability vs burst_i ch "+chan_i_ss.str();
      fMeanTimeStabVsBursti[chan_i]->SetNameTitle(histName.c_str(),histTitle.c_str());

      histName="PeakTimeStabInBurst_ch_"+chan_i_ss.str();
      histTitle="Peak time stability in the burst ch "+chan_i_ss.str();
      fPeakTimeStabInBurst [chan_i]->SetNameTitle(histName.c_str(),histTitle.c_str());
      histName="PeakTimeStabVsBursti_ch_"+chan_i_ss.str();
      histTitle="Peak time stability vs burst_i ch "+chan_i_ss.str();
      fPeakTimeStabVsBursti[chan_i]->SetNameTitle(histName.c_str(),histTitle.c_str());



    }
  }




  void AnalyseRun::Init(){ }

  void AnalyseRun::Process(ChannelDat burstchandat[]){/*{{{*/


    //for(AnalyseBurst::channels_map_t::iterator
    //    chandat_it=burstchandat.begin();
    //    chandat_it!=burstchandat.end();
    //    ++chandat_it){
    //  ChannelDat& chandat=(chandat_it->second);
    //  const int chanID=chandat.GetChannelID();


    for(int chan_i=0;chan_i<8;++chan_i){
      if(!burstchandat[chan_i].IsEnabled())continue;
      ChannelDat& chandat=burstchandat[chan_i];
      const int chanID=chan_i;


      nEvents[chanID]->SetPoint     (fBurst_i,fBurst_i+1,chandat.fEventsNumber);


      double sumAmp=0;
      double sum2Amp=0;
      double sumPeaktime=0;
      double sum2Peaktime=0;
      const unsigned int nSamp=chandat.fAmplitudesPerBlink.size();
      fSumPeakTimes[chanID].resize(nSamp);
      fSum2PeakTimes[chanID].resize(nSamp);
      fSumAmplitudes [chanID].resize(nSamp);
      fSum2Amplitudes[chanID].resize(nSamp);
      double nPheAllburst=0;
      for(unsigned int imp_i=0;imp_i<nSamp;++imp_i){
        const double val=chandat.fAmplitudesPerBlink[imp_i];
        const double peaktime=chandat.fMaxYTime[imp_i];
        const double risetime=chandat.fRiseTime[imp_i];
        const double falltime=chandat.fFallTime[imp_i];

        fSumPeakTimes[chanID] [imp_i]+=peaktime;
        fSum2PeakTimes[chanID][imp_i]+=peaktime*peaktime;

        fSumAmplitudes[chanID][imp_i] +=val;
        fSum2Amplitudes[chanID][imp_i]+=val*val;
        sumAmp+= val;
        sum2Amp+=val*val;
        sumPeaktime+=peaktime;
        sum2Peaktime+=peaktime*peaktime;
        const double nphe=-val/PMT_const::gain/5/1.6;
        nPheAllburst+=nphe;
        nPhotoelElectronSpe[chanID]->Fill(nphe);

        fHist2TotVsIntAmp    [chanID]->Fill(chandat.fTOT[5][imp_i],nphe);
        fHist2RiseVsFall     [chanID]->Fill(risetime,falltime);
        fHist2RiseVsIntAmp   [chanID]->Fill(risetime,nphe);
        fHist2FallVsIntAmp   [chanID]->Fill(falltime,nphe);
        fHistRise            [chanID]->Fill(risetime);
        fHistFall            [chanID]->Fill(falltime);

      }
      nPhotoElectrSum[chanID]->Fill(nPheAllburst);
      const double meanAmp=sumAmp/nSamp;
      const double sigAmp=sqrt(sum2Amp-sumAmp*sumAmp/nSamp)/nSamp;

      const double meanPeakTime=sumPeaktime/nSamp;
      const double sigPeakTime=sqrt(sum2Peaktime-sumPeaktime*sumPeaktime/nSamp)/nSamp;
      //1./1.5e5/*GAIN*/ /50/1.6e-19/1e4/1e9 =  1/1.5/5/1.6
      const double nphe=-meanAmp/PMT_const::gain/5/1.6;
      const double signphe=sigAmp/PMT_const::gain/5/1.6;





      fPeakTimeStabVsBursti[chanID]->SetPoint(fBurst_i,fBurst_i+1,meanPeakTime);
      fPeakTimeStabVsBursti[chanID]->SetPointError(fBurst_i,0,sigPeakTime);




      sigIntegral[chanID]->SetPoint(fBurst_i,fBurst_i+1,meanAmp);
      sigIntegral[chanID]->SetPointError(fBurst_i,0,sigAmp);

      nPhotoElectrons[chanID]->SetPoint(fBurst_i,fBurst_i+1,nphe);
      nPhotoElectrons[chanID]->SetPointError(fBurst_i,0,signphe);


    }
    //for(int chan_i=0;chan_i<v1751_const::gChanMax;++chan_i){
    //  if(!burstAllHists.HasChan(chan_i) )continue;
    //  AnalyseBurst::ChannelHists::hists_per_chan_t&
    //    burstChanHists = burstAllHists.GetChan(chan_i);

    //  nPhotoElectrons[chan_i]-> SetPoint     (fBurst_i,fBurst_i+1,burstChanHists.nPhotoElectronsNorm[0]);
    //  nPhotoElectrons[chan_i]-> SetPointError(fBurst_i,.5,burstChanHists.nPhotoElectronsNorm[1]);

    //nEvents[chan_i]->SetPoint     (fBurst_i,fBurst_i+1,burstChanHists.fEventsNumber);
    //  nEvents[chan_i]->SetPointError(fBurst_i,0,0);
    //}
    fBurst_i++;
  } /*}}}*/ //AnalyseRun::Process





  void AnalyseRun::Finish(){
    TF1 * func=new TF1("mean signal","[0]");
    for(int chan_i=0;chan_i<v1751_const::gChanMax;++chan_i){

      sigIntegral[chan_i]->Fit(func);
      nPhotoElectrons[chan_i]->Fit(func);


      for(unsigned int i=0;i<fSumAmplitudes[chan_i].size();++i){
        const double sumPeaktime =fSumPeakTimes[chan_i][i];
        const double sumPeaktime2=fSum2PeakTimes[chan_i][i];
        double sum=fSumAmplitudes[chan_i][i];
        double sum2=fSum2Amplitudes[chan_i][i];
        double n=fBurst_i;
        double mean=sum/n;
        double sig =sqrt(sum2-sum*sum/n)/n;
        const double meanPeakTime=sumPeaktime/n;
        const double sigPeakTime=sqrt(sumPeaktime2-sumPeaktime/n)/n;

        mean*=-1./PMT_const::gain/5/1.6;
        sig*=1./PMT_const::gain/5/1.6;

        burstStatistics[chan_i]->SetPoint(i,i+1,mean);
        burstStatistics[chan_i]->SetPointError(i,0,sig);

        fPeakTimeStabInBurst [chan_i]->SetPoint(i,i+1,meanPeakTime);
        fPeakTimeStabInBurst [chan_i]->SetPointError(i,0,sigPeakTime);
      }
    }
    delete func;
  }

  void AnalyseRun::WriteToFile(std::string outFileName){/*{{{*/
    fRootFileP=new TFile(outFileName.c_str(),"recreate");
    for(int chan_i=0;chan_i<v1751_const::gChanMax;++chan_i){
      //if enabled
      std::stringstream chan_i_ss;
      chan_i_ss<<chan_i;
      std::string dirName="chan_"+chan_i_ss.str();
      TDirectory* fileDir=fRootFileP;
      TDirectory* dirchan=fileDir->mkdir(dirName.c_str());
      dirchan->cd();

      nPhotoElectrons[chan_i]->Write();
      nEvents[chan_i]->Write();
      sigIntegral[chan_i]->Write();
      burstStatistics[chan_i]->Write();
      nPhotoelElectronSpe[chan_i]->Write();
      nPhotoElectrSum[chan_i]->Write();


        fHist2TotVsIntAmp      [chan_i]->Write();
        fHist2RiseVsFall       [chan_i]->Write();
        fHist2RiseVsIntAmp     [chan_i]->Write();
        fHist2FallVsIntAmp     [chan_i]->Write();
        fMeanTimeStabInBurst   [chan_i]->Write();
        fMeanTimeStabVsBursti  [chan_i]->Write();
        fPeakTimeStabInBurst   [chan_i]->Write();
        fPeakTimeStabVsBursti  [chan_i]->Write();
        fHistRise[chan_i]->Write();
        fHistFall[chan_i]->Write();

      //TDirectory* dirgauspars=dirchan->mkdir("gausPars_vs_burstId");
      //dirgauspars->cd();
    }
    fRootFileP->Write();
    fRootFileP->Close();
    delete fRootFileP;
  }/*}}}*/ //WriteToFile
  }
