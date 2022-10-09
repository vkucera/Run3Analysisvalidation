import sys
from ROOT import TFile

# print(f"Name of the script      : {sys.argv[0]=}")
# print(f"Arguments of the script : {sys.argv[1:]=}")

dic_his = {}
# dic_his["hf-cand-creator-2prong"] = ["hMass2", "hCovPVXX", "hCovSVXX", "hDcaXYProngs", "hDcaZProngs"]
# dic_his["hf-cand-creator-3prong"] = ["hMass3", "hCovPVXX", "hCovSVXX", "hDcaXYProngs", "hDcaZProngs"]
dic_his["hf-tag-sel-tracks"] = ["hPtNoCuts"]
dic_his["hf-tag-sel-collisions"] = ["hEvents"]
dic_his["hf-track-index-skims-creator"] = ["hNTracks", "hVtx2ProngX", "hVtx3ProngX"]

for f in sys.argv[1:]:
    file = TFile.Open(f)
    print(f)
    for task, list_his in dic_his.items():
        for h in list_his:
            name = f"{task}/{h}"
            his = file.Get(name)
            if not his:
                print(f"Error getting {name}")
                sys.exit(1)
            print(f"{name}: {int(his.GetEntries())}")