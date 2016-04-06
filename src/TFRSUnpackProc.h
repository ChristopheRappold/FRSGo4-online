#ifndef TFRSUNPACKPROCESSOR_H
#define TFRSUNPACKPROCESSOR_H

#include "TFRSBasicProc.h"
#include "TFRSUnpackEvent.h"
#include "TFRSParameter.h"

class TFRSUnpackEvent;
class TGo4MbsSubEvent;

class TFRSUnpackProc : public TFRSBasicProc {
public:
  TFRSUnpackProc() ;
  TFRSUnpackProc(const char* name);
  virtual ~TFRSUnpackProc();
   
  //void FRSUnpack(TFRSUnpackEvent* tgt);
  // event processing function, default name
  Bool_t BuildEvent(TGo4EventElement* output);
  Bool_t FillHistograms(TFRSUnpackEvent* event);

private:

  Bool_t Event_Extract(TFRSUnpackEvent* event_out, TGo4MbsSubEvent* rawsub, int type_event=1);
  void UnpackUserSubevent(TGo4MbsSubEvent* psubevt, TFRSUnpackEvent* tgt);

  TFRSParameter* frs ;
  
  TH1I            *hTrigger;         //!
     
  TH1I            *hVME0_8[32];      //! FRS crate
  TH1I            *hVME0_9[32];      //!
  TH1I            *hVME0_11[32];     //!
  TH1I            *hVME0_12[32];     //!
  TH1I            *hVME0_13[32];     //!
  TH1I            *hVME0_14[32];     //!
   
  TH1I            *hVME1_8[32];      //! TPC User crate
  TH1I            *hVME1_9[32];      //!
  TH1I            *hVME1_16[32];     //!
  TH1I            *hVME1_17[32];     //!      
  TH1I            *hVME2_TDC[32];
            
  ClassDef(TFRSUnpackProc,1)
};

#endif //TFRSUNPACKPROCESSOR_H

