#include"main.h"
#include"caen_raw.h"
#include"analyse_run.h"
#include"analyse_burst.h"
#include<iostream>
#include<vector>

namespace caen{

  int gDebug;
  int gNBursts;
  std::string gOutputRootFile;
};

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
  caen::gNBursts=inputFiles.size();

  caen::AnalyseRun anarun;
  anarun.Init();

  for(std::vector<std::string>::iterator if_it=inputFiles.begin();if_it!=inputFiles.end();++if_it){
    std::string& filename=*if_it;
    std::cerr<<"Processing "<<filename<<std::endl;

    caen::FileHandler rawFile(filename);
    caen::Event evt;
    caen::Raw raw(&rawFile,evt);

    caen::AnalyseBurst anaburst(evt);
    while( raw.GetNextRawToEvent(evt)){
      anaburst.Process(evt);
    }
    anaburst.Finish();
    anarun.Process(anaburst.GetHists());

    //if(gDebug>0){
      std::string burstrootfn=filename+".root";
      burstrootfn.erase(0,burstrootfn.find_last_of('/')+1);
      std::cout<<burstrootfn<<std::endl;
      anaburst.WriteToFile(burstrootfn);
    //}

  }
  anarun.Finish();
  anarun.WriteToFile(caen::gOutputRootFile);





  return 0;
}
