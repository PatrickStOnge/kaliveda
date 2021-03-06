//Created by KVClassFactory on Thu Feb  9 00:40:28 2017
//Author: Quentin Fable,,,

#ifndef __KVVAMOSWEIGHTFINDER_H
#define __KVVAMOSWEIGHTFINDER_H

#include "KVBase.h"
#include "KVNumberList.h"
#include "Riostream.h"
#include "TChain.h"
#include "TMath.h"

#include <vector>
#include <cassert>
#include <utility>

class KVVAMOSWeightFinder : public KVBase {
protected:

   Bool_t fkverbose;
   Bool_t fkIsInit;

   Int_t fRunNumber; //run number of the VAMOS events analysed (used to access to DT_corr in weight computation)

   KVNumberList fRunList; //runs to use for the weight computation
   KVNumberList fCompRunList; //complementary list of the given run list, based on experimental run list file
   std::vector< std::vector<Float_t> > fvec_infos; //vector of 'run BrhoRef AngleVamos DT scaler' for the runs in fRunList
   std::vector< std::pair<Float_t, TString> > fvec_TCfiles; //vector of pairs<VamosAngle, path> for each VamosAngle used in weight computation
   std::vector< std::vector<Float_t> > fvec_TCsteps; //vector of steps associated to each trans. coef. file
   TChain* fchain; //chain of trans. coef. TTrees

   //Experimental infos research
   void                 ReadRunListInDataSet();
   void                 ReadInformations(std::ifstream& file);
   std::vector<Float_t> GetInfoLine(Int_t line_number)
   {
      return fvec_infos.at(line_number);
   }
   Float_t              GetInfoValue(Int_t run_number, Int_t num_value);

   //Trans. Coef. research
   void    ReadTransCoefFileListInDataSet();
   void    ReadTransCoefListFile(std::ifstream& file);
   void    ReadTransCoefEnv();
   Int_t   GetNValue(Float_t val_exp, Float_t val_min, Float_t val_max, Float_t val_step);
   Float_t GetClosestValue(Float_t val_exp, Float_t val_step)
   {
      return val_step * TMath::Nint(val_exp / val_step);
   }
   Bool_t  CheckTransCoefSteps();

   //Internal sorting
   void   SortInfoVector();
   static Bool_t SortVectors(const std::vector<Float_t>& vector1, const std::vector<Float_t>& vector2);

public:
   KVVAMOSWeightFinder(Int_t run_number);
   KVVAMOSWeightFinder(const KVVAMOSWeightFinder&);
   virtual ~KVVAMOSWeightFinder();
   virtual void Copy(TObject&) const;

   void cd();
   void SetRunList(KVNumberList& nl);
   void Init();

   Int_t   GetRunPosition(Int_t run_number);
   Float_t GetScalerINDRA(Int_t run_number)
   {
      return GetInfoValue(run_number, 4);
   }
   Float_t GetDeadTime(Int_t run_number)
   {
      return GetInfoValue(run_number, 3);
   }
   Float_t GetThetaVamos(Int_t run_number)
   {
      return GetInfoValue(run_number, 2);
   }
   Float_t GetBrhoRef(Int_t run_number)
   {
      return GetInfoValue(run_number, 1);
   }

   std::pair<Float_t, Float_t> GetTransCoef(Float_t VamosAngle_deg, Float_t delta, Float_t thetaI_rad);

   void PrintRunInfoVector();
   void PrintTransCoefInfoVector();
   void PrintTransCoefStepVector();

   void SetVerbose(Bool_t verbose)
   {
      fkverbose = verbose;
   }

   Float_t GetWeight(Float_t brho_exp, Float_t thetaI_exp);


   ClassDef(KVVAMOSWeightFinder, 1) //Class used to compute normalisation weights for VAMOS events
};

//Global variable
R__EXTERN KVVAMOSWeightFinder* gVAMOSWeightFinder;

#endif
