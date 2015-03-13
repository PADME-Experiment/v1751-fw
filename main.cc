#include"main.h"
#include"caen_raw.h"
#include"analyse_run.h"
#include"analyse_burst.h"
#include<iostream>
#include<vector>

namespace caen{

  int gDebug;
  std::string gOutputRootFile;
}

int main(int argc, char* argv[]) {
  int opt;
  std::vector<std::string> inputFiles,inputLists;
  std::string tmpstring;

  while ((opt = getopt(argc, argv, ":i:l:o:d:")) != -1){
    switch (opt){
      case 'i': tmpstring=optarg;inputFiles.push_back(tmpstring);break;
      case 'l': tmpstring=optarg;inputLists.push_back(tmpstring);break;
      case 'o': caen::gOutputRootFile=optarg;break;
      case 'd': caen::gDebug=atoi(optarg); break;
      default:
                fprintf(
                    stderr,
                    "SYNOPSIS: %s [-i input file] [-l list file] [-o root_file]\n",argv[0]);
                return 1;break;
    }
  }

  if(
      caen::gOutputRootFile.size()==0||
      (inputLists.size()==0&&inputFiles.size()==0))
  {
    fprintf(stderr,"SYNOPSIS: %s [-i input file] [-l list file] [-o root_file]\n",argv[0]);
    return 1;}


  char tmpchar[500];


  for(unsigned int i=0;i<inputLists.size();++i){
    std::ifstream list(inputLists[i].c_str(),std::ifstream::in);
    if(!list.is_open())continue;
    std::cerr<<"proc list "<<inputLists[i]<<std::endl;
    while(!list.eof()){
      //list>>tmpstring;
      list.getline((char*)tmpchar,500,'\n');
      tmpstring=tmpchar;
      if(tmpstring.size()>0) inputFiles.push_back(tmpstring);
    }
  }



  for(std::vector<std::string>::iterator if_it=inputFiles.begin();if_it!=inputFiles.end();++if_it){
    std::string& filename=*if_it;



    //std::string fn="/home/daq/links/daq-optic-data/rawdata_2015_02_05_10_17_50";
    caen::FileHandler rawFile(filename);
    caen::Event evt;

    caen::Raw raw(rawFile.GetInstance(),evt);

    caen::AnalyseBurst ana;
    ana.Init(evt);
    while( raw.GetNextRawToEvent(evt)){
      ana.Process(evt);
    }
    ana.Finish();
  }





  return 0;
}




