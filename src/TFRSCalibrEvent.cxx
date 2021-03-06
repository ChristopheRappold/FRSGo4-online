#include "TFRSCalibrEvent.h"

#include "Riostream.h"

#include "TFRSCalibrProc.h"
#include "Go4EventServer/TGo4FileSource.h"

TFRSCalibrEvent::TFRSCalibrEvent() : TGo4EventElement("FRSCalibrEvent")//, fxProc(0), fxFileSrc(0) { 
{ }

TFRSCalibrEvent::TFRSCalibrEvent(const char* name) : TGo4EventElement(name)//, fxProc(0), fxFileSrc(0) { 
{ }

TFRSCalibrEvent::~TFRSCalibrEvent()
{ }

Int_t TFRSCalibrEvent::Init()
{ 
  Int_t rev=0;
  Clear();
  // if(CheckEventSource("TFRSCalibrProc")){
  //   fxProc = (TFRSCalibrProc*)GetEventSource();
  //   std::cout << "**** " << GetName() << " will be filled by " << fxProc->GetName() << " ****" << std::endl;
  // } else
  // if(CheckEventSource("TGo4FileSource")) {
  //   fxFileSrc = (TGo4FileSource*)GetEventSource();
  //   std::cout << "**** " << GetName() << " will be filled by File Source ****"<< std::endl;
  // } else rev=1;
  return rev;
}

// Int_t TFRSCalibrEvent::Fill() 
// {
//    Clear();
//    if(fxProc) fxProc->FRSCalibr(this); else  // user event processing method
//    if(fxFileSrc)fxFileSrc->BuildEvent(this); // method from framework to restore event from file
//    return 0;
// }

void TFRSCalibrEvent::Clear(Option_t *t)
{ 
  EventFlag = 0;

  seconds = 0;
  tenthsecs = 0;
  hundrethsecs = 0;	//mik
  extraction_cycle = 1;

  freeTrig=0;
  acptTrig=0;

  for (int i=0;i<13;i++)
    {
      mw_xsum[i] = 0;
      mw_ysum[i] = 0;
      mw_x[i] = 0.;
      mw_y[i] = 0.;
      mw_wire[i] = 0.;
      b_mw_xsum[i] = kFALSE; 
      b_mw_ysum[i] = kFALSE; 
  }
  
  z_x_s2 = 0.;
  z_y_s2 = 0.;
  z_x_s4 = 0.;
  z_y_s4 = 0.;
  z_x_s8 = 0.;
  z_y_s8 = 0.;
  
  /* set some "specials" as defined by ancient gurus */

  focx_s2 = 999.;
  focy_s2 = 999.;
  focx_s4 = 999.;
  focy_s4 = 999.;
  focx_s8 = 999.;
  focy_s8 = 999.;
  
  /*  dummy values for dummy events...                             */
  /*  (for those later program parts that use these variables...   */
  angle_x_s2 = -999. ;
  angle_y_s2 = -999. ;
  angle_x_s4 = -999. ;
  angle_y_s4 = -999. ;
  angle_x_s8 = -999. ;
  angle_y_s8 = -999. ;
  focx_s2 = -999. ;
  focy_s2 = -999. ;
  focx_s4 = -999. ;
  focy_s4 = -999. ;
  focx_s8 = -999. ;
  focy_s8 = -999. ;

  tpc_angle_x_s2_foc=0;
  tpc_angle_y_s2_foc=0;
  tpc_angle_x_s4=0;
  tpc_angle_y_s4=0;

  tpc_sc21_x = 0.;      /* SC21                     */
  tpc_sc41_x = 0.;      /* SC41                     */
  tpc_sc21_y = 0.;      /* tracked SC21 Y pos       */
  tpc_sc41_y = 0.;      /* SC41 Y                   */
  
  sc21_x = 0.;          /* SC21                     */
  sc22_x = 0.;          /* SC22 (LAND finger)       */
  sc41_x = 0.;          /* SC41                     */
  itag_stopper_x = 0.;  /* ITAG Stopper             */
  sc42_x = 0.;          /* SC42                     */
  sc43_x = 0.;          /* SC43                     */
  sc21_y = 0.;          /* tracked SC21 Y pos       */
  sc41_y = 0.;          /* SC41 Y                   */
  sc42_y = 0.;          /* SC42 Y                   */
  sc43_y = 0.;          /* SC43 Y                   */
  sc81_x = 0.;          /* SC81          positions  */
  
  music1_x1 = 0.;       /* parameters to calibrate  */
  music1_x2 = 0.;       /* parameters to calibrate  */
  music1_x3 = 0.;       /* parameters to calibrate  */
  music1_x4 = 0.;       /* parameters to calibrate  */
  music1_y1 = 0.;	/* parameters to calibrate  */
  music1_y2 = 0.;	/* parameters to calibrate  */
  music1_y3 = 0.;	/* parameters to calibrate  */
  music1_y4 = 0.;	/* parameters to calibrate  */
  music2_x = 0.;        /* MUSIC1,2 positions       */

  //TPC Part
  for (int i = 0; i<6;i++)
    {
      tpc_x[i] = 9999.9;
      tpc_y[i] = 9999.9;
      b_tpc_xy[i] = kFALSE;
      for (int j=0;j<4;j++)
	{
	  tpc_csum[i][j] =0;
	  b_tpc_csum[i][j] = kFALSE;
	}
    }  
	
// Si
  si_e1=0;
  si_e2=0;
  si_e3=0;
  si_e4=0;
  si_e5=0;
  
}

ClassImp(TFRSCalibrEvent)
