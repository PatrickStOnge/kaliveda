/***************************************************************************
$Id: VAMOSLinkDef.h,v 1.4 2009/01/14 15:59:49 franklan Exp $
 ***************************************************************************/

#ifdef __CINT__

#include "RVersion.h"

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class MinimiserData+;
#pragma link C++ class MonoMinimiserData+;
#pragma link C++ class ThreadedMinimiserData+;

#pragma link C++ class ThreadedMassEstimator+;
#pragma link C++ class MEDetectorStack+;

#pragma link C++ class MonoMinimiserImpl+;
#pragma link C++ class ThreadedMinimiserImpl+;
#pragma link C++ class SiliconEnergyMinimiser+;

#endif
