#ifndef TFRSANLPROC_H
#define TFRSANLPROC_H

#include "TFRSBasicProc.h"
#include "TFRSParameter.h"

class TFRSAnlEvent;
class TFRSSortEvent;
class TFRSCalibrEvent;

class TFRSUnpackEvent;

class TFRSAnlProc : public TFRSBasicProc {
public:
  TFRSAnlProc();
  TFRSAnlProc(const char* name);
  //void FRSEventAnalysis(TFRSAnlEvent* target);
  Bool_t BuildEvent(TGo4EventElement* output);

  virtual ~TFRSAnlProc() ;

  Bool_t bDrawHist;

private:

  void Create_MUSIC_Hist();
  void Create_SCI_Hist();
  void Create_ID_Hist();
  void Create_MRTOF_Hist();
  // void Create_SI_Hist();
  // void Create_CT_Hist();

  void Procceed_MUSIC_Analysis(TFRSSortEvent& srt, TFRSCalibrEvent& clb, TFRSAnlEvent& tgt);  
  void Procceed_SCI_Analysis(TFRSSortEvent& srt, TFRSCalibrEvent& clb, TFRSAnlEvent& tgt);
  void Procceed_ID_Analysis(TFRSSortEvent& srt, TFRSCalibrEvent&  clb, TFRSAnlEvent& tgt);
  void Procceed_MRTOF_Analysis(TFRSSortEvent& srt, TFRSCalibrEvent& clb, TFRSAnlEvent& tgt);       

  TMUSICParameter* music;
  TSCIParameter* sci;
  TFRSParameter* frs;
  TIDParameter* id;
  TMRTOFMSParameter* mrtof;

  //MRTOF data

  TH1F* h_MRtof_Start;
  TH1F* h_MRtof_StopDelay;
  TH1F* h_MRtof_Stop;
  TH1F* h_MRtof_tof;
  TH1F* h_MRtof_status;

  
  // MUSIC data

  TH1I          *hMUSIC1_E[8];
  TH1I          *hMUSIC1_dE;

  TH1I          *hMUSIC2_E[8];
  TH1I          *hMUSIC2_dE;

  TH2I          *hMUSIC1_dE1dE2;
  
  TH1I          *hMUSIC3_E[4];
  TH1I          *hMUSIC3_z_AoQ_E[4][5];
  TH1I          *hMUSIC3_music_z_E[4][5];
  TH1I          *hMUSIC3_x2AoQ_E[4][6];
  TH1I          *hMUSIC3_T[4];
  TH1I          *hMUSIC3_dE;
  TH1I          *hMUSIC_pres[2];
  TH2I          *hMUSIC3_dEx;
  TH1I          *hMUSIC3_dECOR;
  TH2I          *hMUSIC3_dExc;
  TH2I          *h_dEdx_betagammaAll;
  TH2I          *h_dEdx_betagammaAllZoom;
  TH2I          *h_dEdx_betagamma[4];
  TH2I          *h_DiffdEdx_betagamma[4];
  
  TGo4WinCond   *cMusic1_E[8];
  TGo4WinCond   *cMusic2_E[8];
  TGo4WinCond   *cMusic3_T[4];
  TGo4WinCond   *cMusic3_E[4];
  TGo4WinCond   *cMusic3_dec;


  // SCI data

  TH1I          *hSCI_L[12];
  TH1I          *hSCI_R[12];
  TH1I          *hSCI_E[12];
  TH1I          *hSCI_Tx[12];
  TH1I          *hSCI_X[12];


  TH1I          *hSCI_TofLL2;
  TH1I          *hSCI_TofRR2;
  TH1I          *hSCI_Tof2;

  TH2I          *hSCI_dETof2;
  TH2I          *hSCI_dE24;

  TH2I          *hSCI_dEx2;
  TH2I          *hSCI_dEx2del;
  TH2I          *hSCI_dEx5;


  TGo4WinCond   *cSCI_L[12];
  TGo4WinCond   *cSCI_R[12];
  TGo4WinCond   *cSCI_E[12];
  TGo4WinCond   *cSCI_Tx[12];
  TGo4WinCond   *cSCI_X[12];
  TGo4WinCond   *cSCI_TofLL2;
  TGo4WinCond   *cSCI_TofRR2;
  TGo4PolyCond  *cSCI_detof; 

  // ID data
      
  TH2I          *hID_dEToF;
  TH1I          *hID_BRho[2];
  TH1I          *hID_beta;
  TH1I          *hID_beta3;
  TH1I          *hID_AoQ; 
  TH1I          *hID_AoQ_corr; 
  TH1I          *hID_AoQ_corr2; 
  TH1I          *hID_Z; 
  TH1I          *hID_ZmaxRange;
  TH1I          *hID_Z2;
  TH1I          *hID_Z3;
  TH2I          *hID_DeltaBrho_AoQ;
  TH2I          *hID_DeltaBrho_AoQzoom;
  TH2I          *hID_x2AoQ;
  TH2I          *hID_Z_AoQ;
  TH2I		*hID_Z_AoQ_zsame;
  TH2I          *hID_Z_AoQ_corr;
  TH2I          *hID_Z_Sc21E;
  TH2I          *hID_Z_AoQ_corrZoom;
  TH2I          *hID_Z2_AoQ;
  TH2I          *hID_Z3_AoQ;
  TH2I          *hID_Z_Z2;
  TH2I          *hID_Z_Z3;
  TH2I          *hID_Z_dE2;
  TH1I          *hID_x_target1c[5];
  TH1I          *hID_x_target2c[5];
  TH1I          *hID_betac[5];
  TH1I          *hID_brhoc[5];
  TH1I          *hID_y_target1c[5];
  TH1I          *hID_y_target2c[5];
  TH1I          *hID_x4c[5];
  TH1I          *hID_x2c[5];
  TH1I          *hID_target2c[5];
  TH2I          *hID_Z_AoQgate[5];
  TH2I          *hID_x4AoQ_x2AoQgate[6];
  TH2I          *hID_ZAoQ_x2AoQgate[6];
  TH2I          *hID_x4AoQ;
  TH2I          *hID_x4AoQ_zgate;
  TH2I          *hID_x4z;
  TH1I          *hID_Z_Q;
  TH2I          *hID_Sc21_Tx_1Hz;

  TH2I          *hID_x2x4;
  
  TH2I          *hID_xz;
  TH2I          *hID_yz;
  TH2I          *hID_xzc;
  TH2I          *hID_yzc;

  TH2I          *hITAG_dE_TOF;
  TH2I          *hITAG_dE_Brho;
  TH2I          *hITAG_dE_pos;
  TH2I          *hITAG_dE_mus_dE_42;
  TH2I          *hITAG_dE_mus_dE_43;
      
  TGo4WinCond   *cID_x2;
  TGo4WinCond   *cID_x4;
  TGo4PolyCond  *cID_dEToF;
  TGo4PolyCond  *cID_x2AoQ[6];
  TGo4PolyCond  *cID_Z_AoQ[5];
  TGo4PolyCond  *cID_x4AoQ_Z[5]; 
  TGo4WinCond   *cID_Z_Z[5]; 
     


  ClassDef(TFRSAnlProc,1)
};

#endif //--------------- TFRSANLPROC_H
