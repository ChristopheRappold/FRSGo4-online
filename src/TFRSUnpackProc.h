#ifndef TFRSUNPACKPROCESSOR_H
#define TFRSUNPACKPROCESSOR_H

#include "TFRSBasicProc.h"

class TFRSUnpackEvent;
class TGo4MbsSubEvent;

class TFRSUnpackProc : public TFRSBasicProc {
   public:
      TFRSUnpackProc() ;
      TFRSUnpackProc(const char* name);
      virtual ~TFRSUnpackProc();
   
      void FRSUnpack(TFRSUnpackEvent* tgt);
      
   private:

      void UnpackUserSubevent(TGo4MbsSubEvent* psubevt, TFRSUnpackEvent* tgt);

   
      TH1I* MakeH1ISeries(const char* foldername,
                          Int_t seriesnumber,
                          Int_t crate,
                          Int_t number,
                          Bool_t remove);                      
                      
      TH1I* MakeH1ISeries3(const char* foldername,
			   Int_t seriesnumber,
			   Int_t crate,
			   Int_t number,
			   Bool_t remove);  

    
      TGo4Picture* MakeSeriesPicture(const char* foldername,
                                     Int_t seriesnumber,
                                     Bool_t remove);              
                
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

