/*
$Id: KVListView.cpp,v 1.6 2009/04/28 09:11:29 franklan Exp $
$Revision: 1.6 $
$Date: 2009/04/28 09:11:29 $
*/

//Created by KVClassFactory on Wed Apr  9 11:51:38 2008
//Author: franklan

#include "KVListView.h"
#include "TGButton.h"

ClassImp(KVListView)

////////////////////////////////////////////////////////////////////////////////
// BEGIN_HTML <!--
/* -->
<h2>KVListView</h2>
<h4>Slightly modified TGListView</h4>

<img src="images/KVLVContainer.png"><br>

<h3>Example of use</h3>
KVListView takes a list of objects (TObject or derived class) and presents them
as a list of items with different data for each object presented in separate columns.
The user specifies the base-class of the objects to display, the number of columns,
and the data to be presented in each column. The example in the figure shown above
was generated by the following:
<pre>
	lvRuns = new KVListView(<a href="KVDBRun.html">KVDBRun</a>::Class(), cfRuns, 500, 250); //create list view for KVDBRun objects. 'cfRuns' is pointer to GUI frame containing the list.
	lvRuns->SetDataColumns(4);	//set number of columns to 4

	lvRuns->SetDataColumn(0, "Run", "GetNumber");       //by default, the data presented in a column with title "Toto" will be retrieved
	lvRuns->SetDataColumn(1, "Events", "", kTextRight); //from each object by calling the "GetToto" method of the class given to the KVListView ctor.
	lvRuns->SetDataColumn(2, "Date", "GetDatime");      //in case the "getter" method has a non-standard name, it can be given explicitly.

	lvRuns->GetDataColumn(2)-><a href="KVLVColumnData.html#KVLVColumnData:SetIsDateTime">SetIsDateTime()</a>;//some special treatment is afforded to date/time data. see method doc.
	lvRuns->SetDataColumn(3, "Comments", "", kTextLeft);// specify text alignment for data


	lvRuns->ActivateSortButtons();//when clicking a column header, the list is sorted according to that column's data.
	...                           //clicking a second time the same column sorts objects in the opposite sense.
</pre>
To display objects, put them in a TList and call
<pre>
   lvRuns->Display( pointer_to_TList );
</pre>
Items in the list can be selected using either:
		<ul>
		<li>single left click (single object selection),</li>
		<li>CTRL+left-click (selection of multiple non-neighbouring objects),</li>
		<li>or SHIFT+left-click (selection of multiple & neighbouring objects)</li>
		</ul>
When the selection changes, the KVListView emits the <pre>SelectionChanged()</pre> signal.<br>
The list of the currently selected objects can be retrieved with the method <pre>GetSelectedItems()</pre>,
		which returns a TList of the currently selected <a href="KVLVEntry.html">KVLVEntry</a> objects,
		each one's name corresponds to the value displayed in the first column of the list view.<br>
Double-left-clicking an object will execute the <pre>Browse()</pre> method of the object. This method is defined for all TObjects,
it can be overridden in derived classes in order to do something interesting and/or useful.<br>
Right-clicking an object opens the context menu of the object, allowing the usual interaction with objects as in TBrower, TCanvas, etc.
<!-- */
// --> END_HTML
////////////////////////////////////////////////////////////////////////////////

KVListView::KVListView(TClass* obj_class, const TGWindow *p, UInt_t w, UInt_t h,
           UInt_t options,
           Pixel_t back)
		: TGListView(p,w,h,options,back), nselected(0), fObjClass(obj_class)
{
	// Create list view widget for objects of class obj_class.
	// A KVLVContainer object is also created and set as the container in use.
	// The view mode is set to kLVDetails (only one possible, no icons)
	// Scrolling increments are set to 1 (vertical) & 19 (horizontal)

	SetContainer( new KVLVContainer(this, kHorizontalFrame, fgWhitePixel) );
	SetViewMode(kLVDetails);
	SetIncrements(1,19);
	fMaxColumnSize = 100;
}

Bool_t KVListView::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
{
   // Handle messages generated by the list view container

   switch (GET_SUBMSG(msg)) {
      case kCT_SELCHANGED:
			// only emit signal if number selected has really changed
			if((int)parm2!=nselected){
				nselected=(int)parm2;
         	SelectionChanged();
			}
			return kTRUE;
         break;
      default:
         break;
   }
   return TGCanvas::ProcessMessage(msg, parm1, parm2);
}

void KVListView::ActivateSortButtons()
{
	// Connects the column header buttons (if defined) to the container's
	// Sort(const Char_t*) method. Each button, when pressed, sends a "Clicked"
	// signal which activates "Sort" with the name of the button/column as argument.

   TGTextButton** buttons = GetHeaderButtons();
   if (buttons) {
		for(int i=0;i<(int)GetNumColumns();i++){
      	buttons[i]->Connect("Clicked()", "KVLVContainer",
					GetContainer(), Form("Sort(=%d)",i));
		}
   }
}

void KVListView::SetDataColumns(Int_t ncolumns)
{
	TGListView::SetHeaders(ncolumns);
	((KVLVContainer*)GetContainer())->SetDataColumns(ncolumns);
}

void 	KVListView::SetDataColumn	(Int_t index, const Char_t* name, const Char_t* method,
			Int_t mode)
{
	// Define column with index = 0, 1, ...
	// name = name of column (shown in column header button)
	// method = method to call to fill column with data (by default, "Getname")
	// mode = text alignment for data in column (kTextCenterX [default], kTextLeft, kTextRight)
	// column header name is always center aligned
	SetHeader(name, kTextCenterX, mode, index);
	((KVLVContainer*)GetContainer())->SetDataColumn(index, fObjClass, name, method);
}

Int_t KVListView::GetColumnNumber(const Char_t* colname)
{
	// Returns index of data column from its name.
	// Return -1 if not found.

	for(Int_t idx = 0; idx<fNColumns-1; idx++){
		if(fColHeader[idx]){
			if(fColNames[idx]==colname) return idx;
		}
	}
   return -1;
}

//______________________________________________________________________________

void KVListView::SetDefaultColumnWidth(TGVFileSplitter* splitter)
{
   // Set default column width of the columns headers.
	// Limit column size to fMaxColumnSize at most

   TGLVContainer *container = (TGLVContainer *) fVport->GetContainer();

   if (!container) {
      Error("SetDefaultColumnWidth", "no listview container set yet");
      return;
   }
   container->ClearViewPort();

   for (int i = 0; i < fNColumns; ++i) {
      if ( fSplitHeader[i] == splitter ) {
         TString dt = fColHeader[i]->GetString();
         UInt_t bsize = gVirtualX->TextWidth(fColHeader[i]->GetFontStruct(),
                                             dt.Data(), dt.Length());
         UInt_t w = TMath::Max(fColHeader[i]->GetDefaultWidth(), bsize + 20);
         if (i == 0) w = TMath::Max(fMaxSize.fWidth + 10, w);
         if (i > 0)  {
				w = TMath::Max(container->GetMaxSubnameWidth(i) + 40, (Int_t)w);
				//printf("w=%ud\n",w);
				w = TMath::Min(w, fMaxColumnSize);
			}
         fColHeader[i]->Resize(w, fColHeader[i]->GetHeight());
         Layout();
      }
   }
}
