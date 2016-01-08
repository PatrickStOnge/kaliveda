//Created by KVClassFactory on Mon Sep 24 16:08:22 2012
//Author: Dijon Aurore

#ifndef __KVIDGCHIOSI_E494S_H
#define __KVIDGCHIOSI_E494S_H

#include "KVIDGChIoSi.h"
#include "KVIdentificationResult.h"

class KVIDGChIoSi_e494s : public KVIDGChIoSi {

private:
   KVIDLine* fChIoSeuil;    // ChIo threshold line

   void init();

public:

   enum {
      k_BelowSeuilChIo = KVIDGChIoSi::k_RightOfEmaxSi + 1 // point to ID was below of "ChIo threshold" line
   };


   KVIDGChIoSi_e494s();
   virtual ~KVIDGChIoSi_e494s();


   virtual void Identify(Double_t x, Double_t y, KVIdentificationResult*) const;
   virtual void Initialize();


   inline KVIDLine* GetSeuilChIoLine()
   {
      return fChIoSeuil;
   };

   ClassDef(KVIDGChIoSi_e494s, 1) //Specific identification grid for e494s
};

#endif
