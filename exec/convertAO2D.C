R__ADD_INCLUDE_PATH($ALICE_ROOT)
R__ADD_INCLUDE_PATH($ALICE_PHYSICS)
#include <ANALYSIS/macros/train/AddESDHandler.C>
#include <ANALYSIS/macros/train/AddAODHandler.C>
#include <ANALYSIS/macros/train/AddMCHandler.C>
#include <OADB/COMMON/MULTIPLICITY/macros/AddTaskMultSelection.C>
#include <OADB/macros/AddTaskPhysicsSelection.C>
#include <ANALYSIS/macros/AddTaskPIDResponse.C>
#include <RUN3/AddTaskAO2Dconverter.C>
#include <PWGHF/vertexingHF/macros/AddTaskVertexingHFRun3Conversion.C>
#include <PWGLF/STRANGENESS/Cascades/Run2/macros/AddTaskWeakDecayVertexer.C>

TChain* CreateLocalChain(const char* txtfile, const char* type, int nfiles);

bool doWeakDecays = true;
bool doHFConversion = true;

Long64_t convertAO2D(TString listoffiles, bool isMC = 1, bool useAliEvCuts = false, bool isESD = 1, int nmaxevents = -1)
{
  const char* anatype = isESD ? "ESD" : "AOD";
  if (isMC) {
    std::cout << "I AM DOING MC" << std::endl;
  }

  //  TGrid::Connect("alien:");

  // Create the chain based on xml collection or txt file
  // The entries in the txt file can be local paths or alien paths
  TChain* chain = CreateLocalChain(listoffiles.Data(), anatype, -1);
  if (!chain)
    return -1;
  chain->SetNotify(0x0);
  ULong64_t nentries = chain->GetEntries();
  if (nmaxevents != -1)
    nentries = nmaxevents;
  cout << nentries << " entries in the chain." << endl;
  cout << nentries << " converted" << endl;
  AliAnalysisManager* mgr = new AliAnalysisManager("AOD converter");
  if (isESD) {
    AddESDHandler();
  } else {
    AddAODHandler();
  }

  AddTaskMultSelection();
  AddTaskPhysicsSelection(isMC);
  AddTaskPIDResponse();
  if (isMC && isESD)
    AliMCEventHandler* handlerMC = AddMCHandler();

  if (doWeakDecays) {
    auto weakDecays = AddTaskWeakDecayVertexer();
    weakDecays->SetUseImprovedFinding();
    weakDecays->SetupLooseVertexing();
    weakDecays->SetRevertexAllEvents(kTRUE);
    weakDecays->SetForceResetV0s(kTRUE);
    weakDecays->SetForceResetCascades(kTRUE);
  }

  if (doHFConversion) {
    gSystem->Setenv("CHILD_DATASETS", "1");
    // case 1
    // gSystem->Setenv("RUNNO_child_1", "282341");
    // gSystem->Setenv("ALIEN_JDL_child_1_LPMRUNNUMBER", "282341");
    // gSystem->Setenv("ALIEN_JDL_child_1_LPMPRODUCTIONTYPE", "RAW");
    // gSystem->Setenv("ALIEN_JDL_child_1_LPMINTERACTIONTYPE", "pp");
    // gSystem->Setenv("ALIEN_JDL_child_1_LPMPRODUCTIONTAG", "LHC17p");
    // case 100
    gSystem->Setenv("RUNNO_child_1", "244918");
    gSystem->Setenv("ALIEN_JDL_child_1_LPMRUNNUMBER", "244918");
    gSystem->Setenv("ALIEN_JDL_child_1_LPMPRODUCTIONTYPE", "RAW");
    gSystem->Setenv("ALIEN_JDL_child_1_LPMINTERACTIONTYPE", "PbPb");
    gSystem->Setenv("ALIEN_JDL_child_1_LPMPRODUCTIONTAG", "LHC15o");

    AliAnalysisTaskSEVertexingHFRun3Conversion* hfConverter = AddTaskVertexingHFRun3Conversion("");
    if (!hfConverter) {
      Printf("Cannot create HF converter!");
      return -1;
    }
  }

  AliAnalysisTaskAO2Dconverter* converter = AddTaskAO2Dconverter("");
  //converter->SelectCollisionCandidates(AliVEvent::kAny);
  converter->SetStoreHF();
  if (useAliEvCuts)
    converter->SetUseEventCuts(kTRUE);
  if (isMC)
    converter->SetMCMode();

  converter->SetTruncation(true);
  converter->SetCompression(501);
  converter->SetMaxBytes(250000000);
  converter->SetEMCALAmplitudeThreshold(0.075);

  if (!mgr->InitAnalysis())
    return -1;
  //PH   mgr->SetBit(AliAnalysisManager::kTrueNotify);
  //mgr->SetRunFromPath(244918);
  mgr->PrintStatus();

  mgr->SetDebugLevel(1);
  return mgr->StartAnalysis("localfile", chain, nentries, 0);
}

TChain* CreateLocalChain(const char* txtfile, const char* type, int nfiles)
{
  TString treename = type;
  treename.ToLower();
  treename += "Tree";
  printf("***************************************\n");
  printf("    Getting chain of trees %s\n", treename.Data());
  printf("***************************************\n");
  // Open the file
  ifstream in;
  in.open(txtfile);
  Int_t count = 0;
  // Read the input list of files and add them to the chain
  TString line;
  TChain* chain = new TChain(treename);
  while (in.good()) {
    in >> line;
    if (line.IsNull() || line.BeginsWith("#"))
      continue;
    if (count++ == nfiles)
      break;
    TString esdFile(line);
    TFile* file = TFile::Open(esdFile);
    if (file && !file->IsZombie()) {
      chain->Add(esdFile);
      file->Close();
    } else {
      Error("GetChainforTestMode", "Skipping un-openable file: %s", esdFile.Data());
    }
  }
  in.close();
  if (!chain->GetListOfFiles()->GetEntries()) {
    Error("CreateLocalChain", "No file from %s could be opened", txtfile);
    delete chain;
    return nullptr;
  }
  return chain;
}
