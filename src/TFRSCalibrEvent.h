#ifndef TFRSCALIBREVENT_H
#define TFRSCALIBREVENT_H

#include "Go4Event/TGo4EventElement.h"


class TFRSCalibrProc;
class TGo4FileSource;

class TFRSCalibrEvent : public TGo4EventElement {
public:
  TFRSCalibrEvent();
  TFRSCalibrEvent(const char* name);
  virtual ~TFRSCalibrEvent();
  /**
   * Method called by the event owner (analysis step) to fill the
   * event element from the set event source. Event source can
   * be the source of the analysis step (if this is a raw event) 
   * or the event processor (if this is a reduced event).
   */
  //virtual Int_t Fill();

  /**
   * Method called by the event owner (analysis step) to clear the
   * event element.
   */
  virtual void Clear(Option_t *t="");  
      
  virtual Int_t Init();  

  Int_t EventFlag;
  //Scalers
  UInt_t freeTrig;
  UInt_t acptTrig;
  Long64_t mon_inc[64];

  // MON part
  Int_t         seconds;
  Int_t         tenthsecs;
  Int_t         hundrethsecs;	//mik
  Int_t         extraction_cycle;
  Int_t         coin[16];
  Float_t       seetram;
      
  // MW part
  Float_t       mw_xsum[13];     /*                          */
  Float_t       mw_ysum[13];     /*                          */
      
  Float_t       mw_x[13];        /*                          */
  Float_t       mw_y[13];        /*                          */
  Float_t       mw_wire[13];     /* special for Helmut       */

  Float_t       z_x_s2;          
  Float_t       z_y_s2;          
  Float_t       z_x_s4;          
  Float_t       z_y_s4;          
  Float_t       z_x_s8;          
  Float_t       z_y_s8;          
 
  Bool_t        b_mw_xsum[13];   /*  wc on sum               */
  Bool_t        b_mw_ysum[13];   /*                          */

  Float_t       focx_s2;         /*  FRS foci                */  
  Float_t       focy_s2;         /*                          */  
  Float_t       focx_s4;         /*                          */  
  Float_t       focy_s4;         /*                          */  
  Float_t       focx_s8;         /*  FRS foci                */  
  Float_t       focy_s8;         /*                          */
  Float_t       angle_x_s2;      /*                          */
  Float_t       angle_y_s2;      /*                          */
  Float_t       angle_x_s4;      /*                          */
  Float_t       angle_y_s4;      /*                          */
  Float_t       angle_x_s8;      /*                          */
  Float_t       angle_y_s8;      /*                          */

  // TPC part
  Int_t tpc_csum[6][4];
  Float_t tpc_x[6];
  Float_t tpc_y[6];
  Bool_t b_tpc_csum[6][4];
  Bool_t b_tpc_xy[6];

  //TPCs 3 & 4 @ S2 focus
  Float_t tpc_x_s2_foc;
  Float_t tpc_y_s2_foc;
  Float_t tpc_angle_x_s2_foc;
  Float_t tpc_angle_y_s2_foc;
   
  //TPCs 5 & 6 @ s4 focus
  Float_t tpc_x_s4;
  Float_t tpc_y_s4;
  Float_t tpc_angle_x_s4;
  Float_t tpc_angle_y_s4;

  //TPCs 5 & 6 @ S4 OTPC (exp s388)
  Float_t tpc_x_s4_target2;
  Float_t tpc_y_s4_target2;
  

  //TPCs 3 & 4 @ S2 first Si tracking detector (exp s388)
  Float_t tpc_x_s2_target1;
  Float_t tpc_y_s2_target1;
  //      Float_t tpc_angle_x_s4_target2;
  //      Float_t tpc_angle_y_s4_target2;


  Float_t       tpc_sc21_x;      /* SC21 x                    */
  Float_t       tpc_sc41_x;      /* SC41 x                    */
  Float_t       tpc_sc21_y;      /* SC21 y                    */
  Float_t       tpc_sc41_y;      /* SC41 y                    */


  Float_t       sc21_x;          /* SC21                     */
  Float_t       sc22_x;          /* SC22 (LAND finger)       */
  Float_t       sc41_x;          /* SC41                     */
  Float_t       itag_stopper_x;  /* ITAG Stopper             */
  Float_t       sc42_x;          /* SC42                     */
  Float_t       sc43_x;          /* SC43                     */
  Float_t       sc21_y;          /* tracked SC21 Y pos       */
  Float_t       sc41_y;          /* SC41 Y                   */
  Float_t       sc42_y;          /* SC42 Y                   */
  Float_t       sc43_y;          /* SC43 Y                   */
  Float_t       sc81_x;          /* SC81          positions  */
      
  Float_t       music1_x1;       /* parameters to calibrate  */
  Float_t       music1_x2;       /* parameters to calibrate  */
  Float_t       music1_x3;       /* parameters to calibrate  */
  Float_t       music1_x4;       /* parameters to calibrate  */
  Float_t       music2_x;        /* MUSIC1,2 positions       */


  //Si detectors
  Float_t si_e1;
  Float_t si_e2;
  Float_t si_e3;
  Float_t si_e4;


private:
  // TFRSCalibrProc   *fxProc;  //! Don't put this to file
  // TGo4FileSource *fxFileSrc;  //! This is file source

  ClassDef(TFRSCalibrEvent,1)         
};

#endif //TFRSCALIBREVENT_H

