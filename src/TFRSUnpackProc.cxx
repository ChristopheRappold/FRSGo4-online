#include "TFRSUnpackProc.h"

#include "Go4StatusBase/TGo4Picture.h"
#include "Go4EventServer/TGo4MbsEvent.h"

#include "TFRSAnalysis.h"
#include "TFRSUnpackEvent.h"

#include "TH1.h"
#include "TH2.h"

#include <iostream>
//#include  <stdio.h>

TFRSUnpackProc::TFRSUnpackProc() : TFRSBasicProc("FRSUnpackProc")
{
  frs = (TFRSParameter*) GetParameter("FRSPar");

}

TFRSUnpackProc::TFRSUnpackProc(const char* name) :  TFRSBasicProc(name) 
{ 
  hTrigger = MakeH1I("Raw data", "newTrigger", 16, 0.5, 16.5); 
  frs = dynamic_cast<TFRSParameter*>(GetParameter("FRSPar"));

  bool remove_histos = (frs!=nullptr) && (!frs->fill_raw_histos);
  
  for(int n=0;n<32;n++)
    {
    
      hVME0_8[n]  = MakeH1ISeries("Raw data/VME0",  8, 0, n, remove_histos);
      hVME0_9[n]  = MakeH1ISeries("Raw data/VME0",  9, 0, n, remove_histos);
      hVME0_11[n] = MakeH1ISeries("Raw data/VME0", 11, 0, n, remove_histos);
      hVME0_12[n] = MakeH1ISeries("Raw data/VME0", 12, 0, n, remove_histos);
      //    hVME0_13[n] = MakeH1ISeries("Raw data/VME0", 13, 0, n, remove_histos);
      //    hVME0_14[n] = MakeH1ISeries("Raw data/VME0", 14, 0, n, remove_histos);

      hVME1_8[n]  = MakeH1ISeries("Raw data/VME1", 8, 1, n, remove_histos);
      hVME1_9[n]  = MakeH1ISeries("Raw data/VME1", 9, 1, n, remove_histos);
      hVME1_16[n] = MakeH1ISeries("Raw data/VME1", 16, 1, n, remove_histos);
      hVME1_17[n] = MakeH1ISeries("Raw data/VME1", 17, 1, n, remove_histos);

      hVME2_TDC[n] = MakeH1ISeries3("Raw data/VME2", 9, 2, n, remove_histos);
 
      //hVME2_12[n] = MakeH1ISeries("Raw data/VME2", 12, 2, n, remove_histos);
    }

}

TFRSUnpackProc::~TFRSUnpackProc()
{ }


TH1I* TFRSUnpackProc::MakeH1ISeries(const char* foldername, Int_t seriesnumber, Int_t crate, Int_t number, Bool_t remove)
{
  char fullfoldername[100];                                    
  sprintf(fullfoldername,"%s/%02d", foldername, seriesnumber);
  char histoname[50];
  sprintf(histoname,"newVME%02d_%02d_%02d", crate, seriesnumber, number); 
  if (remove)
    {
      char fullname[200]; 
      sprintf(fullname,"%s/%s",fullfoldername,histoname);
      RemoveHistogram(fullname); 
      return 0;   
    }
  return MakeH1I(fullfoldername, histoname, 4096, 0, 4096);
}

TH1I* TFRSUnpackProc::MakeH1ISeries3(const char* foldername, Int_t seriesnumber, Int_t crate, Int_t number, Bool_t remove) 
{
  char fullfoldername[100];                                    
  sprintf(fullfoldername,"%s/%02d", foldername, seriesnumber);
  char histoname[100];
  sprintf(histoname,"VME%02d_%02d", crate, number); 
  if (remove)
    {
      char fullname[200]; 
      sprintf(fullname,"%s/%s",fullfoldername,histoname);
      RemoveHistogram(fullname); 
      return 0;   
    }
  return MakeH1I(fullfoldername, histoname, 8192, 0, 500000);
}




TGo4Picture* TFRSUnpackProc::MakeSeriesPicture(const char* foldername, Int_t seriesnumber, Bool_t remove) 
{
  char picname[100];
  sprintf(picname,"Pic_VME0_%02d", seriesnumber);
  
  if (remove)
    {
      char fullname[200]; 
      sprintf(fullname,"%s/%s",foldername,picname);
      RemovePicture(fullname); 
      return 0;   
    }
  
  TGo4Picture* pic = MakePic(foldername, picname, 4, 8);
   
  if (ObjWasCreated())
    for(int i=0;i<4;i++)
      for(int j=0;j<8;j++)
	{
	  char histoname[100];
	  sprintf(histoname,"newVME0_%02d_%02d", seriesnumber, i*8+j);    
	  pic->AddObjName(i, j, histoname);
	}
      
  return pic;
}

Int_t getbits(Int_t value, int nword, int start, int length)
{
  UInt_t buf = (UInt_t) value;
  buf = buf >> ((nword-1)*16 + start - 1);
  buf = buf & ((1 << length) - 1);
  return buf;
}

Int_t get2bits(Int_t value, int nword, int start, int length)
{
  UInt_t buf = (UInt_t) value;
  buf = buf >> ((nword-1)*32 + start - 1);
  buf = buf & ((1 << length) - 1);
  return buf;
}

void TFRSUnpackProc::UnpackUserSubevent(TGo4MbsSubEvent* psubevt, TFRSUnpackEvent* tgt)
{

  Int_t *pdata = psubevt->GetDataField();	
  Int_t len = 0;  

  /* read the header longword and extract slot, type & length  */
  Int_t vme_geo = getbits(*pdata,2,12,5);
  //Int_t vme_type = getbits(*pdata,2,9,3);
  Int_t vme_nlw =  getbits(*pdata,1,1,6);
  pdata++; len++;

  /* read the data from scaler */
  if (vme_nlw > 0)
    {
      for(int i=0;i<vme_nlw;i++)
	{
	  tgt->vme1[vme_geo][i] = *pdata;
	  pdata++; len++;
	}
      /* read and ignore the expected "end-of-block" longword   */
      pdata++; len++;
    }

  /* for ProcID = 20 - rest of the unpacking */

  while (len < (psubevt->GetDlen()-2)/2)
    {
      
      /* read the header longword and extract slot, type & length  */
      Int_t vme_chn = 0;
      Int_t vme_geo = getbits(*pdata,2,12,5);
      Int_t vme_type = getbits(*pdata,2,9,3);
      Int_t vme_nlw =  getbits(*pdata,1,1,6);
      pdata++; len++;
      
      //    std::cout<<"type = "<<vme_type<<"nlw = "<<vme_nlw<<std::endl;
      /* read the data */
      if ((vme_type == 2) && (vme_nlw > 0))
	{
	  for(int i=0;i<vme_nlw;i++)
	    {  
	      vme_geo = getbits(*pdata,2,12,5);
	      vme_type = getbits(*pdata,2,9,3);
	      vme_chn = getbits(*pdata,2,1,5);
	      tgt->vme1[vme_geo][vme_chn] = getbits(*pdata,1,1,16);	                 
	      //printf("DATA:%d %d %d\n",vme_geo,vme_chn,tgt->vme1[vme_geo][vme_chn] ); 
	      pdata++; len++;
	    }
	  
	  /* read and ignore the expected "end-of-block" longword */
	  pdata++; len++;
	}
      
    }  /* end of the while... loop  */
		
  // if ((frs!=0) && frs->fill_raw_histos)
  //   for(int i=0;i<32;i++)
  //     {
  // 	if (hVME1_8[i]) hVME1_8[i]->Fill(tgt->vme1[8][i] & 0xfff);
  // 	if (hVME1_9[i]) hVME1_9[i]->Fill(tgt->vme1[9][i] & 0xfff);
  // 	if (hVME1_16[i]) hVME1_16[i]->Fill(tgt->vme1[15][i] & 0xfff);
  // 	if (hVME1_17[i]) hVME1_17[i]->Fill(tgt->vme1[17][i] & 0xfff);
  //     }
}  


Bool_t TFRSUnpackProc::BuildEvent(TGo4EventElement* output) 
{
  
  TFRSUnpackEvent* tgt = dynamic_cast<TFRSUnpackEvent*> (output);
  if (tgt==nullptr)
    return kFALSE;
  tgt->SetValid(kFALSE);  // by default output event rejected

  TGo4MbsEvent *fInput = dynamic_cast<TGo4MbsEvent*> (GetInputEvent());
  if (fInput==nullptr)
    return kFALSE;

  
  /* ---------------------- ProcID = 10 ------------------ */

  /*  Check that the EVENT type and subtype are OK:  */
  if((fInput->GetType()!=10) || (fInput->GetSubtype()!=1))
    {
      std::cout << "Wrong event type " << fInput->GetType() << std::endl;
      return kFALSE;
      //  }else{
      //    std::cout << "Good event type " << fInput->GetType() << std::endl;
      //    return;
    }
  
  // If trigger 14 or 15, event fully can be skipped
  if((fInput->GetTrigger()==14) || (fInput->GetTrigger()==15))
    {
      //std::cout << "Trigger = " << fInput->GetTrigger() << ", event skipped" << std::endl;
      return kFALSE;
    }

  /*  Put the event header stuff into output event  */
  
  tgt->qlength   = fInput->GetDlen()   ;
  tgt->qtype     = fInput->GetType()   ;
  tgt->qsubtype  = fInput->GetSubtype();
  tgt->qdummy    = fInput->GetDummy()  ;
  tgt->qtrigger  = fInput->GetTrigger();
  tgt->qevent_nr = fInput->GetCount()  ;

  // Special event
  if(fInput->GetCount()!=0)
    {
      // std::cout << "qtype : " << fInput->GetType() << std::endl;  
      // std::cout << "qlength(=GetDLen) : " << tgt->qlength  << std::endl;
      // std::cout << "qsubtype : " <<  tgt->qsubtype << std::endl;

      /*  Check event length: if it's <= 4 it is empty or screwed up! */
      if(fInput->GetDlen()<=4)
	{
	  std::cout << "Wrong event length: " << fInput->GetDlen() << " =< 4" << std::endl;
	  return kFALSE;
	}
  
      /*  Event OK, now assign pointer to local subevent header! */
      //std::cout << "trigger = " << fInput->GetTrigger() << std::endl;    
      fInput->ResetIterator();
      TGo4MbsSubEvent *psubevt = nullptr;

      while ((psubevt=fInput->NextSubEvent())!= nullptr)
	{
	  // start subevents loop

	  //printf("TRI:%d\n",fInput->GetTrigger());
	  //printf("PROC:%d\n",psubevt->GetProcid());
	  //std::cout << "subevent control " << int(psubevt->GetControl()) << std::endl;
	  //std::cout << "cratesubevent : " << int(psubevt->GetSubcrate()) << std::endl; 
	  
	  //      if (psubevt->GetControl()==29) {  // for tpc crate
	  /*      if(psubevt->GetProcid()==20) { // for tpc crate
	  //printf("TRI:%d\n",fInput->GetTrigger());
	  //printf("PROC:%d\n",psubevt->GetProcid());
	  //std::cout << "subevent control " << int(psubevt->GetControl()) << std::endl;
	  //std::cout << "cratesubevent : " << int(psubevt->GetSubcrate()) << std::endl; 
	  
	  if (fInput->GetTrigger()==5 || fInput->GetTrigger()==6 || fInput->GetTrigger()==7 ||  fInput->GetTrigger()==8 || fInput->GetTrigger()==9 || fInput->GetTrigger()==10 || fInput->GetTrigger()==12 || fInput->GetTrigger()==13) 
	  UnpackUserSubevent(psubevt, tgt);
	  continue;
	  }
	  */
	  /* check for ProcID 10 = standard crate  */
	  //    if((psubevt->GetProcid())!=10) {
	  if( (psubevt->GetProcid()!=10) && (psubevt->GetProcid()!=15) && (psubevt->GetProcid()!=20) && (psubevt->GetProcid()==65) )
	    {
	      std::cout << "Wrong ProcID " << psubevt->GetProcid() << std::endl; 
	      continue; // skip non standard event
	    }
    
	  /*  Check the SUBevent type and subtype!             */
	
	  if( (psubevt->GetType()!=10) || (psubevt->GetSubtype()!=1) )
	    {  // for all data 
	      //std::cout << "getsubtype " << psubevt->GetSubtype() << std::endl;
	      //std::cout << "gettype " << psubevt->GetType() << std::endl;
	      //std::cout << "Wrong subevent type " << psubevt->GetType() << std::endl;
	      continue; // skip subevent SL
	    }

     
	  /*    Now select subcrate number:  */
	  //      if((psubevt->GetSubcrate())!=0) {
	  //	std::cout << "Non supported subcrate " << psubevt->GetSubcrate() << std::endl;
	  //	continue; // skip subevent SL
	  //}
         
	  /************************************************************************/
	  /* Here we go for the different triggers.....                           */
	  /************************************************************************/
	  //      printf("TRI 2:%d\n",fInput->GetTrigger());
	  //      printf("PROC 2:%d\n",psubevt->GetProcid());

	  switch(fInput->GetTrigger())
	    { // trigger value
      
	    case 1: //FRS trigger box
	    case 2: //Ion catcher Si 1
	    case 3: //Ion catcher Si 2
	    case 4: //Ion catcher Si 3 (not used)
	    case 5: //trigger 10 Hz for scaler readout
	    case 12:   // start of extraction, not used
	    case 13:
	      // end of extraction, not used 
	      Event_Extract(tgt,psubevt);
	      break;
	      // end trigger 5,6,7,8,9,10,12,13
	      
	      // start of acquisition (vme system):        
	    case 14: 
	      // stop of acquisition (vme system):         
	    case 15:
	      {
       
		// get pointer on data   
		//Int_t *pdata = psubevt->GetDataField();
	
		// special: TIME STAMP module data (4 longwords)   
		//   no header or end-of-block info, we must hardwire GEO = 20... 
		/* 
		   Int_t vme_chn = 0;
		   for (int i=0;i<4;i++) {
		   tgt->vme0[20][vme_chn++] = getbits(*pdata,1,1,16);
		   tgt->vme0[20][vme_chn++] = getbits(*pdata,2,1,16);
		   pdata++;
		   }
		*/
		break;   
	      } // end case 14-15 
	      
	    default:
	      { 
		std::cout << "Invalid trigger " << fInput->GetTrigger() << std::endl;
		return kFALSE; 
	      }
      
	    }  // switch on trigger value
	
	} // end subevents loop
	

    } // end special event

  FillHistograms(tgt); // fill histograms from output event

  
  tgt->SetValid(kTRUE); // accept event
  return kTRUE;
} // close unpack proc



Bool_t TFRSUnpackProc::Event_Extract(TFRSUnpackEvent* event_out, TGo4MbsSubEvent* psubevt, int)
{

  Int_t *pdata = psubevt->GetDataField();
  Int_t len = 0;	  
  Int_t vme_chn;
  Int_t lenMax = (psubevt->GetDlen()-2)/2;

  switch(psubevt->GetProcid())
    {
    case 10:
      // proc ID=10 - FRS CRATE
      
      // get pointer on data   

      //Int_t *pdata = psubevt->GetDataField();
      //Int_t len = 0;
      
      /** \note FRS TIME STAMP module data (3 longwords)   
       *   has no header or end-of-block info, we must hardwire GEO = 20.
       */
      //      pdata++; len++; // 0x200
      
      //Int_t vme_chn = 0;

      /*
	for (int i=0;i<3;i++) {
	tgt->vme0[20][vme_chn++] = getbits(*pdata,1,1,16);
	tgt->vme0[20][vme_chn++] = getbits(*pdata,2,1,16);
	pdata++; len++;
	}
      */
      
      /** \note FRS SCALER module data (1 longword per channel)   
       *  This module has sequential readout therefore no channel
       *  number contained in the data longwords. 
       */
      /*
      // read the header longword and extract slot, type & length  
      Int_t vme_geo = getbits(*pdata,2,12,5);
      Int_t vme_type = getbits(*pdata,2,9,3);
      Int_t vme_nlw =  getbits(*pdata,1,1,6);
      pdata++; len++;
		      
      // read the data 
      if (vme_nlw > 0) {
      for(int i=0;i<vme_nlw;i++) {
      tgt->vme0[vme_geo][i] = *pdata;
      pdata++; len++;
      }
      // std::cout<<"1Hz unpack, "<<tgt->vme0[6][3]<<std::endl;
      // read and ignore the expected "end-of-block" longword   
      pdata++; len++;
      }
      */
      //  now start with the REST of the unpacking...       
		      
      while (len < (psubevt->GetDlen()-2)/2)
	{
			  
	  // read the header longword and extract slot, type & length  
	  Int_t vme_geo = getbits(*pdata,2,12,5);
	  Int_t vme_type = getbits(*pdata,2,9,3);
	  Int_t vme_nlw =  getbits(*pdata,1,1,6);
	  pdata++; len++;
			  
	  // read the data 
	  if ((vme_type == 2) && (vme_nlw > 0))
	    {
	      for(int i=0;i<vme_nlw;i++)
		{  
		  vme_geo = getbits(*pdata,2,12,5);
		  vme_type = getbits(*pdata,2,9,3);
		  vme_chn = getbits(*pdata,2,1,5);
		  event_out->vme0[vme_geo][vme_chn] = getbits(*pdata,1,1,16);
		  pdata++; len++;
		}
			      
	      // read and ignore the expected "end-of-block" longword 
	      pdata++; len++;
	    }
			  
	}  // end of the while... loop  
		      
		      
      break; 
      // end proc ID=10
		    
    case 15:
      //proc ID=15 - TimeStamp
      // get pointer on data   
      
      //Int_t vme_geo;
      //Int_t vme_nlw;
      //Int_t vme_type;
      
      if (lenMax<2)
	{
	  std::cout << "Bad timming subevent data" << std::endl;
	  return kFALSE;
	}
      
      //Titris information read in subevent 15 as module number 20 in vme0
      //and then system time (2x32 bits words) put in the same way
      vme_chn = 0  ; 
      
      for (int i=0;i<5;i++)
	{
	  event_out->vme0[20][vme_chn++] = getbits(*pdata,1,1,16);
	  event_out->vme0[20][vme_chn++] = getbits(*pdata,2,1,16);
	  pdata++; len++;
	}
      
      
      break;
      // ID=15 loop
      
    case 65:
      //proc ID=65 - User CRATE
      // get pointer on data   
	
      // Int_t *pdata = psubevt->GetDataField();
      // Int_t len = 0;	  
      // //Int_t vme_chn;
      // Int_t lenMax = (psubevt->GetDlen()-2)/2;

	
	
      if (lenMax<2)
	{
	  std::cout << "Bad User subevent data" << std::endl;
	  return kFALSE;
	}
	  
	  
      // std::cout<<"length = "<<lenMax<<std::endl;
      while (len < lenMax) 
	{
	  Int_t vme_geo = getbits(*pdata,2,12,5);
	  Int_t vme_type = getbits(*pdata,2,9,3);
	  Int_t vme_nlw =  getbits(*pdata,1,9,6);
	  //printf("%d %d %d\n",vme_geo,vme_type,vme_nlw);
	  pdata++; len++;
			  
	  // read the data 
	  if ( (vme_nlw > 0) && (vme_type==2) )
	    {
	      for(int i=0;i<vme_nlw;i++)
		{
		  event_out->vme2scaler[i] = *pdata;
		  pdata++; len++;
		}
			      
	      // read and ignore the expected "end-of-block" longword 
	      pdata++; len++;
	    }		      
			  
			  
	  //v1290 TDC              
	  vme_geo = getbits(*pdata,1,1,5);
	  vme_type = getbits(*pdata,2,12,5);
	  Int_t multihit = 0;//, counter = 0;
			  
	  //std::cout<<"mTDC geo = "<<vme_geo<<", type = "<<vme_type<<std::endl;
			  
	  for (int i = 0; i < 32; ++i)
	    {
	      for (int j = 0; j < 10; ++j)
		event_out->vme2s[i][j] = 0;

	      event_out->nhit5[i] = 0;
	    }  
	   
	   
	  if (vme_type == 8)
	    { // if is global header
	      pdata++; len++;
	      vme_type = getbits(*pdata,2,12,5);
	      //multihit = 0;
	      while (vme_type == 0)
		{ // this indicates a TDC measurement
		  Int_t vme_chn = getbits(*pdata,2,6,5);
		  //std::cout << "     tdc vme_chn = " << vme_chn << std::endl;
		  multihit = event_out->nhit5[vme_chn];
		  //std::cout << "     tdc multihit = " << multihit << std::endl;
		  if (multihit < 10)
		    {
		      Int_t value = getbits(*pdata,1,1,21);
		      //std::cout << "     tdc value = " << value << std::endl;
		      if (value > 0)
			{
			  event_out->vme2s[vme_chn][multihit] = value;
			  event_out->nhit5[vme_chn]++;
			  //hVME2_TDC[vme_chn]->Fill(value);
			}
		    }
		  pdata++; len++;
		  vme_type = getbits(*pdata,2,12,5);
		}
	    }
	  if(getbits(*pdata,2,12,5)==16&&len<lenMax)
	    {//EOB
	      pdata++; len++;
	    }
	}
      break;
      // ID=65 loop
		    
    case 20:
      UnpackUserSubevent(psubevt, event_out); //for historical reasons
      break;
    default :
      break;
    } // end switch prodID

  return kTRUE;

}




Bool_t TFRSUnpackProc::FillHistograms(TFRSUnpackEvent* event)
{

  hTrigger->Fill(event->qtrigger);

  if(frs->fill_raw_histos)
    {
      for(int i=0;i<32;i++)
	{
	  if (hVME0_8[i]) hVME0_8[i]->Fill(event->vme0[8][i] & 0xfff);
	  if (hVME0_9[i]) hVME0_9[i]->Fill(event->vme0[9][i] & 0xfff);
	  if (hVME0_11[i]) hVME0_11[i]->Fill(event->vme0[11][i] & 0xfff);
	  if (hVME0_12[i]) hVME0_12[i]->Fill(event->vme0[12][i] & 0xfff);
	  //if (hVME0_13[i]) hVME0_13[i]->Fill(event->vme0[13][i] & 0xfff);
	  //if (hVME0_14[i]) hVME0_14[i]->Fill(event->vme0[14][i] & 0xfff);
	}

      for(auto& channels : event->vme2s)
	for(auto& multihit_value : channels)
	  if(multihit_value > 0)
	    {
	      int vme_chn = &channels-event->vme2s;
	      hVME2_TDC[vme_chn]->Fill(multihit_value);
	    }

      // if(event->qtrigger==20)
      // 	{
      // 	  // from UnpackUserSubevent 
      // 	  // if ((frs!=0) && frs->fill_raw_histos)
      // 	  for(int i=0;i<32;i++)
      // 	    {
      // 	      if (hVME1_8[i]) hVME1_8[i]->Fill(event->vme1[8][i] & 0xfff);
      // 	      if (hVME1_9[i]) hVME1_9[i]->Fill(event->vme1[9][i] & 0xfff);
      // 	      if (hVME1_16[i]) hVME1_16[i]->Fill(event->vme1[15][i] & 0xfff);
      // 	      if (hVME1_17[i]) hVME1_17[i]->Fill(event->vme1[17][i] & 0xfff);
      // 	    }
      // 	}

    }

  return kTRUE;
}




ClassImp(TFRSUnpackProc)

