// Comparison of AliPhysics and O2 histograms

#include "utils_plot.h"

// vectors of histogram specifications
using VecSpecHis = std::vector<std::tuple<TString, TString, TString, int, bool, bool>>;

// Add histogram specification in the vector.
void AddHistogram(VecSpecHis& vec, TString label, TString nameRun1, TString nameRun3, int rebin, bool logH, bool logR)
{
  vec.push_back(std::make_tuple(label, nameRun1, nameRun3, rebin, logH, logR));
}

Int_t Compare(TString filerun3 = "AnalysisResults_O2.root", TString filerun1 = "AnalysisResults_ALI.root", TString options = "", bool doRatio = false)
{
  gStyle->SetOptStat(0);
  gStyle->SetPalette(0);
  gStyle->SetCanvasColor(0);
  gStyle->SetFrameFillColor(0);

  TFile* fRun3 = new TFile(filerun3.Data());
  if (fRun3->IsZombie()) {
    printf("Failed to open file %s\n", filerun3.Data());
    return 1;
  }
  TFile* fRun1 = new TFile(filerun1.Data());
  if (fRun1->IsZombie()) {
    printf("Failed to open file %s\n", filerun1.Data());
    return 1;
  }

  TString pathListRun1 = "HFVertices/clistHFVertices";
  TList* lRun1 = nullptr;
  fRun1->GetObject(pathListRun1.Data(), lRun1);
  if (!lRun1) {
    printf("Failed to load list %s from %s\n", pathListRun1.Data(), filerun1.Data());
    return 1;
  }

  // Histogram specification: axis label, Run 1 name, Run 3 path/name, rebin, log scale histogram, log scale ratio

  VecSpecHis vecHisTracks;
  AddHistogram(vecHisTracks, "#it{p}_{T} before selections (GeV/#it{c})", "hPtAllTracks", "hf-produce-sel-track/hpt_nocuts", 2, 1, 0);
  AddHistogram(vecHisTracks, "#it{p}_{T} after selections (GeV/#it{c})", "hPtSelTracks", "hf-produce-sel-track/hpt_cuts_2prong", 2, 1, 0);
  AddHistogram(vecHisTracks, "DCA XY to prim. vtx. (2-prong sel.) (cm)", "hImpParSelTracks2prong", "hf-produce-sel-track/hdcatoprimxy_cuts_2prong", 2, 1, 0);
  AddHistogram(vecHisTracks, "DCA XY to prim. vtx. (3-prong sel.) (cm)", "hImpParSelTracks3prong", "hf-produce-sel-track/hdcatoprimxy_cuts_3prong", 2, 1, 0);
  AddHistogram(vecHisTracks, "#it{#eta} (2-prong sel.)", "hEtaSelTracks2prong", "hf-produce-sel-track/heta_cuts_2prong", 2, 0, 0);
  AddHistogram(vecHisTracks, "#it{#eta} (3-prong sel.)", "hEtaSelTracks3prong", "hf-produce-sel-track/heta_cuts_3prong", 2, 0, 0);

  VecSpecHis vecHisSkim;
  AddHistogram(vecHisSkim, "secondary vtx x - 2prong (cm)", "h2ProngVertX", "hf-track-index-skims-creator/hvtx2_x", 5, 1, 0);
  AddHistogram(vecHisSkim, "secondary vtx y - 2prong (cm)", "h2ProngVertY", "hf-track-index-skims-creator/hvtx2_y", 5, 1, 0);
  AddHistogram(vecHisSkim, "secondary vtx z - 2prong (cm)", "h2ProngVertZ", "hf-track-index-skims-creator/hvtx2_z", 5, 1, 0);
  AddHistogram(vecHisSkim, "secondary vtx x - 3prong (cm)", "hDplusVertX", "hf-track-index-skims-creator/hvtx3_x", 5, 1, 0);
  AddHistogram(vecHisSkim, "secondary vtx y - 3prong (cm)", "hDplusVertY", "hf-track-index-skims-creator/hvtx3_y", 5, 1, 0);
  AddHistogram(vecHisSkim, "secondary vtx z - 3prong (cm)", "hDplusVertZ", "hf-track-index-skims-creator/hvtx3_z", 5, 1, 0);

  VecSpecHis vecHisCand2;
  AddHistogram(vecHisCand2, "XX element of PV cov. matrix (cm^{2})", "hCovMatPrimVXX2Prong", "hf-cand-creator-2prong/hCovPVXX", 1, 1, 0);
  AddHistogram(vecHisCand2, "XX element of SV cov. matrix (cm^{2})", "hCovMatSecVXX2Prong", "hf-cand-creator-2prong/hCovSVXX", 1, 1, 0);

  VecSpecHis vecHisCand3;
  AddHistogram(vecHisCand3, "XX element of PV cov. matrix (cm^{2})", "hCovMatPrimVXX3Prong", "hf-cand-creator-3prong/hCovPVXX", 1, 1, 0);
  AddHistogram(vecHisCand3, "XX element of SV cov. matrix (cm^{2})", "hCovMatSecVXX3Prong", "hf-cand-creator-3prong/hCovSVXX", 1, 1, 0);

  VecSpecHis vecHisD0;
  AddHistogram(vecHisD0, "#it{p}_{T} prong 0 (GeV/#it{c})", "hPtD0Dau0", "hf-task-d0/hptprong0", 2, 1, 0);
  AddHistogram(vecHisD0, "#it{p}_{T} prong 1 (GeV/#it{c})", "hPtD0Dau1", "hf-task-d0/hptprong1", 2, 1, 0);
  AddHistogram(vecHisD0, "#it{p}_{T} D^{0} (GeV/#it{c})", "hPtD0", "hf-task-d0/hptcand", 2, 1, 0);
  AddHistogram(vecHisD0, "2-prong mass (#pi K) (GeV/#it{c}^{2})", "hInvMassD0", "hf-task-d0/hmass", 2, 0, 0);
  AddHistogram(vecHisD0, "d0d0 (cm^{2})", "hd0Timesd0", "hf-task-d0/hd0d0", 2, 1, 0);
  AddHistogram(vecHisD0, "d0 prong 0 (cm)", "hImpParD0Dau0", "hf-task-d0/hd0Prong0", 2, 1, 0);
  AddHistogram(vecHisD0, "d0 prong 1 (cm)", "hImpParD0Dau1", "hf-task-d0/hd0Prong1", 2, 1, 0);
  AddHistogram(vecHisD0, "impact parameter error (cm)", "hImpParErrD0Dau", "hf-task-d0/hImpParErr", 1, 1, 0);
  AddHistogram(vecHisD0, "decay length (cm)", "hDecLenD0", "hf-task-d0/hdeclength", 2, 1, 0);
  AddHistogram(vecHisD0, "decay length XY (cm)", "hDecLenXYD0", "hf-task-d0/hdeclengthxy", 2, 1, 0);
  AddHistogram(vecHisD0, "decay length error (cm)", "hDecLenErrD0", "hf-task-d0/hDecLenErr", 1, 1, 0);
  AddHistogram(vecHisD0, "decay length XY error (cm)", "hDecLenXYErrD0", "hf-task-d0/hDecLenXYErr", 1, 1, 0);
  AddHistogram(vecHisD0, "cos. pointing angle", "hCosPointD0", "hf-task-d0/hCPA", 2, 1, 0);

  VecSpecHis vecHisDPlus;
  AddHistogram(vecHisDPlus, "#it{p}_{T} prong 0 (GeV/#it{c})", "hPtDplusDau0", "hf-task-dplus/hPtProng0", 2, 1, 0);
  AddHistogram(vecHisDPlus, "#it{p}_{T} prong 1 (GeV/#it{c})", "hPtDplusDau1", "hf-task-dplus/hPtProng1", 2, 1, 0);
  AddHistogram(vecHisDPlus, "#it{p}_{T} prong 2 (GeV/#it{c})", "hPtDplusDau2", "hf-task-dplus/hPtProng2", 2, 1, 0);
  AddHistogram(vecHisDPlus, "#it{p}_{T} D^{+} (GeV/#it{c})", "hPtDplus", "hf-task-dplus/hPt", 2, 1, 0);
  AddHistogram(vecHisDPlus, "3-prong mass (#pi K #pi) (GeV/#it{c}^{2})", "hInvMassDplus", "hf-task-dplus/hMass", 5, 0, 0);
  AddHistogram(vecHisDPlus, "impact par. XY (cm)", "hImpParXYDplus", "hf-task-dplus/hImpactParameterXY", 4, 1, 0);
  AddHistogram(vecHisDPlus, "decay length (cm)", "hDecLenDplus", "hf-task-dplus/hDecayLength", 4, 1, 0);
  AddHistogram(vecHisDPlus, "decay length XY (cm)", "hDecLenXYDplus", "hf-task-dplus/hDecayLengthXY", 4, 1, 0);
  AddHistogram(vecHisDPlus, "norm. decay length XY", "hNormDecLenXYDplus", "hf-task-dplus/hNormalisedDecayLengthXY", 2, 1, 0);
  AddHistogram(vecHisDPlus, "cos. pointing angle", "hCosPointDplus", "hf-task-dplus/hCPA", 2, 1, 0);
  AddHistogram(vecHisDPlus, "cos. pointing angle XY", "hCosPointXYDplus", "hf-task-dplus/hCPAxy", 2, 1, 0);
  AddHistogram(vecHisDPlus, "norm. IP", "hNormIPDplus", "hf-task-dplus/hMaxNormalisedDeltaIP", 4, 1, 0);
  AddHistogram(vecHisDPlus, "decay length error (cm)", "hDecLenErrDplus", "hf-task-dplus/hDecayLengthError", 2, 1, 0);
  AddHistogram(vecHisDPlus, "decay length XY error (cm)", "hDecLenXYErrDplus", "hf-task-dplus/hDecayLengthXYError", 2, 1, 0);
  AddHistogram(vecHisDPlus, "prong 0 impact parameter (cm)", "hImpParDplusDau0", "hf-task-dplus/hd0Prong0", 2, 1, 0);
  AddHistogram(vecHisDPlus, "prong 1 impact parameter (cm)", "hImpParDplusDau1", "hf-task-dplus/hd0Prong1", 2, 1, 0);
  AddHistogram(vecHisDPlus, "prong impact parameter error (cm)", "hImpParErrDplusDau", "hf-task-dplus/hImpactParameterError", 2, 1, 0);
  AddHistogram(vecHisDPlus, "sq. sum of prong imp. par. (cm^{2})", "hSumSqImpParDplusDau", "hf-task-dplus/hImpactParameterProngSqSum", 2, 1, 0);

  VecSpecHis vecHisLc;
  AddHistogram(vecHisLc, "#it{p}_{T} prong 0 (GeV/#it{c})", "hPtLcDau0", "hf-task-lc/hptprong0", 2, 1, 0);
  AddHistogram(vecHisLc, "#it{p}_{T} prong 1 (GeV/#it{c})", "hPtLcDau1", "hf-task-lc/hptprong1", 2, 1, 0);
  AddHistogram(vecHisLc, "#it{p}_{T} prong 2 (GeV/#it{c})", "hPtLcDau2", "hf-task-lc/hptprong2", 2, 1, 0);
  AddHistogram(vecHisLc, "#it{p}_{T} #Lambda_{c}^{#plus} (GeV/#it{c})", "hPtLc", "hf-task-lc/hptcand", 2, 1, 0);
  AddHistogram(vecHisLc, "3-prong mass (p K #pi) (GeV/#it{c}^{2})", "hInvMassLc", "hf-task-lc/hmass", 2, 0, 0);
  AddHistogram(vecHisLc, "decay length (cm)", "hDecLenLc", "hf-task-lc/hdeclength", 2, 1, 0);
  AddHistogram(vecHisLc, "cos. pointing angle", "hCosPointLc", "hf-task-lc/hCPA", 2, 1, 0);

  VecSpecHis vecHisJpsi;
  AddHistogram(vecHisJpsi, "#it{p}_{T} prong 0 (GeV/#it{c})", "hPtJpsiDau0", "hf-task-jpsi/hptprong0", 2, 1, 0);
  AddHistogram(vecHisJpsi, "#it{p}_{T} prong 1 (GeV/#it{c})", "hPtJpsiDau1", "hf-task-jpsi/hptprong1", 2, 1, 0);
  AddHistogram(vecHisJpsi, "#it{p}_{T} J/#psi (GeV/#it{c})", "hPtJpsi", "hf-task-jpsi/hptcand", 2, 1, 0);
  AddHistogram(vecHisJpsi, "2-prong mass (e^{#plus} e^{#minus}) (GeV/#it{c}^{2})", "hInvMassJpsi", "hf-task-jpsi/hmass", 2, 0, 0);
  AddHistogram(vecHisJpsi, "d0d0 (cm^{2})", "hd0Timesd0Jpsi", "hf-task-jpsi/hd0d0", 2, 1, 0);
  AddHistogram(vecHisJpsi, "d0 prong 0 (cm)", "hImpParJpsiDau0", "hf-task-jpsi/hd0Prong0", 2, 1, 0);
  AddHistogram(vecHisJpsi, "d0 prong 1 (cm)", "hImpParJpsiDau1", "hf-task-jpsi/hd0Prong1", 2, 1, 0);
  AddHistogram(vecHisJpsi, "decay length (cm)", "hDecLenJpsi", "hf-task-jpsi/hdeclength", 2, 1, 0);
  AddHistogram(vecHisJpsi, "decay length XY (cm)", "hDecLenXYJpsi", "hf-task-jpsi/hdeclengthxy", 2, 1, 0);
  AddHistogram(vecHisJpsi, "cos. pointing angle", "hCosPointJpsi", "hf-task-jpsi/hCPA", 2, 1, 0);
  AddHistogram(vecHisJpsi, "decay length error (cm)", "hDecLenErrJpsi", "hf-task-jpsi/hDecLenErr", 1, 1, 0);
  AddHistogram(vecHisJpsi, "decay length XY error (cm)", "hDecLenXYErrJpsi", "hf-task-jpsi/hDecLenXYErr", 1, 1, 0);

  // vector of specifications of vectors: name, VecSpecHis, pads X, pads Y
  std::vector<std::tuple<TString, VecSpecHis, int, int>> vecSpecVecSpec;

  // Add vector specifications in the vector.
  if (options.Contains("tracks"))
    vecSpecVecSpec.push_back(std::make_tuple("tracks", vecHisTracks, 5, 3));
  if (options.Contains("skim"))
    vecSpecVecSpec.push_back(std::make_tuple("skim", vecHisSkim, 5, 3));
  if (options.Contains("cand2"))
    vecSpecVecSpec.push_back(std::make_tuple("cand2", vecHisCand2, 5, 3));
  if (options.Contains("cand3"))
    vecSpecVecSpec.push_back(std::make_tuple("cand3", vecHisCand3, 5, 3));
  if (options.Contains("d0"))
    vecSpecVecSpec.push_back(std::make_tuple("d0", vecHisD0, 5, 3));
  if (options.Contains("dplus"))
    vecSpecVecSpec.push_back(std::make_tuple("dplus", vecHisDPlus, 6, 3));
  if (options.Contains("lc"))
    vecSpecVecSpec.push_back(std::make_tuple("lc", vecHisLc, 5, 3));
  if (options.Contains("jpsi"))
    vecSpecVecSpec.push_back(std::make_tuple("jpsi", vecHisJpsi, 5, 3));

  // Histogram plot vertical margins
  Float_t marginHigh = 0.05;
  Float_t marginLow = 0.05;
  bool logScaleH = false;
  // Ratio plot vertical margins
  Float_t marginRHigh = 0.05;
  Float_t marginRLow = 0.05;
  bool logScaleR = false;
  Float_t yMin, yMax;
  Int_t nRun1, nRun3, rebin;

  TH1F* hRun1 = nullptr;
  TH1F* hRun3 = nullptr;
  TH1F* hRatio = nullptr;
  TString labelAxis = "";
  TString nameHisRun1 = "";
  TString nameHisRun3 = "";
  TCanvas* canHis = nullptr;
  TCanvas* canRat = nullptr;

  // loop over lists
  for (const auto& specVecSpec : vecSpecVecSpec) {
    auto nameSpec = std::get<0>(specVecSpec); // list name
    auto vecSpec = std::get<1>(specVecSpec);  // list of histogram specs.
    int nPadsX = std::get<2>(specVecSpec);    // number of horizontal pads
    int nPadsY = std::get<3>(specVecSpec);    // number of vertical pads
    Printf("\nProcessing histogram list: %s (%d)", nameSpec.Data(), (int)vecSpec.size());
    if (nPadsX * nPadsY < vecSpec.size()) {
      Printf("Not enough pads (%d)", nPadsX * nPadsY);
      return 1;
    }

    canHis = new TCanvas(Form("canHis_%s", nameSpec.Data()), Form("Histos_%s", nameSpec.Data()), 3000, 1600);
    SetCanvas(canHis, nPadsX, nPadsY);
    if (doRatio) {
      canRat = new TCanvas(Form("canRat_%s", nameSpec.Data()), Form("Ratios_%s", nameSpec.Data()), 3000, 1600);
      SetCanvas(canRat, nPadsX, nPadsY);
    }

    // loop over histograms
    for (int index = 0; index < vecSpec.size(); index++) {
      auto spec = vecSpec[index];
      labelAxis = std::get<0>(spec);
      nameHisRun1 = std::get<1>(spec);
      nameHisRun3 = std::get<2>(spec);
      rebin = std::get<3>(spec);
      logScaleH = std::get<4>(spec);
      logScaleR = std::get<5>(spec);

      // Get AliPhysics histogram.
      hRun1 = (TH1F*)lRun1->FindObject(nameHisRun1.Data());
      if (!hRun1) {
        printf("Failed to load %s from %s\n", nameHisRun1.Data(), filerun1.Data());
        return 1;
      }

      // Get O2 histogram.
      hRun3 = (TH1F*)fRun3->Get(nameHisRun3.Data());
      if (!hRun3) {
        printf("Failed to load %s from %s\n", nameHisRun3.Data(), filerun3.Data());
        return 1;
      }

      Printf("%d (%s, %s): bins: %d, %d, ranges: %g-%g, %g-%g",
             index, nameHisRun1.Data(), nameHisRun3.Data(),
             hRun1->GetNbinsX(), hRun3->GetNbinsX(),
             hRun1->GetXaxis()->GetBinLowEdge(1), hRun1->GetXaxis()->GetBinUpEdge(hRun1->GetNbinsX()),
             hRun3->GetXaxis()->GetBinLowEdge(1), hRun3->GetXaxis()->GetBinUpEdge(hRun3->GetNbinsX()));

      nRun1 = hRun1->GetEntries();
      nRun3 = hRun3->GetEntries();

      // Histograms
      auto padH = canHis->cd(index + 1);
      hRun1->Rebin(rebin);
      hRun3->Rebin(rebin);
      hRun1->SetLineColor(1);
      hRun1->SetLineWidth(2);
      hRun3->SetLineColor(2);
      hRun3->SetLineWidth(1);
      hRun1->SetTitle(Form("Entries: Run1: %d, Run3: %d;%s;Entries", nRun1, nRun3, labelAxis.Data()));
      hRun1->GetYaxis()->SetMaxDigits(3);
      yMin = TMath::Min(hRun3->GetMinimum(0), hRun1->GetMinimum(0));
      yMax = TMath::Max(hRun3->GetMaximum(), hRun1->GetMaximum());
      SetHistogram(hRun1, yMin, yMax, marginLow, marginHigh, logScaleH);
      SetPad(padH, logScaleH);
      hRun1->Draw();
      hRun3->Draw("same");
      TLegend* legend = new TLegend(0.8, 0.72, 1., 0.92);
      legend->AddEntry(hRun1, "Run1", "L");
      legend->AddEntry(hRun3, "Run3", "L");
      legend->Draw();

      // Ratio
      if (doRatio) {
        auto padR = canRat->cd(index + 1);
        hRatio = (TH1F*)hRun3->Clone(Form("hRatio%d", index));
        hRatio->Divide(hRun1);
        hRatio->SetTitle(Form("Entries ratio: %g;%s;Run3/Run1", (double)nRun3 / (double)nRun1, labelAxis.Data()));
        yMin = hRatio->GetMinimum(0);
        yMax = hRatio->GetMaximum();
        SetHistogram(hRatio, yMin, yMax, marginRLow, marginRHigh, logScaleR);
        SetPad(padR, logScaleR);
        hRatio->Draw();
      }
    }
    canHis->SaveAs(Form("comparison_histos_%s.pdf", nameSpec.Data()));
    if (doRatio) {
      canRat->SaveAs(Form("comparison_ratios_%s.pdf", nameSpec.Data()));
    }
    delete canHis;
    delete canRat;
  }
  return 0;
}
