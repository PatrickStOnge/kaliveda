#ifndef _IDENTIFICATION_CLASS
#define _IDENTIFICATION_CLASS


#include "Rtypes.h"
#include "Defines.h"
#include "LogFile.h"
#include "Random.h"
#include "Reconstructionv.h"
#include "DriftChamberv.h"
#include "IonisationChamberv.h"
#include "Sive503.h"
#include "CsIv.h"
#include "TCutG.h"
#include "CsICalib.h"

#include "KVReconstructedNucleus.h"
#include "KVINDRAReconNuc.h"
#include "KVIdentificationResult.h"

class Identificationv
{
 public:
  Identificationv(LogFile *Log, Reconstructionv *Recon,
		  DriftChamberv *Drift, IonisationChamberv *IonCh, Sive503 *SiD, CsIv *CsID, CsICalib *E);
  virtual ~Identificationv(void);
  
  LogFile *L;
  Reconstructionv *Rec;
  DriftChamberv   *Dr;
  IonisationChamberv *Ic;
  Sive503 *Si;
  CsIv *CsI;
  CsICalib *energytree;

	KVIdentificationResult*	id;
		
  UShort_t TFil1;
  UShort_t TFil2;
  UShort_t EFil1;
  UShort_t EFil2;

    Int_t runNumber;
    Bool_t grids_avail;
  
  Int_t ZZ;
  Float_t AA;
  Int_t DetCsI;
  Int_t DetSi;
  Int_t CsIRaw;
  Int_t SiRaw;
  
  Double_t a_bisec;  
  Double_t e_bisec; 
    
  Double_t initThickness;
  Double_t ECsI;
  Double_t ESi;
  Double_t EEtot;
  Double_t NormVamos;

Double_t PID;
Double_t Z_PID;
Double_t A_PID;

  Double_t dif11[55];
  Double_t dif12[55];

  Double_t dif1[21];	//Z de 3 a 24
  Double_t dif2[21];
  

  Int_t geom[18][6]; 
  Int_t i;  
  Int_t zt;
  Int_t aa;

Int_t runFlag;
    
  bool Present; //true if coordinates determined



  void Init(void); //Init for every event,  variables go to -500. 
  void Calculate(); // Calulate  Initial coordinates
  void Show(void);
  void Treat(void);
  void inAttach(TTree *inT);
  void outAttach(TTree *outT);
  void CreateHistograms();
  void FillHistograms();
  //void PrintCounters(void);
  
	void SetRunFlag(Int_t);
	Int_t GetRunFlag(void);
	
  int Geometry(UShort_t, UShort_t);//temporary method to reconstruct VAMOS telescopes

  Random *Rnd;

  UShort_t T_Raw;


  Float_t D;
  Float_t dE;
  Float_t dE1;
  Float_t E;
  Float_t T;
  Float_t V;
  
  Float_t V_Etot;
  Float_t T_FP;
  
  Float_t V2;
  Float_t Beta;
  Float_t Gamma;
  Float_t M_Q;
  Float_t M_Q_corr;  
  Float_t M_Qcorr;
  Float_t M_Qcorr1;
  Float_t Q;
  Float_t Mr;
  Float_t M_Qr;
  Float_t Qr;
  Float_t Qc;
  Float_t Mc;
  Float_t M;
  Float_t Mcorr;
  Float_t Mcorr1;
  Float_t Z1;
  Float_t Z2;
  
  Float_t Z_tot;
  Float_t Z_si;  
  Double_t ZR;

  //Counters
  Int_t Counter[6];

ClassDef(Identificationv,0)

};

#endif

