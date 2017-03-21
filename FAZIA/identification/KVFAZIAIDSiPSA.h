//Created by KVClassFactory on Mon Feb 17 13:52:51 2014
//Author: John Frankland,,,

#ifndef __KVFAZIAIDSiPSA_H
#define __KVFAZIAIDSiPSA_H

#include "KVFAZIAIDTelescope.h"
#include "KVIDZAGrid.h"
#include "KVFAZIADetector.h"
#include "KVIDCutLine.h"
#include "TGraph.h"

class KVFAZIAIDSiPSA : public KVFAZIAIDTelescope {

   static TGraph* fZThreshold;//! threshold for Z identification from NIM A 701, 145 (2013)
   static TGraph* fAThreshold;//! threshold for A identification from NIM A ?,? (2017)

   KVIDZAGrid* IGrid;    //! telescope's Imax E grid
   KVIDZAGrid* QGrid;    //! telescope's Qrise E grid
   KVFAZIADetector* fSi; //! the silicon detector

public:
   KVFAZIAIDSiPSA();
   virtual ~KVFAZIAIDSiPSA();
   virtual UShort_t GetIDCode()
   {
      return kSi1;
   }

   virtual Bool_t Identify(KVIdentificationResult*, Double_t x = -1., Double_t y = -1.);

   Double_t GetIDMapX(Option_t* opt = "I");
   Double_t GetIDMapY(Option_t* opt = "");
   virtual void Initialize();

   Bool_t CheckTheoreticalIdentificationThreshold(KVNucleus* /*ION*/, Double_t /*EINC*/ = 0.0);
   void SetIdentificationStatus(KVReconstructedNucleus*);

   ClassDef(KVFAZIAIDSiPSA, 1) //PSA identification in first silicon of FAZIA telescopes
};

#endif
