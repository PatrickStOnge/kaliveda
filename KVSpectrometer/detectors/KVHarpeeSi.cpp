//Created by KVClassFactory on Wed Jul 25 10:12:53 2012
//Author: Guilain ADEMARD

#include "KVHarpeeSi.h"
#include "TGeoBBox.h"
#include "KVUnits.h"

ClassImp(KVHarpeeSi)

////////////////////////////////////////////////////////////////////////////////
// BEGIN_HTML <!--
/* -->
<h2>KVHarpeeSi</h2>
<h4>Silicon detectors of Harpee, used at the focal plan of VAMOS</h4>
<!-- */
// --> END_HTML
//
// In order to create a detector, use the KVHarpeeSi::KVHarpeeSi()
// constructor.
//
// Type of detector : "SI"
////////////////////////////////////////////////////////////////////////////////

void KVHarpeeSi::init(){
	// Initialise non-persistent pointers

}
//________________________________________________________________

KVHarpeeSi::KVHarpeeSi(){
   	// Default constructor.
	init();
	SetType("SI");
   	SetName(GetArrayName());
}
//________________________________________________________________

KVHarpeeSi::KVHarpeeSi(UInt_t number, Float_t thick) : KVVAMOSDetector(number, "Si")
{
   // Make a silicon detector of Harpee.
   // Type of detector: "SI"

   init();
   SetType("SI");
   SetName(GetArrayName());

	Float_t w  = 1.;                // width
	Float_t h  = 1.;                // height
	Float_t th = thick*KVUnits::um; // thickness

	
	Double_t dx = w/2;
	Double_t dy = h/2;
	Double_t dz = th/2;

	fTotThick+=th;

	// adding the absorber
	TGeoShape *shape = new TGeoBBox(dx,dy,dz);
	AddAbsorber("Si",shape,0,kTRUE);

}
//________________________________________________________________

KVHarpeeSi::KVHarpeeSi (const KVHarpeeSi& obj)  : KVVAMOSDetector()
{
   // Copy constructor
   // This ctor is used to make a copy of an existing object (for example
   // when a method returns an object), and it is always a good idea to
   // implement it.
   // If your class allocates memory in its constructor(s) then it is ESSENTIAL :-)

   obj.Copy(*this);
}

KVHarpeeSi::~KVHarpeeSi()
{
   // Destructor
}

//________________________________________________________________

void KVHarpeeSi::Copy (TObject& obj) const
{
   // This method copies the current state of 'this' object into 'obj'
   // You should add here any member variables, for example:
   //    (supposing a member variable KVHarpeeSi::fToto)
   //    CastedObj.fToto = fToto;
   // or
   //    CastedObj.SetToto( GetToto() );

   KVVAMOSDetector::Copy(obj);
   //KVHarpeeSi& CastedObj = (KVHarpeeSi&)obj;
}
//________________________________________________________________

const Char_t* KVHarpeeSi::GetArrayName(){
	// Name of detector given in the form 
	// SIE_01 SIE_02 ...
	// to be compatible with GANIL acquisition parameters.
	//
	// The root of the name is the detector type + 'E'.
	
	fFName = Form("%s_%02d",GetType(),GetNumber());
	return fFName.Data();
}
//________________________________________________________________

void KVHarpeeSi::SetACQParams(){
// Setup the energy acquisition parameter for this silicon detector.
// This parameter has the name of the detector and has the type 'E' 
// (for energy).
// 

	KVACQParam *par = new KVACQParam;
	TString name;
	name.Form("%sE_%02d",GetType(),GetNumber());
	par->SetName(name);
	par->SetType("E");
	AddACQParam(par);
//	Info("SetACQParams",name.Data());
}
