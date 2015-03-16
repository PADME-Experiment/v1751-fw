#include"analyse_burst.h"

#include"main.h"
#include"caen_raw.h"

#include<sstream>
#include<vector>
#include<string>


#include<TF1.h>
#include<TMath.h>
//#include<Fit/FitResult.h>
//#include<TFitResultPtr.h>
#include<TFitResult.h>
#include<TGraphErrors.h>



namespace caen{

  ChannelHists::ChannelHists(){/*{{{*/
    for(int i=0;i<gChanMax;++i){
      chanhist[i].hasChan=false;
    }
  }/*}}}*/
  ChannelHists::~ChannelHists(){/*{{{*/
    for(int i=0;i<gChanMax;++i){
      if(chanhist[i].hasChan){
        delete chanhist[i].cumulativeSignalPlot;
        delete chanhist[i].integralOfPeakRegion;
        delete chanhist[i].photoElectronPeaksMerged;
        delete chanhist[i].photoElectronPeaksMerged2;
        delete chanhist[i].gausMean_photoElectrons;
        delete chanhist[i].photPeakMer;
        delete chanhist[i].gausMeanPhotElec;
      }
    }
  }/*}}}*/
  void ChannelHists::MakeChan(int ch){/*{{{*/
    if(chanhist[ch].hasChan)return;

    std::stringstream channame;channame<<ch;
    std::string namestr,titlestr;

    chanhist[ch].hasChan=true;

    namestr="cumulativeSignalPlot_ch_"+channame.str();
    titlestr="cumulative Signal Plot channel"  +channame.str();
    chanhist[ch].cumulativeSignalPlot = new TH2F(namestr.c_str(),titlestr.c_str(),3000,0,3000,1700,-200,1500);

    namestr="integralOfPeakRegion_ch_"+channame.str();
    titlestr="integral Of Peak Region channel "+channame.str();
    chanhist[ch].integralOfPeakRegion = new TH1F(namestr.c_str(),titlestr.c_str(),3000,0,20000);



    chanhist[ch].photoElectronPeaksMerged=0;
    chanhist[ch].photoElectronPeaksMerged2=0;
    chanhist[ch].gausMean_photoElectrons=0;
    chanhist[ch].photPeakMer=0;
    chanhist[ch].gausMeanPhotElec=0;
  }/*}}}*/
  bool ChannelHists::HasChan(int ch){/*{{{*/
    return chanhist[ch].hasChan;
  }/*}}}*/
  ChannelHists::hist_per_chan_t& ChannelHists::GetChan(int ch){/*{{{*/
    if(!HasChan(ch)){std::cerr<<"chan "<<ch<<" has not been made"<<std::endl;exit(1);}
    return chanhist[ch];
  }/*}}}*/



  AnalyseBurst::AnalyseBurst(Event& evt){/*{{{*/
    for(int i=0;i<ChannelHists::gChanMax;++i)
      if(evt.HasChannel(i)){
        hists.MakeChan(i);
      }
  }/*}}}*/
  void AnalyseBurst::Process(Event& evt){/*{{{*/
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
        hists.GetChan(chanId).cumulativeSignalPlot->Fill(i,*samp_it);
        summ+=i>750&&i<850?*samp_it:0;
        //summ+=i>560&&i<620?*samp_it:0;
      }

      hists.GetChan(chanId).integralOfPeakRegion->Fill(summ);
    }
  }/*}}}*/
  void AnalyseBurst::Finish(){/*{{{*/
    for(int channel_i=0;channel_i<ChannelHists::gChanMax;++channel_i){
      if(hists.HasChan(channel_i)){
        TF1* fitSinExp=new TF1("sinExp","[0]*exp(-0.5*((x-[1])/[2])*((x-[1])/[2]))*(sin(x*[3]+[4])+1)",200,1400);
        fitSinExp->SetParLimits(3,
            3.14/(
              hists.GetChan(channel_i).integralOfPeakRegion->GetBinCenter(
                hists.GetChan(channel_i).integralOfPeakRegion->GetNbinsX()
                )) ,
            4*3.14/(
              hists.GetChan(channel_i).integralOfPeakRegion->GetBinCenter(
                hists.GetChan(channel_i).integralOfPeakRegion->GetMaximumBin()
                ))
            );
        fitSinExp->SetParameter(3,.5*3.14/(hists.GetChan(channel_i).integralOfPeakRegion->GetBinCenter(hists.GetChan(channel_i).integralOfPeakRegion->GetMaximumBin())));
        fitSinExp->SetParLimits(4,-3.14,3*3.15);
        fitSinExp->SetParameter(4,0);
        fitSinExp->SetParameter(0,hists.GetChan(channel_i).integralOfPeakRegion->GetMaximum()/2.);
        fitSinExp->SetParLimits(0,1,999999999);
        fitSinExp->FixParameter(1,0);
        fitSinExp->SetParLimits(2,hists.GetChan(channel_i).integralOfPeakRegion->GetRMS(),10*hists.GetChan(channel_i).integralOfPeakRegion->GetRMS());
        fitSinExp->SetParameter(2,2*hists.GetChan(channel_i).integralOfPeakRegion->GetRMS());
        hists.GetChan(channel_i).integralOfPeakRegion->Rebin(3);
        Int_t fitresi=Int_t(hists.GetChan(channel_i).integralOfPeakRegion->Fit("sinExp","W"));


        const double fitSinGaus_A =fitSinExp->GetParameter(0);
        const double fitSinGaus_X0=fitSinExp->GetParameter(1);
        const double fitSinGaus_w =fitSinExp->GetParameter(2);
        const double fitSinGaus_a =fitSinExp->GetParameter(3);
        const double fitSinGaus_b =fitSinExp->GetParameter(4);
        delete fitSinExp;
        if(fitresi)continue;


        std::vector<TF1*> gausFunctions;
        int gaus_i;
        Int_t gausresi;
        do {
          gaus_i=gausFunctions.size();
          std::stringstream funcname;
          funcname<<"gaus_"<<gaus_i;
          TF1* fitGaus=new TF1(
              funcname.str().c_str(),
              "[0]*exp(-0.5*((x-[1])/[2])**2)",
              ((2*gaus_i-.5)*3.14-fitSinGaus_b)/fitSinGaus_a,
              ((2*gaus_i+1.5)*3.14-fitSinGaus_b)/fitSinGaus_a);
          gausFunctions.push_back(fitGaus);

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

          //hists.GetChan(channel_i).integralOfPeakRegion->Fit(funcname.str().c_str(),"R+");
          gausresi=Int_t(hists.GetChan(channel_i).integralOfPeakRegion->Fit(fitGaus,"BR+"));
          std::cout<<gaus_i<<" "<<gausresi<<std::endl;;
        } while (gausresi==0);


        std::stringstream channelssname;
        channelssname<<channel_i;

        std::string photoElectronsName="photoElectronsPeaksMerged_ch_"+channelssname.str();
        std::string photoElectronsTitle="photo Electron Peaks Merged channel "+channelssname.str();
        hists.GetChan(channel_i).photoElectronPeaksMerged=new TH1F(photoElectronsName.c_str(), photoElectronsTitle.c_str(), gaus_i*2, 0, gaus_i);
        //hists.GetChan(channel_i).photoElectronPeaksMerged2=new TH2F(photoElectronsName.c_str(), photoElectronsTitle.c_str(), gaus_i*2, 0, gaus_i,200,0,1000);
        hists.GetChan(channel_i).photoElectronPeaksMerged2=new TH2F(photoElectronsName.c_str(), photoElectronsTitle.c_str(), 40, 0, 20,300,0,5000);
        std::cout<<"gaus_i "<<gaus_i<<std::endl;

        std::string gausMean_photoElectronsName="gausMean_photoElectrons_ch_"+channelssname.str();
        std::string gausMean_photoElectronsTitle="gaus mean photo Electrons channel "+channelssname.str();
        hists.GetChan(channel_i).gausMean_photoElectrons=new TH2F(gausMean_photoElectronsName.c_str(), gausMean_photoElectronsTitle.c_str(), 40, 0, 20,300,0,5000);


        //Double_t x[peak_i];
        for(int peak_i=1;peak_i<=gaus_i;++peak_i){



          hists.GetChan(channel_i).photoElectronPeaksMerged2->Fill(
              peak_i,
              hists.GetChan(channel_i).integralOfPeakRegion->Integral(
                hists.GetChan(channel_i).integralOfPeakRegion->FindBin(
                  ((2*peak_i-.5)*3.14-fitSinGaus_b)/fitSinGaus_a),
                hists.GetChan(channel_i).integralOfPeakRegion->FindBin(
                  ((2*peak_i+1.5)*3.14-fitSinGaus_b)/fitSinGaus_a)));
          hists.GetChan(channel_i).photoElectronPeaksMerged->SetBinContent(
              hists.GetChan(channel_i).photoElectronPeaksMerged->FindBin(peak_i),
              hists.GetChan(channel_i).integralOfPeakRegion->Integral(
                hists.GetChan(channel_i).integralOfPeakRegion->FindBin(
                  ((2*peak_i-.5)*3.14-fitSinGaus_b)/fitSinGaus_a),
                hists.GetChan(channel_i).integralOfPeakRegion->FindBin(
                  ((2*peak_i+1.5)*3.14-fitSinGaus_b)/fitSinGaus_a)));

          hists.GetChan(channel_i).gausMean_photoElectrons->Fill(
          peak_i,
          gausFunctions[peak_i-1]->GetParameter(1)
          );
          delete gausFunctions[peak_i-1];

        }
      }
    }
  }/*}}}*/
  void AnalyseBurst::WriteToFile(std::string filename){/*{{{*/
    TFile* fRootFileP=new TFile(filename.c_str(),"recreate");
    for(int i=0;i<ChannelHists::gChanMax;++i){
      if(hists.HasChan(i)){
        fRootFileP->CurrentDirectory()->WriteTObject(hists.GetChan(i).cumulativeSignalPlot);
        fRootFileP->CurrentDirectory()->WriteTObject(hists.GetChan(i).integralOfPeakRegion);
        if(hists.GetChan(i).photoElectronPeaksMerged)fRootFileP->CurrentDirectory()->WriteTObject(hists.GetChan(i).photoElectronPeaksMerged);
        if(hists.GetChan(i).photoElectronPeaksMerged2)fRootFileP->CurrentDirectory()->WriteTObject(hists.GetChan(i).photoElectronPeaksMerged2);
        if(hists.GetChan(i).gausMean_photoElectrons )fRootFileP->CurrentDirectory()->WriteTObject(hists.GetChan(i).gausMean_photoElectrons);
        if(hists.GetChan(i).photPeakMer             )fRootFileP->CurrentDirectory()->WriteTObject(hists.GetChan(i).photPeakMer     );
        if(hists.GetChan(i).gausMeanPhotElec        )fRootFileP->CurrentDirectory()->WriteTObject(hists.GetChan(i).gausMeanPhotElec);
      }
    }
    fRootFileP->Write();
    fRootFileP->Close();
    delete fRootFileP;
  }/*}}}*/

};
