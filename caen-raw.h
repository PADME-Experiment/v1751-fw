#ifndef _caen_raw_h_
#define _caen_raw_h_

#include<string>
#include<stdint.h>
#include<stdio.h>
#include<vector>
#include<map>

#include<algorithm>

namespace caen{

extern int gDebug;
extern std::string gOutputRootFile;


  class FileHandler{/*{{{*/
    public:
      FileHandler(std::string&);
      ~FileHandler();
      FileHandler* GetInstance(){return this;}
      FileHandler(FileHandler&){;}
      template<typename T> bool GetFileData(T& dat);
      template<typename T> bool GetFileDataArray(T* dat, unsigned int n=1);
      void Reset();
    private:
      FILE* fRawFileP;
  };/*}}}*/

  class ChannelSamples{
    public:
      typedef std::vector<int16_t>::iterator iterator;
      typedef std::vector<int16_t> samples_t;
      void Reserve(uint32_t res){fSamples.reserve(res);}
      void SetChanId(int ch){fChanId=ch;}
      int GetChanId(){return fChanId;}
      void PushSample(int16_t val){fSamples.push_back(val);}
      void Clear(){fSamples.clear();}
      iterator GetSamplesBegin(){return fSamples.begin();}
      iterator GetSamplesEnd  (){return fSamples.end  ();}
      samples_t& GetContainer(){return fSamples;}
    private:
      samples_t fSamples;
      int fChanId;
  };
  class Event{/*{{{*/
    private:
      uint32_t fEventId;
      uint32_t fEventTimeTag;

      std::map<int /**channelId*/, ChannelSamples > fChannelsData;
      std::vector<int /** channelId*/> fListOfChannels;

    public:
      typedef std::map<int /**channelId*/, ChannelSamples >::iterator chan_iterator;

      void Reserve(int channelId, uint32_t numSamples);
      void PushSample(int channelId, int16_t value);
      ChannelSamples& GetChannel(int i){return fChannelsData[i];}

        chan_iterator GetChannelsBegin(){return fChannelsData.begin();}
        chan_iterator GetChannelsEnd()  {return fChannelsData.end();}
        void SetListOfChannels (std::vector<int>& list){fListOfChannels=list;}
        void SetEventId(uint32_t id){fEventId=id;}
        void SetEventTimeTag(uint32_t timeTag){fEventTimeTag=timeTag;}
        std::vector<int>& GetListOfChannels (){return fListOfChannels;}
        uint32_t          GetEventId        (){return fEventId;}
        uint32_t          GetEventTimeTag   (){return fEventTimeTag;}
        void ClearAll();
        void Info();
        bool HasChannel(int chan){
          return (
          std::count(fListOfChannels.begin(),fListOfChannels.end(),chan)>0);}

      };/*}}}*/
      class RawHeader{/*{{{*/
        public:
          typedef struct{/*{{{*/
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
            uint32_t reserved1   :16;
            uint32_t zero        : 1;
            uint32_t reserved2   : 7;
            uint32_t eventCounter:24;
            uint32_t reserved3   : 8;
            uint32_t triggerTimeTag;
          } __attribute__((packed)) header_t;/*}}}*/
          RawHeader(FileHandler* fh);
          ~RawHeader();
          uint32_t GetEventSize() const ;
          unsigned int GetChannelMask(){return fHeader.channelMask;}
          uint32_t GetEventId()const{return fHeader.eventCounter;}
          uint32_t GetEventTimeTag()const{return fHeader.triggerTimeTag;}
          bool PullRawData();
          void Info();
          unsigned int GetNChannels();
          uint32_t GetRawChannelSize();
        private:
          FileHandler* fRawFile;
          header_t fHeader;
      };/*}}}*/
      class Raw{/*{{{*/
        public:
          typedef struct {
            unsigned int sample1 :10;
            unsigned int sample2 :10;
            unsigned int sample3 :10;
            unsigned int nSamples:2;
          } __attribute__((packed)) raw_word_t;
          Raw(FileHandler* fh,Event& evt);
          ~Raw();
          void Info();
          bool GetNextRawToEvent(Event&);
        private:
          bool PullRawData();
          FileHandler* fRawFile;
          RawHeader*   fEventHeader;
          raw_word_t*  fEventAllRaw;
          unsigned int fChannelsPerEvent;
          uint32_t     fChannelSize;  // in uint32_t words
          uint32_t     fEventSize;  // in uint32_t words w/o header
          std::vector<int /** seq num for Ids of collecting data channels*/ > fChannelIdMap;
      };/*}}}*/
  };



#endif
