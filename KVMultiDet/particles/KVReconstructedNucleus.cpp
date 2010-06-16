/***************************************************************************
                          kvreconstructednucleus.cpp  -  description
                             -------------------
    begin                : Fri Oct 18 2002
    copyright            : (C) 2002 by Alexis Mignon
    email                : mignon@ganil.fr

$Id: KVReconstructedNucleus.cpp,v 1.60 2009/03/03 13:36:00 franklan Exp $
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "Riostream.h"
#include "KVReconstructedNucleus.h"
#include "KVTelescope.h"
#include "KVIDTelescope.h"
#include "KVGroup.h"
#include "KVMultiDetArray.h"
#include "KVACQParam.h"

ClassImp(KVReconstructedNucleus);

//////////////////////////////////////////////////////////////////////////////////////////
//KVReconstructedNucleus
//
//Particles reconstructed from data measured in multidetector arrays using identification
//telescopes (see KVIDTelescope class).
//
//If the particle has been identified (IsIdentified()=kTRUE) and calibrated (IsCalibrated()=kTRUE)
//then information on its nature and kinematics can be obtained using the methods of parent
//classes KVNucleus and KVParticle (GetZ(), GetVelocity(), etc.).
//
//Information specific to the detection and identification of the particle:
//
//   GetDetectorList()         -  list of detectors passed through, in reverse order
//   GetNumDet()               -  number of detectors in list = number of detectors passed through
//   GetDetector(Int_t i)      -  ith detector in list (i=0 is stopping detector)
//   GetStoppingDetector()     -  detector in which particle stopped (first in list)
//
//   GetGroup()                -  group in which particle detected/stopped (see KVGroup)
//   GetTelescope()            -  telescope to which stopping detector belongs (see KVTelescope)
//
//   GetIdentifyingTelescope() -  ID telescope which identified this particle (see KVIDTelescope)
//      IsIdentified()               - =kTRUE if particle identification has been completed
//      IsCalibrated()             - =kTRUE if particle's energy has been set
//      GetIDTelescopes()     - list of ID telescopes through which particle passed.
//                                   this list is used to find an ID telescope which can identify the particle.
//    GetRealZ(), GetRealA()  -  return the Z and A determined with floating-point precision by the
//                               identification of the particle.
//

void KVReconstructedNucleus::init()
{
    //default initialisation
    fRealZ = fRealA = 0.;
    fDetNames = "/";
	 fIDTelName = "";
    fIDTelescope = 0;
    fNSegDet = 0;
    fAnalStatus = 99;
    ResetBit(kIsIdentified);
    ResetBit(kIsCalibrated);
    ResetBit(kCoherency);
   fNumDet = 0;
   fNumPar = 0;
   for (register int i = 0; i < MAX_NUM_DET; i++) {
      fEloss[i] = 0.0;
   }
   for (register int i = 0; i < MAX_NUM_DET; i++) {
      fElossCalc[i] = 0.0;
   }
   for (register int i = 0; i < MAX_NUM_PAR; i++) {
      fACQData[i] = 0;
   }
   for (register int i = 0; i < IDRESULTS_DIM; i++) {
      fIDresults[i].Reset();
   }
}


KVReconstructedNucleus::KVReconstructedNucleus() : fDetList(0)
{
    //default ctor.
    init();
}

KVReconstructedNucleus::
KVReconstructedNucleus(const KVReconstructedNucleus &
                       obj) : fDetList(0)
{
    //copy ctor
    init();
#if ROOT_VERSION_CODE >= ROOT_VERSION(3,4,0)
    obj.Copy(*this);
#else
    ((KVReconstructedNucleus &) obj).Copy(*this);
#endif
}

KVReconstructedNucleus::~KVReconstructedNucleus()
{
    //dtor
    // calls KVGroup::Reset() of the group in which this particle was detected
    if (GetGroup()) {
        GetGroup()->Reset();
    }
    if (fDetList){
        //fDetList->Clear();
        delete fDetList;
        fDetList=0;
    }
    init();
}

//______________________________________________________________________________

void KVReconstructedNucleus::Streamer(TBuffer & R__b)
{
    // Stream an object of class KVReconstructedNucleus.
    //Customized streamer.
    //
    //In order to reconstruct detailed information on the particle's reconstruction,
    //fNSegDet is recalculated from the list of detectors. KVGroup::AddHit is
    //called in order to permit (in KVReconstructedEvent::Streamer) the
    //recalculation of fAnalStatus.

    UInt_t R__s, R__c;
    if (R__b.IsReading()) {
        Version_t R__v = R__b.ReadVersion(&R__s, &R__c);
        //if( R__v < 12 ) { }
        R__b.ReadClassBuffer(KVReconstructedNucleus::Class(),this,R__v,R__s,R__c);
        // if the multidetector object exists, update some informations
        // concerning the detectors etc. hit by this particle
        if ( gMultiDetArray ){
            if(!fDetList) fDetList=new KVHashList;
            MakeDetectorList();
            if (GetGroup()) GetGroup()->AddHit(this);
            fIDTelescope = 0;
			if(fIDTelName!="") fIDTelescope = gMultiDetArray->GetIDTelescope( fIDTelName.Data() );
            TIter next_det(fDetList);
            KVDetector *det;register int ndet = 0;UInt_t npar=0;
            while ( (det = (KVDetector*)next_det()) ){
                fNSegDet += det->GetSegment();
                det->AddHit(this);
               det->SetEnergy(fEloss[ndet]);
               det->SetECalc((Double_t)fElossCalc[ndet]);
               ndet++;
               if (det->GetACQParamList()) {
                  TIter next_par(det->GetACQParamList());
                  KVACQParam *par;
                  while ((par = (KVACQParam *) next_par())) {
                     par->SetData(fACQData[npar++]);
                  }
               }
                //modify detector's counters depending on particle's identification state
                if (IsIdentified())
                    det->IncrementIdentifiedParticles();
                else
                    det->IncrementUnidentifiedParticles();
            }
        }
    } else {
        R__b.WriteClassBuffer(KVReconstructedNucleus::Class(),this);
    }
}

//___________________________________________________________________________

void KVReconstructedNucleus::Print(Option_t * option) const
{

    cout << "KVReconstructedNucleus:" << endl;

    if (GetNumDet()) {

        for (int i = GetNumDet() - 1; i >= 0; i--) {
            KVDetector *det = GetDetector(i);
            if(det) det->Print("data");
        }
        for(int i = 1; i<= IDRESULTS_DIM; i++){
        	KVIdentificationResult* idr = const_cast<KVReconstructedNucleus*>(this)->GetIdentificationResult(i);
        	if(idr && idr->IDattempted) idr->Print();
        }
        if(IsCalibrated()){
         	cout << endl << " +++ Calculated & measured energy losses for this particle : " << endl;
        	for (int i = GetNumDet() - 1; i >= 0; i--) {
            	KVDetector *det = GetDetector(i);
            	if(det) {
            		cout << det->GetName() << "    Eloss = " << det->GetEnergy() << " MeV         Calculated = "
            		<< GetElossCalc(det) << " MeV";
            		cout << "          (difference = " << det->GetEnergy()-GetElossCalc(det) << " MeV)";
            		cout << endl;
            	}
        	}
		}
    }
}

//_______________________________________________________________________________

#if ROOT_VERSION_CODE >= ROOT_VERSION(3,4,0)
void KVReconstructedNucleus::Copy(TObject & obj) const
#else
void KVReconstructedNucleus::Copy(TObject & obj)
#endif
{
    //
    //Copy this to obj
    //
    KVNucleus::Copy(obj);
    ((KVReconstructedNucleus &) obj).SetIdentifyingTelescope(GetIdentifyingTelescope());
	 ((KVReconstructedNucleus &) obj).fDetNames = fDetNames;
    ((KVReconstructedNucleus &) obj).SetRealZ(GetRealZ());
    ((KVReconstructedNucleus &) obj).SetRealA(GetRealA());
   ((KVReconstructedNucleus &) obj).SetNumDet(GetNumDet());
   ((KVReconstructedNucleus &) obj).SetNumPar(GetNumPar());
   ((KVReconstructedNucleus &) obj).SetElossTable(GetElossTable());
   ((KVReconstructedNucleus &) obj).SetElossCalcTable(GetElossCalcTable());
   ((KVReconstructedNucleus &) obj).SetACQData(GetACQData());
}



void KVReconstructedNucleus::Clear(Option_t * opt)
{
    //Reset nucleus. Calls KVNucleus::Clear.
    //Calls KVGroup::Reset for the group where it was reconstructed.

    KVNucleus::Clear(opt);
    if (GetGroup())
        GetGroup()->Reset();
    if (fDetList){
        //fDetList->Clear();
        delete fDetList;
        fDetList=0;
    };
    init();
}

void KVReconstructedNucleus::AddDetector(KVDetector * det)
{
    //Add a detector to the list of those through which the particle passed.
    //Put reference to detector into fDetectors array, store detector's energy loss value in
    //fEloss array, increase number of detectors by one.
    //As this is only used in initial particle reconstruction, we add 1 unidentified particle to the detector.
    // Creates KVHashList fDetList in case it does not exist.

    //add name of detector to fDetNames
    fDetNames += det->GetName();
    fDetNames += "/";
    // store pointer to detector
    if (!fDetList) fDetList = new KVHashList;
    fDetList->Add(det);
   if (fNumDet == MAX_NUM_DET) {
      Warning("AddDetector",
              "Cannot store informations for more than %d detectors for reconstructed nucleus. Detector infos not taken into account",
              fNumDet);
   }
   else {
   		fEloss[fNumDet++] = det->GetEnergy();
   		if (det->GetACQParamList()) {
      		if ((fNumPar + det->GetACQParamList()->GetSize()) > MAX_NUM_PAR) {
         		Warning("AddDetector",
                 "Cannot add more than %d DAQ parameters to reconstructed nucleus. Parameters not taken into account",
                 MAX_NUM_PAR);
      		}
      		else{
      			TIter next_par(det->GetACQParamList());
      			KVACQParam *par;
      			while ((par = (KVACQParam *) next_par())) {
         			fACQData[fNumPar++] = par->GetCoderData();
      			}
   			}
   		}
   	}
    //add segmentation index of detector to total segmentation index of particle
    fNSegDet += det->GetSegment();
    //add 1 unidentified particle to the detector
    det->IncrementUnidentifiedParticles();
}

//______________________________________________________________________________________________//

void KVReconstructedNucleus::Reconstruct(KVDetector * kvd)
{
    //Reconstruction of a detected nucleus from the successive energy losses
    //measured in a series of detectors/telescopes.
    //
    //Starting from detector *kvd, collect information from all detectors placed directly
    //in front of *kvd (kvd->GetGroup()->GetAlignedDetectors( kvd )),
    //these are the detectors the particle has passed through.
    //
    //Each one is added to the particle's list (KVReconstructedNucleus::AddDetector), and,
    //if it is not an unsegmented detector, it is marked as having been "analysed"
    //(KVDetector::SetAnalysed) in order to stop it being considered as a starting point for another
    //particle reconstruction.

    fNSegDet = 0;

    //get list of detectors through which particle passed
    if (kvd->GetGroup()) {
        TList *aligned = kvd->GetGroup()->GetAlignedDetectors(kvd);
        if (aligned) {

            TIter next_aligned(aligned);
            KVDetector *d;
            while ((d = (KVDetector *) next_aligned())) {
                AddDetector(d);
                d->AddHit(this);  // add particle to list of particles hitting detector
                d->SetAnalysed(kTRUE);   //cannot be used to seed another particle
            }
            delete aligned;
        }
        kvd->GetGroup()->AddHit(this);
    }
}

//_____________________________________________________________________________________

void KVReconstructedNucleus::Identify()
{
    // Try to identify this nucleus by calling the Identify() function of each
    // ID telescope crossed by it, starting with the telescope where the particle stopped, in order
    //      -  only attempt identification in ID telescopes containing the stopping detector.
    //      -  only telescopes which have been correctly initialised for the current run are used,
    //         i.e. those for which KVIDTelescope::IsReadyForID() returns kTRUE.
    // This continues until a successful identification is achieved or there are no more ID telescopes to try.
    // The identification code corresponding to the identifying telescope is set as the identification code of the particle.

//    TList *idt_list = GetStoppingDetector()->GetTelescopesForIdentification();
    KVList *idt_list = GetStoppingDetector()->GetAlignedIDTelescopes();

    if (idt_list && idt_list->GetSize() > 0) {

        KVIDTelescope *idt;
        TIter next(idt_list);
        Int_t idnumber = 1;

        while ((idt = (KVIDTelescope *) next())) {

            if ( idt->IsReadyForID() ) { // is telescope able to identify for this run ?

				KVIdentificationResult *IDR=GetIdentificationResult(idnumber++);
				if(!IDR) break;
				
				idt->Identify( IDR );
				
                if (IDR->IDOK && !IsIdentified()) {  //Identify()=kTRUE if ID successful
                    //cout << " IDENTIFICATION SUCCESSFUL"<<endl;
                    SetIsIdentified();
                    SetIdentifyingTelescope(idt);
      				SetIdentification( IDR );
                } 
                    //We reduce the "segmentation" index by 1.
                    //If this remains >=2, we carry on trying to identify
                    //However, if it falls to 1, then the particle's identifiability depends
                    //on the rest of the KVGroup where it was detected.
                    //If there are no other _unidentified_ particles in the group, it's fine.
                    //If there _are_ other unidentified particles in the group, then we should wait until
                    //the next ID round (equivalent of particles with GetAnalStatus=1).
                    SetNSegDet(TMath::Max(GetNSegDet() - 1, 0));
                    //if there are other unidentified particles in the group and NSegDet is < 2
                    //then exact status depends on segmentation of the other particles : reanalyse
                    //cout << "...........NSegDet now = " << GetNSegDet() << " && number unidentified in group="
                    //              << (int)GetGroup()->GetNUnidentified() << endl;
                    if (GetNSegDet() < 2 && GetGroup()->GetNUnidentified() > 1)
                        break;
                    //if NSegDet = 0 it's hopeless
                    if (!GetNSegDet())
                        break;

            }

        }

    }
}

//______________________________________________________________________________________________//

void KVReconstructedNucleus::SetElossCalc(KVDetector * det, Double_t energy)
{
    // Store calculated energy loss value for this particle corresponding to given detector
    // The particle's contribution is subtracted from the detector's calculated energy
    // loss and the detector's "reanalysis" flag is set to true.

    if (!det)
        return;
    Int_t index = GetDetectorList()->IndexOf(det);
    if (index < 0)
        return;                   //detector not in list
   fElossCalc[index] = energy;
    det->SetECalc(energy);
}

//______________________________________________________________________________________________//

Double_t KVReconstructedNucleus::GetElossCalc(KVDetector * det) const
{
    //Get calculated energy loss value for this particle corresponding to given detector

    Int_t index = GetDetectorList()->IndexOf(det);
    if (index < 0)
        return -1.0;              //detector not in list
    return fElossCalc[index];
}

//______________________________________________________________________________________________//

void KVReconstructedNucleus::GetAnglesFromTelescope(Option_t * opt)
{
    //Calculate angles theta and phi for reconstructed nucleus based
    //on position of telescope in which it was detected. The nucleus'
    //momentum is set using these angles, its mass and its kinetic energy.
    //The (optional) option string can be "random" or "mean".
    //If "random" (default) the angles are drawn at random between the
    //min/max angles of the telescope.
    //If "mean" the (theta,phi) position of the center of the telescope
    //is used to fix the nucleus' direction.

    //don't try if particle has no correctly defined energy
    if (GetEnergy() <= 0.0)
        return;
    if (!GetTelescope())
        return;

    if (!strcmp(opt, "random")) {
        //random angles
        SetRandomMomentum(GetEnergy(), GetTelescope()->GetThetaMin(),
                          GetTelescope()->GetThetaMax(),
                          GetTelescope()->GetPhiMin(),
                          GetTelescope()->GetPhiMax(), "random");
    } else {
        //middle of telescope
        TVector3 dir = GetTelescope()->GetDirection();
        SetMomentum(GetEnergy(), dir);
    }
}

//______________________________________________________________________________________________//

Int_t KVReconstructedNucleus::GetIDSubCode(const Char_t * id_tel_type,
        KVIDSubCode & code) const
{
    //If the identification of the particle was attempted using a KVIDTelescope of type "id_tel_type",
    //and 'code' holds all the subcodes from the different identification routines tried for this particle,
    //then this method returns the subcode for this particle from telescope type "id_tel_type".
    //
    //In case of problems (no ID telescope of type 'id_tel_type'), the returned value is -65535.
    //
    //If no type is given (first argument = ""), we use the identifying telescope (obviously if the
    //particle has remained unidentified - IsIdentified()==kFALSE - and the GetIdentifyingTelescope()
    //pointer is not set, we return -65535).

    KVIDTelescope *idtel;
    if (strcmp(id_tel_type, ""))
        idtel =
            (KVIDTelescope *) GetIDTelescopes()->
            FindObjectByType(id_tel_type);
    else
        idtel = GetIdentifyingTelescope();
    if (!idtel)
        return -65535;
    return idtel->GetIDSubCode(code);
}

//______________________________________________________________________________________________//

const Char_t *KVReconstructedNucleus::GetIDSubCodeString(const Char_t *
        id_tel_type,
        KVIDSubCode &
        code) const
{
    //If the identification of the particle was attempted using a KVIDTelescope of type "id_tel_type",
    //and 'code' holds all the subcodes from the different identification routines tried for this particle,
    //then this method returns an explanation for the subcode for this particle from telescope type "id_tel_type".
    //
    //If no type is given (first argument = ""), we use the identifying telescope.
    //
    //In case of problems :
    //       no ID telescope of type 'id_tel_type' :  "No identification attempted in id_tel_type"
    //       particle not identified               :  "Particle unidentified. Identifying telescope not set."

    KVIDTelescope *idtel;
    if (strcmp(id_tel_type, ""))
        idtel =
            (KVIDTelescope *) GetIDTelescopes()->
            FindObjectByType(id_tel_type);
    else
        idtel = GetIdentifyingTelescope();
    if (!idtel) {
        if (strcmp(id_tel_type, ""))
            return Form("No identification attempted in %s", id_tel_type);
        else
            return
                Form("Particle unidentified. Identifying telescope not set.");
    }
    return idtel->GetIDSubCodeString(code);
}

//_________________________________________________________________________________

void KVReconstructedNucleus::Calibrate()
{
    //Calculate and set the energy of a (previously identified) reconstructed particle,
    //including an estimate of the energy loss in the target.
    //
    //Starting from the detector in which the particle stopped, we add up the
    //'corrected' energy losses in all of the detectors through which it passed.
    //Whenever possible, for detectors which are not calibrated or not working,
    //we calculate the energy loss. Measured & calculated energy losses are also
    //compared for each detector, and may lead to new particles being seeded for
    //subsequent identification. This is done by KVIDTelescope::CalculateParticleEnergy().
    //
    //For particles whose energy before hitting the first detector in their path has been
    //calculated after this step we then add the calculated energy loss in the target,
    //using gMultiDetArray->GetTargetEnergyLossCorrection().

    KVIDTelescope* idt = GetIdentifyingTelescope();
    idt->CalculateParticleEnergy(this);
    //if calibration was successful, we now read the energy losses from the detectors
    //this is because detectors whose calibration
    //depends on the identity (Z & A) of the particle will only have their energy loss
    //member set when this method is called; otherwise it remains 0.
    //Also, whenever possible, we calculate the energy loss for any uncalibrated or
    //non-functioning detectors during this procedure, so this calculated energy loss
    //will also be stored.
    if ( idt->GetCalibStatus() != KVIDTelescope::kCalibStatus_NoCalibrations ){
        SetIsCalibrated();
        //add correction for target energy loss - charged particles only
        Double_t E_targ = 0.;
        if(GetZ()) {
        	E_targ = gMultiDetArray->GetTargetEnergyLossCorrection(this);
        	SetTargetEnergyLoss( E_targ );
        }
        Double_t E_tot = GetEnergy() + E_targ;
        SetEnergy( E_tot );
		TIter nxt(GetDetectorList()); KVDetector* det; register int ndet = 0;
        while( (det = (KVDetector*)nxt()) ){
          fEloss[ndet] = det->GetEnergy();
          //fElossCalc[ndet] = det->GetECalc();
          ++ndet;
        }
    }
}

void KVReconstructedNucleus::MakeDetectorList()
{
    // Protected method, called when required to fill fDetList with pointers to
    // the detectors whose names are stored in fDetNames.
    // If gMultiDetArray=0x0, fDetList list will be empty.

	fDetList->Clear();
    if ( gMultiDetArray ){
    	fDetNames.Begin("/");
    	while ( !fDetNames.End() ) {
    	    KVDetector* det = gMultiDetArray->GetDetector( fDetNames.Next(kTRUE) );
    	    if ( det ) fDetList->Add(det);
    	} 
    }
}

KVIdentificationResult* KVReconstructedNucleus::GetIdentificationResult(Int_t i)
{
	// Returns the result of the i-th identification attempted for this nucleus.
	// i=1 : identification telescope in which particle stopped
	// i=2 : identification telescope immediately in front of the first
	// etc. etc.
	if(i-1< IDRESULTS_DIM){
		fIDresults[i-1].SetNumber(i);
		return &fIDresults[i-1];
	}
	else return 0;
}

void KVReconstructedNucleus::SetIdentification(KVIdentificationResult* idr)
{
	// Set identification of nucleus from informations in identification result object
               	  SetIDCode( idr->IDcode );
               	  SetZMeasured( idr->Zident );
               	  SetAMeasured( idr->Aident );
               	  SetZ( idr->Z );
               	  if(idr->A > 0) SetA( idr->A );
               	  if(idr->Aident) SetRealA( idr->PID );
               	  else SetRealZ( idr->PID );
}
