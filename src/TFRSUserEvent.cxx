#include "TFRSUserEvent.h"

#include "Riostream.h"
#include "Go4EventServer/TGo4FileSource.h"
#include "TFRSUserProc.h"

TFRSUserEvent::TFRSUserEvent() : 
   TGo4EventElement("FRSUserEvent"), 
   fxProc(0), 
   fxFileSrc(0) 
{ 
}

TFRSUserEvent::TFRSUserEvent(const char* name) :
   TGo4EventElement(name), 
   fxProc(0), 
   fxFileSrc(0) 
{ 
}

TFRSUserEvent::~TFRSUserEvent() 
{ 
}

Int_t TFRSUserEvent::Init() { 
  Int_t rev=0;
  Clear();
  if(CheckEventSource("TFRSUserProc")){
    fxProc = (TFRSUserProc*)GetEventSource();
    std::cout << "**** " << GetName() << " will be filled by " << fxProc->GetName() << " ****" << std::endl;
  } else
  if(CheckEventSource("TGo4FileSource")) {
    fxFileSrc = (TGo4FileSource*)GetEventSource();
    std::cout << "**** " << GetName() << " will be filled by File Source ****"<< std::endl;
  } else rev=1;
  return rev;
}

Int_t TFRSUserEvent::Fill() { 
  Clear();
  if(fxProc) fxProc->FRSUserProc(this); else  // user event processing method
  if(fxFileSrc)fxFileSrc->BuildEvent(this);   // method from framework to restore event from file
  return 0;
}

void TFRSUserEvent::Clear(Option_t *t) 
{

	
}

ClassImp(TFRSUserEvent)
