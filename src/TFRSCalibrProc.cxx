#include "TFRSCalibrProc.h"

#include "TH1.h"
#include "TH2.h"
#include "Go4StatusBase/TGo4Picture.h"	//mik
#include "Go4ConditionsBase/TGo4WinCond.h"

#include "TFRSCalibrEvent.h"
#include "TFRSSortEvent.h"
#include "TFRSParameter.h"
#include "TFRSAnalysis.h"


TFRSCalibrProc::TFRSCalibrProc() : TFRSBasicProc("FRSCalibrProc")
{
  frs = dynamic_cast<TFRSParameter*>(GetParameter("FRSPar"));
  mw  = dynamic_cast<TMWParameter*>(GetParameter("MWPar"));
  tpc = dynamic_cast<TTPCParameter*>(GetParameter("TPCPar"));
  si  = dynamic_cast<TSIParameter*>(GetParameter("SIPar"));

  InitProcessor();
}

TFRSCalibrProc::TFRSCalibrProc(const char* name) : TFRSBasicProc(name)
{ 

  //TFRSAnalysis *myAnalysis = (TFRSAnalysis*) GetObject("FRSanalysis");

  frs = dynamic_cast<TFRSParameter*>(GetParameter("FRSPar"));
  mw = dynamic_cast<TMWParameter*>(GetParameter("MWPar"));
  tpc = dynamic_cast<TTPCParameter*>(GetParameter("TPCPar"));
  si  = dynamic_cast<TSIParameter*>(GetParameter("SIPar"));

  bDrawHist=kTRUE;

  counter=0;
  for(int i=0;i<2;++i)
    {
      check_first_event[i] = 1;
      scaler_time_count[i] = 0; 
      scaler_spill_count[i]= 0; //UInt_t
      scaler_time_check_last[i] = 0;//UInt_t
      scaler_spill_check_last[i] = 0;//UInt_t 
    }
  for(int i=0;i<64;++i)
    {
      check_increase_time[i]   = 0;//UInt_t 
      check_increase_spill[i]  = 0;//UInt_t
      scaler_increase_event[i] = 0;//UInt_t
      scaler_last_event[i]     = 0;
    }
  check_total_sc41 = 0;
  check_total_sc21 = 0;
  check_total_seetram = 0;
  check_total_mrtof_start = 0;
  check_total_mrtof_stop = 0;
  
  Create_MON_Hist();
  Create_MW_Hist();
  Create_TPC_Hist();
  Create_SI_Hist();
  Create_CT_Hist();
  Create_ElCurrent_Hist();

  InitProcessor();
}

TFRSCalibrProc::~TFRSCalibrProc()
{ }

//void TFRSCalibrProc::FRSCalibr(TFRSCalibrEvent* tgt)
Bool_t TFRSCalibrProc::BuildEvent(TGo4EventElement* output)
{
  TFRSCalibrEvent* tgt = dynamic_cast<TFRSCalibrEvent*> (output);
  if (tgt==nullptr)
    return kFALSE;

  tgt->SetValid(kTRUE);  // output event always accepted

  TFRSSortEvent *src = dynamic_cast<TFRSSortEvent*> (GetInputEvent());
  if (src==nullptr)
    return kFALSE;
  
  /*
    if (src->pattern>500){
    }
  */
  tgt->EventFlag = src->EventFlag;
  
  Process_MON_Analysis(*src, *tgt);
  Process_TPC_Analysis(*src, *tgt);
  Process_MW_Analysis(*src, *tgt);
  Process_SI_Analysis(*src, *tgt);
  Process_CT_Analysis(*src, *tgt);
  Process_ElCurrent_Analysis(*src, *tgt);

  return kTRUE;
}


void TFRSCalibrProc::Create_MON_Hist() 
{

  hTimeStamp = MakeH1I("MON","Time Stamp",3600,0,3600,"Time Stamp",2,5,"Time (s)");
  hTimeStampZoom = MakeH1I("MON","Time Stamp Zoom",60,0,60,"Time Stamp",2,5,"Time (s)");
  hTimeSpill = MakeH1I("MON","Time Spill",300,0,30,"Time Spill",2,5,"Time (s)");
  hMON_All = MakeH1I("MON","MON_All",32,0,32,"Scaler FRS Channel ",2,5,"Scaler Counts");
  hMON_All2 = MakeH1I("MON","MON_All2",32,0,32,"Scaler TPC Channel ",2,5,"Scaler Counts"); 
   
  char name[80];
  for(int i=0;i<64;i++)
    {
      sprintf(name,"MON_scaler_%02d",i);  
      hMON_scaler[i] = MakeH1I("MON/MON_scaler",name,28800,0,28800,"",2,5);  
      sprintf(name,"MON_diff_%02d",i);
      hMON_diff[i] = MakeH1I("MON/MON_diff",name,28800,0,28800,"",2,5);  
      sprintf(name,"MON_extr_%02d",i);
      hMON_extr[i] = MakeH1I("MON/MON_extr",name,28800,0,28800,"",2,5);  
    }
   
  if (ObjWasCreated())
    {
      
      hMON_scaler[0] ->SetXTitle("Free Trigger (OR)");           // updated 28-08-11 
      hMON_scaler[1] ->SetXTitle("Accepted Trigger (OR)");
      hMON_scaler[2] ->SetXTitle("Spill counter");
      hMON_scaler[3] ->SetXTitle("1 Hz clock");
      hMON_scaler[4] ->SetXTitle("10 Hz clock");
      hMON_scaler[5] ->SetXTitle("SCI01");
      hMON_scaler[6] ->SetXTitle("IC01 current digitizer (new output)");
      hMON_scaler[7] ->SetXTitle("SCI21");
      hMON_scaler[8] ->SetXTitle("SCI41");
      hMON_scaler[9] ->SetXTitle("Seetram current digitizer (old output)");
      hMON_scaler[10]->SetXTitle("Seetram current digitizer (new output)");
      hMON_scaler[11]->SetXTitle("IC01 current digitizer (old output)");
      hMON_scaler[12]->SetXTitle("Start SIS extraction");
      hMON_scaler[13]->SetXTitle("Stop SIS extraction");
      hMON_scaler[14]->SetXTitle("SC81");  
      hMON_scaler[15]->SetXTitle("Beam transformer digital gated");
      hMON_scaler[16]->SetXTitle("SCI00"); 
      hMON_scaler[17]->SetXTitle("SCI02");  
      hMON_scaler[18]->SetXTitle("IC counter");
      hMON_scaler[19]->SetXTitle("SCI41L"); 
      hMON_scaler[20]->SetXTitle("Si CR-1");
      hMON_scaler[21]->SetXTitle("Si CR-2");
      hMON_scaler[22]->SetXTitle("Si CR-3");
      hMON_scaler[23]->SetXTitle("Si CR-4");
      hMON_scaler[24]->SetXTitle("Si CR-5");
      hMON_scaler[25]->SetXTitle("10 kHz");
      hMON_scaler[26]->SetXTitle("10 kHz veto dead time");
      hMON_scaler[27]->SetXTitle("Nozzel extraction");
      hMON_scaler[28]->SetXTitle("not used");
      hMON_scaler[29]->SetXTitle("Spill IN & dead time");
      hMON_scaler[30]->SetXTitle("Spill OFF & dead time");
      hMON_scaler[31]->SetXTitle("Nozzel & dead time");
    
      hMON_scaler[32] ->SetXTitle("MrTOF 1 Hz");           // updated 28-08-11 
      hMON_scaler[33] ->SetXTitle("MrTOF 100 Hz");
      hMON_scaler[34] ->SetXTitle("MrTOF 10 kHz");
      hMON_scaler[35] ->SetXTitle("MrTOF 10 kHz veto deadtime");
      hMON_scaler[36] ->SetXTitle("MrTOF SC41L");
      hMON_scaler[37] ->SetXTitle("MrTOF start extraction");
      hMON_scaler[38] ->SetXTitle("MrTOF stop extraction");
      hMON_scaler[39] ->SetXTitle("MrTOF start");
      hMON_scaler[40] ->SetXTitle("MrTOF stop");
      hMON_scaler[41] ->SetXTitle("chaneltron 1");
      hMON_scaler[42] ->SetXTitle("chaneltron 2");
      hMON_scaler[43]->SetXTitle("not used"); 
      hMON_scaler[44]->SetXTitle("not used"); 
      hMON_scaler[45]->SetXTitle("not used"); 
      hMON_scaler[46]->SetXTitle("not used"); 
      hMON_scaler[47]->SetXTitle("not used"); 
      hMON_scaler[48]->SetXTitle("not used"); 
      hMON_scaler[49]->SetXTitle("not used"); 
      hMON_scaler[50]->SetXTitle("not used"); 
      hMON_scaler[51]->SetXTitle("not used"); 
      hMON_scaler[52]->SetXTitle("not used"); 
      hMON_scaler[53]->SetXTitle("not used"); 
      hMON_scaler[54]->SetXTitle("not used"); 
      hMON_scaler[55]->SetXTitle("not used"); 
      hMON_scaler[56]->SetXTitle("not used"); 
      hMON_scaler[57]->SetXTitle("not used"); 
      hMON_scaler[58]->SetXTitle("not used"); 
      hMON_scaler[59]->SetXTitle("not used"); 
      hMON_scaler[60]->SetXTitle("not used"); 
      hMON_scaler[61]->SetXTitle("not used"); 
      hMON_scaler[62]->SetXTitle("not used"); 
      hMON_scaler[63]->SetXTitle("not used"); 

      hMON_diff[0] ->SetXTitle("Free Trigger (OR)");           // FRS scaler update 13.08.11
      hMON_diff[1] ->SetXTitle("Accepted Trigger (OR)");
      hMON_diff[2] ->SetXTitle("Spill counter");
      hMON_diff[3] ->SetXTitle("1 Hz clock");
      hMON_diff[4] ->SetXTitle("10 Hz clock");
      hMON_diff[5] ->SetXTitle("SCI01");
      hMON_diff[6] ->SetXTitle("IC01 current digitizer (new output)");
      hMON_diff[7] ->SetXTitle("SCI21");
      hMON_diff[8] ->SetXTitle("SCI41");
      hMON_diff[9] ->SetXTitle("Seetram current digitizer (old output)");
      hMON_diff[10]->SetXTitle("Seetram current digitizer (new output)");
      hMON_diff[11]->SetXTitle("IC01 current digitizer (old output)");
      hMON_diff[12]->SetXTitle("Start extraction");
      hMON_diff[13]->SetXTitle("Stop extraction");
      hMON_diff[14]->SetXTitle("SC81");  
      hMON_diff[15]->SetXTitle("Beam transformer digital gated");
      hMON_diff[16]->SetXTitle("SCI00");                
      hMON_diff[17]->SetXTitle("SCI02");                
      hMON_diff[18]->SetXTitle("IC counter");           
      hMON_diff[19]->SetXTitle("SCI41L");               
      hMON_diff[20]->SetXTitle("Si CR-1");              
      hMON_diff[21]->SetXTitle("Si CR-2");              
      hMON_diff[22]->SetXTitle("Si CR-3");              
      hMON_diff[23]->SetXTitle("Si CR-4");              
      hMON_diff[24]->SetXTitle("Si CR-5");              
      hMON_diff[25]->SetXTitle("10 kHz");               
      hMON_diff[26]->SetXTitle("10 kHz veto dead time");
      hMON_diff[27]->SetXTitle("Nozzel extraction");    
      hMON_diff[28]->SetXTitle("not used");             
      hMON_diff[29]->SetXTitle("Spill IN & dead time"); 
      hMON_diff[30]->SetXTitle("Spill OFF & dead time");
      hMON_diff[31]->SetXTitle("Nozzel & dead time");

      hMON_diff[32] ->SetXTitle("MrTOF 1 Hz"); // 0
      hMON_diff[33] ->SetXTitle("MrTOF 100 Hz"); // 1
      hMON_diff[34] ->SetXTitle("MrTOF 10 kHz"); // 2
      hMON_diff[35] ->SetXTitle("MrTOF 10 kHz veto deadtime"); // 3
      hMON_diff[36] ->SetXTitle("MrTOF SC41L"); // 4
      hMON_diff[37] ->SetXTitle("MrTOF start extraction"); // 5  
      hMON_diff[38] ->SetXTitle("MrTOF stop extraction");  // 6
      hMON_diff[39] ->SetXTitle("MrTOF start"); // 7
      hMON_diff[40] ->SetXTitle("MrTOF stop"); // 8
      hMON_diff[41] ->SetXTitle("chaneltron 1"); // 9
      hMON_diff[42] ->SetXTitle("chaneltron 2"); // 10
      hMON_diff[43] ->SetXTitle("");
      hMON_diff[44] ->SetXTitle("");
      hMON_diff[45] ->SetXTitle("");
      hMON_diff[46] ->SetXTitle("");
      hMON_diff[47] ->SetXTitle("");
      hMON_diff[48] ->SetXTitle("");   // from here not used
      hMON_diff[49] ->SetXTitle("");
      hMON_diff[50] ->SetXTitle("");
      hMON_diff[51] ->SetXTitle("");
      hMON_diff[52] ->SetXTitle("");
      hMON_diff[53] ->SetXTitle("");
      hMON_diff[54] ->SetXTitle("");
      hMON_diff[55] ->SetXTitle("");
      hMON_diff[56] ->SetXTitle("");
      hMON_diff[57] ->SetXTitle("");
      hMON_diff[58] ->SetXTitle("");
      hMON_diff[59] ->SetXTitle("");
      hMON_diff[60] ->SetXTitle("");
      hMON_diff[61] ->SetXTitle("");
      hMON_diff[62] ->SetXTitle("");
      hMON_diff[63] ->SetXTitle("");

      for(int i=0; i<64; i++)
	{ //YKT 23.05
	  char channel_name_temp[256];//YKT 23.05
	  sprintf(channel_name_temp,"%s", hMON_diff[i]->GetXaxis()->GetTitle());//YKT 23.05
	  //
	  sprintf(name,"MON_rate_time_%s_%02d",channel_name_temp,i); //YKT 23.05   
	  hSCALER_TIME[i] = MakeH1I("MON/scaler_rate_time",name,3000,0,3000,"time [s]",2,3); //YKT 23.05 
	  sprintf(name,"MON_rate_spill_%s_%02d",channel_name_temp,i); //YKT 23.05     
	  hSCALER_SPILL[i] = MakeH1I("MON/scaler_rate_spill",name,300,0,300,"spill",2,3); //YKT 23.05 
	  hSCALER_TIME[i] ->SetYTitle(Form("%s [/s]",channel_name_temp));//YKT 23.05            
	  hSCALER_SPILL[i] ->SetYTitle(Form("%s [/spill]",channel_name_temp));        //YKT 23.05        
	  //
	  sprintf(name,"MON_rate_time_short_%s_%02d",channel_name_temp,i); //YKT 23.05
	  hSCALER_TIME_SHORT[i] = MakeH1I("MON/scaler_rate_time_short",name,300,0,300,"time [s]",2,3); //YKT 23.05 
	  sprintf(name,"MON_rate_spill_short_%s_%02d",channel_name_temp,i); //YKT 23.05
	  hSCALER_SPILL_SHORT[i] = MakeH1I("MON/scaler_rate_spill_short",name,30,0,30,"time [s]",2,3); //YKT 23.05   
	  hSCALER_TIME_SHORT[i] ->SetYTitle(Form("%s [/s]",channel_name_temp));//YKT 23.05
	  hSCALER_SPILL_SHORT[i] ->SetYTitle(Form("%s [/spill]",channel_name_temp));	//YKT 23.05
	} //YKT 23.05

    }

  /*   
       for(int i=0;i<8;i++) {
       sprintf(name,"MON_seecalA(%d)",i); 
       hMON_seecalA[i] = MakeH1I("MON/seecalA",name,3600,0,36000,"",2,5);
       sprintf(name,"MON_seecalB(%d)",i); 
       hMON_seecalB[i] = MakeH1I("MON/seecalB",name,3600,0,36000,"",2,5);
       }

       if (ObjWasCreated()) {
       hMON_seecalA[0]->SetXTitle("free triggers");
       hMON_seecalA[1]->SetXTitle("acc. triggers");
       hMON_seecalA[2]->SetXTitle("SE01 old CD");
       hMON_seecalA[3]->SetXTitle("SC01 free");
       hMON_seecalA[4]->SetXTitle("SE01 new CD");
       hMON_seecalA[5]->SetXTitle("IC particles");
       hMON_seecalA[6]->SetXTitle("IC old CD");
       hMON_seecalA[7]->SetXTitle("IC new CD");
       hMON_seecalB[0]->SetXTitle("free triggers");
       hMON_seecalB[1]->SetXTitle("acc. triggers");
       hMON_seecalB[2]->SetXTitle("SE01 old CD");
       hMON_seecalB[3]->SetXTitle("SC01 free");
       hMON_seecalB[4]->SetXTitle("SE01 new CD");
       hMON_seecalB[5]->SetXTitle("IC particles");
       hMON_seecalB[6]->SetXTitle("IC old CD");
       hMON_seecalB[7]->SetXTitle("IC new CD");
       }
  */

  hMON_Pattern = MakeH1I("MON","MON_Pattern",4096,0,4096,"Pattern Unit Channel * 10",2,5,"Counts");
  hMON_PatternH = MakeH1I("MON","MON_Pattern_Hit",16,0,16,"Pattern Unit Channel",2,5,"Counts");

  /*
    hMON_TOE = MakeH1I("MON","MON_TOE",40000,0,4000,"Time after start flat top [ms]",2,5,"Events");
    hMONDtDiff = MakeH1I("MON","MON_DtDiff",10000,0,1000000,"Time between adjacent events [us] bin=100!",2,5,"Events");
    hMON_SC01 = MakeH1I("MON/SC","MON_SC01",1000,0,10000,"Extraction Cycle",2,5,"Free SC01 counts/cycle");
    hMON_SC21 = MakeH1I("MON/SC","MON_SC21",1000,0,10000,"Extraction Cycle",2,5,"Free SC21 counts/cycle");
    hMON_SC41 = MakeH1I("MON/SC","MON_SC41",1000,0,10000,"Extraction Cycle",2,5,"Free SC41 counts/cycle");
    hMON_Trigger = MakeH1I("MON","MON_Trigger",1000,0,10000,"Extraction Cycle",2,5,"Ungated Triggers/1000/10 cycles");
    hMON_LAM = MakeH1I("MON","MON_LAM",1000,0,10000,"Extraction Cycle",2,5,"Gated Triggers(LAMs)/1000/10 cycles");
    hMON_SE01 = MakeH1I("MON","MON_SE01",32765,0,32765,"Time after start [sec]",2,5,"Seetram (Current digitizer)/1000 Counts/sec");
    hMON_ICDE = MakeH1I("MON","MON_ICDE",2048,0,4095,"IC Energy loss [channels]",2,5,"Counts");
    hMON_IC1 = MakeH1I("MON/IC1","MON_IC1",32765,0,32765,"Time after start [sec]",2,5,"IC scaler 1");
    hMON_IC1d = MakeH1I("MON/IC1","MON_IC1d",32765,0,32765,"Time after start [sec]",2,5,"IC scaler 1");
    hMON_IC2 = MakeH1I("MON/IC2","MON_IC2",32765,0,32765,"Time after start [sec]",2,5,"IC scaler 2");
    hMON_IC2d = MakeH1I("MON/IC2","MON_IC2d",32765,0,32765,"Time after start [sec]",2,5,"IC scaler 2");
    hMON_IC3 = MakeH1I("MON/IC3","MON_IC3",32765,0,32765,"Time after start [sec]",2,5,"IC scaler 3");
    hMON_IC3d = MakeH1I("MON/IC3","MON_IC3d",32765,0,32765,"Time after start [sec]",2,5,"IC scaler 3");


    MakePic("MON","Mon",3,1,hMON_SE01,hMON_Trigger,hMON_LAM);
    MakePic("MON","Mon_Deadtime",2,2,hMON_Trigger,hMON_TOE,hMON_LAM,hMON_DtDiff);
    MakePic("MON","Mon_Deadtime",2,2,hMON_Trigger,hMON_TOE,hMON_LAM,hMON_DtDiff);
    MakePic(0,"Seetram", 5,1,hMON_diff[2],  hMON_diff[5],  hMON_diff[10],  hMON_diff[11],  hMON_diff[12]);
    MakePic(0,"Seetram2",5,1,hMON_scaler[2],hMON_scaler[5],hMON_scaler[10],hMON_scaler[11],hMON_scaler[12]);
  */    

}

void TFRSCalibrProc::Create_MW_Hist() 
{
  Float_t lim_xsum[4][2] = {
    {1,8000},  // MW11
    {1,8000},  // MW21
    {1,8000},  // MW22
    {1,8000}};  // MW31


  Float_t lim_ysum[4][2] = {
    {2,8000},
    {2,8000},
    {2,8000},
    {2,8000}};

  for(int i=0;i<4;i++)
    {  // up to MW31
      char fname[100]; 
      sprintf(fname,"MW/MW%s/MW%s", mw_folder_ext1[i],mw_folder_ext2[i]); 

      hMW_AN[i] = MakeH1I_MW(fname,"MW_an",i,1000,1,4096,"Anode (channels)",2,3);
      hMW_XL[i] = MakeH1I_MW(fname,"MW_XL",i,1000,1,4096,"X-left (channels)",2,3);
      hMW_XR[i] = MakeH1I_MW(fname,"MW_XR",i,1000,1,4096,"X-right (channels)",2,3);
      hMW_YU[i] = MakeH1I_MW(fname,"MW_YU",i,1000,1,4096,"Y-up (channels)",2,3);
      hMW_YD[i] = MakeH1I_MW(fname,"MW_YD",i,1000,1,4096,"Y-down (channels)",2,3);

      hMW_XSUM[i] = MakeH1I_MW(fname,"MW_XSUM",i,800,5,8005,"Sum XL+XR (channels)",2,5);
      hMW_YSUM[i] = MakeH1I_MW(fname,"MW_YSUM",i,800,5,8005,"Sum YU+YD (channels)",2,5);

      char title[100];
      if((i==0) || (i>3))
	sprintf(title,"(stiff) X at MW%s [mm] (soft)", mw_folder_ext2[i]);
      else
	sprintf(title,"(soft) X at MW%s [mm] (stiff)", mw_folder_ext2[i]);
      hMW_X[i] = MakeH1I_MW(fname,"MW_X",i,201,-100.5,+100.5,title,2,7);

      sprintf(title,"Y at MW%s [mm]", mw_folder_ext2[i]);
      hMW_Y[i] = MakeH1I_MW(fname,"MW_Y",i,201,-100.5,+100.5,title,2,7);

      char xyname[100];
      sprintf(xyname,"MW_XY%s", mw_name_ext[i]);
      hMW_XY[i] = MakeH2I(fname,xyname,40,-100,100,40,-100,100,"x [mm]","y [mm]",2);

      char condname[40];
      sprintf(condname,"XSUM%s",mw_name_ext[i]);
      cMW_XSUM[i] = MakeWindowCond("MW/XSUM", condname, lim_xsum[i][0], lim_xsum[i][1], hMW_XSUM[i]->GetName());

      sprintf(condname,"YSUM%s",mw_name_ext[i]);
      cMW_YSUM[i] = MakeWindowCond("MW/YSUM", condname, lim_ysum[i][0], lim_ysum[i][1], hMW_YSUM[i]->GetName());
    }

  hMW_SC21x  = MakeH1I("MW","MW_SC21x", 221,-110.5,110.5,"X tracked at SC21 [mm]",2,7);
  hMW_SC22x  = MakeH1I("MW","MW_SC22x", 221,-110.5,110.5,"X tracked at SC22 [mm]",2,7);

  hMW_xAngS2 = MakeH1I("MW/S2/Ang","MW_xAngS2",201,-100.5,100.5,"X-Angles at S2 [mrad]",2,7); 
  hMW_yAngS2 = MakeH1I("MW/S2/Ang","MW_yAngS2",201,-100.5,100.5,"Y-Angles at S2 [mrad]",2,7);
  hMW_xFocS2 = MakeH1I("MW/S2/Foc","MW_xFocS2",201,-100.5,100.5,"X-Position at S2 Focus [mm]",2,7);
  hMW_yFocS2 = MakeH1I("MW/S2/Foc","MW_yFocS2",201,-100.5,100.5,"Y-Position at S2 Focus [mm]",2,7);
  hMW_zxS2   = MakeH1I("MW/S2/Pos","MW_zxS2",350,-100,600,"Distance from TS3QT33 at S2 Focus [cm]",2,7);
  hMW_zyS2   = MakeH1I("MW/S2/Pos","MW_zyS2",350,-100,600,"Distance from TS3QT33 at S2 Focus [cm]",2,7);
  hMW_FocS2  = MakeH2I("MW/S2/Foc","MW_FocS2", 240,-120.,120.,  240,-120.,120.,
		       "X [mm] tracked to S2 focus","Y [mm] tracked to S2 focus", 2);

  for (int i=0;i<4;i++)
    {  // up to MW31
      char fname[100], pname[100];
      sprintf(fname,"MW/MW%s", mw_folder_ext1[i]);
      sprintf(pname,"MW%s", mw_folder_ext2[i]);
      MakePic(fname,pname,2,2,hMW_XL[i],hMW_XR[i],hMW_YU[i],hMW_YD[i]);
    }   
}


//Creation of TPC data & histograms
void TFRSCalibrProc::Create_TPC_Hist()
{

  Float_t lim_csum1[6][2]={{300,1800},{350,1800},{300,1800},{300,1700},{300,2000},{300,2000}};
  Float_t lim_csum2[6][2]={{300,1800},{300,1800},{300,1800},{300,1700},{300,2000},{300,2000}};
  Float_t lim_csum3[6][2]={{300,1800},{300,1840},{300,1800},{300,1700},{300,2000},{300,2000}};
  Float_t lim_csum4[6][2]={{300,1880},{300,1810},{300,1800},{300,1700},{300,2000},{300,2000}};

  for(int i=0;i<6;i++)
    {
      char condname[40];
      sprintf(condname,"TPC/CSUM1%s",tpc_name_ext1[i]);
      //    cTPC_CSUM[i][0]=MakeWindowCond("TPC/CSUM1",condname,lim_csum1[i][0],
      //				   lim_csum1[i][1],"CSUM1");
      cTPC_CSUM[i][0]=MakeWinCond(condname,lim_csum1[i][0],lim_csum1[i][1],"CSUM1");



      sprintf(condname,"CSUM2%s",tpc_name_ext1[i]);
      cTPC_CSUM[i][1]=MakeWindowCond("TPC/CSUM2",condname,lim_csum2[i][0],
				     lim_csum2[i][1],"CSUM2");
      sprintf(condname,"CSUM3%s",tpc_name_ext1[i]);
      cTPC_CSUM[i][2]=MakeWindowCond("TPC/CSUM3",condname,lim_csum3[i][0],
				     lim_csum3[i][1],"CSUM3");
      sprintf(condname,"CSUM4%s",tpc_name_ext1[i]);
      cTPC_CSUM[i][3]=MakeWindowCond("TPC/CSUM4",condname,lim_csum4[i][0],
				     lim_csum4[i][1],"CSUM4");
    }

  for(int i=0;i<6;i++)
    {        
      char fname[100];
      char name[100];
      sprintf(fname,"TPC/%s/",tpc_folder_ext1[i]);
      //(HAPOL-25/03/06) Duplicated histograms for double delay line (0 and 1) in new TPCs

      hTPC_L0[i]=MakeH1I_TPC(fname,"L0",i,4000,5,4005,
			     "Left DL0 ADC (channels)",2,3);
      hTPC_R0[i]=MakeH1I_TPC(fname,"R0",i,4000,5,4005,
			     "Right DL0 ADc (channels)",2,3);
      hTPC_LT0[i]=MakeH1I_TPC(fname,"LT0",i,4000,5,4005,
			      "Left DL0 time (channels)",2,3);
      hTPC_RT0[i]=MakeH1I_TPC(fname,"RT0",i,4000,5,4005,
			      "Right DL0 time(channels)",2,3);
      hTPC_L1[i]=MakeH1I_TPC(fname,"L1",i,4000,5,4005,
			     "Left DL1 ADC (channels)",2,3);
      hTPC_R1[i]=MakeH1I_TPC(fname,"R1",i,4000,5,4005,
			     "Right DL1 ADC (channels)",2,3);
      hTPC_LT1[i]=MakeH1I_TPC(fname,"LT1",i,4000,5,4005,
			      "Left DL1 time (channels)",2,3);
      hTPC_RT1[i]=MakeH1I_TPC(fname,"RT1",i,4000,5,4005,
			      "Right DL1 time(channels)",2,3);
      hTPC_DT[i][0]=MakeH1I_TPC(fname,"DT1",i,4000,5,4005,
				"drift time 1 (channels)",2,3);
      hTPC_DT[i][1]=MakeH1I_TPC(fname,"DT2",i,4000,5,4005,
				"drift time 2(channels)",2,3);
      hTPC_DT[i][2]=MakeH1I_TPC(fname,"DT3",i,4000,5,4005,
				"drift time 3(channels)",2,3);
      hTPC_DT[i][3]=MakeH1I_TPC(fname,"DT4",i,4000,5,4005,
				"drift time 4(channels)",2,3);
      hTPC_A[i][0]=MakeH1I_TPC(fname,"A1",i,4000,5,4005,
			       "anode 1 ADC(channels)",2,3);
      hTPC_A[i][1]=MakeH1I_TPC(fname,"A2",i,4000,5,4005,
			       "anode 2 ADC(channels)",2,3);
      hTPC_A[i][2]=MakeH1I_TPC(fname,"A3",i,4000,5,4005,
			       "anode 3 ADC(channels)",2,3);
      hTPC_A[i][3]=MakeH1I_TPC(fname,"A4",i,4000,5,4005,
			       "anode 4 ADC(channels)",2,3);
      hTPC_CSUM[i][0]=MakeH1I_TPC(fname,"CSUM1",i,4000,-2000,4005,
				  "control sum 1(channels)",2,3);
      hTPC_CSUM[i][1]=MakeH1I_TPC(fname,"CSUM2",i,4000,-2000,4005,
				  "control sum 2(channels)",2,3);
      hTPC_CSUM[i][2]=MakeH1I_TPC(fname,"CSUM3",i,4000,-2000,4005,
				  "control sum 3(channels)",2,3);
      hTPC_CSUM[i][3]=MakeH1I_TPC(fname,"CSUM4",i,4000,-2000,4005,
				  "control sum 4(channels)",2,3);

      hTPC_XRAW0[i]=MakeH1I_TPC(fname,"Xraw0",i,1000,-2000,2000,
				"First delay line  x(ch)",2,3);
      hTPC_XRAW1[i]=MakeH1I_TPC(fname,"Xraw1",i,1000,-2000,2000,
				"Second delay line x(ch)",2,3);
      hTPC_YRAW[i]=MakeH1I_TPC(fname,"Yraw",i,4000,0.0,4000.,
			       "y(ch)",2,3);
      hTPC_X[i]=MakeH1I_TPC(fname,"X",i,800,-100.,100.,
			    "x[mm]",2,3);
      hTPC_Y[i]=MakeH1I_TPC(fname,"Y",i,800,-100.,100.,
			    "y[mm]",2,3);


      sprintf(name,"%s%s",tpc_name_ext1[i],"XY");
      hcTPC_XY[i]=MakeH2I(fname,name, 120,-120.,120., 120,-120.,120.,
			  "X [mm] ","Y [mm] ", 2);

      sprintf(name,"%s%s",tpc_name_ext1[i],"LTRT");
      hTPC_LTRT[i]=MakeH2I(fname,name, 2048,0,4095, 2048,0,4095,
			   "LT [ch]","RT[ch] ", 2);			
      hTPC_DELTAX[i]=MakeH1I_TPC(fname,"x0-x1",i,100,-10.,10.,
				 "x0-x1[mm]",2,3);

    }

  //Calculated positions and angles from tracking

  hTPC_X_S2=MakeH1I_TPC("TPC/S2","S2_X",-1,1000,-100.,100.,
			"x at S2 focus [mm]",2,3);
  hTPC_Y_S2=MakeH1I_TPC("TPC/S2","S2_Y",-1,1000,-100.,100.,
			"y at S2 focus [mm]",2,3);
  hTPC_AX_S2=MakeH1I_TPC("TPC/S2","S2_AX",-1,1000,-100.5,100.5,
			 "angl_x at S2 focus [mrad]",2,3);
  hTPC_AY_S2=MakeH1I_TPC("TPC/S2","S2_AY",-1,1000,-100.5,100.5,
			 "angl_y at S2 focus [mrad]",2,3);

  hTPC_X_S2_target1=MakeH1I_TPC("TPC/S2","S2_X_silicon1",-1,1000,-100.5,100.5,
				"x at Si tracker 1 [mm]",2,3);
  hTPC_Y_S2_target1=MakeH1I_TPC("TPC/S2","S2_Y_silicon1",-1,1000,-100.5,100.5,
				"y at Si tracker 1 [mm]",2,3);
  hTPC_XY_S2_target1=MakeH2I("TPC/S2","S2_XY_silicon1",1000,-100.5,100.5,1000,-100.5,100.5,
			     "x at Si tracker 1 [mm]","y at Si tracker 1 [mm]",2);


  hTPC_X_S4=MakeH1I_TPC("TPC/S4","S4_X",-1,1000,-100.5,100.5,
			"x at S4 focus [mm]",2,3);
  hTPC_Y_S4=MakeH1I_TPC("TPC/S4","S4_Y",-1,1000,-100.5,100.5,
			"y at S4 focus [mm]",2,3);
  hTPC_AX_S4=MakeH1I_TPC("TPC/S4","S4_AX",-1,1000,-100.5,100.5,
			 "angl_x at S4 [mrad]",2,3);
  hTPC_AY_S4=MakeH1I_TPC("TPC/S4","S4_AY",-1,1000,-100.5,100.5,
			 "angl_y at S4 [mrad]",2,3);
  hTPC_X_S4_target2=MakeH1I_TPC("TPC/S4","S4_X_OTPC",-1,1000,-100.5,100.5,
				"x at OTPC entrance [mm]",2,3);
  hTPC_Y_S4_target2=MakeH1I_TPC("TPC/S4","S4_Y_OTPC",-1,1000,-100.5,100.5,
				"y at OTPC entrance [mm]",2,3);
  //  hTPC_X_S4_target1d=MakeH1I_TPC("TPC/S4","S4_X_target1d",-1,1000,-100.5,100.5,
  //			"x at S4 at H2 target [mm]",2,3);
  //  hTPC_Y_S4_target1d=MakeH1I_TPC("TPC/S4","S4_Y_target1d",-1,1000,-100.5,100.5,
  //			"y at S4 at H2 target [mm]",2,3);

  hTPC_XY_S4_target2=MakeH2I("TPC/S4","S4_XY_OTPC",1000,-100.5,100.5,1000,-100.5,100.5,
			     "x at OTPC entrance [mm]","y at OTPC entrance [mm]",2);
  //  hTPC_XY_S4_target1d=MakeH2I("TPC/S4","S4_XY_target1d",1000,-100.5,100.5,1000,-100.5,100.5,
  //			"x-S4 at H2 target [mm]","y-S4 at H2 target [mm]",2);
  hTPC_XAX_S4=MakeH2I("TPC/S4","angle_vs_x_S4", 400,-100.,100., 250,-25.0,25.0,
		      "X at S4 [mm] ","x angle [mrad] ", 2);  
  hTPC_XS4_AX_S2 =MakeH2I("TPC/S4","angle_s2_at vs x_S4", 400,-100.,100., 250,-25.0,25.0,
			  "X at S4 [mm] ","angle(x) at S2 [mrad] ", 2);


  //  hTPC_X_S4_target2d=MakeH1I_TPC("TPC/S4","S4_X_target2d",-1,1000,-100.5,100.5,
  //			"x at S4 at target 2 [mm]",2,3);
  //  hTPC_Y_S4_target2d=MakeH1I_TPC("TPC/S4","S4_Y_target2d",-1,1000,-100.5,100.5,
  //			"y at S4 at target 2 [mm]",2,3);
  //  hTPC_AX_S4_target2=MakeH1I_TPC("TPC/S4","S4_AX_target2",-1,1000,-100.5,100.5,
  //			"angl_x at S4 at target 2 [mrad]",2,3);
  //  hTPC_AY_S4_target2=MakeH1I_TPC("TPC/S4","S4_AY_target2",-1,1000,-100.5,100.5,
  //			"angl_y at S4 at target 2 [mrad]",2,3);
  //  hTPC_XY_S4_target2d=MakeH2I("TPC/S4","S4_XY_target2d",1000,-100.5,100.5,1000,-100.5,100.5,
  //			"x-S4 at target 2 [mm]","y at S4 at target 2 [mm]",2);

  hTPC_SC21x  = MakeH1I_TPC("TPC/Detector","TPC_SC21x",-1, 221,-110.5,110.5,"X tracked at SC21 [mm]",2,3);
  hTPC_SC21y  = MakeH1I_TPC("TPC/Detector","TPC_SC21y",-1, 221,-110.5,110.5,"Y tracked at SC21 [mm]",2,3);
  hTPC_SC41x  = MakeH1I_TPC("TPC/Detector","TPC_SC41x",-1, 221,-110.5,110.5,"X tracked at SC41 [mm]",2,3);
  hTPC_SC41y  = MakeH1I_TPC("TPC/Detector","TPC_SC41y",-1, 221,-110.5,110.5,"Y tracked at SC41 [mm]",2,3);

  for(int i=0;i<6;i++){
    char name[100];
    sprintf(name,"TPC/TPC%d",i+1);
    MakePic(name,"Amplitudes",4,2,hTPC_A[i][0],hTPC_A[i][1],hTPC_A[i][2],hTPC_A[i][3],hTPC_L0[i],hTPC_R0[i],hTPC_L1[i],hTPC_R1[i]);
  }
}

// Creation of Si data & histograms
void TFRSCalibrProc::Create_SI_Hist()
{
  hsi_e1 = MakeH1I("Si/e1","Si_1 DU energy [keV]",5000,0,10000);
  hsi_e2 = MakeH1I("Si/e2","Si_2 DU energy [keV]",5000,0,10000);
  hsi_e3 = MakeH1I("Si/e3","Si_3 gate energy [keV]",5000,0,10000);
  hsi_e4 = MakeH1I("Si/e4","Si_4 sled energy [keV]",5000,0,10000);

  hsi_e1Trig = MakeH1I("Si/e1Trig","Si_1 DU energy with self trigger  [keV]",5000,0,10000);
  hsi_e2Trig = MakeH1I("Si/e2Trig","Si_2 DU energy  with self trigger [keV]",5000,0,10000);
  hsi_e3Trig = MakeH1I("Si/e3Trig","Si_3 gate energy with self trigger [keV]",5000,0,10000);
  hsi_e4Trig = MakeH1I("Si/e4Trig","Si_4 sled energy  with self trigger [keV]",5000,0,10000);

}
// Creation of Channeltron data & histograms
void TFRSCalibrProc::Create_CT_Hist()
{
  hct_all = MakeH1I("Ct","Ct_all",5000,0,10000);
  hct_trigger_DU = MakeH1I("Ct","Ct_trigger_DU",5,0,5);
  hct_trigger_SY = MakeH1I("Ct","Ct_trigger_SY",5,0,5);
  hct_DU = MakeH1I("Ct","Ct_DU",5000,0,10000);
  hct_SY = MakeH1I("Ct","Ct_SY",5000,0,10000);
}

// Creation of Electron Current data & histograms
void TFRSCalibrProc::Create_ElCurrent_Hist()
{
  helcurrent = MakeH1I("El Current","El Current",5000,0,10000);
}

void TFRSCalibrProc::InitProcessor()
{
  // MW initialization 

  fbFirstEvent = kTRUE; 
  for (int i=0;i<32;i++)
    scaler_save[i] = 0; 
  firstsec = 0;
  firsttenthsec = 0;
  firsthundrethsec = 0;	//mik
  firstcycle = 0;
  dtime = 0.;
  dt_last = 0.;
  dt_diff = 0.;

  scalercycle_Sec=0;
  scalercycle_TenthSec=0;
  scalercycle_HundrethSec=0;
  scalercycle_Sec=0;
  scalercycle_Cycle=0;
  scalercycle_Seetram=0;

   
  // MON initialization
  focx_s2m = 0.;
  focy_s2m = 0.;
  angle_x_s2m = 0.;
  angle_y_s2m = 0.;
}

void TFRSCalibrProc::Process_MON_Analysis(const TFRSSortEvent& src, TFRSCalibrEvent& tgt)
{

  //----- from here added by YKT 23.05 --------//   
  /////// 
  ///////  scaler_time_count,  scaler_spill_count,  scaler_time_check_last,   scaler_spill_check_last,  scaler_increase_event[64]     UInt_t
  ///////  check_increase_time[i]   check_increase_spill[i],   scaler_last_event[i]    UInt_t 

  // special channels for normalization
  int scaler_channel_spill_all[2] = {12, 5}; double normalization_factor_spill_all[2] = {1.0,1.0}; //   YKT 23.05
  int scaler_channel_time_all[2]  = {4,1}; double normalization_factor_time_all[2] = {10.0,100.}; //  YKT 23.05 
  Long64_t nmax_V830 = 0x100000000;

  int index_flag = -1;
  if(tgt.EventFlag==0x100)
    index_flag=0;
  else if(tgt.EventFlag==0x200)
    index_flag=1;
  else
    std::cout<<"E> EventFlag unknown !"<<tgt.EventFlag<<std::endl;

  int scaler_channel_spill = scaler_channel_spill_all[index_flag];
  double normalization_factor_spill = normalization_factor_spill_all[index_flag]; //   YKT 23.05
  int scaler_channel_time  = scaler_channel_time_all[index_flag];
  double normalization_factor_time = normalization_factor_time_all[index_flag] ; //  YKT 23.05 

  UInt_t tempCurrentTime = index_flag == 0 ? src.sc_long[scaler_channel_time] : src.sc_long2[scaler_channel_time];
  //----initialize values defined in this file---
  if(1==check_first_event[index_flag]) // if first event has 0 (for all channels) >> use next event as a first event 
    {
      //      std::cout<<"Init Scaler :"<<index_flag<<" "<<tgt.EventFlag<<" | "<<check_first_event[index_flag]<<" "<<tempCurrentTime<<"\n";
      if(0!=tempCurrentTime)
	{
	  //std::cout<<" +--> set initial values ";
	  scaler_time_count[index_flag]  = 0; //UInt_t
	  scaler_spill_count[index_flag] = 0; //UInt_t
	  scaler_time_check_last[index_flag] = 0;//UInt_t
	  scaler_spill_check_last[index_flag] = 0;//UInt_t
	  for(size_t i=0; i<32; i++)
	    {
	      check_increase_time[i]   =0;//UInt_t
	      check_increase_spill[i]  =0;//UInt_t
	      scaler_increase_event[i] =0;//UInt_t
	      scaler_last_event[i] = static_cast<Long64_t>(src.sc_long[i]);//UInt_t
	    }
	  for(size_t i=32;i<64;++i)
	    {
	      check_increase_time[i]   =0;//UInt_t
	      check_increase_spill[i]  =0;//UInt_t
	      scaler_increase_event[i] =0;//UInt_t
	      scaler_last_event[i] = static_cast<Long64_t>(src.sc_long2[i-32]);//UInt_t
	    }
	  //std::cout<<" done !\n";
	}
    }
  for(int i=0; i<64; i++)
    scaler_increase_event[i]=0;  
  
  //------------ scaler_increase_event[i] and scaler_last_event[i] -----------
  //////
  // sometimes V830 data from sort is empty (all 0)
  // in such case, we skip updating { scaler_increase_event[i] and scaler_last_event[i] }
  if(0!=tempCurrentTime)
    {
      if(0==check_first_event[index_flag])
	{
	  if(index_flag==0)
	    {
	      for(size_t i=0; i<32; i++)
		{
		  if(static_cast<Long64_t>(src.sc_long[i]) >= scaler_last_event[i])
		    scaler_increase_event[i] = src.sc_long[i] - scaler_last_event[i];
		  else
		    {
		      //printf("src.sc_long[i], scaler_last_event[i]:%d %d\n", src.sc_long[i] , scaler_last_event[i]);
		      scaler_increase_event[i] =  src.sc_long[i]  + nmax_V830 - scaler_last_event[i];
		    }
		  scaler_last_event[i]     = src.sc_long[i];
		}
	    }
	  if(index_flag==1)
	    {
	      for(size_t i=32; i<64; i++)
		{
		  if(static_cast<Long64_t>(src.sc_long2[i-32]) >= scaler_last_event[i])
		    scaler_increase_event[i] = src.sc_long2[i-32] - scaler_last_event[i];
		  else
		    {
		      //printf("src.sc_long[i], scaler_last_event[i]:%d %d\n", src.sc_long[i] , scaler_last_event[i]);
		      scaler_increase_event[i] =  src.sc_long2[i-32]  + nmax_V830 - scaler_last_event[i];
		    }
		  scaler_last_event[i]     = src.sc_long2[i-32];
		}	  
	    }
	}
    }
  //-----switch off initial event check---
  if(1==check_first_event[index_flag] && (0!=tempCurrentTime)) // if first event has 0 (for all channels) >> use next event as a first event
    { 
      check_first_event[index_flag] = 0;
    }


  // add {increase from last event} to the counters.
  if(0!=tempCurrentTime)
    {
      for(int i=0; i<64; i++)
	{
	  check_increase_spill[i] += scaler_increase_event[i];  
	  check_increase_time[i]  += scaler_increase_event[i];
	}
    }
 
  // integrated count from the beginning
  if(0!=tempCurrentTime)
    {
      scaler_time_count[index_flag]  += scaler_increase_event[scaler_channel_time]; //
      scaler_spill_count[index_flag] += scaler_increase_event[scaler_channel_spill];//
    }
  
  int scaler_time_check  = scaler_time_count[index_flag]/((int)normalization_factor_time);
  int scaler_spill_check = scaler_spill_count[index_flag];

  //  printf("scaler_time_count = %d, src.sc_long[4]=%d, src.sc_long[3]=%d, \n",scaler_time_count, src.sc_long[4], src.sc_long[3]);
 

  // when scaler_time_check is increased
  if( 0<(scaler_time_check - scaler_time_check_last[index_flag]) )
    {
      //   printf("scaler_time_check = %d, scaler_time_check_last = %d \n",scaler_time_check,scaler_time_check_last);
      if(10<(scaler_time_check - scaler_time_check_last[index_flag]))
	{
	  //printf("scaler_time_check - scaler_time_check_last = %d ...\n",);
	  std::cout<<"EventFlag"<<tgt.EventFlag<<"|"<<index_flag<<"scaler_time_check - scaler_time_check_last = "<<(scaler_time_check - scaler_time_check_last[index_flag])<<"...\n";
	} 
      for(int i=0; i<64; i++)
	{
	  int x_bin = (scaler_time_check % 3000);
	  int x_bin_short = (scaler_time_check % 300);
	  int y_set;
	  if(check_increase_time[i]>0)
	    {
	      y_set =  (int)(normalization_factor_time*((Float_t)( check_increase_time[i] ))/((Float_t)( check_increase_time[scaler_channel_time] ))); 
	    }
	  else
	    {
	      y_set = 0;
	    }
	  hSCALER_TIME[i]  -> SetBinContent(1+x_bin,y_set);
	  hSCALER_TIME[i]  -> SetBinContent(1+(x_bin+100)%3000,0);
	  hSCALER_TIME[i]  -> SetBinContent(1+(x_bin+101)%3000,0);
	  hSCALER_TIME[i]  -> SetBinContent(1+(x_bin+102)%3000,0);
	  hSCALER_TIME[i]  -> SetBinContent(1+(x_bin+103)%3000,0);
	  hSCALER_TIME[i]  -> SetBinContent(1+(x_bin+104)%3000,0);
	  hSCALER_TIME[i]  -> SetBinContent(1+(x_bin+105)%3000,0);
	  hSCALER_TIME_SHORT[i]  -> SetBinContent(1+x_bin_short,y_set);
	  hSCALER_TIME_SHORT[i]  -> SetBinContent(1+(x_bin_short+21)%300,0);
	  hSCALER_TIME_SHORT[i]  -> SetBinContent(1+(x_bin_short+22)%300,0);
	  hSCALER_TIME_SHORT[i]  -> SetBinContent(1+(x_bin_short+23)%300,0);
	  hSCALER_TIME_SHORT[i]  -> SetBinContent(1+(x_bin_short+24)%300,0);
	  hSCALER_TIME_SHORT[i]  -> SetBinContent(1+(x_bin_short+25)%300,0);
	}
      for(int i=0; i<64; i++)
	{
	  check_increase_time[i]=0;
	}//reset
      scaler_time_check_last[index_flag] = scaler_time_check;  
    }

  // when scaler_time_check is increased
  if( 0<(scaler_spill_check - scaler_spill_check_last[index_flag]) )
    {
      if(10<(scaler_spill_check - scaler_spill_check_last[index_flag]))
	{
	  //printf("scaler_spill_check - scaler_spill_check_last = %d ...\n",(scaler_spill_check - scaler_spill_check_last));
	  std::cout<<"EventFlag"<<tgt.EventFlag<<"|"<<index_flag<<"scaler_spill_check - scaler_spill_check_last = "<<(scaler_spill_check - scaler_spill_check_last[index_flag])<<"...\n";
	}
      for(int i=0; i<64; i++)
	{
	  int x_bin = (scaler_spill_check % 300);
	  int x_bin_short = (scaler_spill_check % 30);
	  int y_set;
	  if(check_increase_spill[i]>0)
	    {
	      y_set =  (int)(normalization_factor_spill*((Float_t)( check_increase_spill[i] ))/((Float_t)( check_increase_spill[scaler_channel_spill] )));
	    }
	  else
	    {
	      y_set = 0;
	    }
	  hSCALER_SPILL[i]  -> SetBinContent(1+x_bin,y_set);
	  hSCALER_SPILL[i]  -> SetBinContent(1+(x_bin+10)%300,0);
	  hSCALER_SPILL[i]  -> SetBinContent(1+(x_bin+11)%300,0);
	  hSCALER_SPILL[i]  -> SetBinContent(1+(x_bin+12)%300,0);
	  hSCALER_SPILL[i]  -> SetBinContent(1+(x_bin+13)%300,0);
	  hSCALER_SPILL[i]  -> SetBinContent(1+(x_bin+14)%300,0);
	  hSCALER_SPILL[i]  -> SetBinContent(1+(x_bin+15)%300,0);
	  hSCALER_SPILL_SHORT[i]  -> SetBinContent(1+x_bin_short,y_set);
	  hSCALER_SPILL_SHORT[i]  -> SetBinContent(1+(x_bin_short+1)%30,0);
	  hSCALER_SPILL_SHORT[i]  -> SetBinContent(1+(x_bin_short+2)%30,0);
	  hSCALER_SPILL_SHORT[i]  -> SetBinContent(1+(x_bin_short+3)%30,0);
	  hSCALER_SPILL_SHORT[i]  -> SetBinContent(1+(x_bin_short+4)%30,0);
	}
      //
      if(index_flag==0)
	{
	  check_total_sc21 += check_increase_spill[7];
	  check_total_sc41 += check_increase_spill[8];
	  check_total_seetram += check_increase_spill[10];
      //printf("Total SC41 = %d,  Total SC21 = %d, Total SEETRAM = %d \n",check_total_sc41,check_total_sc21,check_total_seetram);
	}
      if(index_flag==1)
	{
	  check_total_mrtof_start += check_increase_spill[38];
	  check_total_mrtof_stop += check_increase_spill[39];
	    
	}
      for(int i=0; i<64; i++)
	{
	  check_increase_spill[i]=0;
	}//reset                                                                                                                                                       
      scaler_spill_check_last[index_flag] = scaler_spill_check;
      //           
    }
  //----- up to here added by YKT 23.05 --------// 

   




   
  UInt_t first[64];
  //  Int_t  first[64]; 

  if (fbFirstEvent)
    { //
      for (int i=0;i<32;i++)
	{
	  //std::cout <<"In first event loop"<<std::endl ; 
	  scaler_save[i] = src.sc_long[i];
	  //		 std::cout <<"  "<<scaler_save[i] <<"  "<<src.sc_long[i]<<std::endl ; 
	  first[i]=src.sc_long[i];
	}

      for (int i=32;i<64;i++)
	{
	  scaler_save[i] = src.sc_long2[i-32];
	  first[i]=src.sc_long2[i-32];
	}

      //    std::cout <<"1Hz  "<<src.sc_long[3]<<std::endl;
      //    std::cout <<"10Hz "<<src.sc_long[4]<<std::endl;  

      if(src.sc_long[3]!=0)
	{     
	  firstsec = src.sc_long[3]; // TIME //
	  firsttenthsec = src.sc_long[4];
	  firsthundrethsec = src.sc_long[25];	//mik
	  firstcycle = src.sc_long[2];
	  firstseetram= src.sc_long[10]; // new SE01
	  //      firstseetram= src.sc_long[9]; // old SE01

	  fbFirstEvent = kFALSE; 
	  return;  // skip event   
	}
    }


  //  Int_t tgt.mon_inc[64];

  Double_t over_scale=  4000000000.;
  //  printf("%15f",over_scale);

  // for 1st module
  for (int i=0;i<32;i++)
    if(0!=src.sc_long[scaler_channel_time])
      {
	Int_t overload = 0; 
	//  if(src.sc_long[i]!=0){            
	if ( scaler_save[i] > static_cast<Long64_t>(src.sc_long[i]))
	  {
	    if (src.trigger==12 || src.trigger==13)
	      continue ;

	    //std::cout <<"Hey I got overloaded !!! channel "<<i<<std::endl ; 
	    //std::cout <<scaler_save[i] <<"  "<<src.sc_long[i]<<std::endl ;
	    //std::cout <<"trigger is : "<<src.trigger<<std::endl ; 
	    //scaler_save[i] = scaler_save[i] - 4294967295;
	    tgt.mon_inc[i] = static_cast<Long64_t>(src.sc_long[i])+4294967295 - scaler_save[i]; //
	    //scaler_save[i] = (Long64_t)src.sc_long[i]);
	    overload = 1;
	  }
	else
	  tgt.mon_inc[i] = static_cast<Long64_t>(src.sc_long[i]) - scaler_save[i]; //
	 
	scaler_save[i] = static_cast<Long64_t>(src.sc_long[i]);
	//
	//if(overload != 0)
	//std::cout<<"case overload"<<std::endl;
	if (tgt.mon_inc[i]<0)
	  {
	    //std::cout <<"  "<<i<<"  "<<tgt.mon_inc[i]<<"  "<<src.sc_long[i]<<"  "<<scaler_save[i]<<" | "<<overload<<std::endl ; 
	    std::cout <<"!> tgt.mon_inc["<<i<<"] -> WRONG"<<std::endl ;
	  } 


	/* save the current value so that it can be used next time around... */
	//if(tgt.mon_inc[i]>4000000000)tgt.mon_inc[i]=0; 
	//  if(tgt.mon_inc[i]>over_scale)tgt.mon_inc[i]=0; 
      
      } 

  // for 2nd module
  for (int i=32;i<64;i++)
    if(0!=src.sc_long[scaler_channel_time])
      {
	Int_t overload = 0;
	if (scaler_save[i] > src.sc_long2[i-32])
	  { 
	    //scaler_save[i] = scaler_save[i] - 4294967295;
	    tgt.mon_inc[i] = static_cast<Long64_t>(src.sc_long2[i-32]) + 4294967295 - scaler_save[i];
	    overload =1;
	  }
	else
	  tgt.mon_inc[i] = src.sc_long2[i-32] - scaler_save[i];
	   
	
	if(tgt.mon_inc[i] > over_scale)
	  tgt.mon_inc[i]=0;

	//save the current value so that it can be used next time around... 
	scaler_save[i] = src.sc_long2[i-32];

      
      }

  //Store scalers for later
  tgt.freeTrig=tgt.mon_inc[0];
  tgt.acptTrig=tgt.mon_inc[1];
  
  
  if(src.sc_long[3]!=0)
    {
      if(src.sc_long[3]<firstsec)
	++scalercycle_Sec;
      //firstsec = src.sc_long[3];	  
      
      if(src.sc_long[4]<firsttenthsec)
	++scalercycle_TenthSec;
	
      if(src.sc_long2[9]<firsthundrethsec)		//mik
	++scalercycle_HundrethSec;
	
      if(src.sc_long[2]<firstcycle)
	++scalercycle_Cycle;


      tgt.seconds = src.sc_long[3] + 4294967295*scalercycle_Sec -firstsec;
      tgt.tenthsecs = src.sc_long[4] + 4294967295*scalercycle_TenthSec - firsttenthsec;
      tgt.hundrethsecs = src.sc_long2[9] + 4294967295*scalercycle_HundrethSec - firsthundrethsec;  //mik
      tgt.extraction_cycle = src.sc_long[2] + 4294967295*scalercycle_Cycle - firstcycle;
    }

  hTimeStamp->Fill((Int_t)((Double_t)src.timestamp / 5E7) % 3600);
  hTimeStampZoom->Fill((Int_t)((Double_t)src.timestamp / 5E7) % 60);
  hTimeSpill->Fill(src.timespill);
  hMON_Pattern->Fill(src.pattern);

  for(int i=0;i<16;i++)
    {
      tgt.coin[i]=src.pattern>>i & 0x0001;
      //   printf("%d %d %d %d\n",src.pattern,i,tgt.coin[i],src.pattern>>i);
      if(tgt.coin[i]==1)hMON_PatternH->Fill(i);
    }

  // printf("%d\n",src.pattern);

  // for 1st module
  for(int i=0;i<32;i++)
    {
      if (tgt.mon_inc[i]>=0)
	hMON_All->SetBinContent(i+1,(UInt_t) tgt.mon_inc[i]);
      
      /* per extraction cycle:  */
      hMON_extr[i]->Fill(tgt.extraction_cycle, tgt.mon_inc[i]);
    
      /* per second:  */
      hMON_scaler[i]->Fill(src.sc_long[i]);
    
      /* NOTE: spec$ is not the same as accu$!!!  */
      hMON_diff[i]->Fill(tgt.seconds, tgt.mon_inc[i]);
      //    hMON_diff[i]->Fill(tgt.tenthsecs, mon_inc[i]);	//mik  //tgt.hundrethsecs
    }

  for(int i=32;i<64;i++)
    {      
      hMON_All2->Fill(i-32,tgt.mon_inc[i]);
      
      /* per extraction cycle:  */
      //hMON_extr[i]->Fill(tgt.extraction_cycle, tgt.mon_inc[i]);
      
      /* per second:  */
      hMON_scaler[i]->Fill(src.sc_long2[i-32]);
      
      /* NOTE: spec$ is not the same as accu$!!!  */
      hMON_diff[i]->Fill(tgt.seconds, tgt.mon_inc[i]);
    } 

  
  /**  for SeeTram calibration purposes:   **/

  /* All spectra defined per tenth of second:  */
  /*  total number of scaler counts:  */
  //  hMON_seecalA[0]->Fill(tgt.tenthsecs, src.sc_long[0]);
  //  hMON_seecalA[1]->Fill(tgt.tenthsecs, src.sc_long[1]);
  //  hMON_seecalA[2]->Fill(tgt.tenthsecs, src.sc_long[9]);
  //  hMON_seecalA[3]->Fill(tgt.tenthsecs, src.sc_long[5]);
  //  hMON_seecalA[4]->Fill(tgt.tenthsecs, src.sc_long[10]);
  //  hMON_seecalA[5]->Fill(tgt.tenthsecs, src.sc_long[21]);
  //  hMON_seecalA[6]->Fill(tgt.tenthsecs, src.sc_long[11]);
  //  hMON_seecalA[7]->Fill(tgt.tenthsecs, src.sc_long[6]);

  /*  differential number of scaler counts:  */
  //  hMON_seecalB[0]->Fill(tgt.tenthsecs, tgt.mon_inc[0]);
  //  hMON_seecalB[1]->Fill(tgt.tenthsecs, tgt.mon_inc[1]);
  //  hMON_seecalB[2]->Fill(tgt.tenthsecs, tgt.mon_inc[9]);
  //  hMON_seecalB[3]->Fill(tgt.tenthsecs, tgt.mon_inc[5]);
  //  hMON_seecalB[4]->Fill(tgt.tenthsecs, tgt.mon_inc[10]);
  //  hMON_seecalB[5]->Fill(tgt.tenthsecs, tgt.mon_inc[21]);
  //  hMON_seecalB[6]->Fill(tgt.tenthsecs, tgt.mon_inc[11]);
  //  hMON_seecalB[7]->Fill(tgt.tenthsecs, tgt.mon_inc[6]);

  //tgt.10HzClock = hMON_scaler[4]->Integral();
}


void TFRSCalibrProc::Process_MW_Analysis(const TFRSSortEvent& src, TFRSCalibrEvent& tgt)
{

  /*   Naming conventions:  index     detector   focus #                     */
  /*                         1         MW11        1                         */
  /*                         2         MW21        2                         */
  /*                         3         MW22                                  */
  /*                         4         MW31        3                         */
  ////////////////////////////////////////////////////////////////////////////
  int max_index = 4; /*  up to S3 */

  for(int i=0;i<max_index;i++)
    {	
      /***************/
      /* Raw Spectra */
      /***************/
      hMW_AN[i]->Fill(src.mw_an[i]);
      hMW_XL[i]->Fill(src.mw_xl[i]);
      hMW_XR[i]->Fill(src.mw_xr[i]);
      hMW_YU[i]->Fill(src.mw_yu[i]);
      hMW_YD[i]->Fill(src.mw_yd[i]);
      
      /********************************************/
      /* Calculate the sum spectra and conditions */
      /********************************************/
      
      /* better test first existence of xl, xr, an before filling of xsum */
      if(src.mw_an[i] && src.mw_xl[i] && src.mw_xr[i])
	{
	  // if(src.mw_xl[i]&&src.mw_xr[i]) {
	  tgt.mw_xsum[i] = 1000+(src.mw_xl[i] - src.mw_an[i]) + (src.mw_xr[i] - src.mw_an[i]);
      
	  // tgt.mw_xsum[i] = (src.mw_xl[i]) + (src.mw_xr[i]); //when an doesn't work
	  hMW_XSUM[i]->Fill(tgt.mw_xsum[i]);
	}

      tgt.b_mw_xsum[i] = cMW_XSUM[i]->Test(tgt.mw_xsum[i]);
      
      /* better test first existence of yu, yd, an before filling of ysum */
      if(src.mw_an[i] && src.mw_yu[i] && src.mw_yd[i])
	{
	  // if(src.mw_yu[i]&&src.mw_yd[i]) {
	  tgt.mw_ysum[i] = 1000+(src.mw_yu[i] - src.mw_an[i]) + (src.mw_yd[i] - src.mw_an[i]);
	  //	tgt.mw_ysum[i] = (src.mw_yu[i]) + (src.mw_yd[i]); //when an doesn't work			
	  hMW_YSUM[i]->Fill(tgt.mw_ysum[i]);
	}
      tgt.b_mw_ysum[i] = cMW_YSUM[i]->Test(tgt.mw_ysum[i]);
    
      /*******************************************************************/
      /* If the signals in x and y are valid, calculate position spectra */
      /*******************************************************************/
      if (tgt.b_mw_xsum[i])
	{
	  //      Int_t r_x = src.mw_xl[i] - src.mw_xr[i];
	  Float_t r_x = src.mw_xl[i] *  mw->gain_tdc[1][i] - src.mw_xr[i] *  mw->gain_tdc[2][i]; //14.09.05 CN+AM
	  tgt.mw_x[i] = mw->x_factor[i] * r_x + mw->x_offset[i];
	  hMW_X[i]->Fill(tgt.mw_x[i]);
	}
    
      if (tgt.b_mw_ysum[i])
	{
	  //      Int_t r_y = src.mw_yd[i] - src.mw_yu[i];
	  Float_t r_y = src.mw_yd[i] *  mw->gain_tdc[4][i] - src.mw_yu[i] *  mw->gain_tdc[3][i]; //14.09.05 CN+AM
	  tgt.mw_y[i] = mw->y_factor[i] * r_y + mw->y_offset[i];
	  hMW_Y[i]->Fill(tgt.mw_y[i]);
	}

    
      if(tgt.b_mw_xsum[i] && tgt.b_mw_ysum[i])
	{
	  hMW_XY[i]->Fill(tgt.mw_x[i], tgt.mw_y[i]);  
	}

    } // for(int i=0;i<max_index;i++)


  /*********************************************/
  /* S2 Angle and transformed position spectra */
  /*********************************************/
  
  Float_t  dist_MW21_MW22  = frs->dist_MW22  - frs->dist_MW21;
  Float_t  dist_MW22_focS2 = frs->dist_focS2 - frs->dist_MW22;
  Float_t  dist_MW22_SC21  = frs->dist_SC21  - frs->dist_MW22;
  Float_t  dist_MW22_SC22  = frs->dist_SC22  - frs->dist_MW22;
  

  if (tgt.b_mw_xsum[1] && tgt.b_mw_xsum[2])
    {
      /* MW21 and MW22 X okay */
      /*  X angle at S2  [mrad]:  */
      tgt.angle_x_s2 = (tgt.mw_x[2]-tgt.mw_x[1])/dist_MW21_MW22*1000.;
      hMW_xAngS2->Fill(tgt.angle_x_s2);

      /*  X at nominal S2 focus:  */
      tgt.focx_s2 = tgt.mw_x[2] + dist_MW22_focS2 * tgt.angle_x_s2/1000.;
      hMW_xFocS2->Fill(tgt.focx_s2);
      
      /*  X at SC21 position:    */
      tgt.sc21_x = tgt.mw_x[2] + dist_MW22_SC21 * tgt.angle_x_s2/1000.;
      hMW_SC21x->Fill(tgt.sc21_x);

      /*  X at SC22 position:    */
      tgt.sc22_x = tgt.mw_x[2] + dist_MW22_SC22 * tgt.angle_x_s2/1000.;
      hMW_SC22x->Fill(tgt.sc22_x);
      

      /* 'real' z-position of S2 X focus (cm) */
      Float_t rh = (tgt.angle_x_s2 - angle_x_s2m);
      if(fabs(rh)>1e-4)
	{
	  tgt.z_x_s2 = ((focx_s2m - tgt.focx_s2)/rh)*100. + frs->dist_focS2/10.;  
	  hMW_zxS2->Fill(tgt.z_x_s2);
	}
    
      /* keep values for next event */
      focx_s2m = tgt.focx_s2; 
      angle_x_s2m = tgt.angle_x_s2;
    }
  
  if (tgt.b_mw_ysum[1] && tgt.b_mw_ysum[2])
    {
      /* MW21 and MW22 Y okay */
      /*  Y angle at S2 [mrad]:   */
      tgt.angle_y_s2 = (tgt.mw_y[2] - tgt.mw_y[1])/dist_MW21_MW22*1000.;
      hMW_yAngS2->Fill(tgt.angle_y_s2);
      
      /*  Y at nominal S2 focus:  */
      tgt.focy_s2 = tgt.mw_y[2] + dist_MW22_focS2 * tgt.angle_y_s2/1000.;
      hMW_yFocS2->Fill(tgt.focy_s2);

      /* 'real' z-position of S2 Y focus (cm) */
      Float_t rh = (tgt.angle_y_s2 - angle_y_s2m);
      if(fabs(rh)>1.e-4)
	{
	  tgt.z_y_s2 = ((focy_s2m - tgt.focy_s2)/rh)*100. + frs->dist_focS2/10.;  
	  hMW_zyS2->Fill(tgt.z_y_s2);
	}
      
      /* keep values for next event */
      focy_s2m = tgt.focy_s2; 
      angle_y_s2m = tgt.angle_y_s2; 
    }
  
  if (tgt.b_mw_ysum[1] && tgt.b_mw_ysum[2] && tgt.b_mw_xsum[1] && tgt.b_mw_xsum[2])
    hMW_FocS2->Fill(tgt.focx_s2,tgt.focy_s2); 

}


//////////////////////////////////////////////////////////////////
///  ***********TPC Analysis*************************** //////////
void TFRSCalibrProc::Process_TPC_Analysis(const TFRSSortEvent& src, TFRSCalibrEvent& tgt)
{


  for(int i=0;i<6;i++)
    {  
      if (bDrawHist)
	{
	  hTPC_L0[i]->Fill(src.tpc_l[i][0]);
	  hTPC_R0[i]->Fill(src.tpc_r[i][0]);
	  hTPC_L1[i]->Fill(src.tpc_l[i][1]);
	  hTPC_R1[i]->Fill(src.tpc_r[i][1]);
	  hTPC_LT0[i]->Fill(src.tpc_lt[i][0]);
	  hTPC_RT0[i]->Fill(src.tpc_rt[i][0]);
	  hTPC_LT1[i]->Fill(src.tpc_lt[i][1]);
	  hTPC_RT1[i]->Fill(src.tpc_rt[i][1]);
	  
	  hTPC_LTRT[i]->Fill(src.tpc_lt[i][0],src.tpc_rt[i][1]);
	}
      int count =0;
      Int_t r_y = 0;
      
      int countx =0;
      Int_t r_x0 = 0;
      Int_t r_x1 = 0;
      
      tgt.tpc_y[i] = 0;
      tgt.tpc_x[i] = 9999;

      for(int j=0;j<4;j++)
	{
	  if (bDrawHist)
	    {
	      hTPC_DT[i][j]->Fill(src.tpc_dt[i][j]);
	      hTPC_A[i][j]->Fill(src.tpc_a[i][j]);
	    }

	  //// calculate control sums
	  if(j < 2)
	    tgt.tpc_csum[i][j] = (src.tpc_lt[i][0] + src.tpc_rt[i][0]- 2*src.tpc_dt[i][j]);
	  else
	    tgt.tpc_csum[i][j] = (src.tpc_lt[i][1] + src.tpc_rt[i][1]- 2*src.tpc_dt[i][j]);


	  //      if((src.de_42l>230&&src.de_42l<450)||(src.de_42r>540&&src.de_42r<750)){
	  if (bDrawHist) 
	    hTPC_CSUM[i][j]->Fill(tgt.tpc_csum[i][j]);
	  tgt.b_tpc_csum[i][j] = cTPC_CSUM[i][j]->Test(tgt.tpc_csum[i][j]);
	  //      }
	  
	  if(src.tpc_lt[i][0]==0 && src.tpc_rt[i][0]==0 && j<2)
	    tgt.b_tpc_csum[i][j]=0;
      
	  if(src.tpc_lt[i][1]==0 && src.tpc_rt[i][1]==0 && j>1)
	    tgt.b_tpc_csum[i][j]=0;
      
	  if (tgt.b_tpc_csum[i][j])
	    {
	      r_y += src.tpc_dt[i][j];
	      tgt.tpc_y[i] += tpc->y_factor[i][j]*src.tpc_dt[i][j] + tpc->y_offset[i][j];
	      count++;
	    }
	}

      if (tgt.b_tpc_csum[i][0] || tgt.b_tpc_csum[i][1])
	{
	  r_x0 =  src.tpc_lt[i][0]-src.tpc_rt[i][0];
	  //          r_x0 =  src.tpc_lt[i][0];
	  
	  tgt.tpc_x[i]=tpc->x_factor[i][0]*r_x0 + tpc->x_offset[i][0];
	  countx++;
	}
      
      if (tgt.b_tpc_csum[i][2] || tgt.b_tpc_csum[i][3])
	{
	  r_x1 =  src.tpc_lt[i][1]-src.tpc_rt[i][1];
	  if (countx == 0)
	    tgt.tpc_x[i]=tpc->x_factor[i][1]*r_x1 + tpc->x_offset[i][1];
	  else
	    tgt.tpc_x[i]+=tpc->x_factor[i][1]*r_x1 + tpc->x_offset[i][1];
	  countx++;
	}

      tgt.b_tpc_xy[i] = kFALSE;  // HWE

      if (countx > 0)
	{
	  tgt.tpc_y[i] = tgt.tpc_y[i]/count;
	  tgt.tpc_x[i] = tgt.tpc_x[i]/countx;
	  //Int_t r_x=src.tpc_lt[i]-src.tpc_rt[i];
	  //tgt.tpc_x[i]=tpc->x_factor[i]*r_x + tpc->x_offset[i];
	  
	  //      if(r_x0<-40)
	  
	  if (bDrawHist)
	    {
	      hTPC_XRAW0[i]->Fill(r_x0);
	      hTPC_XRAW1[i]->Fill(r_x1);
	      hTPC_YRAW[i]->Fill(r_y/count); 
	      //	      if(tgt.coin[15]==1){
	      hTPC_X[i]->Fill(tgt.tpc_x[i]);
	      hTPC_Y[i]->Fill(tgt.tpc_y[i]);
	      hcTPC_XY[i]->Fill(tgt.tpc_x[i],tgt.tpc_y[i]);
	      //    	 }
	    }
	  tgt.b_tpc_xy[i] = kTRUE;
	}
      
      if(countx>1)
	{
	  float x0=tpc->x_factor[i][0]*r_x0 + tpc->x_offset[i][0];
	  float x1=tpc->x_factor[i][1]*r_x1 + tpc->x_offset[i][1];
	  if (bDrawHist)
	    hTPC_DELTAX[i]->Fill(x0-x1);
	}    
    }


  // *****Relative distances for tracking (S388)
  //  Float_t dist_TPC1_TPC2 = 1.;         
  //distances at S2 for exp s388 measured 'by eye' (i.e. not very precise)! AE, 8.8.2012
  // Float_t dist_TPC4_target1 = 455.;  // 8.8.12, s388: taget1 is first Si detector at S2 
  //                                    // using: TPC4->chamber= 150 mm, chamber->Si(1)= 305 mm
  // Float_t dist_TPC3_TPC4 = 610.; // exp S417+S411 Oct2014
  // //Float_t dist_TPC3_focS2 = 2860 - 2013 ; //exp S411 Oct2014
  // Float_t dist_TPC3_focS2 = 0. ; //exp S417 Oct2014 focus on TPC21
  // Float_t dist_TPC5_TPC6 = 1595. - 385. ; // Oct.2014, exp s411 
  // Float_t dist_TPC6_focS4  = 2349. - 1595. ; // Oct.2014, exp s411

  Float_t dist_TPC6_target2  = 1015.; // 8.08.12, exp s388 target2= OTPC entrance (check distance)

  Float_t dist_TPC4_target1 = 455.;  // 8.8.12, s388: taget1 is first Si detector at S2 
                                     // using: TPC4->chamber= 150 mm, chamber->Si(1)= 305 mm

  Float_t dist_TPC3_TPC4 = 1782.0- 604.; 
  Float_t dist_TPC3_focS2 = 604.-2165. ; 
  Float_t dist_TPC5_TPC6 = 2063.5 - 382.5 ; 
  Float_t dist_TPC6_focS4  = 2200. - 2063.5 ;


  //Position S2 tracked with TPCs 3 and 4 for focus
  if (tgt.b_tpc_xy[2]&&tgt.b_tpc_xy[3])
    {
      tgt.tpc_angle_x_s2_foc = (tgt.tpc_x[3] - tgt.tpc_x[2])/dist_TPC3_TPC4*1000.;
      tgt.tpc_angle_y_s2_foc = (tgt.tpc_y[3] - tgt.tpc_y[2])/dist_TPC3_TPC4*1000.;
      tgt.tpc_x_s2_foc = -tgt.tpc_angle_x_s2_foc * dist_TPC3_focS2/1000. + tgt.tpc_x[2];
      tgt.tpc_y_s2_foc = -tgt.tpc_angle_y_s2_foc * dist_TPC3_focS2/1000. + tgt.tpc_y[2];
      
      if (bDrawHist)
	{
	  hTPC_X_S2->Fill(tgt.tpc_x_s2_foc);
	  hTPC_Y_S2->Fill(tgt.tpc_y_s2_foc);
	  hTPC_AX_S2->Fill(tgt.tpc_angle_x_s2_foc);
	  hTPC_AY_S2->Fill(tgt.tpc_angle_y_s2_foc);
	}


      //Position S2 tracked with TPCs 3 and 4 to first Si tracker in exp S388
      tgt.tpc_x_s2_target1 = tgt.tpc_angle_x_s2_foc * dist_TPC4_target1/1000. + tgt.tpc_x[3];
      tgt.tpc_y_s2_target1 = tgt.tpc_angle_y_s2_foc * dist_TPC4_target1/1000. + tgt.tpc_y[3];
      
      if (bDrawHist)
	{
	  hTPC_X_S2_target1->Fill(tgt.tpc_x_s2_target1);
	  hTPC_Y_S2_target1->Fill(tgt.tpc_y_s2_target1);
	  hTPC_XY_S2_target1->Fill(tgt.tpc_x_s2_target1,tgt.tpc_y_s2_target1);
	}
    }
  else  tgt.tpc_x_s2_foc= -999 ;
  
  //S4 positions tracked with TPCs 5 and 6
  if (tgt.b_tpc_xy[4]&&tgt.b_tpc_xy[5])
    {
      
      //S4 focus tracked with TPCs 5 and 6 
      tgt.tpc_angle_x_s4 = (tgt.tpc_x[5] - tgt.tpc_x[4])/dist_TPC5_TPC6*1000.;
      tgt.tpc_angle_y_s4 = (tgt.tpc_y[5] - tgt.tpc_y[4])/dist_TPC5_TPC6*1000.;
      tgt.tpc_x_s4 = tgt.tpc_angle_x_s4 * dist_TPC6_focS4/1000. + tgt.tpc_x[5];
      tgt.tpc_y_s4 = tgt.tpc_angle_y_s4 * dist_TPC6_focS4/1000. + tgt.tpc_y[5];
      
      tgt.music1_x1 = tgt.tpc_angle_x_s4 * (frs->dist_MUSIC3+frs->dist_MUSICa1)/1000. + tgt.tpc_x[5];
      tgt.music1_x2 = tgt.tpc_angle_x_s4 * (frs->dist_MUSIC3+frs->dist_MUSICa2)/1000. + tgt.tpc_x[5];
      tgt.music1_x3 = tgt.tpc_angle_x_s4 * (frs->dist_MUSIC3+frs->dist_MUSICa3)/1000. + tgt.tpc_x[5];
      tgt.music1_x4 = tgt.tpc_angle_x_s4 * (frs->dist_MUSIC3+frs->dist_MUSICa4)/1000. + tgt.tpc_x[5];

      tgt.music1_y1 = tgt.tpc_angle_y_s4 * (frs->dist_MUSIC3+frs->dist_MUSICa1)/1000. + tgt.tpc_y[5];
      tgt.music1_y2 = tgt.tpc_angle_y_s4 * (frs->dist_MUSIC3+frs->dist_MUSICa2)/1000. + tgt.tpc_y[5];
      tgt.music1_y3 = tgt.tpc_angle_y_s4 * (frs->dist_MUSIC3+frs->dist_MUSICa3)/1000. + tgt.tpc_y[5];
      tgt.music1_y4 = tgt.tpc_angle_y_s4 * (frs->dist_MUSIC3+frs->dist_MUSICa4)/1000. + tgt.tpc_y[5];


      if (bDrawHist)
	{
	  hTPC_X_S4->Fill(tgt.tpc_x_s4);
	  hTPC_Y_S4->Fill(tgt.tpc_y_s4);
	  hTPC_AX_S4->Fill(tgt.tpc_angle_x_s4);
	  hTPC_AY_S4->Fill(tgt.tpc_angle_y_s4);
	  hTPC_XAX_S4->Fill(tgt.tpc_x_s4,tgt.tpc_angle_x_s4);
	  
	  hTPC_XS4_AX_S2->Fill(tgt.tpc_x_s4,tgt.tpc_angle_x_s2_foc);
	}
      
      //S4 entrance of OTPC tracked with TPCs 5 and 6 for s388
    
      tgt.tpc_x_s4_target2 = tgt.tpc_angle_x_s4 * dist_TPC6_target2/1000. + tgt.tpc_x[5];
      tgt.tpc_y_s4_target2 = tgt.tpc_angle_y_s4 * dist_TPC6_target2/1000. + tgt.tpc_y[5];
    
     
      if (bDrawHist)
	{
	  hTPC_X_S4_target2->Fill(tgt.tpc_x_s4_target2);
	  hTPC_Y_S4_target2->Fill(tgt.tpc_y_s4_target2);
	  hTPC_XY_S4_target2->Fill(tgt.tpc_x_s4_target2,tgt.tpc_y_s4_target2);
	}
    }
  

  /// ***** tracking to musics and scis (S323,410 exp.)

  Float_t dist_TPC3_SC21  = 1375.5;
  Float_t dist_TPC6_SC41  = 510.; //8.09.11 //check AEv

  /*  X position at SC21 position:    */
  //SC21 is before TPC3 and TPC4
  tgt.tpc_sc21_x = tgt.tpc_x[2] - dist_TPC3_SC21 * tgt.tpc_angle_x_s2_foc/1000.;
  hTPC_SC21x->Fill(tgt.tpc_sc21_x);
 

  /* Y position at SC21 */
  tgt.tpc_sc21_y = tgt.tpc_y[2] - dist_TPC3_SC21 * tgt.tpc_angle_y_s2_foc/1000.;
  hTPC_SC21y->Fill(tgt.tpc_sc21_y);

  
  /* X position at SC41 */
  //SC41 is after TPC6
  tgt.tpc_sc41_x  = tgt.tpc_x[5] + dist_TPC6_SC41 * tgt.tpc_angle_x_s4 / 1000.;
  hTPC_SC41x->Fill(tgt.tpc_sc41_x);
  
  /* Y position at SC41 */
  tgt.tpc_sc41_y = tgt.tpc_y[5] + dist_TPC6_SC41 * tgt.tpc_angle_y_s4 /1000.;
  hTPC_SC41y->Fill(tgt.tpc_sc41_y);


}
//////////////////////////////////////////////////////////////////
///  ***********Si Analysis**************************** //////////


void TFRSCalibrProc::Process_SI_Analysis(const TFRSSortEvent& src, TFRSCalibrEvent& tgt)
{

  // Si alpha
  tgt.si_e1 = (si->si_factor1*src.si_adc1)+si->si_offset1;
  tgt.si_e2 = (si->si_factor2*src.si_adc2)+si->si_offset2;
  tgt.si_e3 = (si->si_factor3*src.si_adc3)+si->si_offset3;
  tgt.si_e4 = (si->si_factor4*src.si_adc4)+si->si_offset4;
  tgt.si_e5 = (si->si_factor5*src.si_adc5)+si->si_offset5;

  // fill histogram for first si detector trigger ==3

  hsi_e1->Fill(tgt.si_e1);
  if(src.trigger==3)
    hsi_e1Trig->Fill(tgt.si_e1);

  // fill histogram for second si detector trigger ==4
  //if(src.trigger==3)
  hsi_e2->Fill(tgt.si_e2);
  if(src.trigger==4)
    hsi_e2Trig->Fill(tgt.si_e2);
  // fill histogram for first si detector trigger ==5
  //if(src.trigger==2)
  hsi_e3->Fill(tgt.si_e3);
  if(src.trigger==5)
    hsi_e3Trig->Fill(tgt.si_e3);
  // fill histogram for first si detector trigger ==6
  //if(src.trigger==2)
  hsi_e4->Fill(tgt.si_e4);
  if(src.trigger==6)
    hsi_e4Trig->Fill(tgt.si_e4);
}

//////////////////////////////////////////////////////////////////
///  ***********Channeltron Analysis********************//////////
void TFRSCalibrProc::Process_CT_Analysis(const TFRSSortEvent& src, TFRSCalibrEvent& tgt)
{
  hct_all->Fill(src.ct_signal);

  hct_trigger_DU->Fill(src.ct_trigger_DU);
  hct_trigger_SY->Fill(src.ct_trigger_SY);

  if(src.ct_trigger_DU==kTRUE)
    hct_DU->Fill(src.ct_signal);

  if(src.ct_trigger_SY==kTRUE)
    hct_SY->Fill(src.ct_signal);
  
}

//////////////////////////////////////////////////////////////////
///  ***********Electron Current Analysis********************//////////
void TFRSCalibrProc::Process_ElCurrent_Analysis(const TFRSSortEvent& src, TFRSCalibrEvent& tgt)
{
  helcurrent->Fill(src.ec_signal);
}


ClassImp(TFRSCalibrProc)
