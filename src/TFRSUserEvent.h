#ifndef TFRSUSEREVENT_H
#define TFRSUSEREVENT_H

#include "Go4Event/TGo4EventElement.h"

class TFRSUserProc;
class TGo4FileSource;

class TFRSUserEvent : public TGo4EventElement 
{
   public:
      TFRSUserEvent();
      TFRSUserEvent(const char* name);
      virtual ~TFRSUserEvent();
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

		//Variables to be converted to Analysis ROOT Files for s323

   private:
  //  TFRSUserProc   *fxProc;     //! This is processor
  //  TGo4FileSource *fxFileSrc;  //! This is file source
   ClassDef(TFRSUserEvent,1)         

};

#endif //TFRSUSEREVENT_H
