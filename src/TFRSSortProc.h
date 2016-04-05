#ifndef TFRSSORTPROCESSOR_H
#define TFRSSORTPROCESSOR_H

#include "TFRSBasicProc.h"

class TFRSSortEvent;

class TFRSSortProc : public TFRSBasicProc {
   public:
      TFRSSortProc() ;
      TFRSSortProc(const char* name);
      virtual ~TFRSSortProc() ;
   
      void FRSSort(TFRSSortEvent* tgt);
      
   protected:
      Long64_t StartOfSpilTime;
      Long64_t StartOfSpilTime2; //does not reset at end of extraction

      Int_t counter;


   ClassDef(TFRSSortProc,1)
};

#endif //TFRSSORTPROCESSOR_H

