import sys
from ROOT import TFile

# print(f"Name of the script      : {sys.argv[0]=}")
# print(f"Arguments of the script : {sys.argv[1:]=}")

dic_trees = {}
dic_trees["O2hf2prong"] = ["fHFflag"]
dic_trees["O2hf3prong"] = ["fHFflag"]

for f in sys.argv[1:]:
    file = TFile.Open(f)
    # print(f)
    list_dirs = file.GetListOfKeys()
    # list_dirs = [0]
    for d in list_dirs:
        dir_name = d.GetName()
        # dir_name = "DF_2820500075897029248"
        dir = file.Get(dir_name)
        if not dir or dir.ClassName() != "TDirectoryFile":
            continue
        # print(i.GetName())
        for t, list_branch in dic_trees.items():
            tree = dir.Get(t)
            if not tree or tree.ClassName() != "TTree":
                print(f"{dir_name}/{t} not a tree")
                continue
            entries = tree.GetEntries()
            print(f"{dir_name}/{t}: {entries}")
            for entry in tree:
                print(f"{entry.fIndexTracks_0} {entry.fIndexTracks_1}")
