#include<string>
#include<list>


#include "TNtuple.h"
#include "TProfile.h"






#include<TROOT.h>
#include <TApplication.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TBox.h>


#include<TDirectory.h>
#include<TFile.h>
#include<TH1F.h>
#include<TApplication.h>


void plot_same(){
  std::list<std::string> filelist;



//  filelist.push_back("rawdata_2015_03_12_15_46_26.root");

filelist.push_back("rawdata_2015_03_12_15_38_58.root");/*{{{*/
filelist.push_back("rawdata_2015_03_12_15_39_02.root");
filelist.push_back("rawdata_2015_03_12_15_39_06.root");
filelist.push_back("rawdata_2015_03_12_15_39_10.root");
filelist.push_back("rawdata_2015_03_12_15_39_14.root");
filelist.push_back("rawdata_2015_03_12_15_39_19.root");
filelist.push_back("rawdata_2015_03_12_15_39_23.root");
filelist.push_back("rawdata_2015_03_12_15_39_27.root");
filelist.push_back("rawdata_2015_03_12_15_39_31.root");
filelist.push_back("rawdata_2015_03_12_15_39_35.root");
filelist.push_back("rawdata_2015_03_12_15_39_40.root");
filelist.push_back("rawdata_2015_03_12_15_39_44.root");
filelist.push_back("rawdata_2015_03_12_15_39_48.root");
filelist.push_back("rawdata_2015_03_12_15_39_52.root");
filelist.push_back("rawdata_2015_03_12_15_39_56.root");
filelist.push_back("rawdata_2015_03_12_15_40_01.root");
filelist.push_back("rawdata_2015_03_12_15_40_05.root");
filelist.push_back("rawdata_2015_03_12_15_40_09.root");
filelist.push_back("rawdata_2015_03_12_15_40_13.root");
filelist.push_back("rawdata_2015_03_12_15_40_17.root");
filelist.push_back("rawdata_2015_03_12_15_40_22.root");
filelist.push_back("rawdata_2015_03_12_15_40_26.root");
filelist.push_back("rawdata_2015_03_12_15_40_30.root");
filelist.push_back("rawdata_2015_03_12_15_40_34.root");
filelist.push_back("rawdata_2015_03_12_15_40_38.root");
filelist.push_back("rawdata_2015_03_12_15_40_43.root");
filelist.push_back("rawdata_2015_03_12_15_40_47.root");
filelist.push_back("rawdata_2015_03_12_15_40_51.root");
filelist.push_back("rawdata_2015_03_12_15_40_55.root");
filelist.push_back("rawdata_2015_03_12_15_40_59.root");
filelist.push_back("rawdata_2015_03_12_15_41_03.root");
filelist.push_back("rawdata_2015_03_12_15_41_08.root");
filelist.push_back("rawdata_2015_03_12_15_41_12.root");
filelist.push_back("rawdata_2015_03_12_15_41_16.root");
filelist.push_back("rawdata_2015_03_12_15_41_20.root");
filelist.push_back("rawdata_2015_03_12_15_41_24.root");
filelist.push_back("rawdata_2015_03_12_15_41_29.root");
filelist.push_back("rawdata_2015_03_12_15_41_33.root");
filelist.push_back("rawdata_2015_03_12_15_41_37.root");
filelist.push_back("rawdata_2015_03_12_15_41_41.root");
filelist.push_back("rawdata_2015_03_12_15_41_45.root");
filelist.push_back("rawdata_2015_03_12_15_41_50.root");
filelist.push_back("rawdata_2015_03_12_15_41_54.root");
filelist.push_back("rawdata_2015_03_12_15_41_58.root");
filelist.push_back("rawdata_2015_03_12_15_42_02.root");
filelist.push_back("rawdata_2015_03_12_15_42_06.root");
filelist.push_back("rawdata_2015_03_12_15_42_11.root");
filelist.push_back("rawdata_2015_03_12_15_42_15.root");
filelist.push_back("rawdata_2015_03_12_15_42_19.root");
filelist.push_back("rawdata_2015_03_12_15_42_23.root");
filelist.push_back("rawdata_2015_03_12_15_42_27.root");
filelist.push_back("rawdata_2015_03_12_15_42_32.root");
filelist.push_back("rawdata_2015_03_12_15_42_36.root");
filelist.push_back("rawdata_2015_03_12_15_42_40.root");
filelist.push_back("rawdata_2015_03_12_15_42_44.root");
filelist.push_back("rawdata_2015_03_12_15_42_48.root");
filelist.push_back("rawdata_2015_03_12_15_42_53.root");
filelist.push_back("rawdata_2015_03_12_15_42_57.root");
filelist.push_back("rawdata_2015_03_12_15_43_01.root");
filelist.push_back("rawdata_2015_03_12_15_43_05.root");
filelist.push_back("rawdata_2015_03_12_15_43_09.root");
filelist.push_back("rawdata_2015_03_12_15_43_13.root");
filelist.push_back("rawdata_2015_03_12_15_43_18.root");
filelist.push_back("rawdata_2015_03_12_15_43_22.root");
filelist.push_back("rawdata_2015_03_12_15_43_26.root");
filelist.push_back("rawdata_2015_03_12_15_43_30.root");
filelist.push_back("rawdata_2015_03_12_15_43_34.root");
filelist.push_back("rawdata_2015_03_12_15_43_39.root");
filelist.push_back("rawdata_2015_03_12_15_43_43.root");
filelist.push_back("rawdata_2015_03_12_15_43_47.root");
filelist.push_back("rawdata_2015_03_12_15_43_51.root");
filelist.push_back("rawdata_2015_03_12_15_43_55.root");
filelist.push_back("rawdata_2015_03_12_15_44_00.root");
filelist.push_back("rawdata_2015_03_12_15_44_04.root");
filelist.push_back("rawdata_2015_03_12_15_44_08.root");
filelist.push_back("rawdata_2015_03_12_15_44_12.root");
filelist.push_back("rawdata_2015_03_12_15_44_16.root");
filelist.push_back("rawdata_2015_03_12_15_44_21.root");
filelist.push_back("rawdata_2015_03_12_15_44_25.root");
filelist.push_back("rawdata_2015_03_12_15_44_29.root");
filelist.push_back("rawdata_2015_03_12_15_44_33.root");
filelist.push_back("rawdata_2015_03_12_15_44_37.root");
filelist.push_back("rawdata_2015_03_12_15_44_42.root");
filelist.push_back("rawdata_2015_03_12_15_44_46.root");
filelist.push_back("rawdata_2015_03_12_15_44_50.root");
filelist.push_back("rawdata_2015_03_12_15_44_54.root");
filelist.push_back("rawdata_2015_03_12_15_44_58.root");
filelist.push_back("rawdata_2015_03_12_15_45_03.root");
filelist.push_back("rawdata_2015_03_12_15_45_07.root");
filelist.push_back("rawdata_2015_03_12_15_45_11.root");
filelist.push_back("rawdata_2015_03_12_15_45_15.root");
filelist.push_back("rawdata_2015_03_12_15_45_19.root");
filelist.push_back("rawdata_2015_03_12_15_45_24.root");
filelist.push_back("rawdata_2015_03_12_15_45_28.root");
filelist.push_back("rawdata_2015_03_12_15_45_32.root");
filelist.push_back("rawdata_2015_03_12_15_45_36.root");
filelist.push_back("rawdata_2015_03_12_15_45_40.root");
filelist.push_back("rawdata_2015_03_12_15_45_44.root");
filelist.push_back("rawdata_2015_03_12_15_45_49.root");
filelist.push_back("rawdata_2015_03_12_15_45_53.root");
filelist.push_back("rawdata_2015_03_12_15_45_57.root");
filelist.push_back("rawdata_2015_03_12_15_46_01.root");
filelist.push_back("rawdata_2015_03_12_15_46_05.root");
filelist.push_back("rawdata_2015_03_12_15_46_10.root");
filelist.push_back("rawdata_2015_03_12_15_46_14.root");
filelist.push_back("rawdata_2015_03_12_15_46_18.root");
filelist.push_back("rawdata_2015_03_12_15_46_22.root");
filelist.push_back("rawdata_2015_03_12_15_46_26.root");
filelist.push_back("rawdata_2015_03_12_15_46_31.root");
filelist.push_back("rawdata_2015_03_12_15_46_35.root");
filelist.push_back("rawdata_2015_03_12_15_46_39.root");
filelist.push_back("rawdata_2015_03_12_15_46_43.root");
filelist.push_back("rawdata_2015_03_12_15_46_47.root");
filelist.push_back("rawdata_2015_03_12_15_46_52.root");
filelist.push_back("rawdata_2015_03_12_15_46_56.root");
filelist.push_back("rawdata_2015_03_12_15_47_00.root");
filelist.push_back("rawdata_2015_03_12_15_47_04.root");
filelist.push_back("rawdata_2015_03_12_15_47_08.root");
filelist.push_back("rawdata_2015_03_12_15_47_13.root");
filelist.push_back("rawdata_2015_03_12_15_47_17.root");
filelist.push_back("rawdata_2015_03_12_15_47_21.root");
filelist.push_back("rawdata_2015_03_12_15_47_25.root");
filelist.push_back("rawdata_2015_03_12_15_47_29.root");
filelist.push_back("rawdata_2015_03_12_15_47_34.root");
filelist.push_back("rawdata_2015_03_12_15_47_38.root");
filelist.push_back("rawdata_2015_03_12_15_47_42.root");
filelist.push_back("rawdata_2015_03_12_15_47_46.root");
filelist.push_back("rawdata_2015_03_12_15_47_50.root");
filelist.push_back("rawdata_2015_03_12_15_47_54.root");
filelist.push_back("rawdata_2015_03_12_15_47_59.root");
filelist.push_back("rawdata_2015_03_12_15_48_03.root");
filelist.push_back("rawdata_2015_03_12_15_48_07.root");
filelist.push_back("rawdata_2015_03_12_15_48_11.root");
filelist.push_back("rawdata_2015_03_12_15_48_15.root");
filelist.push_back("rawdata_2015_03_12_15_48_20.root");
filelist.push_back("rawdata_2015_03_12_15_48_24.root");
filelist.push_back("rawdata_2015_03_12_15_48_28.root");
filelist.push_back("rawdata_2015_03_12_15_48_32.root");
filelist.push_back("rawdata_2015_03_12_15_48_36.root");
filelist.push_back("rawdata_2015_03_12_15_48_41.root");
filelist.push_back("rawdata_2015_03_12_15_48_45.root");
filelist.push_back("rawdata_2015_03_12_15_48_49.root");
filelist.push_back("rawdata_2015_03_12_15_48_53.root");
filelist.push_back("rawdata_2015_03_12_15_48_57.root");
filelist.push_back("rawdata_2015_03_12_15_49_02.root");
filelist.push_back("rawdata_2015_03_12_15_49_06.root");
filelist.push_back("rawdata_2015_03_12_15_49_10.root");
filelist.push_back("rawdata_2015_03_12_15_49_14.root");
filelist.push_back("rawdata_2015_03_12_15_49_18.root");
filelist.push_back("rawdata_2015_03_12_15_49_23.root");
filelist.push_back("rawdata_2015_03_12_15_49_27.root");
filelist.push_back("rawdata_2015_03_12_15_49_31.root");
filelist.push_back("rawdata_2015_03_12_15_49_35.root");
filelist.push_back("rawdata_2015_03_12_15_49_39.root");
filelist.push_back("rawdata_2015_03_12_15_49_44.root");
filelist.push_back("rawdata_2015_03_12_15_49_48.root");
filelist.push_back("rawdata_2015_03_12_15_49_52.root");
filelist.push_back("rawdata_2015_03_12_15_49_56.root");
filelist.push_back("rawdata_2015_03_12_15_50_00.root");
filelist.push_back("rawdata_2015_03_12_15_50_04.root");
filelist.push_back("rawdata_2015_03_12_15_50_09.root");
filelist.push_back("rawdata_2015_03_12_15_50_13.root");
filelist.push_back("rawdata_2015_03_12_15_50_17.root");
filelist.push_back("rawdata_2015_03_12_15_50_21.root");
filelist.push_back("rawdata_2015_03_12_15_50_25.root");
filelist.push_back("rawdata_2015_03_12_15_50_30.root");
filelist.push_back("rawdata_2015_03_12_15_50_34.root");
filelist.push_back("rawdata_2015_03_12_15_50_38.root");
filelist.push_back("rawdata_2015_03_12_15_50_42.root");
filelist.push_back("rawdata_2015_03_12_15_50_46.root");
filelist.push_back("rawdata_2015_03_12_15_50_51.root");
filelist.push_back("rawdata_2015_03_12_15_50_55.root");
filelist.push_back("rawdata_2015_03_12_15_50_59.root");
filelist.push_back("rawdata_2015_03_12_15_51_03.root");
filelist.push_back("rawdata_2015_03_12_15_51_07.root");
filelist.push_back("rawdata_2015_03_12_15_51_12.root");
filelist.push_back("rawdata_2015_03_12_15_51_16.root");
filelist.push_back("rawdata_2015_03_12_15_51_20.root");
filelist.push_back("rawdata_2015_03_12_15_51_24.root");
filelist.push_back("rawdata_2015_03_12_15_51_28.root");
filelist.push_back("rawdata_2015_03_12_15_51_33.root");
filelist.push_back("rawdata_2015_03_12_15_51_37.root");
filelist.push_back("rawdata_2015_03_12_15_51_41.root");
filelist.push_back("rawdata_2015_03_12_15_51_45.root");
filelist.push_back("rawdata_2015_03_12_15_51_49.root");
filelist.push_back("rawdata_2015_03_12_15_51_54.root");
filelist.push_back("rawdata_2015_03_12_15_51_58.root");
filelist.push_back("rawdata_2015_03_12_15_52_02.root");
filelist.push_back("rawdata_2015_03_12_15_52_06.root");
filelist.push_back("rawdata_2015_03_12_15_52_10.root");
filelist.push_back("rawdata_2015_03_12_15_52_14.root");
filelist.push_back("rawdata_2015_03_12_15_52_19.root");
filelist.push_back("rawdata_2015_03_12_15_52_23.root");
filelist.push_back("rawdata_2015_03_12_15_52_27.root");
filelist.push_back("rawdata_2015_03_12_15_52_31.root");
filelist.push_back("rawdata_2015_03_12_15_52_35.root");
filelist.push_back("rawdata_2015_03_12_15_52_40.root");
filelist.push_back("rawdata_2015_03_12_15_52_44.root");
filelist.push_back("rawdata_2015_03_12_15_52_48.root");
filelist.push_back("rawdata_2015_03_12_15_52_52.root");
filelist.push_back("rawdata_2015_03_12_15_52_56.root");
filelist.push_back("rawdata_2015_03_12_15_53_01.root");
filelist.push_back("rawdata_2015_03_12_15_53_05.root");
filelist.push_back("rawdata_2015_03_12_15_53_09.root");
filelist.push_back("rawdata_2015_03_12_15_53_13.root");
filelist.push_back("rawdata_2015_03_12_15_53_17.root");
filelist.push_back("rawdata_2015_03_12_15_53_22.root");
filelist.push_back("rawdata_2015_03_12_15_53_26.root");
filelist.push_back("rawdata_2015_03_12_15_53_30.root");
filelist.push_back("rawdata_2015_03_12_15_53_34.root");
filelist.push_back("rawdata_2015_03_12_15_53_38.root");
filelist.push_back("rawdata_2015_03_12_15_53_43.root");
filelist.push_back("rawdata_2015_03_12_15_53_47.root");
filelist.push_back("rawdata_2015_03_12_15_53_51.root");
filelist.push_back("rawdata_2015_03_12_15_53_55.root");
filelist.push_back("rawdata_2015_03_12_15_53_59.root");
filelist.push_back("rawdata_2015_03_12_15_54_04.root");
filelist.push_back("rawdata_2015_03_12_15_54_08.root");
filelist.push_back("rawdata_2015_03_12_15_54_12.root");
filelist.push_back("rawdata_2015_03_12_15_54_16.root");
filelist.push_back("rawdata_2015_03_12_15_54_20.root");
filelist.push_back("rawdata_2015_03_12_15_54_24.root");
filelist.push_back("rawdata_2015_03_12_15_54_29.root");
filelist.push_back("rawdata_2015_03_12_15_54_33.root");
filelist.push_back("rawdata_2015_03_12_15_54_37.root");
filelist.push_back("rawdata_2015_03_12_15_54_41.root");
filelist.push_back("rawdata_2015_03_12_15_54_45.root");
filelist.push_back("rawdata_2015_03_12_15_54_50.root");
filelist.push_back("rawdata_2015_03_12_15_54_54.root");
filelist.push_back("rawdata_2015_03_12_15_54_58.root");
filelist.push_back("rawdata_2015_03_12_15_55_02.root");
filelist.push_back("rawdata_2015_03_12_15_55_06.root");
filelist.push_back("rawdata_2015_03_12_15_55_11.root");
filelist.push_back("rawdata_2015_03_12_15_55_15.root");
filelist.push_back("rawdata_2015_03_12_15_55_19.root");
filelist.push_back("rawdata_2015_03_12_15_55_23.root");
filelist.push_back("rawdata_2015_03_12_15_55_27.root");
filelist.push_back("rawdata_2015_03_12_15_55_32.root");
filelist.push_back("rawdata_2015_03_12_15_55_36.root");
filelist.push_back("rawdata_2015_03_12_15_55_40.root");
filelist.push_back("rawdata_2015_03_12_15_55_44.root");
filelist.push_back("rawdata_2015_03_12_15_55_48.root");
filelist.push_back("rawdata_2015_03_12_15_55_53.root");
filelist.push_back("rawdata_2015_03_12_15_55_57.root");
filelist.push_back("rawdata_2015_03_12_15_56_01.root");
filelist.push_back("rawdata_2015_03_12_15_56_05.root");
filelist.push_back("rawdata_2015_03_12_15_56_09.root");
filelist.push_back("rawdata_2015_03_12_15_56_14.root");
filelist.push_back("rawdata_2015_03_12_15_56_18.root");
filelist.push_back("rawdata_2015_03_12_15_56_22.root");
filelist.push_back("rawdata_2015_03_12_15_56_26.root");
filelist.push_back("rawdata_2015_03_12_15_56_30.root");
filelist.push_back("rawdata_2015_03_12_15_56_34.root");
filelist.push_back("rawdata_2015_03_12_15_56_39.root");
filelist.push_back("rawdata_2015_03_12_15_56_43.root");
filelist.push_back("rawdata_2015_03_12_15_56_47.root");
filelist.push_back("rawdata_2015_03_12_15_56_51.root");
filelist.push_back("rawdata_2015_03_12_15_56_55.root");
filelist.push_back("rawdata_2015_03_12_15_57_00.root");
filelist.push_back("rawdata_2015_03_12_15_57_04.root");
filelist.push_back("rawdata_2015_03_12_15_57_08.root");
filelist.push_back("rawdata_2015_03_12_15_57_12.root");
filelist.push_back("rawdata_2015_03_12_15_57_16.root");
filelist.push_back("rawdata_2015_03_12_15_57_21.root");
filelist.push_back("rawdata_2015_03_12_15_57_25.root");
filelist.push_back("rawdata_2015_03_12_15_57_29.root");
filelist.push_back("rawdata_2015_03_12_15_57_33.root");
filelist.push_back("rawdata_2015_03_12_15_57_37.root");
filelist.push_back("rawdata_2015_03_12_15_57_42.root");
filelist.push_back("rawdata_2015_03_12_15_57_46.root");
filelist.push_back("rawdata_2015_03_12_15_57_50.root");
filelist.push_back("rawdata_2015_03_12_15_57_54.root");
filelist.push_back("rawdata_2015_03_12_15_57_58.root");
filelist.push_back("rawdata_2015_03_12_15_58_03.root");
filelist.push_back("rawdata_2015_03_12_15_58_07.root");
filelist.push_back("rawdata_2015_03_12_15_58_11.root");
filelist.push_back("rawdata_2015_03_12_15_58_15.root");
filelist.push_back("rawdata_2015_03_12_15_58_19.root");
filelist.push_back("rawdata_2015_03_12_15_58_24.root");
filelist.push_back("rawdata_2015_03_12_15_58_28.root");
filelist.push_back("rawdata_2015_03_12_15_58_32.root");
filelist.push_back("rawdata_2015_03_12_15_58_36.root");
filelist.push_back("rawdata_2015_03_12_15_58_40.root");
filelist.push_back("rawdata_2015_03_12_15_58_44.root");
filelist.push_back("rawdata_2015_03_12_15_58_49.root");
filelist.push_back("rawdata_2015_03_12_15_58_53.root");
filelist.push_back("rawdata_2015_03_12_15_58_57.root");
filelist.push_back("rawdata_2015_03_12_15_59_01.root");
filelist.push_back("rawdata_2015_03_12_15_59_05.root");
filelist.push_back("rawdata_2015_03_12_15_59_10.root");
filelist.push_back("rawdata_2015_03_12_15_59_14.root");
filelist.push_back("rawdata_2015_03_12_15_59_18.root");
filelist.push_back("rawdata_2015_03_12_15_59_22.root");
filelist.push_back("rawdata_2015_03_12_15_59_26.root");
filelist.push_back("rawdata_2015_03_12_15_59_31.root");
filelist.push_back("rawdata_2015_03_12_15_59_35.root");
filelist.push_back("rawdata_2015_03_12_15_59_39.root");
filelist.push_back("rawdata_2015_03_12_15_59_43.root");
filelist.push_back("rawdata_2015_03_12_15_59_47.root");
filelist.push_back("rawdata_2015_03_12_15_59_52.root");
filelist.push_back("rawdata_2015_03_12_15_59_56.root");
filelist.push_back("rawdata_2015_03_12_16_00_00.root");
filelist.push_back("rawdata_2015_03_12_16_00_04.root");
filelist.push_back("rawdata_2015_03_12_16_00_08.root");
filelist.push_back("rawdata_2015_03_12_16_00_13.root");
filelist.push_back("rawdata_2015_03_12_16_00_17.root");
filelist.push_back("rawdata_2015_03_12_16_00_21.root");
filelist.push_back("rawdata_2015_03_12_16_00_25.root");
filelist.push_back("rawdata_2015_03_12_16_00_29.root");
filelist.push_back("rawdata_2015_03_12_16_00_34.root");
filelist.push_back("rawdata_2015_03_12_16_00_38.root");
filelist.push_back("rawdata_2015_03_12_16_00_42.root");
filelist.push_back("rawdata_2015_03_12_16_00_46.root");
filelist.push_back("rawdata_2015_03_12_16_00_50.root");
filelist.push_back("rawdata_2015_03_12_16_00_54.root");
filelist.push_back("rawdata_2015_03_12_16_00_59.root");
filelist.push_back("rawdata_2015_03_12_16_01_03.root");
filelist.push_back("rawdata_2015_03_12_16_01_07.root");
filelist.push_back("rawdata_2015_03_12_16_01_11.root");
filelist.push_back("rawdata_2015_03_12_16_01_15.root");
filelist.push_back("rawdata_2015_03_12_16_01_20.root");
filelist.push_back("rawdata_2015_03_12_16_01_24.root");
filelist.push_back("rawdata_2015_03_12_16_01_28.root");
filelist.push_back("rawdata_2015_03_12_16_01_32.root");
filelist.push_back("rawdata_2015_03_12_16_01_36.root");
filelist.push_back("rawdata_2015_03_12_16_01_41.root");
filelist.push_back("rawdata_2015_03_12_16_01_45.root");
filelist.push_back("rawdata_2015_03_12_16_01_49.root");
filelist.push_back("rawdata_2015_03_12_16_01_53.root");
filelist.push_back("rawdata_2015_03_12_16_01_57.root");
filelist.push_back("rawdata_2015_03_12_16_02_02.root");
filelist.push_back("rawdata_2015_03_12_16_02_06.root");
filelist.push_back("rawdata_2015_03_12_16_02_10.root");
filelist.push_back("rawdata_2015_03_12_16_02_14.root");
filelist.push_back("rawdata_2015_03_12_16_02_18.root");
filelist.push_back("rawdata_2015_03_12_16_02_23.root");
filelist.push_back("rawdata_2015_03_12_16_02_27.root");
filelist.push_back("rawdata_2015_03_12_16_02_31.root");
filelist.push_back("rawdata_2015_03_12_16_02_35.root");
filelist.push_back("rawdata_2015_03_12_16_02_39.root");
filelist.push_back("rawdata_2015_03_12_16_02_44.root");
filelist.push_back("rawdata_2015_03_12_16_02_48.root");
filelist.push_back("rawdata_2015_03_12_16_02_52.root");
filelist.push_back("rawdata_2015_03_12_16_02_56.root");
filelist.push_back("rawdata_2015_03_12_16_03_00.root");
filelist.push_back("rawdata_2015_03_12_16_03_05.root");
filelist.push_back("rawdata_2015_03_12_16_03_09.root");
filelist.push_back("rawdata_2015_03_12_16_03_13.root");
filelist.push_back("rawdata_2015_03_12_16_03_17.root");
filelist.push_back("rawdata_2015_03_12_16_03_21.root");
filelist.push_back("rawdata_2015_03_12_16_03_25.root");
filelist.push_back("rawdata_2015_03_12_16_03_30.root");
filelist.push_back("rawdata_2015_03_12_16_03_34.root");
filelist.push_back("rawdata_2015_03_12_16_03_38.root");
filelist.push_back("rawdata_2015_03_12_16_03_42.root");
filelist.push_back("rawdata_2015_03_12_16_03_46.root");
filelist.push_back("rawdata_2015_03_12_16_03_51.root");
filelist.push_back("rawdata_2015_03_12_16_03_55.root");
filelist.push_back("rawdata_2015_03_12_16_03_59.root");
filelist.push_back("rawdata_2015_03_12_16_04_03.root");
filelist.push_back("rawdata_2015_03_12_16_04_07.root");
filelist.push_back("rawdata_2015_03_12_16_04_12.root");
filelist.push_back("rawdata_2015_03_12_16_04_16.root");
filelist.push_back("rawdata_2015_03_12_16_04_20.root");
filelist.push_back("rawdata_2015_03_12_16_04_24.root");
filelist.push_back("rawdata_2015_03_12_16_04_28.root");
filelist.push_back("rawdata_2015_03_12_16_04_33.root");
filelist.push_back("rawdata_2015_03_12_16_04_37.root");
filelist.push_back("rawdata_2015_03_12_16_04_41.root");
filelist.push_back("rawdata_2015_03_12_16_04_45.root");
filelist.push_back("rawdata_2015_03_12_16_04_49.root");
filelist.push_back("rawdata_2015_03_12_16_04_54.root");
filelist.push_back("rawdata_2015_03_12_16_04_58.root");
filelist.push_back("rawdata_2015_03_12_16_05_02.root");
filelist.push_back("rawdata_2015_03_12_16_05_06.root");
filelist.push_back("rawdata_2015_03_12_16_05_10.root");
filelist.push_back("rawdata_2015_03_12_16_05_15.root");
filelist.push_back("rawdata_2015_03_12_16_05_19.root");
filelist.push_back("rawdata_2015_03_12_16_05_23.root");
filelist.push_back("rawdata_2015_03_12_16_05_27.root");
filelist.push_back("rawdata_2015_03_12_16_05_31.root");
filelist.push_back("rawdata_2015_03_12_16_05_35.root");
filelist.push_back("rawdata_2015_03_12_16_05_40.root");
filelist.push_back("rawdata_2015_03_12_16_05_44.root");
filelist.push_back("rawdata_2015_03_12_16_05_48.root");
filelist.push_back("rawdata_2015_03_12_16_05_52.root");
filelist.push_back("rawdata_2015_03_12_16_05_56.root");
filelist.push_back("rawdata_2015_03_12_16_06_01.root");
filelist.push_back("rawdata_2015_03_12_16_06_05.root");
filelist.push_back("rawdata_2015_03_12_16_06_09.root");
filelist.push_back("rawdata_2015_03_12_16_06_13.root");
filelist.push_back("rawdata_2015_03_12_16_06_17.root");
filelist.push_back("rawdata_2015_03_12_16_06_22.root");
filelist.push_back("rawdata_2015_03_12_16_06_26.root");
filelist.push_back("rawdata_2015_03_12_16_06_30.root");
filelist.push_back("rawdata_2015_03_12_16_06_34.root");
filelist.push_back("rawdata_2015_03_12_16_06_38.root");
filelist.push_back("rawdata_2015_03_12_16_06_43.root");
filelist.push_back("rawdata_2015_03_12_16_06_47.root");
filelist.push_back("rawdata_2015_03_12_16_06_51.root");
filelist.push_back("rawdata_2015_03_12_16_06_55.root");
filelist.push_back("rawdata_2015_03_12_16_06_59.root");
filelist.push_back("rawdata_2015_03_12_16_07_04.root");
filelist.push_back("rawdata_2015_03_12_16_07_08.root");
filelist.push_back("rawdata_2015_03_12_16_07_12.root");
filelist.push_back("rawdata_2015_03_12_16_07_16.root");
filelist.push_back("rawdata_2015_03_12_16_07_20.root");
filelist.push_back("rawdata_2015_03_12_16_07_25.root");
filelist.push_back("rawdata_2015_03_12_16_07_29.root");
filelist.push_back("rawdata_2015_03_12_16_07_33.root");
filelist.push_back("rawdata_2015_03_12_16_07_37.root");
filelist.push_back("rawdata_2015_03_12_16_07_41.root");
filelist.push_back("rawdata_2015_03_12_16_07_45.root");
filelist.push_back("rawdata_2015_03_12_16_07_50.root");
filelist.push_back("rawdata_2015_03_12_16_07_54.root");
filelist.push_back("rawdata_2015_03_12_16_07_58.root");
filelist.push_back("rawdata_2015_03_12_16_08_02.root");
filelist.push_back("rawdata_2015_03_12_16_08_06.root");
filelist.push_back("rawdata_2015_03_12_16_08_11.root");
filelist.push_back("rawdata_2015_03_12_16_08_15.root");
filelist.push_back("rawdata_2015_03_12_16_08_19.root");
filelist.push_back("rawdata_2015_03_12_16_08_23.root");
filelist.push_back("rawdata_2015_03_12_16_08_27.root");
filelist.push_back("rawdata_2015_03_12_16_08_32.root");
filelist.push_back("rawdata_2015_03_12_16_08_36.root");
filelist.push_back("rawdata_2015_03_12_16_08_40.root");
filelist.push_back("rawdata_2015_03_12_16_08_44.root");
filelist.push_back("rawdata_2015_03_12_16_08_48.root");
filelist.push_back("rawdata_2015_03_12_16_08_53.root");
filelist.push_back("rawdata_2015_03_12_16_08_57.root");
filelist.push_back("rawdata_2015_03_12_16_09_01.root");
filelist.push_back("rawdata_2015_03_12_16_09_05.root");
filelist.push_back("rawdata_2015_03_12_16_09_09.root");
filelist.push_back("rawdata_2015_03_12_16_09_14.root");
filelist.push_back("rawdata_2015_03_12_16_09_18.root");
filelist.push_back("rawdata_2015_03_12_16_09_22.root");
filelist.push_back("rawdata_2015_03_12_16_09_26.root");
filelist.push_back("rawdata_2015_03_12_16_09_30.root");
filelist.push_back("rawdata_2015_03_12_16_09_35.root");
filelist.push_back("rawdata_2015_03_12_16_09_39.root");
filelist.push_back("rawdata_2015_03_12_16_09_43.root");
filelist.push_back("rawdata_2015_03_12_16_09_47.root");
filelist.push_back("rawdata_2015_03_12_16_09_51.root");
filelist.push_back("rawdata_2015_03_12_16_09_55.root");
filelist.push_back("rawdata_2015_03_12_16_10_00.root");
filelist.push_back("rawdata_2015_03_12_16_10_04.root");
filelist.push_back("rawdata_2015_03_12_16_10_08.root");
filelist.push_back("rawdata_2015_03_12_16_10_12.root");
filelist.push_back("rawdata_2015_03_12_16_10_16.root");
filelist.push_back("rawdata_2015_03_12_16_10_21.root");
filelist.push_back("rawdata_2015_03_12_16_10_25.root");
filelist.push_back("rawdata_2015_03_12_16_10_29.root");
filelist.push_back("rawdata_2015_03_12_16_10_33.root");
filelist.push_back("rawdata_2015_03_12_16_10_37.root");
filelist.push_back("rawdata_2015_03_12_16_10_42.root");
filelist.push_back("rawdata_2015_03_12_16_10_46.root");
filelist.push_back("rawdata_2015_03_12_16_10_50.root");
filelist.push_back("rawdata_2015_03_12_16_10_54.root");
filelist.push_back("rawdata_2015_03_12_16_10_58.root");
filelist.push_back("rawdata_2015_03_12_16_11_03.root");
filelist.push_back("rawdata_2015_03_12_16_11_07.root");
filelist.push_back("rawdata_2015_03_12_16_11_11.root");
filelist.push_back("rawdata_2015_03_12_16_11_15.root");
filelist.push_back("rawdata_2015_03_12_16_11_19.root");
filelist.push_back("rawdata_2015_03_12_16_11_24.root");
filelist.push_back("rawdata_2015_03_12_16_11_28.root");
filelist.push_back("rawdata_2015_03_12_16_11_32.root");
filelist.push_back("rawdata_2015_03_12_16_11_36.root");
filelist.push_back("rawdata_2015_03_12_16_11_40.root");
filelist.push_back("rawdata_2015_03_12_16_11_45.root");
filelist.push_back("rawdata_2015_03_12_16_11_49.root");
filelist.push_back("rawdata_2015_03_12_16_11_53.root");
filelist.push_back("rawdata_2015_03_12_16_11_57.root");
filelist.push_back("rawdata_2015_03_12_16_12_01.root");
filelist.push_back("rawdata_2015_03_12_16_12_05.root");
filelist.push_back("rawdata_2015_03_12_16_12_10.root");
filelist.push_back("rawdata_2015_03_12_16_12_14.root");
filelist.push_back("rawdata_2015_03_12_16_12_18.root");
filelist.push_back("rawdata_2015_03_12_16_12_22.root");
/*}}}*/






  std::list<TH1F*> hist_gausA;
  std::list<TH1F*> hist_gausX;
  std::list<TH1F*> hist_gausW;
  for(std::list<std::string>::iterator file_it=filelist.begin();file_it!=filelist.end();++file_it){
    TFile* file=new TFile(file_it->c_str(),"read");
    if((TH1F*)file->FindObjectAny("gausAmplitude_photoElectrons_ch_1"))
      hist_gausA.push_back((TH1F*)file->FindObjectAny("gausAmplitude_photoElectrons_ch_1"));
    if((TH1F*)file->FindObjectAny("gausSigma_photoElectrons_ch_1"))
      hist_gausX.push_back((TH1F*)file->FindObjectAny("gausSigma_photoElectrons_ch_1"));
    if((TH1F*)file->FindObjectAny("gausMean_photoElectrons_ch_1"))
      hist_gausW.push_back((TH1F*)file->FindObjectAny("gausMean_photoElectrons_ch_1"));
  }




//TROOT root("plot","plot");


//TApplication tapp("App");
int nopt=0;
char *opts[20];
TApplication tapp("asd",&nopt,opts);

  TCanvas *c1 = new TCanvas("c1","",0,0,800,600);
  TCanvas *c2 = new TCanvas("c2","",0,0,800,600);
  TCanvas *c3 = new TCanvas("c3","",0,0,800,600);

  c1->cd();
  for(std::list<TH1F*>::iterator hist_it=hist_gausA.begin();hist_it!=hist_gausA.end();++hist_it){
    (*hist_it)->Draw("same");
  }
  c1->Update();

  c2->cd();
  for(std::list<TH1F*>::iterator hist_it=hist_gausX.begin();hist_it!=hist_gausX.end();++hist_it){
    (*hist_it)->Draw("same");
  }
  c2->Update();

  c3->cd();
  for(std::list<TH1F*>::iterator hist_it=hist_gausW.begin();hist_it!=hist_gausW.end();++hist_it){
    (*hist_it)->Draw("same");
  }
  c3->Update();

tapp.Run();




}


int main(){
  plot_same();
}
