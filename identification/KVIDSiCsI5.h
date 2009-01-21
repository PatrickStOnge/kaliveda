/***************************************************************************
                          KVIDSiCsI5.h  -  description
                             -------------------
    begin                : Fri Feb 20 2004
    copyright            : (C) 2004 by J.D. Frankland
    email                : frankland@ganil.fr
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KVIDSICSI5_H
#define KVIDSICSI5_H

#include "KVIDSiCsI.h"
#include "KVTGIDManager.h"

class KVMultiDetArray;

class KVIDSiCsI5:public KVIDSiCsI, public KVTGIDManager {

   enum {
      kHasPG2 = BIT(0)
   };
   Int_t fWhichGrid;            //!internal IdentifyZ status code, tells us which Z grid was last used

   enum {                       //grid identifiers
      k_GG, k_PG1, k_PG2
   };

 public:

   enum {                       //identification status subcodes
      k_OK_GG = KVTGIDManager::kStatus_OK,      //identification OK GG(either Z & A or just Z)
      k_noTGID_GG,              //no KVTGIDZ GG for Z identification
      k_OutOfIDRange_GG,        //point to identify outside of Z identification range of KVTGIDZ GG
      k_OK_PG1,                 //identification OK PG1 (just Z)
      k_noTGID_PG1,             //no KVTGIDZ PG1 for Z identification
      k_OutOfIDRange_PG1,       //point to identify outside of Z identification range of KVTGIDZ PG1
      k_OK_PG2,                 //identification OK PG2 (just Z)
      k_noTGID_PG2,             //no KVTGIDZ PG2 for Z identification
      k_OutOfIDRange_PG2,       //point to identify outside of Z identification range of KVTGIDZ PG2
      kZOK_AnoTGID,             //Z id OK, mass ID attempted but no KVTGIDZA found
      kZOK_AOutOfIDRange,       //Z id OK, mass ID attempted but point to identify outside of identification range of KVTGIDZA
      kZOK_A_ZtooSmall,         //Z id ok, mass ID attempted but Z passed to IdentA too small (<1)
      kZOK_A_ZtooLarge,         //Z id ok, mass ID attempted but Z passed to IdentA larger than max Z defined by KVTGIDZA              
   };

    KVIDSiCsI5() {
      fWhichGrid = -1;
   };
   virtual ~ KVIDSiCsI5() {
   };

   virtual Double_t IdentifyZ(Double_t & funLTG);

   virtual Bool_t Identify(KVReconstructedNucleus *);

   void SetHasPG2(Bool_t yes=kTRUE) {
      SetBit(kHasPG2, yes);
   };
   Bool_t HasPG2() const {
      return TestBit(kHasPG2);
   };

   void Print(Option_t * opt = "") const;

   virtual Double_t GetIDMapX(Option_t * opt = "");
   virtual Double_t GetIDMapY(Option_t * opt = "");

   const Char_t *GetIDSubCodeString(KVIDSubCode & concat) const;
   virtual Bool_t SetIdentificationParameters(const KVMultiDetArray*);
   virtual void RemoveIdentificationParameters();

   virtual void Initialize();

   ClassDef(KVIDSiCsI5, 2)      //5th campaign INDRA identification using Si-CsI matrices
};

#endif
