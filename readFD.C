#include <TChain.h>
#include <TString.h>
#include <TClonesArray.h>
#include <TH1.h>

// @(#)bmnroot/macro/howTo:$Id$
// Author: Pavel Batyuk <pavel.batyuk@jinr.ru> 2017-07-18

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// readEve.C                                                                  //
//                                                                            //
// An example how to read data (MC) from bmnsim.root                          //
// It demonstrates how to select GEM hits which belong to the GEM track       //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <Rtypes.h>
R__ADD_INCLUDE_PATH($VMCWORKDIR)
//#include "macro/run/bmnloadlibs.C"
#include "bmnloadlibs.C"
        
void readFD(TString fileName = "bmnsim.root") {
    if (fileName == "") {
        cout << "File not specified!" << endl;
        return;
    }

    TChain* out = new TChain("bmndata");
    out->Add(fileName.Data());
    cout << "#recorded entries = " << out->GetEntries() << endl;

    //Creating histograms
    TH1I *histAmp = new TH1I("histAmp", "Amplitude distribution;Amplitude;Number", 100, 0, 0.01);
    TH1I *histMod = new TH1I("histMod", "Module distribution;Module;Number", 65, -1, 63);
    TFile f("histo.root", "recreate");

    TClonesArray* fdDigits = nullptr;
    out->SetBranchAddress("FDDigit", &fdDigits);

    Int_t nEntries = 0;


    for (Int_t iEv = 0; iEv < out->GetEntries(); iEv++) {
        out->GetEntry(iEv);
        
        if( fdDigits->GetEntriesFast() > 2 ) {
            for (Int_t iDigit = 0; iDigit < fdDigits->GetEntriesFast(); iDigit++) {
                nEntries++;
                BmnTrigDigit* digi = (BmnTrigDigit*) fdDigits->UncheckedAt(iDigit);
                
                histMod->AddBinContent(digi->GetMod());
                histAmp->Fill(digi->GetAmp());
            
            }
        }
    }   
    
    histMod->SetEntries(nEntries); 
    histAmp->SetEntries(nEntries);
    histMod->Write();
    histAmp->Write();
}
