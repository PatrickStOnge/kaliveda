/*
$Id: KVRawDataReader.cpp,v 1.2 2007/05/31 09:59:22 franklan Exp $
$Revision: 1.2 $
$Date: 2007/05/31 09:59:22 $
*/

//Created by KVClassFactory on Wed May 16 15:52:21 2007
//Author: franklan

#include "KVRawDataReader.h"
#include "KVBase.h"
#include "TPluginManager.h"

ClassImp(KVRawDataReader)

////////////////////////////////////////////////////////////////////////////////
// BEGIN_HTML <!--
/* -->
<h2>KVRawDataReader</h2>
<h4>Pure abstract base class for reading raw data</h4>
Classes derived from this one must implement the methods
<li>
<ui>GetNextEvent</ui>
<ui>GetFiredDataParameters</ui>
</li>
<!-- */
// --> END_HTML
////////////////////////////////////////////////////////////////////////////////

KVRawDataReader::KVRawDataReader()
{
   //Default constructor
}

KVRawDataReader::~KVRawDataReader()
{
   //Destructor
}

