#ifndef TFRSCALIBRPROCESSOR_H
#define TFRSCALIBRPROCESSOR_H

#include "TFRSBasicProc.h"
#include  <iostream>

#include  "TMath.h"
#include  "TRandom3.h"

#include "TFRSCalibrEvent.h"

#include "TFRSParameter.h"

class TFRSCalibrEvent;
class TFRSSortEvent;

class TFRSCalibrProc : public TFRSBasicProc {
public:
  TFRSCalibrProc() ;
  TFRSCalibrProc(const char* name);
  virtual ~TFRSCalibrProc() ;
   
  //void FRSCalibr(TFRSCalibrEvent* tgt);
  // event processing function, default name
  Bool_t BuildEvent(TGo4EventElement* output);
  //Bool_t FillHistograms(TFRSCalibrEvent* event);
      
  void InitProcessor();
  Bool_t bDrawHist;

  Long64_t  check_total_sc41 ;
  Long64_t  check_total_sc21 ;
  Long64_t  check_total_seetram ;
     
protected: 
  Int_t counter;
private:
  void Create_MON_Hist();
  void Create_MW_Hist();
  void Create_TPC_Hist();
  void Create_SI_Hist();
  void Create_CT_Hist();
  void Create_ElCurrent_Hist();
      
  void Process_MON_Analysis(const TFRSSortEvent& src, TFRSCalibrEvent& tgt);
  void Process_MW_Analysis(const TFRSSortEvent& src, TFRSCalibrEvent& tgt);
  void Process_TPC_Analysis(const TFRSSortEvent& src, TFRSCalibrEvent& tgt);
  void Process_SI_Analysis(const TFRSSortEvent& src, TFRSCalibrEvent& tgt);
  void Process_CT_Analysis(const TFRSSortEvent& src, TFRSCalibrEvent& tgt);
  void Process_ElCurrent_Analysis(const TFRSSortEvent& src, TFRSCalibrEvent& tgt);

  TFRSParameter* frs ;
  TMWParameter* mw ;
  TTPCParameter* tpc;
  TSIParameter* si;
  
  // from MON analysis
  Bool_t        fbFirstEvent;
  //unsigned int  scaler_save[64];
  Long64_t         scaler_save[64];
  UInt_t         firstsec;
  UInt_t         firsttenthsec;
  UInt_t         firsthundrethsec;	//mik
  UInt_t         firstcycle;
  UInt_t         firstseetram;
  int scalercycle_Sec;
  int scalercycle_TenthSec;
  int scalercycle_HundrethSec;
  int scalercycle_Cycle;
  int scalercycle_Seetram;

  Float_t       dtime;
  Float_t       dt_last;
  Float_t       dt_diff;
      
  // MON data declarations
      
  TH1I          *hTimeStamp;
  TH1I          *hTimeStampZoom;
  TH1I          *hTimeSpill;
  TH1I          *hMON_All;
  TH1I          *hMON_All2;
  TH1I          *hMON_scaler[64];
  TH1I          *hMON_diff[64];
  TH1I          *hMON_extr[64];
  TH1I          *hMON_seecalA[8];
  TH1I          *hMON_seecalB[8];
  TH1I          *hMON_Pattern;
  TH1I          *hMON_PatternH;

  TH1I          *hSCALER_TIME[64];
  TH1I          *hSCALER_SPILL[64];
  TH1I          *hSCALER_TIME_SHORT[64];
  TH1I          *hSCALER_SPILL_SHORT[64];

  int check_first_event[2];
  Long64_t  scaler_time_count[2]  ; 
  Long64_t  scaler_spill_count[2] ; //UInt_t
  Long64_t  scaler_time_check_last[2] ;//UInt_t
  Long64_t  scaler_spill_check_last[2];//UInt_t 
  Long64_t  check_increase_time[64]   ;//UInt_t 
  Long64_t  check_increase_spill[64]  ;//UInt_t
  Long64_t  scaler_increase_event[64] ;//UInt_t
  Long64_t  scaler_last_event[64] ;

  

  /*
    TH1I          *hMON_TOE;
    TH1I          *hMON_DtDiff;
    TH1I          *hMON_SC01;
    TH1I          *hMON_SC21;
    TH1I          *hMON_SC41;
    TH1I          *hMON_Trigger;
    TH1I          *hMON_LAM;
    TH1I          *hMON_SE01;
    TH1I          *hMON_ICDE;
    TH1I          *hMON_IC1;
    TH1I          *hMON_IC1d;
    TH1I          *hMON_IC2;
    TH1I          *hMON_IC2d;
    TH1I          *hMON_IC3;
    TH1I          *hMON_IC3d;
  */
  // MW data declarations
      
  //**** Raw spectra
  TH1I          *hMW_AN[4];
  TH1I          *hMW_XL[4];  
  TH1I          *hMW_XR[4];
  TH1I          *hMW_YU[4];
  TH1I          *hMW_YD[4];

  //**** Sum spectra
  TH1I          *hMW_XSUM[4];
  TH1I          *hMW_YSUM[4];
      
  //**** Position spectra
  TH1I          *hMW_X[4];   
  TH1I          *hMW_Y[4];
      
  TH2I          *hMW_XY[4];
      
  TH1I          *hMW_SC21x;
  TH1I          *hMW_SC22x;
  TH1I          *hitag_stopper_x;
  TH1I          *hMW_MUSICx;
 
  TH2I          *hMW21x_Time;
      
      
  //**** Focal spectra S2
  TH1I          *hMW_xAngS2;  
  TH1I          *hMW_yAngS2;
  TH1I          *hMW_xFocS2;
  TH1I          *hMW_yFocS2;
  TH1I          *hMW_zxS2;
  TH1I          *hMW_zyS2;
  TH2I          *hMW_FocS2;
 
      
  TGo4WinCond   *cMW_XSUM[4];
  TGo4WinCond   *cMW_YSUM[4];
      

  // TPC part
  // rawdata
  TH1I *hTPC_L0[6];
  TH1I *hTPC_R0[6];
  TH1I *hTPC_LT0[6];
  TH1I *hTPC_RT0[6];
  TH1I *hTPC_L1[6];
  TH1I *hTPC_R1[6];
  TH1I *hTPC_LT1[6];
  TH1I *hTPC_RT1[6];
  // [index][anode_no]
  TH1I *hTPC_DT[6][4];
  TH1I *hTPC_A[6][4];

  /// positions and control sum
  TH1I *hTPC_XRAW0[6];
  TH1I *hTPC_XRAW1[6];
  TH1I *hTPC_YRAW[6];
  TH1I *hTPC_X[6];
  TH1I *hTPC_Y[6];
  TH2I *hcTPC_XY[6];
  TH2I *hTPC_LTRT[6];
  TH1I *hTPC_DELTAX[6];

  // CSUM[index][anode_no]
  TH1I *hTPC_CSUM[6][4];


  TH1I *hTPC_X_S2;            //Positions @ S2 focus
  TH1I *hTPC_Y_S2;
  TH1I *hTPC_AX_S2;           //Angles @ S2 focus
  TH1I *hTPC_AY_S2;
  TH1I *hTPC_X_S4;                //Positions @ S4 focus
  TH1I *hTPC_Y_S4;
  TH1I *hTPC_AX_S4;               //Angles @ S4 focus
  TH1I *hTPC_AY_S4;
  TH1I *hTPC_X_S4_target2;        //Positions @ OTPC
  TH1I *hTPC_Y_S4_target2;
  TH1I *hTPC_X_S2_target1;        //Positions @ first Si tracker in S2
  TH1I *hTPC_Y_S2_target1;
  //      TH1I *hTPC_AX_S4_target2;       //Angles @ Second Target @ S4
  //      TH1I *hTPC_AY_S4_target2;

  TH2I *hTPC_XY_S4_target2;
  TH2I *hTPC_XY_S2_target1;

  //      TH2I *hTPC_XY_S4_target1d;
  //      TH2I *hTPC_XY_S4_target2d;
  //      TH1I *hTPC_X_S4_target1d;
  //      TH1I *hTPC_Y_S4_target1d;
  //      TH1I *hTPC_X_S4_target2d;
  //      TH1I *hTPC_Y_S4_target2d;

  TH2I *hTPC_XAX_S4;
  TH2I *hTPC_XS4_AX_S2;

  TH1I *hTPC_SC21x;
  TH1I *hTPC_SC21y;
  TH1I *hTPC_SC41x;
  TH1I *hTPC_SC41y;
 

  ////condtition for control sum
  TGo4WinCond *cTPC_CSUM[6][4];
  TGo4PolyCond *cTPC_XY[6];

  //**** keep values from previous event
  Float_t       focx_s2m;
  Float_t       focy_s2m;
  Float_t       angle_x_s2m;
  Float_t       angle_y_s2m;


  //Si detector
  TH1I *hsi_e1;
  TH1I *hsi_e2;
  TH1I *hsi_e3;
  TH1I *hsi_e4;

  //Channeltron detector
  TH1I 	*hct_all;
  TH1I 	*hct_trigger_DU;
  TH1I 	*hct_trigger_SY;
  TH1I 	*hct_DU;
  TH1I 	*hct_SY;

  //Electron Current
  TH1I 	*helcurrent;

  ClassDef(TFRSCalibrProc,1)
};

#endif //TFRSCALIBRPROCESSOR_H
