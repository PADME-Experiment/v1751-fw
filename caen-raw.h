#ifndef _caen_raw_h_
#define _caen_raw_h_

#include<string>
#include<stdint.h>
#include<stdio.h>


class FileHandler{
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
};

class EventHeader{
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
    EventHeader(FileHandler* fh);
    ~EventHeader();
    uint32_t GetEventSize() const ;
    bool PullRawData();
    void Info();
    unsigned int GetNChannels();
    uint32_t GetRawChannelSize();
  private:
    FileHandler* fRawFile;
    header_t fHeader;
};


class Event{
  public:
  typedef struct {
    unsigned int sample1 :10;
    unsigned int sample2 :10;
    unsigned int sample3 :10;
    unsigned int nSamples:2;
  } __attribute__((packed)) raw_word_t;
    Event(FileHandler* fh);
    ~Event();
    bool PullRawData();
    void Info();
  private:
    FileHandler* fRawFile;
    EventHeader* fEventHeader;
    raw_word_t*  fEventAllRaw;
    unsigned int fChannelsPerEvent;
    uint32_t     fChannelSize;  // in uint32_t words
    uint32_t     fEventSize;  // in uint32_t words

};


#endif
