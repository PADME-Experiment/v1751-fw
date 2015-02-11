#ifndef _caen_raw_h_
#define _caen_raw_h_

#include<string.h>
#include<stdint.h>
#include<stdio.h>

  class File{/*{{{*/
  public:
  File(std::string& ifn);
  ~File();
  template<typename T> bool GetFileData(T& dat);
  template<typename T> bool GetFileDataArray(T* dat);
  private:
  FILE *fRawFileP;
  };/*}}}*/


//class EventHeader{
//  typedef struct{
//    uint32_t eventSize   :28;
//    uint32_t header1010  : 4;
//    union {
//      char ch[8]:1;
//    }channelMask;
//    uint32_t reserved1   :16;
//    uint32_t zero        : 1;
//    uint32_t reserved2   : 7;
//    uint32_t eventCounter:24;
//    uint32_t reserved3   : 8;
//    uint32_t triggerTimeTag;
//  }header_t;
//
//  public:
//  EventHeader(FileHandler* fh);
//  ~EventHeader();
//  uint32_t GetEventSize() const {return fHeader.eventSize;}
//  bool PullRawData();
//  void Info();
//
//  private:
//  FileHandler* fRawFile;
//  header_t fHeader;
//};
//
//
//class Event{
//  public:
//    Event(FileHandler* fh);
//    ~Event();
//    bool PullRawData();
//    void Info();
//  private:
//    FileHandler* fRawFile;
//    EventHeader* fEventHeader;
//
//};


#endif
