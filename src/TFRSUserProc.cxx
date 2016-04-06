#include "TFRSUserProc.h"

#include "TH1.h"

#include "TFRSSortEvent.h"
#include "TFRSCalibrEvent.h"
#include "TFRSAnlEvent.h"
#include "TFRSUserEvent.h"
#include "TFRSParameter.h"


TFRSUserProc::TFRSUserProc() : TFRSBasicProc("FRSUserProc"),evtFile(nullptr),allEvtTree(nullptr),impTree(nullptr),decTree(nullptr)
{ }

TFRSUserProc::TFRSUserProc(const char* name) : TFRSBasicProc(name),evtFile(nullptr),allEvtTree(nullptr),impTree(nullptr),decTree(nullptr)
{ 

  // Create_ROOT_Tree_Data();
}

TFRSUserProc::~TFRSUserProc() 
{
  //Write and close the new root file 
  //evtFile=allEvtTree->GetCurrentFile();
  if(evtFile!=nullptr)
    {
      evtFile->Write();
      evtFile->Close();
    }
}

void TFRSUserProc::FRSUserProc(TFRSUserEvent* tgt) {

  tgt->SetValid(kTRUE);  // by default output event accepted

  TFRSSortEvent *srt = dynamic_cast<TFRSSortEvent*> (GetInputEvent("Calibr"));
  TFRSCalibrEvent *cal = dynamic_cast<TFRSCalibrEvent*> (GetInputEvent("Analysis"));
  TFRSAnlEvent *anl = dynamic_cast<TFRSAnlEvent*> (GetInputEvent());

  if ((tgt==0) || (cal==0) || (srt==0) || (anl==0)) return;

  Process_ROOT_Tree_Analysis(*srt,*cal,*anl,*tgt);
    
}


void TFRSUserProc::Create_ROOT_Tree_Data() {


}



void TFRSUserProc::Process_ROOT_Tree_Analysis(TFRSSortEvent &srt, TFRSCalibrEvent &cal, TFRSAnlEvent &anl, TFRSUserEvent &tgt) 
{


}


ClassImp(TFRSUserProc)
