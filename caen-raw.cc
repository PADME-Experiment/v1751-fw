#include"caen-raw.h"
#include<string>
#include<iostream>
#include<iomanip>
#include<stdio.h>


//File /*{{{*/
FileHandler::FileHandler(std::string& ifn){
  fRawFileP=NULL;
  fRawFileP=fopen(ifn.c_str(),"r");
  if(fRawFileP==NULL){
    std::cerr<<"File not found "<<ifn<<std::endl;
  }else{
    std::cerr<<"File "<<ifn<<" successfuly oppened"<<std::endl;
  }
}
FileHandler::~FileHandler(){
  std::cout<<fRawFileP<<std::endl;
  fclose(fRawFileP);
}
template<typename T> bool FileHandler::GetFileData(T& dat){
  int bytesRead=fread((void*)&dat,sizeof(T),1,fRawFileP);
  if(bytesRead==sizeof(T)) return true;
  std::cerr<<"EOF"<<std::endl;
  return false;
}
template<typename T> bool FileHandler::GetFileDataArray(T* dat,unsigned int n){
  int bytesRead=fread((void*)dat,sizeof(T),n,fRawFileP);
  std::cout<<"sizof(t) "<<sizeof(T)<< " sizeof(dat)" <<sizeof(dat)<<"  bytes "<<bytesRead<<std::endl;
  if(bytesRead==sizeof(T)*n) return true;
  std::cerr<<"EOF"<<std::endl;
  return false;
}
void FileHandler::Reset(){
  fseek(fRawFileP,0,SEEK_SET);
  return;
}
/*}}}*/



EventHeader::EventHeader(FileHandler* fh){
  fRawFile=fh;
  PullRawData();
}

EventHeader::~EventHeader(){
}
bool EventHeader::PullRawData(){
  fRawFile->GetFileData(fHeader);
  return true;
}

void EventHeader::Info(){/*{{{*/

  std::cout<<std::endl
    <<"eventSize      " << std::setfill('0') << std::setw(8)<<std::hex<<fHeader.eventSize       <<std::endl
    <<"header1010     " << std::setfill('0') << std::setw(8)<<std::hex<<fHeader.header1010      <<std::endl
    <<"channelMask    " << std::setfill('0') << std::setw(8)<<std::hex<<fHeader.channelMask   <<"  "<<GetNChannels()<<"  bits set"<<std::endl
    <<"reserved1      " << std::setfill('0') << std::setw(8)<<std::hex<<fHeader.reserved1       <<std::endl
    <<"zero           " << std::setfill('0') << std::setw(8)<<std::hex<<fHeader.zero            <<std::endl
    <<"reserved2      " << std::setfill('0') << std::setw(8)<<std::hex<<fHeader.reserved2       <<std::endl
    <<"eventCounter   " << std::setfill('0') << std::setw(8)<<std::hex<<fHeader.eventCounter    <<std::endl
    <<"reserved3      " << std::setfill('0') << std::setw(8)<<std::hex<<fHeader.reserved3       <<std::endl
    <<"triggerTimeTag " << std::setfill('0') << std::setw(8)<<std::hex<<fHeader.triggerTimeTag  <<std::endl
    <<std::endl;
  return;
}/*}}}*/

uint32_t EventHeader::GetEventSize() const {
  return fHeader.eventSize-4;
  }

unsigned int EventHeader::GetNChannels(){
  unsigned int ret=0;
  for(int i=0;i<8;i++){
    ret+=fHeader.channelMask>>i&0x1;
  }
  return ret;
}

uint32_t EventHeader::GetRawChannelSize(){
  uint32_t evtSize=GetEventSize();
  unsigned int nChans=GetNChannels();
  unsigned int ret=evtSize/nChans;
  if(evtSize%nChans!=0){
    std::cerr<<"warning GetRawChannelSize"<<std::endl;
  }
  return ret;
}




Event::Event(FileHandler* fh){
  fRawFile=fh;
  fEventHeader=new EventHeader(fh);
  fEventHeader->Info();
  fEventSize=fEventHeader->GetEventSize();
  fChannelSize=fEventHeader->GetRawChannelSize();
  fChannelsPerEvent=fEventHeader->GetNChannels();

  fEventAllRaw=new raw_word_t[fEventSize];
  fRawFile->Reset();// go to the beginning of the file


}

Event::~Event(){
  delete fEventHeader;
  delete[] fEventAllRaw;
}

bool Event::PullRawData(){
  fEventHeader->PullRawData();
  fRawFile->GetFileDataArray(fEventAllRaw,fEventSize);
}


void Event::Info(){
  fEventHeader->Info();

  for(int ch=0;ch<fChannelsPerEvent;++ch){
    for(int word=0;word<fChannelSize;++word){
      raw_word_t& rawWord=fEventAllRaw[ch*word];
      if(rawWord.nSamples==3){
        std::cout<<" chan "<<ch<<"   word "<<word<<" fields "
          <<rawWord.sample1<<" "
          <<rawWord.sample2<<" "
          <<rawWord.sample3
          <<std::endl;
      }else if(rawWord.nSamples==2){
        std::cout<<" chan "<<ch<<"   word "<<word<<" fields "
          <<rawWord.sample1<<" "
          <<rawWord.sample2
          <<std::endl;
      }else if(rawWord.nSamples==1){
        std::cout<<" chan "<<ch<<"   word "<<word<<" fields "
          <<rawWord.sample1
          <<std::endl;
      }else std::cerr<<"dummy word"<<std::endl;
    }
  }

}












int main(){

  std::string fn="/home/daq/links/daq-optic-data/rawdata_2015_02_05_10_17_50";
  FileHandler rawFile(fn);
  Event evt(rawFile.GetInstance());
  evt.PullRawData();
  evt.Info();
  evt.PullRawData();
  evt.Info();

  return 0;
}
