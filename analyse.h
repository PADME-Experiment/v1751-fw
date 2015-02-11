#ifndef _caen_analyse_h_
#define _caen_analyse_h_

#include"caen-raw.h"

namespace caen{
  class Analyse{
    public:
      void Init();
      void Process(Event& evt);
      void Finish();
  };
};
#endif
