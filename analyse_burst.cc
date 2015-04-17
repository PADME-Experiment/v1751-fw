#include"analyse_burst.h"
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

  AnalyseBurst::ChannelHists::ChannelHists(){/*{{{*/
    for(int i=0;i<v1751_const::gChanMax;++i){
      fChanHists[i].hasChan=false;
    }
  }/*}}}*/
  AnalyseBurst::ChannelHists::~ChannelHists(){/*{{{*/
    for(int chan_i=0;chan_i<v1751_const::gChanMax;++chan_i){
      if(HasChan(chan_i)){
        hists_per_chan_t& chanHists = GetChan(chan_i);
        delete chanHists.numPhotoElectronsDistr;
        delete chanHists.cumulativeSignalPlot;
        delete chanHists.integralOfPeakRegion;
        delete chanHists.integralNoise;
        delete chanHists.netSignal;
        delete chanHists.gausMean_photoElectrons;
        delete chanHists.gausAmplitude_photoElectrons;
        delete chanHists.gausSigma_photoElectrons;
      }
    }
  }/*}}}*/
  void AnalyseBurst::ChannelHists::MakeChan(int ch){/*{{{*/
    if(fChanHists[ch].hasChan)return;

    std::stringstream channame;channame<<ch;
    std::string namestr,titlestr;


    fChanHists[ch].hasChan=true;
    const unsigned int nsamp=v1751_const::gNumSamples;
    namestr="cumulativeSignalPlot_ch_"+channame.str();
    titlestr="cumulative Signal Plot channel"  +channame.str();
    fChanHists[ch].cumulativeSignalPlot = new TH2F(namestr.c_str(),titlestr.c_str(),nsamp,0,nsamp,1700,-200,1500);

    namestr="integralOfPeakRegion_ch_"+channame.str();
    titlestr="integral Of Peak Region channel "+channame.str();
    fChanHists[ch].integralOfPeakRegion = new TH1D(namestr.c_str(),titlestr.c_str(),0000,0,20000);

    namestr="integralNoise_ch_"+channame.str();
    titlestr="integral Noise channel"+channame.str();
    fChanHists[ch].integralNoise = new TH1F(namestr.c_str(),titlestr.c_str(),2000,0,20000);

    namestr="netSignal_ch_"+channame.str();
    titlestr="Net Signal channel "+channame.str();
    fChanHists[ch].netSignal = new TH1F(namestr.c_str(),titlestr.c_str(),2000,0,20000);

    namestr="numPhotoElectronsDistr_ch_"+channame.str();
    titlestr="Number of photo electrons chan "+channame.str();
    fChanHists[ch].numPhotoElectronsDistr= new TH1F(namestr.c_str(),titlestr.c_str(),20,0,20);

    fChanHists[ch].gausMean_photoElectrons     = new TGraphErrors (20);
    fChanHists[ch].gausAmplitude_photoElectrons= new TGraphErrors (20);
    fChanHists[ch].gausSigma_photoElectrons    = new TGraphErrors (20);

    namestr="gausMean_photoElectrons_ch_"+channame.str();
    fChanHists[ch].gausMean_photoElectrons      ->
      SetNameTitle(namestr.c_str(),titlestr.c_str());

    namestr="gausAmplitude_photoElectrons_ch_"+channame.str();
    fChanHists[ch].gausAmplitude_photoElectrons ->
      SetNameTitle(namestr.c_str(),titlestr.c_str());

    namestr="gausSigma_photoElectrons_ch_"+channame.str();
    fChanHists[ch].gausSigma_photoElectrons     ->
      SetNameTitle(namestr.c_str(),titlestr.c_str());
  }/*}}}*/

  bool AnalyseBurst::ChannelHists::HasChan(int ch){/*{{{*/
    return fChanHists[ch].hasChan;
  }/*}}}*/
  AnalyseBurst::ChannelHists::hists_per_chan_t& AnalyseBurst::ChannelHists::GetChan(int ch){/*{{{*/
    if(!HasChan(ch)){std::cerr<<"chan "<<ch<<" has not been made"<<std::endl;exit(1);}
    return fChanHists[ch];
  }/*}}}*/



  AnalyseBurst::AnalyseBurst(Event& evt){/*{{{*/
    for(int i=0;i<v1751_const::gChanMax;++i)
      if(evt.HasChannel(i)){
        fHists.MakeChan(i);
      }
  }/*}}}*/

  void AnalyseBurst::Process(Event& evt){/*{{{*/
    for(Event::chan_iterator chan_it=evt.GetChannelsBegin();
        chan_it!=evt.GetChannelsEnd();
        ++chan_it){
      ChannelSamples& chan=chan_it->second;
      int chanId=chan_it->first;
      ChannelHists::hists_per_chan_t& chanHists = fHists.GetChan(chanId);

      double sumHits=0;
      double sumNoise=0;
      const int startSignalTimeRegion= 750;
      const int stopSignalTimeRegion = 850;
      int sample_i=0;
      for(ChannelSamples::iterator samp_it=chan.GetSamplesBegin();
          samp_it!=chan.GetSamplesEnd();
          ++samp_it){
        const ChannelSamples::sample_value_t sampValue=*samp_it;
        chanHists.cumulativeSignalPlot->Fill(sample_i,sampValue);
        sumHits+=
          sample_i>=startSignalTimeRegion &&
          sample_i<stopSignalTimeRegion?sampValue
          :0;
        sumNoise+=
          sample_i>=0 &&
          sample_i<stopSignalTimeRegion-startSignalTimeRegion?sampValue
          :0;
        ++sample_i;
      }
      chanHists.integralOfPeakRegion->Fill(sumHits);
      chanHists.integralNoise->Fill(sumNoise);
    }
  }/*}}}*/

  void AnalyseBurst::Finish(){/*{{{*/
    for(int channel_i=0;channel_i<v1751_const::gChanMax;++channel_i){
      if(fHists.HasChan(channel_i)){
        ChannelHists::hists_per_chan_t& chanHists=fHists.GetChan(channel_i);
        chanHists.nEvt=chanHists.integralOfPeakRegion->GetEntries();
        TF1* fitSinExp=new TF1("sinExp","[0]*exp(-0.5*((x-[1])/[2])*((x-[1])/[2]))*(sin(x*[3]+[4])+1)",200,1400);
        fitSinExp->SetParLimits(3,
            3.14/(
              chanHists.integralOfPeakRegion->GetBinCenter(
                chanHists.integralOfPeakRegion->GetNbinsX()
                )) ,
            4*3.14/(
              chanHists.integralOfPeakRegion->GetBinCenter(
                chanHists.integralOfPeakRegion->GetMaximumBin()
                ))
            );
        fitSinExp->SetParameter(3,.5*3.14/(chanHists.integralOfPeakRegion->GetBinCenter(chanHists.integralOfPeakRegion->GetMaximumBin())));
        fitSinExp->SetParLimits(4,-3.14,3*3.15);
        fitSinExp->SetParameter(4,0);
        fitSinExp->SetParameter(0,chanHists.integralOfPeakRegion->GetMaximum()/2.);
        fitSinExp->SetParLimits(0,1,999999999);
        fitSinExp->FixParameter(1,0);
        fitSinExp->SetParLimits(2,chanHists.integralOfPeakRegion->GetRMS(),10*chanHists.integralOfPeakRegion->GetRMS());
        fitSinExp->SetParameter(2,2*chanHists.integralOfPeakRegion->GetRMS());
        Int_t fitresi=Int_t(chanHists.integralOfPeakRegion->Fit("sinExp","W"));

        const double fitSinGaus_A =fitSinExp->GetParameter(0);
        const double fitSinGaus_X0=fitSinExp->GetParameter(1);
        const double fitSinGaus_w =fitSinExp->GetParameter(2);
        const double fitSinGaus_a =fitSinExp->GetParameter(3);
        const double fitSinGaus_b =fitSinExp->GetParameter(4);
        delete fitSinExp;
        if(fitresi)continue;

        int gaus_i=0;
        Int_t gausresi;
        TFitResultPtr gausFitRes;
        TF1* fitGaus;
        do {  // per peak gaus fits
          std::stringstream funcname;
          funcname<<"gaus_"<<gaus_i;
          fitGaus=new TF1(
              funcname.str().c_str(),
              "([0]/([2]*sqrt(2.*acos(-1.))) ) * exp(-0.5*((x-[1])/[2])**2)",
              ((2*gaus_i-.5)*3.14-fitSinGaus_b)/fitSinGaus_a,
              ((2*gaus_i+1.5)*3.14-fitSinGaus_b)/fitSinGaus_a);
          chanHists.gausFunctions.push_back(fitGaus);

          fitGaus->SetParameter(1,((2*gaus_i+.5)*3.14-fitSinGaus_b)/fitSinGaus_a);
          fitGaus->SetParLimits(1,
              ((2*gaus_i)*3.14-fitSinGaus_b)/fitSinGaus_a,
              ((2*gaus_i+1)*3.14-fitSinGaus_b)/fitSinGaus_a);
          fitGaus->SetParameter(2,1./fitSinGaus_a);
          fitGaus->SetParLimits(2,.05/fitSinGaus_a,10/fitSinGaus_a);
          fitGaus->SetParameter(0,2*fitSinGaus_A*exp(-.5*(fitSinGaus_X0-fitGaus->GetParameter(1))/fitSinGaus_w*(fitSinGaus_X0-fitGaus->GetParameter(1))/fitSinGaus_w));

          std::cout
            <<"fitGaus->GetParameter(1) "<<fitGaus->GetParameter(1) <<std::endl
            <<"fitGaus->GetParameter(2) "<<fitGaus->GetParameter(2) <<std::endl
            <<"fitGaus->GetParameter(0) "<<fitGaus->GetParameter(0) <<std::endl;

          gausFitRes=chanHists.integralOfPeakRegion->Fit(funcname.str().c_str(),"R+");
          gausresi=Int_t(gausFitRes);
          std::cout<<gaus_i<<" "<<gausresi<<std::endl;

          chanHists.gausAmplitude_photoElectrons->
            SetPoint(gaus_i,gaus_i+1,fitGaus->GetParameter(0));
          chanHists.gausMean_photoElectrons->
            SetPoint(gaus_i,gaus_i+1,fitGaus->GetParameter(1));
          chanHists.gausSigma_photoElectrons->
            SetPoint(gaus_i,gaus_i+1,fitGaus->GetParameter(2));

          chanHists.gausAmplitude_photoElectrons->
            SetPointError(gaus_i,.5,fitGaus->GetParError(0));
          chanHists.gausMean_photoElectrons->
            SetPointError(gaus_i,.5,fitGaus->GetParError(1));
          chanHists.gausSigma_photoElectrons->
            SetPointError(gaus_i,.5,fitGaus->GetParError(2));


          gaus_i++;
        } while (gausresi==0);
        chanHists.gausAmplitude_photoElectrons->Set(gaus_i);
        chanHists.gausMean_photoElectrons->Set(gaus_i);
        chanHists.gausSigma_photoElectrons->Set(gaus_i);


        chanHists.integralOfPeakRegion->Sumw2();
        chanHists.integralNoise->Sumw2();
        chanHists.netSignal->Add(
            chanHists.integralOfPeakRegion,
            chanHists.integralNoise,
            1,-1);
        TF1* gain_f=new TF1("gain_f","[0]*x+[1]");
        gain_f->SetParameters(500,0);
        chanHists.gausMean_photoElectrons->Fit(gain_f);

        chanHists.offset[0]=gain_f->GetParameter(1);
        chanHists.offset[1]=gain_f->GetParError(1);
        chanHists.gain[0]=gain_f->GetParameter(0);
        chanHists.gain[1]=gain_f->GetParError(0);

        delete gain_f;

        chanHists.nPhotoElectrons[0]=chanHists.nPhotoElectrons[1]=0;
        for(int bin_i=1;bin_i<chanHists.netSignal->GetNbinsX();++bin_i){
          chanHists.nPhotoElectrons[0]+=
            chanHists.netSignal->GetBinContent(bin_i)*(chanHists.netSignal->GetBinCenter(bin_i)-chanHists.offset[0])/
            chanHists.gain[0];
          std::cout<<"bin_i "<<bin_i<<"  cont "<<chanHists.netSignal->GetBinContent(bin_i)<<"   center "<<chanHists.netSignal->GetBinCenter(bin_i)<<"    gaus_i "<< (chanHists.netSignal->GetBinCenter(bin_i)-chanHists.offset[0])/ chanHists.gain[0]<<std::endl;

          chanHists.numPhotoElectronsDistr->SetBinContent(
              round((chanHists.netSignal->GetBinCenter(bin_i)-chanHists.offset[0])/
                chanHists.gain[0])-1,
              chanHists.numPhotoElectronsDistr->GetBinContent(
                round((chanHists.netSignal->GetBinCenter(bin_i)-chanHists.offset[0])/
                  chanHists.gain[0])-1)+
              chanHists.netSignal->GetBinContent(bin_i)
              );

          chanHists.nPhotoElectrons[1]+=
            chanHists.netSignal->GetBinContent(bin_i)*
            chanHists.netSignal->GetBinError(bin_i)*
            chanHists.netSignal->GetBinError(bin_i);
        }
        chanHists.nPhotoElectrons[1]=sqrt(chanHists.nPhotoElectrons[1]);
        chanHists.nPhotoElectrons[1]=1;

        chanHists.nPhotoElectronsNorm[0]=
          chanHists.nPhotoElectrons[0]/chanHists.nEvt;
        chanHists.nPhotoElectronsNorm[1]=
          chanHists.nPhotoElectrons[1]/chanHists.nEvt;



      }
    }
  }/*}}}*/

  void AnalyseBurst::WriteToFile(std::string filename){/*{{{*/
    TFile* fRootFileP=new TFile(filename.c_str(),"recreate");
    for(int chan_i=0;chan_i<v1751_const::gChanMax;++chan_i){
      if(fHists.HasChan(chan_i)){
        TDirectory* fileDir=fRootFileP;
        std::stringstream chan_i_ss;
        chan_i_ss<<chan_i;
        std::string dirName="chan_"+chan_i_ss.str();
        TDirectory* dirchan=fileDir->mkdir(dirName.c_str());
        dirchan->cd();

        ChannelHists::hists_per_chan_t& fChanHists=fHists.GetChan(chan_i);
        fChanHists.cumulativeSignalPlot          ->Write();
        fChanHists.integralOfPeakRegion          ->Write();
        fChanHists.integralNoise                 ->Write();
        fChanHists.netSignal                     ->Write();
        fChanHists.numPhotoElectronsDistr        ->Write();
        fChanHists.gausAmplitude_photoElectrons  ->Write();
        fChanHists.gausMean_photoElectrons       ->Write();
        fChanHists.gausSigma_photoElectrons      ->Write();

      }
    }
    fRootFileP->Write();
    fRootFileP->Close();
    delete fRootFileP;
  }/*}}}*/

  AnalyseBurst::~AnalyseBurst(){/*{{{*/
    for(int chan_i=0;chan_i<v1751_const::gChanMax;++chan_i){
      if(fHists.HasChan(chan_i)){
        ChannelHists::hists_per_chan_t& fChanHists=fHists.GetChan(chan_i);
        for(
            ChannelHists::gausFuncIter_t
            gaus_it=fChanHists.gausFunctions.begin();
            gaus_it!=fChanHists.gausFunctions.end();
            ++gaus_it){
          delete *gaus_it;
        }
      }
    }
  }/*}}}*/
};
