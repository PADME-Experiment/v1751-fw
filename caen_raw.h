#ifndef _caen_raw_h_
#define _caen_raw_h_

#include<iostream>

#include<string>
#include<stdint.h>
#include<stdio.h>
#include<vector>
#include<map>

#include<algorithm>

namespace RAW{
  class FileHandler{/*{{{*/
    public:
      FileHandler(std::string&);
      ~FileHandler();
      bool IsOpened(){return fRawFileP!=NULL;}
      //FileHandler* GetInstance(){return this;}
      //FileHandler(FileHandler&){;}
      //template<typename T> bool GetFileData(T& dat);
      //template<typename T> bool GetFileDataArray(T* dat, unsigned int n=1);
      uint32_t GetFileWord(){
        uint32_t ret=ftmp;
        Readtmp();
        if(feof(fRawFileP))std::cout<<"EOF"<<std::endl;
        return ret;}
      bool Eof(){return feof(fRawFileP);}
    private:
      bool fEOF;
      FILE* fRawFileP;
      uint32_t ftmp;
      void Readtmp(){ fread(&ftmp,sizeof(uint32_t),1,fRawFileP); }

  };/*}}}*/


  class Header{
      private:
      typedef union{/*{{{*/
        uint32_t rows[4];
        struct __attribute__((packed)){
          uint32_t row1;
          uint32_t row2;
          uint32_t row3;
          uint32_t row4;
        };
        struct __attribute__((packed)){
          uint32_t eventSize   :28;
          uint32_t header1010  : 4;
          union __attribute__((packed)){
            struct __attribute__((packed)){
              unsigned int ch0:1;
              unsigned int ch1:1;
              unsigned int ch2:1;
              unsigned int ch3:1;
              unsigned int ch4:1;
              unsigned int ch5:1;
              unsigned int ch6:1;
              unsigned int ch7:1;
            };
            unsigned int channelMask:8 ;
          };
          uint32_t patternLVDS   :16;
          uint32_t zero        : 1;
          uint32_t res   : 2;
          uint32_t boardID:5;
          uint32_t eventCounter:24;
          uint32_t reserved3   : 8;
          uint32_t triggerTimeTag;
        };
      } __attribute__((packed)) header_t;/*}}}*/
    public:
      Header(){fRow_i=0;}
      ~Header(){}
      void PushBinData(uint32_t data){
        //fHeader.rows[3-fRow_i]=data;
        fRow_i++;
        if(fRow_i==1)fHeader.row1=data;
        if(fRow_i==2)fHeader.row2=data;
        if(fRow_i==3)fHeader.row3=data;
        if(fRow_i==4)fHeader.row4=data;
        if(fRow_i==4)PostProc();


        }

      uint32_t GetEventSize()            const{return fEventSize;}
      unsigned int GetChannelMask()      const{return fChannelMask   ;}
      uint32_t GetEventId()              const{return fEventCounter  ;}
      uint32_t GetEventTimeTag()         const{return fTriggerTimeTag;}
      unsigned int GetNChannels()        const{return fNChannels;}
      uint32_t GetRawChannelSize()       const{return fChannelSize;}
      int      GetChannelOccurNumber(int chID)const{

        return fChannelOccurNumber[chID];}
      void Info();
    protected:
      uint32_t     fEventSize;
      unsigned int fChannelMask;
      uint32_t     fEventCounter;
      uint32_t     fTriggerTimeTag;
      unsigned int fNChannels;
      uint32_t     fChannelSize;
      int          fChannelOccurNumber[8];
      //std::vector<unsigned int> fIthChannelNames;
    private:
      int fRow_i;
      void PostProc();
      header_t fHeader;
  };

  class Samples{
    private:
      typedef union{/*{{{*/
        struct __attribute__((packed)){
          uint32_t event0   :10;
          uint32_t event1   :10;
          uint32_t event2   :10;
          uint32_t nEvents  : 2;
        };
        uint32_t word;
      }__attribute__((packed)) sample_word_t;/*}}}*/
    public:
      Samples(){fChannel_i=0;}
      ~Samples(){}



      typedef int16_t sample_t;
      typedef std::vector<int16_t> samples_vector_t;



      void PushBinData(uint32_t data){
        fSampleWord.word=data;
        if(fSampleWord.nEvents==3){
          fChannelSamples[fChannel_i].  push_back(fSampleWord.event0);
          fChannelSamples[fChannel_i].  push_back(fSampleWord.event1);
          fChannelSamples[fChannel_i].  push_back(fSampleWord.event2);
          //std::cout<<" has 3"<<std::endl;
        }else{
          if(fSampleWord.nEvents==1){
            //std::cout<<" has 1"<<std::endl;
            fChannelSamples[fChannel_i].  push_back(fSampleWord.event0);
          }else
            if(fSampleWord.nEvents==2){
              fChannelSamples[fChannel_i].push_back(fSampleWord.event1);
              //std::cout<<" has 2"<<std::endl;
            }else{
              //std::cout<<" has 0"<<std::endl;
            }
          ++fChannel_i;
        }
      }
      samples_vector_t GetChannelSamples(int ch){
        return fChannelSamples[ch];}
    protected:
      samples_vector_t fChannelSamples[8];
    private:
      int fChannel_i; //sequential number in the event
      sample_word_t fSampleWord;
      //friend unsigned int GetIthChannelID(int i);

  };

  class Event:public Header, public Samples{
    public:
      Event():Header::Header(),Samples::Samples(){
        fWord_i=0;
      }
      void PushBinData(uint32_t data){
        if(fWord_i<4)
          Header::PushBinData(data);
        else
          Samples::PushBinData(data);
        fWord_i++;
      }
      samples_vector_t GetChannel(int ch){
        return fChannelSamples[GetChannelOccurNumber(ch)];}
      bool HasChannel(int chanID)const{
        return fChannelOccurNumber[chanID]>=0;}
    private:
      unsigned int fWord_i;
  };
  class Raw:public FileHandler{
    public:
      Raw(std::string&fn):FileHandler(fn){ }
      ~Raw(){}
      void GetEvent(Event& evt){
        evt.PushBinData(GetFileWord());
        evt.PushBinData(GetFileWord());
        evt.PushBinData(GetFileWord());
        evt.PushBinData(GetFileWord());
        for(unsigned int i=0;i<evt.GetEventSize();++i){
          evt.PushBinData(GetFileWord());
          //std::cout<<i<<std::endl;
        }
      }

  };
}























namespace caen{
  namespace v1751_const{
    const int gChanMax=8;
    extern int gNumSamples;
  };
};



#endif
