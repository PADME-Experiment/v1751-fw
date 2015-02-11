#include"caen-raw.h"
#include<string.h>
#include<iostream>
#include<stdio.h>

using namespace std;
 //File /*{{{*/
 File::File(std::string& ifn){
 fRawFileP=NULL;
 fRawFileP=fopen(ifn.c_str(),"r");
 if(fRawFileP==NULL){
 cerr<<"File not found "<<ifn<<endl;
 }else{
 cerr<<"File "<<ifn<<" successfuly oppened"<<endl;
 }
 }
 File::~File(){
 cout<<fRawFileP<<endl;
 fclose(fRawFileP);
 }
 template<typename T> bool File::GetFileData(T& dat){
 int bytesRead=fread((void*)&dat,sizeof(T),1,fRawFileP);
 if(bytesRead==sizeof(T)) return true;
 cerr<<"EOF"<<endl;
 return false;
 }
 template<typename T> bool File::GetFileDataArray(T* dat){
 int bytesRead=fread((void*)dat,sizeof(T)*sizeof(dat),1,fRawFileP);
 cout<<"sizof(t) "<<sizeof(T)<< " sizeof(dat)" <<sizeof(dat)<<endl;
 if(bytesRead==sizeof(T)) return true;
 cerr<<"EOF"<<endl;
 return false;
 }
 /*}}}*/



//EventHeader::EventHeader(FileHandler* fh){
//  fRawFile=fh;
//  PullRawData();
//}
//
//EventHeader::~EventHeader(){
//}
//
//
//
//
//Event::Event(FileHandler* fh){
//  fRawFile=fh;
//  fEventHeader=new EventHeader(fh);
//  fEventHeader->GetEventSize();
//}
//
//Event::~Event(){
//  delete fEventHeader;
//}
//
//
//
//
