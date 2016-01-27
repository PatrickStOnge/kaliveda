// Author: Peter C. Wigg
// Created Sun 17 Jan 20:40:12  2016

///////////////////////////////////////////////////////////////////////////////
//
// ThreadedMinimiserImpl.h
//
// Description
//
// Threaded implementation of the silicon energy minimiser. This class
// implements the minimisation using the ThreadedMassEstimator.
//
// Warning: This implementation does not include a tolerance or a consistent
// maximisation check (maximisation is declared as soon as delta increases, i.e.
// we do not wait to see if delta is maximising consistently). It is recommended
// that you use the standard implementation (MonoMinimiserImpl) if you can as it
// does not require pthreads, is less complex and probably does the job well
// enough. If not then you are free to implement the aforementioned changes :)
//
// Peter C. Wigg <peter.wigg.314159@gmail.com>
// Sun 17 Jan 20:40:12  2016
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __THREADED_MINIMISER_IMPL_H__
#define __THREADED_MINIMISER_IMPL_H__

#if __cplusplus >= 201103L
#include <memory>
#endif

#include "ThreadedMassEstimator.h"

class ThreadedMinimiserImpl {

public:

   // Default constructor
   ThreadedMinimiserImpl();
   // Destructor
   virtual ~ThreadedMinimiserImpl();

   // Initialisation function (to be called post-construction)
   void Init();

   // Multi threaded minimisation implementation
   //
   // We use two threads operating from the same starting point (A=2Z) but
   // working in opposite directions. This is essentially the multi-threaded
   // version of the standard MonoMinimiserImpl minimiser implementation.
   //
   // Parameter: z_value - Measured Z value
   // Parameter: si_energy - Measured silicon energy (MeV)
   // Parameter: csi_light - Measured raw caesium iodide light (Channel)
   //
   // WARNING: This implementation does not include a tolerance or a consistent
   // maximisation check at the moment (maximisation is declared as soon as
   // delta increases, i.e. we do not wait to see if delta is maximising
   // consistently). It is recommended that you use the standard implementation
   // (MonoMinimiserImpl) if you can as it does not require pthreads, is less
   // complex and probably does the job well enough. If not then you are free to
   // implement the aforementioned changes :)
   //
   // See SiliconEnergyMinimiser::Minimise
   // See ThreadedMassEstimator
   Int_t Minimise(UInt_t z_value, Double_t si_energy,
                  Double_t csi_light);

   // Set the identification telescope
   // Parameter: telescope_name - As returned by KVIDTelescope::GetName()
   Bool_t SetIDTelescope(const TString& telescope_name);

   // Set maximum number of iterations per thread
   void SetMaximumIterations(UInt_t max_iterations);

   // Set the algorithm tolerance
   // Parameter: tolerance - Algorithm tolerance (if applicable)
   void SetTolerance(Double_t tolerance = 0.05);

   // Print minimiser implementation information
   void Print() const;

private:

   // Copy constructor (Declared private to prevent copying as it is not worth
   // implementing yet).
   ThreadedMinimiserImpl(const ThreadedMinimiserImpl& rhs);
   // Assignment operator (Declared private to prevent copying as it is not
   // worth implementing yet).
   ThreadedMinimiserImpl& operator=(const ThreadedMinimiserImpl& rhs);

   // Initialisation status
   Bool_t kInitialised_;

   // Minimisation routine error codes
   enum ThreadErrorCodes {
      kNotInitialised = -1,
      kGetPossibleAFailed = -2,
      kEstimateAFailed = -3,
      kEstimateAErrors = -4
   };

#if __cplusplus < 201103L
   // Struct to contain the ThreadedMassEstimator input data
   struct me::EstimatorInput* estimator_input_;
   // Vector to contain list of possible A values
   std::vector<Int_t>* possible_a_values_;

   // Struct to retrieve the ThreadedMassEstimator output data
   struct me::EstimatorResult* estimator_result_;
   // ThreadedMassEstimator (Minimiser)
   ThreadedMassEstimator* mass_estimator_;
#else
   // Struct to contain the ThreadedMassEstimator input data
   std::unique_ptr<struct me::EstimatorInput> estimator_input_;
   // Vector to contain list of possible A values
   std::unique_ptr<std::vector<Int_t> > possible_a_values_;
   // Struct to retrieve the ThreadedMassEstimator output data
   std::unique_ptr<struct me::EstimatorResult> estimator_result_;
   // ThreadedMassEstimator (Minimiser)
   std::unique_ptr<ThreadedMassEstimator> mass_estimator_;
#endif

   ClassDef(ThreadedMinimiserImpl, 1)
};

#endif
