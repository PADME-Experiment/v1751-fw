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

    namestr="cumulativeSignalPlot_ch_"+channame.str();
    titlestr="cumulative Signal Plot channel"  +channame.str();
    chanhist[ch].cumulativeSignalPlot = new TH2F(namestr.c_str(),titlestr.c_str(),3000,0,3000,1700,-200,1500);

    namestr="integralOfPeakRegion_ch_"+channame.str();
    titlestr="integral Of Peak Region channel "+channame.str();
    chanhist[ch].integralOfPeakRegion = new TH1F(namestr.c_str(),titlestr.c_str(),3000,0,20000);
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
        hists.GetChan(chanId).cumulativeSignalPlot->Fill(i,*samp_it);
        summ+=i>750&&i<850?*samp_it:0;
        //summ+=i>560&&i<620?*samp_it:0;
      }

      hists.GetChan(chanId).integralOfPeakRegion->Fill(summ);
    }
  }


  void AnalyseBurst::Finish(){
    for(int i=0;i<ChannelHists::gChanMax;++i){
      if(hists.HasChan(i)){
        TF1* fitSinExp=new TF1("sinExp","[0]*exp(-0.5*((x-[1])/[2])*((x-[1])/[2]))*(sin(x*[3]+[4])+1)",200,1400);
        fitSinExp->SetParLimits(3,
            3.14/(
              hists.GetChan(i).integralOfPeakRegion->GetBinCenter(
                hists.GetChan(i).integralOfPeakRegion->GetNbinsX()
                )) ,
            4*3.14/(
              hists.GetChan(i).integralOfPeakRegion->GetBinCenter(
                hists.GetChan(i).integralOfPeakRegion->GetMaximumBin()
                ))
            );
        fitSinExp->SetParameter(3,.5*3.14/(hists.GetChan(i).integralOfPeakRegion->GetBinCenter(hists.GetChan(i).integralOfPeakRegion->GetMaximumBin())));
        fitSinExp->SetParLimits(4,-3.14,3*3.15);
        fitSinExp->SetParameter(4,0);
        fitSinExp->SetParameter(0,hists.GetChan(i).integralOfPeakRegion->GetMaximum()/2.);
        fitSinExp->SetParLimits(0,1,999999999);
        fitSinExp->FixParameter(1,0);
        fitSinExp->SetParLimits(2,hists.GetChan(i).integralOfPeakRegion->GetRMS(),10*hists.GetChan(i).integralOfPeakRegion->GetRMS());
        fitSinExp->SetParameter(2,2*hists.GetChan(i).integralOfPeakRegion->GetRMS());
        hists.GetChan(i).integralOfPeakRegion->Rebin(3);
        Int_t fitresi=Int_t(hists.GetChan(i).integralOfPeakRegion->Fit("sinExp","W"));

        if(fitresi)continue;

        const double fitSinGaus_A =fitSinExp->GetParameter(0);
        const double fitSinGaus_X0=fitSinExp->GetParameter(1);
        const double fitSinGaus_w =fitSinExp->GetParameter(2);
        const double fitSinGaus_a =fitSinExp->GetParameter(3);
        const double fitSinGaus_b =fitSinExp->GetParameter(4);


        std::vector<TF1*> gausFunctions;
        int gaus_i;
        Int_t gausresi;
        do {
          int gaus_i=gausFunctions.size();
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

          //hists.GetChan(i).integralOfPeakRegion->Fit(funcname.str().c_str(),"R+");
          gausresi=Int_t(hists.GetChan(i).integralOfPeakRegion->Fit(fitGaus,"BR+"));
          std::cout<<gaus_i<<" "<<gausresi<<std::endl;;
        } while (gausresi==0);
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
