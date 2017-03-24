void FilteredEventAnalysisTemplate::InitAnalysis()
{
   // INITIALISATION PERFORMED AT BEGINNING OF ANALYSIS
   // Here you define:
   //   - global variables
   //   - histograms
   //   - trees
   //
   // NB: no access to multidetector array or reaction
   //     kinematics yet: see InitRun()

   // DEFINITION OF GLOBAL VARIABLES FOR ANALYSIS

   // charged particle multiplicity
   KVVarGlob* v = AddGV("KVVGSum", "Mcha");
   v->SetOption("mode", "mult");
   v->SetSelection(KVParticleCondition("_NUC_->GetZ()>0"));

   AddGV("KVZtot", "ZTOT");//total charge
   AddGV("KVZVtot", "ZVTOT");//total pseudo-momentum
   ZMAX = (KVZmax*)AddGV("KVZmax", "ZMAX");//fragments sorted by Z

   // DEFINITION OF TREE USED TO STORE RESULTS
   CreateTreeFile();

   TTree* t = new TTree("data", GetOpt("SimulationInfos"));

   // add a branch to tree for each defined global variable
   GetGVList()->MakeBranches(t);

   // add branches to be filled by user
   t->Branch("mult", &mult);
   t->Branch("Z", Z, "Z[mult]/I");
   t->Branch("A", A, "A[mult]/I");
   t->Branch("array", array, "array[mult]/I");
   t->Branch("idcode", idcode, "idcode[mult]/I");
   t->Branch("ecode", ecode, "ecode[mult]/I");
   t->Branch("Ameasured", Ameasured, "Ameasured[mult]/I");
   t->Branch("Vper", Vper, "Vper[mult]/D");
   t->Branch("Vpar", Vpar, "Vpar[mult]/D");
   t->Branch("ELab", ELab, "ELab[mult]/D");
   t->Branch("ThetaLab", ThetaLab, "ThetaLab[mult]/D");
   t->Branch("PhiLab", PhiLab, "PhiLab[mult]/D");

   AddTree(t);

}

//____________________________________________________________________________________

void FilteredEventAnalysisTemplate::InitRun()
{
   // INITIALISATION PERFORMED JUST BEFORE ANALYSIS
   // In this method the multidetector array/setup used to filter
   // the simulation is available (gMultiDetArray)
   // The kinematics of the reaction is available (KV2Body*)
   // using gDataAnalyser->GetKinematics()

   // normalize ZVtot to projectile Z*v
   const KV2Body* kin = gDataAnalyser->GetKinematics();
   GetGV("ZVTOT")->SetParameter("Normalization",
                                kin->GetNucleus(1)->GetVpar()*kin->GetNucleus(1)->GetZ());
}

//____________________________________________________________________________________

Bool_t FilteredEventAnalysisTemplate::Analysis()
{
   // EVENT BY EVENT ANALYSIS

   // Reject events with less good particles than acquisition trigger for run
   if (!GetEvent()->IsOK()) return kTRUE;

   mult = GetEvent()->GetMult("ok");

   for (int i = 0; i < mult; i++) {
      KVReconstructedNucleus* part = (KVReconstructedNucleus*)ZMAX->GetZmax(i);
      Z[i] = part->GetZ();
      A[i] = part->GetA();
      idcode[i] = part->GetIDCode();
      ecode[i] = part->GetECode();
      Ameasured[i] = part->IsAMeasured();
      // Example for events filtered with FAZIA@INDRA set-up
      if (part->GetParameters()->GetTStringValue("ARRAY") == "INDRA") array[i] = 0;
      else if (part->GetParameters()->GetTStringValue("ARRAY") == "FAZIA") array[i] = 1;
      else array[i] = -1;
      Vper[i] = part->GetFrame("cm")->GetVperp();
      Vpar[i] = part->GetFrame("cm")->GetVpar();
      ELab[i] = part->GetEnergy();
      ThetaLab[i] = part->GetTheta();
      PhiLab[i] = part->GetPhi();
   }

   GetGVList()->FillBranches();
   FillTree();

   return kTRUE;
}

