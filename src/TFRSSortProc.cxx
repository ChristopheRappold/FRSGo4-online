#include "TFRSSortProc.h"
#include "TFRSSortEvent.h"
#include "TFRSUnpackEvent.h"

#include "TFRSParameter.h"

//#include  <iostream.h>
#include  <stdio.h>

TFRSSortProc::TFRSSortProc() : TFRSBasicProc("FRSSortProc") 
{
  StartOfSpilTime = -1;
}

TFRSSortProc::TFRSSortProc(const char* name) : TFRSBasicProc(name) 
{ 
  StartOfSpilTime = -1;
  counter = 0;
}

TFRSSortProc::~TFRSSortProc() {
}

Bool_t TFRSSortProc::BuildEvent(TGo4EventElement* output)
{

  TFRSSortEvent* tgt = dynamic_cast<TFRSSortEvent*> (output);
  if (tgt==nullptr)
    return kFALSE;

  tgt->SetValid(kTRUE);  // all events after unpack always accepted

  TFRSUnpackEvent *src = dynamic_cast<TFRSUnpackEvent*> (GetInputEvent());
  if (src==nullptr)
    return kFALSE;
  
  /* now we can assign the parameters according to cabling:  */

  /* ### timestamp: */
  
  //  tgt->ts_id = src->vme0[20][0];
  tgt->ts_word[0] = src->vme0[20][0];
  tgt->ts_word[1] = src->vme0[20][1];
  tgt->ts_word[2] = src->vme0[20][2];
  tgt->ts_word[3] = src->vme0[20][3];

  tgt->timestamp = Long64_t(1)*tgt->ts_word[0] + Long64_t(0x10000)*tgt->ts_word[1] + Long64_t(0x100000000)*tgt->ts_word[2] + Long64_t(0x1000000000000)*tgt->ts_word[3];
  // printf("qtrigger=%d timestamp=%ld \n",src->qtrigger,tgt->timestamp);

  tgt->tsys_word[0] = src->vme0[20][6] ; //s time low word
  tgt->tsys_word[1] = src->vme0[20][7] ; //s time high worid... we do not use it
  tgt->tsys_word[2] = src->vme0[20][8] ; //ms time since the previous s time (ftime routine)

  tgt->systemtime_s = tgt->tsys_word[0] ; 
  tgt->systemtime_ms = 1000*(tgt->systemtime_s)+tgt->tsys_word[2] ; 


  if (src->qtrigger==12)
    {
      StartOfSpilTime = tgt->timestamp; 
      //StartOfSpilTime = 0; 
      StartOfSpilTime2 = tgt->timestamp;
      //StartOfSpilTime2 = 0;  
      // printf("12 spill start at %ld ",StartOfSpilTime);
    }   
  else if (src->qtrigger==13)
    {
      StartOfSpilTime = -1;
    }
  //else                         //changed 170309
  // rest are interesting only if trigger == 1
  //if (src->qtrigger!=1 ) return;             
  
  // calculate time from spill start in sec
  if (StartOfSpilTime>=0) 
    { 
      tgt->timespill = (tgt->timestamp - StartOfSpilTime) / 50000000.;
      //tgt->timespill = 1;
      //printf("timespill= %f \n",tgt->timespill);    
    }
  
  tgt->timespill2 = (tgt->timestamp - StartOfSpilTime2) / 50000000.;
  //tgt->timespill2 = 1; 

  /* ### Pattern ###*/
  tgt->pattern = src->vme0[5][0];
  tgt->trigger = src->qtrigger;
  

  /* ### scalers:  */
  /* these are treated as 32-bit integers directly  */
  for(int i=0;i<32;i++)
    {
      tgt->sc_long[i] = src->vme2scaler[i];    
      tgt->sc_long2[i] = src->vme1[5][i];
    }

  //std::cout<<"1Hz sort,"<<src->vme0[6][3]<<std::endl;       
 
  /* ### TA Ionization Chamber dE:  */
  //  tgt->ic_de = src->vme0[13][16] & 0xfff;
  
    
  /* ### MW anodes:  */
  for(int i=0;i<4;i++)
    tgt->mw_an[i] = src->vme0[8][i] & 0xfff;
    
  /* ### MW cathodes:  */

  /* ----- MW 11 (No. 1) ----- */
  tgt->mw_xr[0] = src->vme0[8][16] & 0xfff;
  tgt->mw_xl[0] = src->vme0[8][17] & 0xfff;
  tgt->mw_yu[0] = src->vme0[8][18] & 0xfff;
  tgt->mw_yd[0] = src->vme0[8][19] & 0xfff;

  /* ----- MW 21 (No. 2) ----- */
  tgt->mw_xr[1] = src->vme0[8][20] & 0xfff;
  tgt->mw_xl[1] = src->vme0[8][21] & 0xfff;
  tgt->mw_yu[1] = src->vme0[8][22] & 0xfff;
  tgt->mw_yd[1] = src->vme0[8][23] & 0xfff;

  /* ----- MW 22 (No. 3) ----- */
  tgt->mw_xr[2] = src->vme0[8][24] & 0xfff;
  tgt->mw_xl[2] = src->vme0[8][25] & 0xfff;
  tgt->mw_yu[2] = src->vme0[8][26] & 0xfff;
  tgt->mw_yd[2] = src->vme0[8][27] & 0xfff;

  /* ----- MW 31 (No. 4) ----- */
  tgt->mw_xr[3] = src->vme0[8][28] & 0xfff;
  tgt->mw_xl[3] = src->vme0[8][29] & 0xfff;
  tgt->mw_yu[3] = src->vme0[8][30] & 0xfff;
  tgt->mw_yd[3] = src->vme0[8][31] & 0xfff;



  //////////////////////////////////////
  // TPC part                         //
  //                                  //
  //////////////////////////////////////
  //ADC

  //TPC 3 + TPC 4 (TPC 23 + TPC 24 @ S2) 
  //TPC 3
  tgt->tpc_a[2][0]=src->vme1[15][16] & 0xfff;
  tgt->tpc_a[2][1]=src->vme1[15][17] & 0xfff;
  tgt->tpc_a[2][2]=src->vme1[15][18] & 0xfff;
  tgt->tpc_a[2][3]=src->vme1[15][19] & 0xfff;
  tgt->tpc_l[2][0]=src->vme1[15][20] & 0xfff;
  tgt->tpc_r[2][0]=src->vme1[15][21] & 0xfff;
  tgt->tpc_l[2][1]=src->vme1[15][22] & 0xfff;
  tgt->tpc_r[2][1]=src->vme1[15][23] & 0xfff;

  // TPC 4 
  tgt->tpc_a[3][0]=src->vme1[15][24] & 0xfff;
  tgt->tpc_a[3][1]=src->vme1[15][25] & 0xfff;
  tgt->tpc_a[3][2]=src->vme1[15][26] & 0xfff;
  tgt->tpc_a[3][3]=src->vme1[15][27] & 0xfff;
  tgt->tpc_l[3][0]=src->vme1[15][28] & 0xfff;
  tgt->tpc_r[3][0]=src->vme1[15][29] & 0xfff;
  tgt->tpc_l[3][1]=src->vme1[15][30] & 0xfff;
  tgt->tpc_r[3][1]=src->vme1[15][31] & 0xfff;
  
  // TPC 5 + TPC 6 (TPC 41 + TPC 42 @ S4)
  // TPC 5
  tgt->tpc_a[4][0]=src->vme1[17][0] & 0xfff;
  tgt->tpc_a[4][1]=src->vme1[17][1] & 0xfff;
  tgt->tpc_a[4][2]=src->vme1[17][2] & 0xfff;
  tgt->tpc_a[4][3]=src->vme1[17][3] & 0xfff;
  tgt->tpc_l[4][0]=src->vme1[17][4] & 0xfff;
  tgt->tpc_r[4][0]=src->vme1[17][5] & 0xfff;
  tgt->tpc_l[4][1]=src->vme1[17][6] & 0xfff;
  tgt->tpc_r[4][1]=src->vme1[17][7] & 0xfff;

  //TPC 6
  tgt->tpc_a[5][0]=src->vme1[17][8] & 0xfff;
  tgt->tpc_a[5][1]=src->vme1[17][9] & 0xfff;
  tgt->tpc_a[5][2]=src->vme1[17][10] & 0xfff;
  tgt->tpc_a[5][3]=src->vme1[17][11] & 0xfff;
  tgt->tpc_l[5][0]=src->vme1[17][12] & 0xfff;
  tgt->tpc_r[5][0]=src->vme1[17][13] & 0xfff;
  tgt->tpc_l[5][1]=src->vme1[17][14] & 0xfff;
  tgt->tpc_r[5][1]=src->vme1[17][15] & 0xfff;

  //TPC 1
  tgt->tpc_a[0][0]=src->vme1[16][0] & 0xfff;
  tgt->tpc_a[0][1]=src->vme1[16][1] & 0xfff;
  tgt->tpc_a[0][2]=src->vme1[16][2] & 0xfff;
  tgt->tpc_a[0][3]=src->vme1[16][3] & 0xfff;
  tgt->tpc_l[0][0]=src->vme1[16][4] & 0xfff;
  tgt->tpc_r[0][0]=src->vme1[16][5] & 0xfff;
  tgt->tpc_l[0][1]=src->vme1[16][6] & 0xfff;
  tgt->tpc_r[0][1]=src->vme1[16][7] & 0xfff;

  // TPC 2
  tgt->tpc_a[1][0]=src->vme1[16][8] & 0xfff;
  tgt->tpc_a[1][1]=src->vme1[16][9] & 0xfff;
  tgt->tpc_a[1][2]=src->vme1[16][10] & 0xfff;
  tgt->tpc_a[1][3]=src->vme1[16][11] & 0xfff;
  tgt->tpc_l[1][0]=src->vme1[16][12] & 0xfff;
  tgt->tpc_r[1][0]=src->vme1[16][13] & 0xfff;
  tgt->tpc_l[1][1]=src->vme1[16][14] & 0xfff;
  tgt->tpc_r[1][1]=src->vme1[16][15] & 0xfff;
 
  //TDC

  // TPC 3 + TPC 4 (TPC 23 + TPC 24 @ S2)  
  //TPC 3
  tgt->tpc_dt[2][0]=src->vme1[8][16] & 0xfff;
  tgt->tpc_dt[2][1]=src->vme1[8][17] & 0xfff;
  tgt->tpc_dt[2][2]=src->vme1[8][18] & 0xfff;
  tgt->tpc_dt[2][3]=src->vme1[8][19] & 0xfff;
  tgt->tpc_lt[2][0]=src->vme1[8][20] & 0xfff;
  tgt->tpc_rt[2][0]=src->vme1[8][21] & 0xfff;
  tgt->tpc_lt[2][1]=src->vme1[8][22] & 0xfff;
  tgt->tpc_rt[2][1]=src->vme1[8][23] & 0xfff;

  //TPC 4
  tgt->tpc_dt[3][0]=src->vme1[8][24] & 0xfff;
  tgt->tpc_dt[3][1]=src->vme1[8][25] & 0xfff;
  tgt->tpc_dt[3][2]=src->vme1[8][26] & 0xfff;
  tgt->tpc_dt[3][3]=src->vme1[8][27] & 0xfff;
  tgt->tpc_lt[3][0]=src->vme1[8][28] & 0xfff;
  tgt->tpc_rt[3][0]=src->vme1[8][29] & 0xfff;
  tgt->tpc_lt[3][1]=src->vme1[8][30] & 0xfff;
  tgt->tpc_rt[3][1]=src->vme1[8][31] & 0xfff;

  // TPC 5 + TPC 6 (TPC 41 + TPC 42 @ S4)  
  //TPC 5
  tgt->tpc_dt[4][0]=src->vme1[9][0] & 0xfff;
  tgt->tpc_dt[4][1]=src->vme1[9][1] & 0xfff;
  tgt->tpc_dt[4][2]=src->vme1[9][2] & 0xfff;
  tgt->tpc_dt[4][3]=src->vme1[9][3] & 0xfff;
  tgt->tpc_lt[4][0]=src->vme1[9][4] & 0xfff;
  tgt->tpc_rt[4][0]=src->vme1[9][5] & 0xfff;
  tgt->tpc_lt[4][1]=src->vme1[9][6] & 0xfff;
  tgt->tpc_rt[4][1]=src->vme1[9][7] & 0xfff;

  //TPC 6
  tgt->tpc_dt[5][0]=src->vme1[9][8] & 0xfff;
  tgt->tpc_dt[5][1]=src->vme1[9][9] & 0xfff;
  tgt->tpc_dt[5][2]=src->vme1[9][10] & 0xfff;
  tgt->tpc_dt[5][3]=src->vme1[9][11] & 0xfff;
  tgt->tpc_lt[5][0]=src->vme1[9][12] & 0xfff;
  tgt->tpc_rt[5][0]=src->vme1[9][13] & 0xfff;
  tgt->tpc_lt[5][1]=src->vme1[9][14] & 0xfff;
  tgt->tpc_rt[5][1]=src->vme1[9][15] & 0xfff;

  //TPC 1
  tgt->tpc_dt[0][0]=src->vme1[8][0] & 0xfff;
  tgt->tpc_dt[0][1]=src->vme1[8][1] & 0xfff;
  tgt->tpc_dt[0][2]=src->vme1[8][2] & 0xfff;
  tgt->tpc_dt[0][3]=src->vme1[8][3] & 0xfff;
  tgt->tpc_lt[0][0]=src->vme1[8][4] & 0xfff;
  tgt->tpc_rt[0][0]=src->vme1[8][5] & 0xfff;
  tgt->tpc_lt[0][1]=src->vme1[8][6] & 0xfff;
  tgt->tpc_rt[0][1]=src->vme1[8][7] & 0xfff;

  //TPC 2
  tgt->tpc_dt[1][0]=src->vme1[8][8] & 0xfff;
  tgt->tpc_dt[1][1]=src->vme1[8][9] & 0xfff;
  tgt->tpc_dt[1][2]=src->vme1[8][10] & 0xfff;
  tgt->tpc_dt[1][3]=src->vme1[8][11] & 0xfff;
  tgt->tpc_lt[1][0]=src->vme1[8][12] & 0xfff;
  tgt->tpc_rt[1][0]=src->vme1[8][13] & 0xfff;
  tgt->tpc_lt[1][1]=src->vme1[8][14] & 0xfff;
  tgt->tpc_rt[1][1]=src->vme1[8][15] & 0xfff;
  
  /* ### SCI dE:  */
  tgt->de_21l = src->vme0[11][16] & 0xfff;
  tgt->de_21r = src->vme0[11][17] & 0xfff;
  tgt->de_41l = src->vme0[11][18] & 0xfff;
  tgt->de_41r = src->vme0[11][19] & 0xfff;  
  tgt->de_v1l = 0;
  tgt->de_v1r = 0;  
  tgt->de_v2l = 0;
  tgt->de_v2r = 0;
  tgt->de_v3  = 0;
  //  tgt->de_42l = src->vme0[11][26] & 0xfff;
  //  tgt->de_42r = rc->vme0[11][27] & 0xfff;
  
  //tgt->de_21ld = 0;    // 
  //tgt->de_21rd = 0;    //

  /* ### SCI times:  */
  tgt->dt_21l_21r = src->vme0[12][0] & 0xfff;
  tgt->dt_41l_41r = src->vme0[12][1] & 0xfff;
  tgt->dt_21l_41l = src->vme0[12][2] & 0xfff;
  tgt->dt_21r_41r = src->vme0[12][3] & 0xfff;
  tgt->dt_42l_42r = src->vme0[12][7] & 0xfff;  
  //  tgt->dt_42l_21l = 0; //
  //  tgt->dt_42r_21r = 0; //
  

  /* ### MUSIC OLD:  */
  for(int i=0;i<4;i++)
    {
      tgt->music_t3[i] = src->vme1[9][16+i] & 0xfff;    //TIME
      //tgt->music_t3[i] = 0;
      tgt->music_e3[i] = src->vme0[12][16+i] & 0xfff;    //ENERGY
      //tgt->music_e3[i] = 0;
    }
               
  /* ### TUM MUSIC dE:  */
  for(int i=0;i<8;i++)
    {
      tgt->music_e1[i] = src->vme0[12][8+i] & 0xfff;
      tgt->music_e2[i] = src->vme0[12][24+i] & 0xfff;    
    }
    
  /* ### MUSIC temp & pressure:  */
  tgt->music_pres[0] = src->vme0[12][23] & 0xfff;
  //tgt->music_pres[0] = 0; 
  tgt->music_temp[0] = src->vme0[12][22] & 0xfff;
  //tgt->music_temp[0] = 0;

  /*  not included in readout! */
  tgt->music_pres[1] = 0;
  tgt->music_temp[1] = 0;

  /*  not included in readout! */
  tgt->music_pres[2] = 0;
  tgt->music_temp[2] = 0;


  //Si detectors 
  tgt->si_adc1 = src->vme0[12][20]& 0xfff; 
  tgt->si_adc2 = src->vme0[12][21]& 0xfff;

  //Channeltron detectors (vme must be adjusted)
  tgt->ct_signal = src->vme2scaler[20]& 0xfff; 
  tgt->ct_trigger_DU = src->vme2scaler[5]& 0xfff;
  tgt->ct_trigger_SY = src->vme2scaler[6]& 0xfff;

  //Electron current measurement (vme must be adjused)
  tgt->ec_signal = src->vme0[12][1]& 0xfff;

  return kTRUE;
}

ClassImp(TFRSSortProc)
