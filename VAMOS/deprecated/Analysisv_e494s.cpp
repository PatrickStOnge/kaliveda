#include "Analysisv_e494s.h"

/**
   WARNING: This class has been deprecated and will eventually be removed. Do
   not use!

   This class is only compiled if the deprecated code is enabled in the build
   configuration (e.g. cmake -DUSE_DEPRECATED_VAMOS=yes). If you enable the
   deprecated code then a large number of warnings will be printed to the
   terminal. To disable these warnings (not advised) compile VAMOS with
   -Wno-deprecated-declarations. Despite these warnings the code should compile
   just fine. The warnings are there to prevent the unwitting use of the
   deprecated code (which should be strongly discouraged).

   BY DEFAULT THIS CLASS IS NOT COMPILED.

   Deprecated by: Peter Wigg (peter.wigg.314159@gmail.com)
   Date:          Thu 17 Dec 17:24:38 GMT 2015
*/

using namespace std;

ClassImp(Analysisv_e494s)

////////////////////////////////////////////////////////////////////////////////
// BEGIN_HTML <!--
/* -->
<h2>Analysisv_e494s</h2>
<h4>VAMOS calibration for E494S</h4>
<p>
Part of the VAMOS analysis package kindly contributed by Maurycy Rejmund (GANIL).
</p>
<!-- */
// --> END_HTML
////////////////////////////////////////////////////////////////////////////////

Analysisv_e494s::Analysisv_e494s(LogFile* Log)
   : Analysisv(Log)
{
   //Default constructor
#ifdef DEBUG
   cout << "Analysisv_e494s::Constructor" << endl;
#endif

#ifdef ACTIVEBRANCHES
   cout << "ActiveBranches defined" << endl;
   L->Log << "ActiveBranches defined" << endl;
#else
   cout << "ActiveBranches NOT defined" << endl;
   L->Log << "ActiveBranches NOT defined" << endl;
#endif

#ifdef MULTIPLEPEAK
   cout << "Multiple peak rejection defined" << endl;
   L->Log << "Multiple peak rejection defined" << endl;
#else
   cout << "Multiple peak rejection NOT defined" << endl;
   L->Log << "Multiple peak rejection NOT defined" << endl;
#endif

#ifdef SECHIP
   cout << "SECHIP defined" << endl;
   L->Log << "SECHIP defined" << endl;
#else
   cout << "SECHIP NOT defined" << endl;
   L->Log << "SECHIP peak rejection NOT defined" << endl;
#endif

#ifdef WEIGHTEDAVERAGE
   cout << "Weighted Average defined" << endl;
   L->Log << "Weighted Average defined" << endl;
#else
   cout << "Weighted Average NOT defined" << endl;
   L->Log << "Weighted Average NOT defined" << endl;
#endif

#ifdef FOLLOWPEAKS
   cout << "Follow Peaks defined" << endl;
   L->Log << "Follow Peaks defined" << endl;
#else
   cout << "Follow Peaks NOT defined" << endl;
   L->Log << "Follow Peaks NOT defined" << endl;
#endif
#ifdef EGCORR
   cout << "EGCORR defined" << endl;
   L->Log << "EGCORR defined" << endl;
#else
   cout << "EGCORR NOT defined" << endl;
   L->Log << "EGCORR NOT defined" << endl;
#endif


#ifdef PLASTIC
   cout << "Plastic defined" << endl;
   L->Log << "Plastic defined" << endl;

   Pl = new Plasticv(L);
   if (!Pl) {
      cout << "Coud not allocate memory to hold Plastic !" << endl;
      exit(EXIT_FAILURE);
   }
#else
   cout << "Plastic will not be treated !" << endl;
   L->Log << "Plastic will not be treated !" << endl;
#endif

#ifdef IONCHAMBER
   cout << "IonisationChamber defined" << endl;
   L->Log << "IonisationChamber defined" << endl;
   Ic = new IonisationChamberv(L);
   if (!Ic) {
      cout << "Coud not allocate memory to hold IonisationChamber !" << endl;
      exit(EXIT_FAILURE);
   }
#else
   cout << "IonisationChamber will not be treated !" << endl;
   L->Log << "IonisationChamber will not be treated !" << endl;
#endif

#ifdef SI
   cout << "Si defined" << endl;
   L->Log << "Si defined" << endl;
   Si = new Siv(L);
   if (!Ic) {
      cout << "Coud not allocate memory to hold Si !" << endl;
      exit(EXIT_FAILURE);
   }
#else
   cout << "Si will not be treated !" << endl;
   L->Log << "Si will not be treated !" << endl;
#endif

#ifdef DRIFT
   cout << "DriftChamber defined" << endl;
   L->Log << "DriftChamber defined" << endl;
   Dr = new DriftChamberv(L, Si);
   if (!Dr) {
      cout << "Coud not allocate memory to hold DriftChamber !" << endl;
      exit(EXIT_FAILURE);
   }
   RC = new Reconstructionv(L, Dr);
   if (!RC) {
      cout << "Coud not allocate memory to hold Reconstruction !" << endl;
      exit(EXIT_FAILURE);
   }
   Id = new Identificationv(L, RC, Dr, Ic, Si);
   if (!Id) {
      cout << "Coud not allocate memory to hold Identification !" << endl;
      exit(EXIT_FAILURE);
   }

#else
   cout << "DriftChamber will not be treated !" << endl;
   L->Log << "DriftChamber will not be treated !" << endl;
   cout << "Reconstruction will not be treated !" << endl;
   L->Log << "Reconstruction will not be treated !" << endl;
   cout << "Identification will not be treated !" << endl;
   L->Log << "Identification will not be treated !" << endl;

#endif

#ifdef SED1
   cout << "SED1 defined" << endl;
   L->Log << "SED1 defined" << endl;
   SeD1 = new SeDv(L, 1);
   if (!SeD1) {
      cout << "Coud not allocate memory to hold SeD1 !" << endl;
      exit(EXIT_FAILURE);
   }
#else
   cout << "SeD1 will not be treated !" << endl;
   L->Log << "SeD1 will not be treated !" << endl;

#endif
#ifdef SED2
   cout << "SED2 defined" << endl;
   L->Log << "SED2 defined" << endl;
   SeD2 = new SeDv(L, 2);
   if (!SeD1) {
      cout << "Coud not allocate memory to hold SeD2 !" << endl;
      exit(EXIT_FAILURE);
   }
#else
   cout << "SeD2 will not be treated !" << endl;
   L->Log << "SeD2 will not be treated !" << endl;

#endif
#ifdef SED12
   cout << "SED12 defined" << endl;
   L->Log << "SED12 defined" << endl;
   SeD12 = new SeD12v(L, SeD1, SeD2);
   if (!SeD12) {
      cout << "Coud not allocate memory to hold SeD12 !" << endl;
      exit(EXIT_FAILURE);
   }
   RC = new ReconstructionSeDv(L, SeD12);
   if (!RC) {
      cout << "Coud not allocate memory to hold ReconstructioSeDn !" << endl;
      exit(EXIT_FAILURE);
   }
#else
   cout << "SeD12 will not be treated !" << endl;
   L->Log << "SeD12 will not be treated !" << endl;
   cout << "ReconstructionSeD will not be treated !" << endl;
   L->Log << "ReconstructionSeD will not be treated !" << endl;

#endif



#ifdef EXOGAM
   cout << "Exogam defined" << endl;
   L->Log << "Exogam defined" << endl;
   Ex = new Exogamv(L, RC, Id, Si);
   if (!Ex) {
      cout << "Coud not allocate memory to hold Exogam !" << endl;
      exit(EXIT_FAILURE);
   }
#else
   cout << "Exogam will not be treated !" << endl;
   L->Log << "Exogam will not be treated !" << endl;
#endif

}

Analysisv_e494s::~Analysisv_e494s()
{
   //Destructor
#ifdef DEBUG
   cout << "Analysisv_e494s::Destuctor" << endl;
#endif

#ifdef PLASTIC
   delete Pl;
#endif

#ifdef DRIFT
   delete Dr;
   delete RC;
   delete Id;
#endif

#ifdef SED1
   delete SeD1;
#endif
#ifdef SED2
   delete SeD2;
#endif
#ifdef SED12
   delete SeD12;
   delete RC;
#endif


#ifdef IONCHAMBER
   delete Ic;
#endif

#ifdef SI
   delete Si;
#endif
#ifdef EXOGAM
   delete Ex;
#endif
}

void Analysisv_e494s::Treat()
{
#ifdef DEBUG
   cout << "Analysisv_e494s::Treat " << endl;
#endif

#ifdef PLASTIC
   Pl->Treat();
#endif

#ifdef IONCHAMBER
   Ic->Treat();
#endif

#ifdef SI
   Si->Treat();
#endif
#ifdef DRIFT
   Dr->Treat();
   RC->Treat();
   Id->Treat();
#endif

#ifdef SED1
   SeD1->Treat();
#endif

#ifdef SED2
   SeD2->Treat();
#endif
#ifdef SED12
   SeD12->Treat();
   RC->Treat();
#endif



#ifdef EXOGAM
   Ex->Treat();
#endif
}


void Analysisv_e494s::CreateHistograms()
{
#ifdef DEBUG
   cout << "Analysisv_e494s::CreateHistograms : " << endl;
#endif

#ifdef PLASTIC
   Pl->CreateHistograms();
#endif

#ifdef DRIFT
   Dr->CreateHistograms();
   RC->CreateHistograms();
   Id->CreateHistograms();
#endif

#ifdef SED1
   SeD1->CreateHistograms();
#endif

#ifdef SED2
   SeD2->CreateHistograms();
#endif

#ifdef SED12
   SeD12->CreateHistograms();
   RC->CreateHistograms();
#endif


#ifdef IONCHAMBER
   Ic->CreateHistograms();
#endif

#ifdef EXOGAM
   Ex->CreateHistograms();
#endif
#ifdef SI
   Si->CreateHistograms();
#endif



}
void Analysisv_e494s::FillHistograms()
{
#ifdef DEBUG
   cout << "Analysisv_e494s::FillHistograms : " << endl;
#endif

#ifdef PLASTIC
   Pl->FillHistograms();
#endif

#ifdef DRIFT
   Dr->FillHistograms();
   RC->FillHistograms();
   Id->FillHistograms();
#endif

#ifdef SED1
   SeD1->FillHistograms();
#endif

#ifdef SED2
   SeD2->FillHistograms();
#endif

#ifdef SED12
   SeD12->FillHistograms();
   RC->FillHistograms();
#endif

#ifdef IONCHAMBER
   Ic->FillHistograms();
#endif

#ifdef EXOGAM
   Ex->FillHistograms();
#endif
#ifdef SI
   Si->FillHistograms();
#endif

}

void Analysisv_e494s::outAttach()
{
#ifdef DEBUG
   cout << "Analysisv_e494s::outAttach " << endl;
#endif

#ifdef PLASTIC
   Pl->outAttach(outT);
#endif

#ifdef DRIFT
   Dr->outAttach(outT);
   RC->outAttach(outT);
   Id->outAttach(outT);
#endif

#ifdef SED1
   SeD1->outAttach(outT);
#endif
#ifdef SED2
   SeD2->outAttach(outT);
#endif
#ifdef SED12
   SeD12->outAttach(outT);
   RC->outAttach(outT);
#endif


#ifdef IONCHAMBER
   Ic->outAttach(outT);
#endif

#ifdef EXOGAM
   Ex->outAttach(outT);
#endif
#ifdef SI
   Si->outAttach(outT);
#endif

   outT->Branch("PILEUP", T_Raw + 0, "PILEUP/s");
   outT->Branch("GATCONF", T_Raw + 1, "GATCONF/s");
   outT->Branch("TSED1_HF", T_Raw + 2, "TSeD1_Hf/s");
   outT->Branch("TSED2_HF", T_Raw + 3, "TSeD2_Hf/s");
   outT->Branch("TSED1_SED2", T_Raw + 4, "TSeD1_SeD2/s");
   outT->Branch("TSI_SED1", T_Raw + 5, "TSi_SeD1/s");
   outT->Branch("TSI_HF", T_Raw + 6, "TSi_Hf/s");


}

void Analysisv_e494s::inAttach()
{
#ifdef DEBUG
   cout << "Analysisv_e494s::inAttach " << endl;
#endif



#ifdef PLASTIC
   Pl->inAttach(inT);
#endif

#ifdef DRIFT
   Dr->inAttach(inT);
   // No inAttach in Reconstruction
   Id->inAttach(inT);
#endif
#ifdef SED1
   SeD1->inAttach(inT);
#endif
#ifdef SED2
   SeD2->inAttach(inT);
#endif
#ifdef SED12
   SeD12->inAttach(inT);
   // No inAttach in Reconstruction
#endif


#ifdef IONCHAMBER
   Ic->inAttach(inT);
#endif

#ifdef EXOGAM
   Ex->inAttach(inT);
#endif
#ifdef SI
   Si->inAttach(inT);
#endif

   inT->SetBranchStatus("PILEUP", 1);

   inT->SetBranchAddress("PILEUP", T_Raw + 0);
   inT->SetBranchStatus("GATCONF", 1);

   inT->SetBranchAddress("GATCONF", T_Raw + 1);

   inT->SetBranchStatus("TSED1_HF", 1);

   inT->SetBranchAddress("TSED1_HF", T_Raw + 2);
   inT->SetBranchStatus("TSED2_HF", 1);

   inT->SetBranchAddress("TSED2_HF", T_Raw + 3);
   inT->SetBranchStatus("TSED1_SED2", 1);

   inT->SetBranchAddress("TSED1_SED2", T_Raw + 4);
   inT->SetBranchStatus("TSI_SED1", 1);

   inT->SetBranchAddress("TSI_SED1", T_Raw + 5);
   inT->SetBranchStatus("TSI_HF", 1);

   inT->SetBranchAddress("TSI_HF", T_Raw + 6);

}

