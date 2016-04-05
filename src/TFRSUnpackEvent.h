#ifndef TFRSUNPACKEVENT_H
#define TFRSUNPACKEVENT_H


#include "Go4Event/TGo4EventElement.h"


class TFRSUnpackProc;
class TGo4FileSource;

class TFRSUnpackEvent : public TGo4EventElement {
public:
  TFRSUnpackEvent();
  TFRSUnpackEvent(const char* name);
  virtual ~TFRSUnpackEvent();
  // /**
  //  * Method called by the event owner (analysis step) to fill the
  //  * event element from the set event source. Event source can
  //  * be the source of the analysis step (if this is a raw event) 
  //  * or the event processor (if this is a reduced event).
  //  */
  // virtual Int_t Fill();

  /**
   * Method called by the event owner (analysis step) to clear the
   * event element.
   */
  virtual void Clear(Option_t *t="");  
      
  virtual Int_t Init();  

  UInt_t vme0[21][32];         // FRS crate                                
  UInt_t vme1[21][32];         // TPC crate 

  Int_t vme2scaler[32];         // User Crate Messhure
  Int_t vme2s[32][10];        // User TDC (V1290) 
  Int_t nhit5[32];            // multiplicity (V1290)


  Int_t         qlength;         /* From event header     */
  Int_t         qtype;           /*                       */
  Int_t         qsubtype;        /*                       */
  Int_t         qdummy;          /*                       */
  Int_t         qtrigger;        /*                       */
  Int_t         qevent_nr;       /*                       */

private:
  //      TFRSUnpackProc *fxProc;     //! This is processor
  //      TGo4FileSource *fxFileSrc;  //! This is file source

  ClassDef(TFRSUnpackEvent,1)         
};
#endif //TFRSUNPACKEVENT_H
