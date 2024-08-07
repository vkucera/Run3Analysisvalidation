#!/bin/bash
# shellcheck disable=SC2034 # Ignore unused parameters.

# Input specification for runtest.sh
# (Modifies input parameters.)

INPUT_CASE=2            # Input case

NFILESMAX=1             # Maximum number of processed input files. (Set to -0 to process all; to -N to process all but the last N files.)

# Number of input files per job. (Will be automatically optimised if set to 0.)
NFILESPERJOB_CONVERT=0  # Conversion
NFILESPERJOB_ALI=0      # AliPhysics
NFILESPERJOB_O2=1       # O2

# Maximum number of simultaneously running O2 jobs. (Adjust it based on available memory.)
NJOBSPARALLEL_O2=$(python3 -c "print(min(10, round($(nproc) / 2)))")

# Default settings:
# INPUT_LABEL="nothing"           # Input description
# INPUT_DIR="$PWD"                # Input directory
# INPUT_FILES="AliESDs.root"      # Input file pattern
# INPUT_SYS="pp"                  # Collision system ("pp", "PbPb")
# INPUT_RUN=2                     # LHC Run (2, 3, 5)
# INPUT_IS_O2=0                   # Input files are in O2 format.
# INPUT_IS_MC=0                   # Input files are MC data.
# INPUT_PARENT_MASK=""            # Path replacement mask for the input directory of parent files in case of linked derived O2 input. Set to ";" if no replacement needed. Syntax: "alien:///path/in/alien;/local/path"
# INPUT_TASK_CONFIG=""            # Input-specific task configuration (e.g. enabling converters), overriding the task configuration in CONFIG_TASKS. String of space-separated commands.
# JSON="dpl-config.json"          # O2 device configuration

INPUT_BASE="/data2/data" # alicecerno2

case $INPUT_CASE in
  1)
    INPUT_LABEL="Run 2, p-p 5.02 TeV LHC17p, real"
    INPUT_DIR="$INPUT_BASE/Run2/pp_5.02TeV/real/LHC17p_pass1_CENT_woSDD"
    ;;
  2) # reference
    INPUT_LABEL="Run 2, p-p 5.02 TeV LHC17p, MC LHC18a4a2_cent"
    INPUT_DIR="$INPUT_BASE/Run2/pp_5.02TeV/sim/LHC18a4a2_cent/282099"
    INPUT_IS_MC=1
    INPUT_TASK_CONFIG="DOO2_CONV_ZDC=1 DOO2_CONV_BC=1 DOO2_CONV_TRKEX=1"
    ;;
  3)
    INPUT_LABEL="Run 2, p-p 5.02 TeV LHC17p, MC LHC18a4a2_cent"
    INPUT_DIR="$INPUT_BASE/Run2/pp_5.02TeV/sim/LHC18a4a2_cent/282341"
    INPUT_IS_MC=1
    ;;
  4)
    INPUT_LABEL="Run 2, Pb-Pb 5.02 TeV LHC15o, real"
    INPUT_DIR="$INPUT_BASE/Run2/PbPb_5.02TeV/real/LHC15o"
    INPUT_SYS="PbPb"
    ;;
  5)
    INPUT_LABEL="Run 2, Pb-Pb 5.02 TeV LHC15o, MC LHC15k1a3"
    INPUT_DIR="$INPUT_BASE/Run2/PbPb_5.02TeV/sim/LHC15k1a3"
    INPUT_SYS="PbPb"
    INPUT_IS_MC=1
    ;;
  6)
    INPUT_LABEL="Run 2, p-p 13 TeV LHC16p, MC LHC19g6f3, dedicated Ξc"
    INPUT_DIR="$INPUT_BASE/Run2/pp_13TeV/sim/LHC19g6f3"
    INPUT_IS_MC=1
    ;;
  7)
    INPUT_LABEL="Run 3, p-p 13.6 TeV, LHC22o, real"
    INPUT_DIR="$INPUT_BASE/Run3/pp_13.6TeV/real/LHC22o"
    INPUT_FILES="AO2D.root"
    INPUT_IS_O2=1
    INPUT_RUN=3
    ;;
  8)
    INPUT_LABEL="Run 3, p-p 13.6 TeV, LHC22r, real, low interaction rate (100 kHz)"
    INPUT_DIR="$INPUT_BASE/Run3/pp_13.6TeV/real/LHC22r"
    INPUT_FILES="AO2D.root"
    INPUT_IS_O2=1
    INPUT_RUN=3
    ;;
  9)
    INPUT_LABEL="Run 3, p-p 13.6 TeV, MC LHC21k6, general purpose"
    INPUT_DIR="$INPUT_BASE/Run3/pp_13.6TeV/sim/LHC21k6"
    INPUT_FILES="AO2D.root"
    INPUT_IS_O2=1
    INPUT_RUN=3
    INPUT_IS_MC=1
    ;;
  10)
    INPUT_LABEL="Run 2, p-p 13 TeV LHC18f, MC LHC20f4a (ESD)"
    INPUT_DIR="$INPUT_BASE/Run2/pp_13TeV/sim/LHC20f4a"
    INPUT_IS_MC=1
    ;;
  11)
    INPUT_LABEL="Run 2, p-p 13 TeV LHC18f, MC LHC20f4a (AO2D)"
    INPUT_DIR="$INPUT_BASE/Run2/pp_13TeV/sim_converted/LHC20f4a"
    INPUT_FILES="AO2D.root"
    INPUT_IS_O2=1
    INPUT_IS_MC=1
    ;;
  12)
    INPUT_LABEL="Run 2, p-p 13 TeV, LHC17j (AO2D)"
    INPUT_DIR="$INPUT_BASE/Run2/pp_13TeV/real_converted/LHC17j_20220601" # converted good AO2Ds
    INPUT_FILES="AO2D.root"
    INPUT_IS_O2=1
    ;;
  13)
    INPUT_LABEL="Run 2, p-p 13 TeV, LHC18p, real"
    INPUT_DIR="$INPUT_BASE/Run2/pp_13TeV/real/LHC18p/294009"
    ;;
  esac
