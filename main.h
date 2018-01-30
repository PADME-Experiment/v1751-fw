#ifndef _caen_analyser_main_h_
#define _caen_analyser_main_h_

#include<string>




namespace PMT_const{
  extern double gain; //*1E5
}


namespace caen{
  extern int gDebug;
  extern std::string gOutputRootFile;
  extern unsigned int gNBursts;
};
#endif
