#include"caen-raw.h"
#include"analyse.h"
#include<string>
#include<iostream>
#include<iomanip>
#include<stdio.h>



namespace caen{
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
    unsigned int bytesRead=fread((void*)&dat,sizeof(T),1,fRawFileP);
    if(bytesRead==1) return true;
    std::cerr<<"GetFileData EOF"<<std::endl;
    return false;
  }
  template<typename T> bool FileHandler::GetFileDataArray(T* dat,unsigned int n){
    unsigned int bytesRead=fread((void*)dat,sizeof(T),n,fRawFileP);
    //std::cout<<"sizof(t) "<<sizeof(T)<< " sizeof(dat)" <<sizeof(dat)<<"  bytes "<<bytesRead<<std::endl;
    if(bytesRead==n) return true;
    std::cerr<<"EOF"<<std::endl;
    return false;
  }
  void FileHandler::Reset(){
    fseek(fRawFileP,0,SEEK_SET);
    return;
  }
  /*}}}*/
  RawHeader::RawHeader(FileHandler* fh){
    fRawFile=fh;
    PullRawData();
  }

  RawHeader::~RawHeader(){
  }
  bool RawHeader::PullRawData(){
    if(fRawFile->GetFileData(fHeader))
      return true;
    return false;
  }

  void RawHeader::Info(){/*{{{*/

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

  uint32_t RawHeader::GetEventSize() const {
    return fHeader.eventSize-4;
  }

  unsigned int RawHeader::GetNChannels(){
    unsigned int ret=0;
    for(int i=0;i<8;i++){
      ret+=fHeader.channelMask>>i&0x1;
    }
    return ret;
  }

  uint32_t RawHeader::GetRawChannelSize(){
    uint32_t evtSize=GetEventSize();
    unsigned int nChans=GetNChannels();
    unsigned int ret=evtSize/nChans;
    if(evtSize%nChans!=0){
      std::cerr<<"warning GetRawChannelSize"<<std::endl;
    }
    return ret;
  }




  Raw::Raw(FileHandler* fh,Event& evt){
    fRawFile=fh;
    fEventHeader=new RawHeader(fh);
    fEventHeader->Info();
    fEventSize=fEventHeader->GetEventSize();
    fChannelSize=fEventHeader->GetRawChannelSize();
    fChannelsPerEvent=fEventHeader->GetNChannels();

    const unsigned int mask=fEventHeader->GetChannelMask();
    for(int i=0;i<8;++i){
      if(mask>>i&1){
        fChannelIdMap.push_back(i);
        evt.Reserve(i,fChannelSize*3);
      }
    }
    evt.SetListOfChannels(fChannelIdMap);

    fEventAllRaw=new raw_word_t[fEventSize];
    fRawFile->Reset();// go to the beginning of the file


  }

  Raw::~Raw(){
    delete fEventHeader;
    delete[] fEventAllRaw;
  }

  bool Raw::PullRawData(){
    if(
        fEventHeader->PullRawData() &&
        fRawFile->GetFileDataArray(fEventAllRaw,fEventSize)
      )return true;
    return false;
  }




  void Raw::Info(){
    fEventHeader->Info();
    for (int i=0;i<fEventSize;++i){
      raw_word_t& rawWord=fEventAllRaw[i];
      int ch=fChannelIdMap[i/fChannelSize];
      int word=i%fChannelSize;
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

  bool Raw::GetNextRawToEvent(Event& evt){
    if(!PullRawData())return false;
    evt.ClearAll();
    evt.SetEventId(fEventHeader->GetEventId());
    evt.SetEventTimeTag(fEventHeader->GetEventTimeTag());
    for (int i=0;i<fEventSize;++i){
      raw_word_t& rawWord=fEventAllRaw[i];
      int ch=fChannelIdMap[i/fChannelSize];
      int word=i%fChannelSize;
      if(rawWord.nSamples==3){
        evt.PushSample(ch,rawWord.sample1);
        evt.PushSample(ch,rawWord.sample2);
        evt.PushSample(ch,rawWord.sample3);
      }else if(rawWord.nSamples==2){
        evt.PushSample(ch,rawWord.sample1);
        evt.PushSample(ch,rawWord.sample2);
      }else if(rawWord.nSamples==1){
        evt.PushSample(ch,rawWord.sample1);
      }else std::cerr<<"dummy word"<<std::endl;
    }
    return true;
  }








  void Event::Reserve(int channelId, uint32_t numSamples){
    fChannelsData[channelId].Reserve(numSamples);
    return;
  }
  void Event::PushSample(int channelId, int16_t value){
    fChannelsData[channelId].PushSample(value);
    return;
  }
  void Event::ClearAll(){
    for(iterator it=GetChannelsBegin();
        it!=GetChannelsEnd();
        ++it){
      it->second.Clear();
    }
    return;
  }
  void Event::Info(){
    std::cout<<GetEventId()<<std::endl;
    return;
  }










};



int main(){
  std::string fn="/home/daq/links/daq-optic-data/rawdata_2015_02_05_10_17_50";
  caen::FileHandler rawFile(fn);
  caen::Event evt;

  caen::Raw raw(rawFile.GetInstance(),evt);

  caen::Analyse ana;
  ana.Init();
  while( raw.GetNextRawToEvent(evt)){
    ana.Process(evt);
  }
  ana.Finish();

  return 0;
}
