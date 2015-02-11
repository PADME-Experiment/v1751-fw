#ifndef _caen_raw_h_
#define _caen_raw_h_

#include<string>
#include<stdint.h>
#include<stdio.h>
#include<vector>
#include<map>


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

class EventChannel{
};
class Event{
  public:
    void Reserve(int channelId, uint32_t numSamples);
    void PushSample(int channelId, int16_t value);
    std::vector<int16_t>& GetChannel(int i);
    std::map<int,std::vector<int16_t> >::iterator GetChannelsBegin(){return fChannelsData.begin();}
    std::map<int,std::vector<int16_t> >::iterator GetChannelsEnd()  {return fChannelsData.end();}
    std::vector<int>& GetListOfChannels();
    void SetListOfChannels (std::vector<int>&);

  private:
    std::map<int /**channelId*/, std::vector<int16_t> > fChannelsData;
    std::vector<int /** channelId*/> fListOfChannels;
};




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
    void GetNextRawToEvent(Event&);
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





#endif
