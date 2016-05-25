#include "Riostream.h"
    
void setup_s411_jun16()
{
  // look up analysis object and all parameters 
    
  TFRSAnalysis* an = dynamic_cast<TFRSAnalysis*> (TGo4Analysis::Instance());
  if (an==0) {
    cout << "!!!  Script should be run in FRS analysis" << endl;
    return;
  }
   
  TFRSParameter* frs = dynamic_cast<TFRSParameter*> (an->GetParameter("FRSPar"));
  if (frs==0) {
    cout << "!!!  Parameter FRSPar not found" << endl;
    return;
  }

  TMWParameter* mw = dynamic_cast<TMWParameter*> (an->GetParameter("MWPar"));
  if (mw==0) {
    cout << "!!!  Parameter MWPar not found" << endl;
    return;
  }

  TMUSICParameter* music = dynamic_cast<TMUSICParameter*> (an->GetParameter("MUSICPar"));
  if (music==0) {
    cout << "!!!  Parameter MUSICPar not found" << endl;
    return;
  }

  TSCIParameter* sci = dynamic_cast<TSCIParameter*> (an->GetParameter("SCIPar"));
  if (sci==0) {
    cout << "!!!  Parameter SCIPar not found" << endl;
    return;
  }

  TIDParameter* id = dynamic_cast<TIDParameter*> (an->GetParameter("IDPar"));
  if (id==0) {
    cout << "!!!  Parameter IDPar not found" << endl;
    return;
  }
   
  TTPCParameter* tpc = dynamic_cast<TTPCParameter*> (an->GetParameter("TPCPar"));
  if (tpc==0) {
    cout << "!!!  Parameter TPCPar not found" << endl;
    return;
  }

  TSIParameter* si = dynamic_cast<TSIParameter*> (an->GetParameter("SIPar"));
  if (si==0) {
    cout << "!!!  Parameter SIPar not found" << endl;
    return;
  }

  TMRTOFMSParameter* mrtof = dynamic_cast<TMRTOFMSParameter*> (an->GetParameter("MRTOFMSPar"));
  if (mrtof==0) {
    cout << "!!!  Parameter MR-TOF-MSPar not found" << endl;
    return;
  }
  
  TModParameter* ElecMod = dynamic_cast<TModParameter*>(an->GetParameter("ModPar"));
   
  cout << endl << "setup script started" << endl;
   

  // setup part 
  an->SetupH2("ID_x4AoQ", 500, 2.3, 2.8, 200, -100, +100, "A/Q", "X2 [mm]");     
  an->SetupH2("ID_Z_AoQ", 600, 2.25, 2.8,600 , 30, 95, "A/Q", "Z"); 
  an->SetupH2("ID_Z_AoQ_corr", 500, 2.0, 2.75, 600, 50, 98, "A/Q (a2 corr)", "Z"); 
  an->SetupH2("ID_x4z", 450, 50, 60.0, 500, -100, 100, "Z", "X4 [mm]"); 

  //      ID_dEToF
  Float_t my_cID_Z_AoQ_points[5][2] =
  // s411_57 133I
  //	{{ 2.5184., 55.8700},
  //	{ 2.5184., 57.1700},
  //	{ 2.5305., 57.1700},
  //	{ 2.5305., 55.7700},
  //	{ 2.5184., 55.8700}};
  // s411_57 133Te
  //	{{ 2.5461., 56.2410},
  //	{ 2.5461., 55.3040},
  //	{ 2.5612., 55.3040},
  //	{ 2.5612., 56.2410},
  //	{ 2.5461., 56.2410}};
  // s411_ 213Fr
    {{ 2.45205, 86.7875},
     { 2.46124, 86.7458},
     { 2.46202, 88.1625},
     { 2.4496, 88.1417},
     { 2.45205, 86.7875}};
  an->SetupPolyCond("cID_Z_AoQ(0)", 5, my_cID_Z_AoQ_points);


  Float_t my_cID_dEToF_points[4][2] = 
    {{    0.,    0.},
     {    0., 4000.},
     {40000., 4000.},
     {40000.,    0.}}; 
  an->SetupPolyCond("cID_dEToF", 4, my_cID_dEToF_points);
   

  /* 20Mg */
  Float_t my_20mg_points[6][2]=
    {{1.665,        13.1000},
     {1.705,        12.9500},
     {1.705,        12.2500},
     {1.665,        12.1000},
     {1.624,        12.2500},
     {1.624,        12.9500}};

  an->SetupPolyCond("cID_Z_AoQ(3)", 6, my_20mg_points);


  // setup FRS parameter

  mw->x_factor[0] = 0.25; // MW11 [mm/ns] 14.09.05 CN+AM 2ns/mm delay line 
  mw->x_factor[1] = 0.25; // MW21
  mw->x_factor[2] = 0.25; // MW22
  mw->x_factor[3] = 0.25; // MW31
  mw->x_factor[4] = 0.25; // MW41 
  mw->x_factor[5] = 0.25; // MW42
  mw->x_factor[8] = 0.25; // MW71
  mw->x_factor[9] = 0.25; // MW81   not modified
  mw->x_factor[10] = 0.125; // MW82 [mm/ns] 11.05.06  CN 4ns/mm delay line

   
  //   mw->x_offset[0] = -0.4; //  MW11  17.01.09 TESTED VALUE WITH SLITS
  mw->x_offset[0] = 5.0; // Feb 2014
  //   mw->x_offset[1] = 2.0;  //  MW21  17.01.09 TESTED VALUE WITH SLITS
  mw->x_offset[1] = -2.0; // Feb 2014
  //mw->x_offset[2] = -1.0; //  MW22  17.01.09 TESTED VALUE WITH SLITS
  mw->x_offset[2] = -1.5; // Feb 2014
  mw->x_offset[3] = -0.205; // MW31 //10/05/06, ok also 20.01.09, ok also Feb 2014
  mw->x_offset[4] = 0.;  // MW41
  mw->x_offset[5] = -9.; // MW42 20.01.09 TESTED VALUE WITH SLITS
  mw->x_offset[8] = 1.642; // MW71 //15/05/06
  mw->x_offset[9] = 1.;   // MW81 //11/05/06
  mw->x_offset[10] = -5.; // MW82 //27-MAY-2007

  mw->y_factor[0] = 0.25; // MW11 [mm/ns] 14.09.05 CN+AM 2ns/mm delay line 
  mw->y_factor[1] = 0.25; // MW21
  mw->y_factor[2] = 0.25; // MW22
  mw->y_factor[3] = 0.25; // MW31
  mw->y_factor[4] = 0.25; // MW41
  mw->y_factor[5] = 0.25; // MW42
  mw->y_factor[8] = 0.25; // MW71
  mw->y_factor[9] = 0.25; // MW81
  mw->y_factor[10] = 0.125; // MW82  [mm/ns] 11.05.06  CN 4ns/mm delay line

   
  mw->y_offset[0] = -14.0; // MW11 27-MAY-2007 TESTED VALUE WITH SLITS, ok Feb 2014
  //   mw->y_offset[1] = -5.0;  // MW21 27-MAY-2007 TESTED VALUE WITH SLITS
  mw->y_offset[1] = 21.0;   // Feb 2014
  mw->y_offset[2] = -1.0;  // MW22 27-MAY-2007 TESTED VALUE WITH SLITS, ok Feb 2014
  mw->y_offset[3] = 0.0;   // MW31 18-MAY-2007, ok Feb 2014
  mw->y_offset[4] = 0.;  // MW41
  mw->y_offset[5] = 0.;  // MW42
  mw->y_offset[8] = -2.736;  // MW71 //15/05/06
  mw->y_offset[9] = 3.2;     // MW81 //11/05/06
  mw->y_offset[10] = 0.764;  // MW82 //11/05/06


  mw->gain_tdc[0][0] = 0.302929; //  MW11 Anode (#ch  0 TDC V775a) // 13.01.2008
  mw->gain_tdc[1][0] = 0.303253; //  MW11 XL    (#ch 17 TDC V775a)
  mw->gain_tdc[2][0] = 0.303975; //  MW11 XR    (#ch 16 TDC V775a)
  mw->gain_tdc[3][0] = 0.308414; //  MW11 YU    (#ch 18 TDC V775a)
  mw->gain_tdc[4][0] = 0.309826; //  MW11 YD    (#ch 19 TDC V775a)

  mw->gain_tdc[0][1] = 0.306064; //  MW21 Anode (#ch  1 TDC V775a) // 13.01.2008
  mw->gain_tdc[1][1] = 0.306958; //  MW21 XL    (#ch 21 TDC V775a)
  mw->gain_tdc[2][1] = 0.307799; //  MW21 XR    (#ch 20 TDC V775a)
  mw->gain_tdc[3][1] = 0.297774; //  MW21 YU    (#ch 22 TDC V775a)
  mw->gain_tdc[4][1] = 0.310235; //  MW21 YD    (#ch 23 TDC V775a)

  mw->gain_tdc[0][2] = 0.301179;  // MW22 Anode (#ch  2 TDC V775a) // 13.01.2008
  mw->gain_tdc[1][2] = 0.311121; //  MW22 XL    (#ch 25 TDC V775a)
  mw->gain_tdc[2][2] = 0.303233; //  MW22 XR    (#ch 24 TDC V775a)
  mw->gain_tdc[3][2] = 0.300558; //  MW22 YU    (#ch 26 TDC V775a)
  mw->gain_tdc[4][2] = 0.301105; //  MW22 YD    (#ch 27 TDC V775a)

  mw->gain_tdc[0][3] = 0.304426; //  MW31 Anode (#ch  3 TDC V775a) // 13.01.2008
  mw->gain_tdc[1][3] = 0.312163; //  MW31 XL    (#ch 29 TDC V775a)
  mw->gain_tdc[2][3] = 0.305609; //  MW31 XR    (#ch 28 TDC V775a)
  mw->gain_tdc[3][3] = 0.304716; //  MW31 YU    (#ch 30 TDC V775a)
  mw->gain_tdc[4][3] = 0.293695; //  MW31 YD    (#ch 31 TDC V775a)

  mw->gain_tdc[0][4] = 0.298871; //  MW41 Anode (#ch  4 TDC V775a) // 13.01.2008
  mw->gain_tdc[1][4] = 0.284086; //  MW41 XL    (#ch 1 TDC V775b)
  mw->gain_tdc[2][4] = 0.288656; //  MW41 XR    (#ch 0 TDC V775b)
  mw->gain_tdc[3][4] = 0.286589; //  MW41 YU    (#ch 2 TDC V775b)
  mw->gain_tdc[4][4] = 0.29269;  //  MW41 YD    (#ch 3 TDC V775b)

  mw->gain_tdc[0][5] = 0.297881; //  MW42 Anode (#ch  5 TDC V775a) // 13.01.2008
  mw->gain_tdc[1][5] = 0.287364; //  MW42 XL    (#ch 5 TDC V775b)
  mw->gain_tdc[2][5] = 0.289636; //  MW42 XR    (#ch 4 TDC V775b)
  mw->gain_tdc[3][5] = 0.291135; //  MW42 YU    (#ch 6 TDC V775b)
  mw->gain_tdc[4][5] = 0.289867; //  MW42 YD    (#ch 7 TDC V775b)

  mw->gain_tdc[0][6] = 0.307892; //  MW51 Anode (#ch  6 TDC V775a) // 13.01.2008
  mw->gain_tdc[1][6] = 0.289894; //  MW51 XL    (#ch  9 TDC V775b)
  mw->gain_tdc[2][6] = 0.292366; //  MW51 XR    (#ch  8 TDC V775b)
  mw->gain_tdc[3][6] = 0.284708; //  MW51 YU    (#ch 10 TDC V775b)
  mw->gain_tdc[4][6] = 0.28186;  //  MW51 YD    (#ch 11 TDC V775b)

  mw->gain_tdc[0][7] = 0.298266; //  MW61 Anode (#ch  7 TDC V775a) // 13.01.2008
  mw->gain_tdc[1][7] = 0.311; //  MW61 XL    (#ch ? TDC V775b)
  mw->gain_tdc[2][7] = 0.305; //  MW61 XR    (#ch ? TDC V775b)
  mw->gain_tdc[3][7] = 0.337; //  MW61 YU    (#ch ? TDC V775b)
  mw->gain_tdc[4][7] = 0.289; //  MW61 YD    (#ch ? TDC V775b)

  mw->gain_tdc[0][8] = 0.303602; //  MW71 Anode (#ch  8 TDC V775a) // 13.01.2008
  mw->gain_tdc[1][8] = 0.300082; //  MW71 XL    (#ch 13 TDC V775b)
  mw->gain_tdc[2][8] = 0.286092; //  MW71 XR    (#ch 12 TDC V775b)
  mw->gain_tdc[3][8] = 0.294287; //  MW71 YU    (#ch 14 TDC V775b)
  mw->gain_tdc[4][8] = 0.291341; //  MW71 YD    (#ch 15 TDC V775b)

  mw->gain_tdc[0][9] = 0.306041; //  MW81 Anode (#ch  9 TDC V775a) // 13.01.2008
  mw->gain_tdc[1][9] = 0.288468; //  MW81 XL    (#ch 17 TDC V775b)
  mw->gain_tdc[2][9] = 0.293831; //  MW81 XR    (#ch 16 TDC V775b)
  mw->gain_tdc[3][9] = 0.281296; //  MW81 YU    (#ch 18 TDC V775b)
  mw->gain_tdc[4][9] = 0.279099; //  MW81 YD    (#ch 19 TDC V775b)

  mw->gain_tdc[0][10] = 0.31314;  //  MW82 Anode (#ch 10 TDC V775a) // 13.01.2008
  mw->gain_tdc[1][10] = 0.287279; //  MW82 XL    (#ch 21 TDC V775b)
  mw->gain_tdc[2][10] = 0.284028; //  MW82 XR    (#ch 20 TDC V775b)
  mw->gain_tdc[3][10] = 0.28051;  //  MW82 YU    (#ch 22 TDC V775b)
  mw->gain_tdc[4][10] = 0.28743;  //  MW82 YD    (#ch 23 TDC V775b)

  mw->gain_tdc[0][11] = 0.299973; //  MWB21 Anode (#ch 11 TDC V775a) // 13.01.2008
  mw->gain_tdc[1][11] = 0.311; //  MWB21 XL    (#ch ? TDC V775b)
  mw->gain_tdc[2][11] = 0.305; //  MWB21 XR    (#ch ? TDC V775b)
  mw->gain_tdc[3][11] = 0.337; //  MWB21 YU    (#ch ? TDC V775b)
  mw->gain_tdc[4][11] = 0.289; //  MWB21 YD    (#ch ? TDC V775b)

  mw->gain_tdc[0][12] = 0.306923; //  MWB22 Anode (#ch 12 TDC V775a) // 13.01.2008
  mw->gain_tdc[1][12] = 0.311; //  MWB22 XL    (#ch ? TDC V775b)
  mw->gain_tdc[2][12] = 0.305; //  MWB22 XR    (#ch ? TDC V775b)
  mw->gain_tdc[3][12] = 0.337; //  MWB22 YU    (#ch ? TDC V775b)
  mw->gain_tdc[4][12] = 0.289; //  MWB22 YD    (#ch ? TDC V775b)


  // frs->dist_focS2   = 2280.0; // [mm] used in exp s441_oct12
  // frs->dist_focS2   = 3450.0; // [mm] 8.8.2012 for s388: s271 optics with focus 1170 mm after S2 midplane
  //   frs->dist_focS2   = 1228.0; // [mm] 8.8.2012 for s388 position calculated at S2 X-slits
  frs->dist_focS2   = 2013.; // [mm] at wedge position S411
  //frs->dist_focS2   = 2860.; // [mm] at wedge position S417 for dE

  frs->dist_MW21    =  604.0; // 
  frs->dist_MW22    = 1782.5; // 
  frs->dist_SC21    = 1554.5; // 
  //   frs->dist_MW41    = 1140.0; // ok on air
  //   frs->dist_MW42    = 2600.0; // ok on air


  //**S323,410 exp.
  //   frs->dist_SC41    = 2180.0; // 8.09.11
  //frs->dist_focS4   = 2600.0; // from exp s411
  //frs->dist_SC41    = 2250.0; //   

  // frs->dist_focS4   = 2330.0; // from exp s410
  // frs->dist_focS4   = 2200.0; // theory for s388 8.8.2012
  frs->dist_focS4   = 2349.0; // at SC411   

  //**S323,410 exp.
  frs->dist_MUSIC1  = 555.0;  // TUM1
  frs->dist_MUSIC2  = 1210.0; // TUM2 , now removed
  frs->dist_MUSIC3  = 1210.0; // Music2 to be adjusted

 
  frs->dist_MUSICa1 = 52.5;  // do not change
  frs->dist_MUSICa2 = 157.5; // do not change
  frs->dist_MUSICa3 = 262.5; // do not change
  frs->dist_MUSICa4 = 367.5; // do not change

  music->e1_off[0]   = 0.; //MUSIC1 offsets
  music->e1_off[1]   = 0.; 
  music->e1_off[2]   = 0.;
  music->e1_off[3]   = 0.;
  music->e1_off[4]   = 0.;
  music->e1_off[5]   = 0.;
  music->e1_off[6]   = 0.;
  music->e1_off[7]   = 0.;


  music->e1_gain[0]   = 1.; // MUSIC1 gains
  music->e1_gain[1]   = 1.; 
  music->e1_gain[2]   = 1.;
  music->e1_gain[3]   = 1.;
  music->e1_gain[4]   = 1.;
  music->e1_gain[5]   = 1.;
  music->e1_gain[6]   = 1.;
  music->e1_gain[7]   = 1.;

  music->e2_off[0]   = 0.; //MUSIC2 offsets
  music->e2_off[1]   = 0.; 
  music->e2_off[2]   = 0.;
  music->e2_off[3]   = 0.;
  music->e2_off[4]   = 0.;
  music->e2_off[5]   = 0.;
  music->e2_off[6]   = 0.;
  music->e2_off[7]   = 0.;


  music->e2_gain[0]   = 1.; // MUSIC2 gains
  music->e2_gain[1]   = 1.; 
  music->e2_gain[2]   = 1.;
  music->e2_gain[3]   = 1.;
  music->e2_gain[4]   = 1.;
  music->e2_gain[5]   = 1.;
  music->e2_gain[6]   = 1.;
  music->e2_gain[7]   = 1.;

  music->e3_off[0]   = 0.; //MUSIC3 offsets
  music->e3_off[1]   = 0.; 
  music->e3_off[2]   = 0.;
  music->e3_off[3]   = 0.;


  music->e3_gain[0]   = 1.; // MUSIC3 gains
  music->e3_gain[1]   = 1.; 
  music->e3_gain[2]   = 1.;
  music->e3_gain[3]   = 1.;


  music->pos_a1[0]   = 1.0;   // C0...Cn position correction not used
  music->pos_a1[1]   = 0.0;
  music->pos_a1[2]   = 0.0;
  music->pos_a1[3]   = 0.0;
  music->pos_a1[4]   = 0.0;
  music->pos_a1[5]   = 0.0;
  music->pos_a1[6]   = 0.0;


  //TPC3   from agata 

  tpc->x_factor[2][0] = 0.070128;  //L-R time 0
  tpc->x_factor[2][1] = 0.071489;  //L-R time 1
  tpc->y_factor[2][0] = -0.036481; //drift time 0
  tpc->y_factor[2][1] = -0.036863; //drift time 1
  tpc->y_factor[2][2] = -0.037120; //drift time 2
  tpc->y_factor[2][3] = -0.037125; //drift time 3

  tpc->x_offset[2][0] = 0.81+3.24-0.14; //21.10.2014
  tpc->x_offset[2][1] = 0.03+3.24-0.14; //21.10.2014
  //   tpc->x_offset[2][0] = 0.81;
  //   tpc->x_offset[2][1] = 0.03;

  tpc->y_offset[2][0] = 19.2;
  tpc->y_offset[2][1] = 19.7;
  tpc->y_offset[2][2] = 18.9;
  tpc->y_offset[2][3] = 19.2;



  // TPC4
  tpc->x_factor[3][0] = 0.068179;
  tpc->x_factor[3][1] = 0.066689;
  tpc->y_factor[3][0] = -0.0412;
  tpc->y_factor[3][1] = -0.0411;
  tpc->y_factor[3][2] = -0.0404;
  tpc->y_factor[3][3] = -0.0418;

  tpc->x_offset[3][0] = -0.37+6.0-1.3; //21.10.2014
  tpc->x_offset[3][1] = 2.3+6.0-1.3; //21.10.2014
  tpc->y_offset[3][0] = 28.0;
  tpc->y_offset[3][1] = 28.4;
  tpc->y_offset[3][2] = 28.4;
  tpc->y_offset[3][3] = 27.8;

  

  // TPC5
  
  tpc->x_factor[4][0] = 0.0701334;
  tpc->x_factor[4][1] = 0.0719196;
  tpc->y_factor[4][0] =-0.03644;
  tpc->y_factor[4][1] =-0.03624;
  tpc->y_factor[4][2] =-0.03709;
  tpc->y_factor[4][3] =-0.03729;

  tpc->x_offset[4][0] = 0.86+5.4-4.1; //21.10.2014
  tpc->x_offset[4][1] =-2.35+5.4-4.1; //21.10.2014
  tpc->y_offset[4][0] = 30.6;
  tpc->y_offset[4][1] = 30.9;
  tpc->y_offset[4][2] = 32.2;
  tpc->y_offset[4][3] = 32.1;


  // TPC6 
  tpc->x_factor[5][0] = 0.071256;
  tpc->x_factor[5][1] = 0.072858;
  tpc->y_factor[5][0] =-0.04215;
  tpc->y_factor[5][1] =-0.04161;
  tpc->y_factor[5][2] =-0.04100;
  tpc->y_factor[5][3] =-0.04025;

  tpc->x_offset[5][0] = -0.4-0.3+0.35; //21.10.2014
  tpc->x_offset[5][1] = 1.1-0.3+0.35; //21.10.2014
  tpc->y_offset[5][0] = 22.6;
  tpc->y_offset[5][1] = 21.9;
  tpc->y_offset[5][2] = 20.7;
  tpc->y_offset[5][3] = 20.4;
 

  // TPC1
  tpc->x_factor[0][0] = 0.070623;
  tpc->x_factor[0][1] = 0.07248;
  tpc->y_factor[0][0] =-0.035723;
  tpc->y_factor[0][1] =-0.034725;
  tpc->y_factor[0][2] =-0.0364399;
  tpc->y_factor[0][3] =-0.035037; 
	 
  tpc->x_offset[0][0] =-0.5;
  tpc->x_offset[0][1] =-1.058;
  tpc->y_offset[0][0] = 38.1838;
  tpc->y_offset[0][1] = 38.37;
  tpc->y_offset[0][2] = 39.1557;
  tpc->y_offset[0][3] = 39.097; 


  // TPC2
  tpc->x_factor[1][0] = 0.0716;
  tpc->x_factor[1][1] = 0.070329;
  tpc->y_factor[1][0] = -0.0362752;
  tpc->y_factor[1][1] = -0.0365182;
  tpc->y_factor[1][2] = -0.0357276;
  tpc->y_factor[1][3] = -0.0360721;  

  tpc->x_offset[1][0] = -0.81;
  tpc->x_offset[1][1] =  1.9;
  tpc->y_offset[1][0] =  36.77;
  tpc->y_offset[1][1] =  38.0;
  tpc->y_offset[1][2] =  36.123;
  tpc->y_offset[1][3] =  37.13; 

  // index 2 for Sc21  
  sci->x_a[0][2] =  0;  //  SC21 calibration ch->mm 
  sci->x_a[1][2] =  1; //  
  sci->x_a[2][2] =  0.000000;  // 
  sci->x_a[3][2] =  0.000000;  //                            
  sci->x_a[4][2] =  0.000000;  //                             
  sci->x_a[5][2] =  0.000000;  //                             
  sci->x_a[6][2] =  0.000000;  //  
  
  /*
  // index 2 for Sc21  
  sci->x_a[0][2] = 1184.51;  //  SC21 calibration ch->mm 
  sci->x_a[1][2] =  -0.5206; //  s323 test run: Xe fragments run
  sci->x_a[2][2] =  0.000000;  // 
  sci->x_a[3][2] =  0.000000;  //                            
  sci->x_a[4][2] =  0.000000;  //                             
  sci->x_a[5][2] =  0.000000;  //                             
  sci->x_a[6][2] =  0.000000;  //    
  */

  // index 5 for Sc41
  sci->x_a[0][5] = 0.;  //  SC41 calibration ch->mm 
  sci->x_a[1][5] = 1.;  //
  sci->x_a[2][5] = 0.000000;   //
  sci->x_a[3][5] = 0.000000;   //                            
  sci->x_a[4][5] = 0.000000;   //                             
  sci->x_a[5][5] = 0.000000;   //                             
  sci->x_a[6][5] = 0.000000;   //    

  // index 6 for Sc42
  sci->x_a[0][6] = 0.; //   SC42 calibration ch->mm 
  sci->x_a[1][6] = 1.; // 
  sci->x_a[2][6] = 0.000000;  // 
  sci->x_a[3][6] = 0.000000;  //                            
  sci->x_a[4][6] = 0.000000;  //                             
  sci->x_a[5][6] = 0.000000;  //                             
  sci->x_a[6][6] = 0.000000;  //    


  // TOF 
  sci->tac_off[0] = 0.;  //SC21L-R  // not used online
  sci->tac_off[1] = 0.;  //SC41L-R 
  sci->tac_off[2] = 0.;  //SC41L-SC21L
  sci->tac_off[3] = 0.;  //SC41R-SC21R
  sci->tac_off[4] = 0.;  //SC42L-SC42R
  sci->tac_off[5] = 0.;  //SC42L-SC21L
  sci->tac_off[6] = 0.;  //SC42R-SC21R


  // TAC calibration factors assumed for agata
  //   sci->tac_factor[0] = 5.571;   //SC21L-R [ps/ch]  , range 25 ns
  //   sci->tac_factor[1] = 5.604;   //SC41L-R [ps/ch]  , range 25 ns  
  sci->tac_factor[0] = 5.89623;   //SC21L-R [ps/ch]  , range 25 ns, CH 19.10.2014
  sci->tac_factor[1] = 5.51268;   //SC41L-R [ps/ch]  , range 25 ns, CH 19.10.2014
  sci->tac_factor[4] = 5.609;      //SC42L-R [ps/ch]  , range 25 ns

  //   sci->tac_factor[2] = 16.809;  // SC41L-SC21L [ps/ch] , range 75 ns
  //   sci->tac_factor[3] = 17.149;  // SC41R-SC21R [ps/ch] , range 75 ns 
  sci->tac_factor[2] = 10.55619;  // SC41L-SC21L [ps/ch] , range 50 ns, CH 19.10.2014
  sci->tac_factor[3] = 11.17427;  // SC41R-SC21R [ps/ch] , range 50 ns, CH 19.10.2014 
  sci->tac_factor[5] = 1.;   //
  sci->tac_factor[6] = 1.;   //


  sci->tof_bll2  = 1.;    // not used online [ps/ch]
  sci->tof_brr2  = 1.;    // not used online
  sci->tof_bll3  = 1.;    // not used online 
  sci->tof_brr3  = 1.;    // not used online

  sci->tof_a2 = 0.; // [ps] offset   Tof S41-S21
  sci->tof_a3 = 0.; // [ps] offset   Tof S41-S21_raw


  //   id->id_tofoff2  = 197689.0; // [ps]  S41-S21 from 9.Oct on because of TAC-LL shift
  //   id->id_path2    = 119120.0;

  // used for ToF S2-S4, extracted from agata calib
  //id->id_tofoff2  = 175940.0; // [ps]  S411 21.10.2014
  //id->id_path2    = 126160.0; // path/c [ps]  S411_48 21.10.2014 
  //id->id_tofoff2  = 170680.0; // [ps]  S411 22.10.2014
  id->id_tofoff2  = 170470.0; // [ps]  S411 shifted 25.10.2014
  id->id_path2    = 121730.0; // path/c [ps]  S411_48 22.10.2014 


  //not used
  id->id_tofoff3  = 210000.;   // [ps] 
  id->id_path3    = 120000.;   // path/c [ps]  


  // B-rho values : change according to the FRS settings
  // from S411 211Po
  //    frs->bfield[0] = 1.17195;      // FRS D3 field [Tesla] 
  //    frs->bfield[1] = 1.15554;      // FRS D4 field [Tesla] 
  //    frs->bfield[2] = 0.8046;       // FRS D3 field [Tesla] 
  //    frs->bfield[3] = 0.80495;      // FRS D4 field [Tesla]


  // B-rho values : change according to the FRS settings
  // from S411 211Po
  //    frs->bfield[0] = 1.17195;      // FRS D3 field [Tesla] 
  //    frs->bfield[1] = 1.15554;      // FRS D4 field [Tesla] 
  //    frs->bfield[2] = 0.8520;       // FRS D3 field [Tesla] 
  //    frs->bfield[3] = 0.85245;      // FRS D4 field [Tesla]

  // from S411 220Ra
  //    frs->bfield[0] = 1.16395;      // FRS D3 field [Tesla] 
  //    frs->bfield[1] = 1.14754;      // FRS D4 field [Tesla] 
  //    frs->bfield[2] = 0.8261;       // FRS D3 field [Tesla] 
  //    frs->bfield[3] = 0.82645;      // FRS D4 field [Tesla]

  // from S411 fragments_234  133Te setting file 57
  frs->bfield[0] = 1.01805;      // FRS D3 field [Tesla] 
  frs->bfield[1] = 1.00384;      // FRS D4 field [Tesla] 
  frs->bfield[2] = 0.7990;       // FRS D3 field [Tesla] 
  frs->bfield[3] = 0.79975;      // FRS D4 field [Tesla]

  // from S411 fragments_249  217at setting 60
  //    frs->bfield[0] = 1.16365;      // FRS D3 field [Tesla] 
  //    frs->bfield[1] = 1.14744;      // FRS D4 field [Tesla] 
  //    frs->bfield[2] = 0.8261;       // FRS D3 field [Tesla] 
  //    frs->bfield[3] = 0.8265;      // FRS D4 field [Tesla]

  // from S411 fragments_249  217at setting 60
  //    frs->bfield[0] = 1.19145;      // FRS D3 field [Tesla] 
  //    frs->bfield[1] = 1.17504;      // FRS D4 field [Tesla] 
  //    frs->bfield[2] = 0.8680;       // FRS D3 field [Tesla] 
  //    frs->bfield[3] = 0.8689;      // FRS D4 field [Tesla]

  // from S411 fragments_249  216Fr setting 59
  //    frs->bfield[0] = 1.15565;      // FRS D3 field [Tesla] 
  //    frs->bfield[1] = 1.13964;      // FRS D4 field [Tesla] 
  //    frs->bfield[2] = 0.8223;       // FRS D3 field [Tesla] 
  //    frs->bfield[3] = 0.82275;      // FRS D4 field [Tesla]

  // from S411 fragments_249  218Rn setting 58
  //    frs->bfield[0] = 1.18215;      // FRS D3 field [Tesla] 
  //    frs->bfield[1] = 1.16574;      // FRS D4 field [Tesla] 
  //    frs->bfield[2] = 0.8539;       // FRS D3 field [Tesla] 
  //    frs->bfield[3] = 0.8534;      // FRS D4 field [Tesla]

  // from S411 fragments_172  134Te/134I setting
  //    frs->bfield[0] = 1.01715;      // FRS D3 field [Tesla] 
  //    frs->bfield[1] = 1.00354;      // FRS D4 field [Tesla] 
  //    frs->bfield[2] = 0.7990;       // FRS D3 field [Tesla] 
  //    frs->bfield[3] = 0.79985;      // FRS D4 field [Tesla]

  // from S411 fragments_213Rn_0074 (S411_55)
  //    frs->bfield[0] = 1.15285;      // FRS D3 field [Tesla] 
  //    frs->bfield[1] = 1.13724;      // FRS D4 field [Tesla] 
  //    frs->bfield[2] = 0.8241;       // FRS D3 field [Tesla] 
  //    frs->bfield[3] = 0.82445;      // FRS D4 field [Tesla]

  // from S411 fragments_213Fr_0074 (S411_54)
  //    frs->bfield[0] = 1.13825;      // FRS D3 field [Tesla] 
  //    frs->bfield[1] = 1.12264;      // FRS D4 field [Tesla] 
  //    frs->bfield[2] = 0.8048;       // FRS D3 field [Tesla] 
  //    frs->bfield[3] = 0.80505;      // FRS D4 field [Tesla]

  // from S411 238U 1.6gBe+Nb, 4200mg/cm2 Al (S411_) file 78
  //    frs->bfield[0] = 1.20365;      // FRS D3 field [Tesla] 
  //    frs->bfield[1] = 1.18794;      // FRS D4 field [Tesla] 
  //    frs->bfield[2] = 0.8510;       // FRS D3 field [Tesla] 
  //    frs->bfield[3] = 0.85195;      // FRS D4 field [Tesla]

  // from S411_53 fragments_0069
  //    frs->bfield[0] = 1.15125;      // FRS D3 field [Tesla] 
  //    frs->bfield[1] = 1.13544;      // FRS D4 field [Tesla] 
  //    frs->bfield[2] = 0.8048;       // FRS D3 field [Tesla] 
  //    frs->bfield[3] = 0.80505;      // FRS D4 field [Tesla]

  // from S411_ U
  //    frs->bfield[0] = 1.20375;      // FRS D3 field [Tesla] 
  //    frs->bfield[1] = 1.18804;      // FRS D4 field [Tesla] 
  //    frs->bfield[2] = 0.8509;       // FRS D3 field [Tesla] 
  //    frs->bfield[3] = 0.85195;      // FRS D4 field [Tesla]

  // from S411_48 21.10.2014
  //    frs->bfield[0] = 1.15135;      // FRS D3 field [Tesla] 
  //    frs->bfield[1] = 1.13554;      // FRS D4 field [Tesla] 
  //    frs->bfield[2] = 0.8041;       // FRS D3 field [Tesla] 
  //    frs->bfield[3] = 0.80435;      // FRS D4 field [Tesla]


  // from S411_48 21.10.2014
  //    frs->bfield[0] = 1.15135;      // FRS D3 field [Tesla] 
  //    frs->bfield[1] = 1.13554;      // FRS D4 field [Tesla] 
  //    frs->bfield[2] = 0.8041;       // FRS D3 field [Tesla] 
  //    frs->bfield[3] = 0.80435;      // FRS D4 field [Tesla]

  // from S411_49 21.10.2014
  //frs->bfield[0] = 1.31305;      // FRS D3 field [Tesla] 
  //frs->bfield[1] = 1.29574;      // FRS D4 field [Tesla] 
  //frs->bfield[2] = 1.2592;       // FRS D3 field [Tesla] 
  //frs->bfield[3] = 1.26045;      // FRS D4 field [Tesla]

  frs->dispersion[0] = -6.474266;     // FRS dispersion TA-S2     (TA2B-Rising optics: TA-S2)
  frs->dispersion[1] =  7.2202681;    // FRS dispersion S2-S4     (TA2B-Rising optics: TA-S4) 
  frs->magnification[1] = 1.115225;   // FRS magnification S2-S4  (TA2B-Rising optics: TA-S4)



  // taken from agata s431_15
  frs->rho0[0]   = 11.22374;    // FRS (D2) radius [m]    
  frs->rho0[1]   = 11.282095;   // FRS (D3+D4)/2 radius [m]   

  //   frs->rho0[0]   = 11.203 ;   // FRS (D1+D2)/2 radius [m]    
  //   frs->rho0[1]   = 11.2641 ;   // FRS (D3+D4)/2 radius [m]   16.09.11


  frs->primary_z = 92.;  // U beam
  frs->offset_z  =  0.0 ;
  frs->offset_z2  = 0.0 ;
  frs->offset_z3  = 0.0 ;

  frs->a2AoQCorr = 0.00095;
  frs->a4AoQCorr = 0.0015;

	
  // MUSIC 1 velocity correction 
  //id->vel_a[0] =  11612 ;  // S411 oct2014 first days
  id->vel_a[0] =  11673 ; // shifted calib 25.10.2104  
  id->vel_a[1] =  -17462;  
  id->vel_a[2] =  7682;
  id->vel_a[3] =  0.0; 

  // MUSIC 2 velocity correction 
  //id->vel_a2[0] =  19577; // S411 oct2014 first days
  id->vel_a2[0] =  19602 ;  // shifted calib 25.10.2104
  id->vel_a2[1] =  -34442;
  id->vel_a2[2] =  17273;
  id->vel_a2[3] =  0.0; 
	
  // MUSIC 3 velocity correction
  id->vel_a3[0] =  0.; //
  id->vel_a3[1] =  1.; 
  id->vel_a3[2] =  0.;
  id->vel_a3[3] =  0.0;     

  // gate on Z 
  id->zgate_low = 85;
  id->zgate_high = 90;

  //Si
   
  si->si_factor1=3.2596; //CH 18.10.2014
  si->si_offset1=-550.59; //CH 18.10.2014
  si->si_factor2=3.809; //CH 18.10.2014
  si->si_offset2=-529.01; //CH 18.10.2014
  si->si_factor3=3.2596; //CH 21.05.2016
  si->si_offset3=-550.59; //CH 21.05.2016|
  si->si_factor4=3.2596; //CH 21.05.2016
  si->si_offset4=-550.59; //CH 21.05.2016
  
  //MR-TOF-MS
  mrtof->MRTOFMS_a=0.069;
  mrtof->MRTOFMS_b=0.71;
  mrtof->MRTOFMS_t0=0;
  mrtof->MRTOFMS_tFRS=0;

  cout << "Focus distance S4: " << frs->dist_focS4 << endl;


  ElecMod->Nb_Modules = 64;
  ElecMod->Nb_QDC = 3;
  ElecMod->Nb_ADC = 1;
  ElecMod->Nb_TDC = 5;
  ElecMod->Nb_Scaler = 2;
  ElecMod->Nb_TimeStamp = 2;
  
  for(int i = 0;i<64;++i)
    ElecMod->Nb_Channels.insert(std::pair<int,int>(i,32));

  
  // Crate FRS procID 10
  int IdMod = 0;
  //std::unordered_map<int,int> CrateFRS;
  std::map<int,int> CrateFRS;
  CrateFRS.insert(std::pair<int,int>(1,IdMod++));
  CrateFRS.insert(std::pair<int,int>(9,IdMod++));
  CrateFRS.insert(std::pair<int,int>(8,IdMod++));
  CrateFRS.insert(std::pair<int,int>(11,IdMod++));
  CrateFRS.insert(std::pair<int,int>(12,IdMod++));

  //std::unordered_map<int,int> CrateTPC;
  std::map<int,int> CrateTPC;
  //CrateTPC.insert(std::pair<int,int>(5,IdMod++));
  CrateTPC.insert(std::pair<int,int>(8,IdMod++));
  CrateTPC.insert(std::pair<int,int>(9,IdMod++));
  CrateTPC.insert(std::pair<int,int>(15,IdMod++));
  CrateTPC.insert(std::pair<int,int>(3,IdMod++));
  
  //std::unordered_map<int,int> CrateUser;
  std::map<int,int> CrateUser;
  CrateUser.insert(std::pair<int,int>(0,IdMod++));

  std::map<int,int> CrateMT;
  CrateMT.insert(std::pair<int,int>(0,IdMod++));
  CrateMT.insert(std::pair<int,int>(2,IdMod++));
  
  Map1* temp1 = new Map1("temp1");
  Map1* temp2 = new Map1("temp2");
  Map1* temp3 = new Map1("temp3");
  Map1* temp4 = new Map1("temp4");
  temp1->map=CrateFRS;
  temp2->map=CrateTPC;
  temp3->map=CrateUser;
  temp4->map=CrateMT;
  TObjString* key1 = new TObjString("10");
  TObjString* key2 = new TObjString("20");
  TObjString* key3 = new TObjString("30");
  TObjString* key4 = new TObjString("40");
  ElecMod->Maptemp.Add(key1,temp1);
  ElecMod->Maptemp.Add(key2,temp2);
  ElecMod->Maptemp.Add(key3,temp3);
  ElecMod->Maptemp.Add(key4,temp4);
  // ElecMod->MapCrates.insert(std::pair<int,std::unordered_map<int,int> >(10,CrateFRS));
  // ElecMod->MapCrates.insert(std::pair<int,std::unordered_map<int,int> >(20,CrateTPC));
  // ElecMod->MapCrates.insert(std::pair<int,std::unordered_map<int,int> >(30,CrateUser));
  
  //ElecMod->MapCrates.insert(std::pair<int,std::map<int,int> >(10,CrateFRS));
  //ElecMod->MapCrates.insert(std::pair<int,std::map<int,int> >(20,CrateTPC));
  //ElecMod->MapCrates.insert(std::pair<int,std::map<int,int> >(30,CrateUser));

  
  ElecMod->ModType.push_back("FRS_SC_1");
  ElecMod->ModType.push_back("FRS_TDC_1");
  ElecMod->ModType.push_back("FRS_TDC_2");
  ElecMod->ModType.push_back("FRS_QDC_1");
  ElecMod->ModType.push_back("FRS_ADC_1");
  ElecMod->ModType.push_back("TPC_TDC_1");
  ElecMod->ModType.push_back("TPC_TDC_2");
  ElecMod->ModType.push_back("TPC_QDC_1");
  ElecMod->ModType.push_back("TPC_QDC_2");
  ElecMod->ModType.push_back("USER_MTDC_1");
  ElecMod->ModType.push_back("MT_SC_1");
  ElecMod->ModType.push_back("MT_MTDC_1");

  ElecMod->Scaler32bit=1;
  //ElecMod->EventFlags.push_back(0x00000000);
  ElecMod->EventFlags.push_back(0x00000100);
  ElecMod->EventFlags.push_back(0x00000200);
  
  // ElecMod->ModType.push_back("QDC_11");
  // ElecMod->ModType.push_back("QDC_12");
  // ElecMod->ModType.push_back("QDC_13");
  // ElecMod->ModType.push_back("QDC_14");
  // ElecMod->ModType.push_back("QDC_15");
  // ElecMod->ModType.push_back("QDC_16");
  // ElecMod->ModType.push_back("QDC_17");
  // ElecMod->ModType.push_back("QDC_18");

  //ElecMod->Print();
  
  cout << "Setup done " << endl;




}
    
    
    
