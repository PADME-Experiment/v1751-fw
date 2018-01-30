#include"caen_raw.h"
//#include"analyse_burst.h"
#include<string>
#include<iostream>
#include<iomanip>
#include<stdio.h>


namespace RAW{
  FileHandler::FileHandler(std::string& ifn){
    fRawFileP=NULL;
    fEOF=false;
    fRawFileP=fopen(ifn.c_str(),"r");
    if(fRawFileP==NULL){
      std::cerr<<"File not found \""<<ifn<<"\""<<std::endl;
    }else{
      std::cerr<<"File \""<<ifn<<"\" successfuly oppened"<<std::endl;
    }
    Readtmp();
  }
  FileHandler::~FileHandler(){
    std::cout<<fRawFileP<<std::endl;
    if(fRawFileP)
      fclose(fRawFileP);
  }


     void Header::Info(){/*{{{*/
       std::cout<<std::endl
         << std::setfill('0') << std::setw(8)<<std::hex
         << std::setfill('0') << std::setw(8)<<fHeader.row1       <<std::endl
         << std::setfill('0') << std::setw(8)<<fHeader.row2       <<std::endl
         << std::setfill('0') << std::setw(8)<<fHeader.row3       <<std::endl
         << std::setfill('0') << std::setw(8)<<fHeader.row4       <<std::endl
         <<"eventSize      " << std::setfill('0') << std::setw(8)<<std::hex<<fHeader.eventSize       <<std::endl
         <<"header1010     " << std::setfill('0') << std::setw(8)<<std::hex<<fHeader.header1010      <<std::endl
         <<"channelMask    " << std::setfill('0') << std::setw(8)<<std::hex<<fHeader.channelMask   <<"  "<<GetNChannels()<<"  bits set"<<std::endl
         //<<"reserved1      " << std::setfill('0') << std::setw(8)<<std::hex<<fHeader.reserved1       <<std::endl
         <<"zero           " << std::setfill('0') << std::setw(8)<<std::hex<<fHeader.zero            <<std::endl
         //<<"reserved2      " << std::setfill('0') << std::setw(8)<<std::hex<<fHeader.RES       <<std::endl
         <<"eventCounter   " << std::setfill('0') << std::setw(8)<<std::hex<<fHeader.eventCounter    <<std::endl
         <<"reserved3      " << std::setfill('0') << std::setw(8)<<std::hex<<fHeader.reserved3       <<std::endl
         <<"triggerTimeTag " << std::setfill('0') << std::setw(8)<<std::hex<<fHeader.triggerTimeTag  <<std::endl
         <<std::endl;
       return;
     }/*}}}*/
}

namespace caen{
  namespace v1751_const{
    //int const gChanMax=8;
    int gNumSamples;//=334;
  };
};
