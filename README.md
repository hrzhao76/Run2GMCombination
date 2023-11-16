# Run2GMCombination

## Setup

``` bash
. Scripts/sync_eos.sh # sync eos to local 

# Setup TRexFitter
setupATLAS
asetup StatAnalysis 0.2.4
```
## Run my Configs 

### Rebin the ExportHistograms.root 

The histogram naming convention of the output file is `{Region}_{Sample}(_{Syst})`.    

``` bash 
mkdir -p /data/hrzhao/MyOutputs/Run2GMCombination/WZ/TRexFitter/FitInputs/rebin_root/
root -b -l -q WZ/TRexFitter/rebin_ExportHistograms/makehist_WZ_allmass.C
```

### Run the BONLY fit 
``` bash 
. Scripts/run_single_trexfitter.sh WZ/TRexFitter/Configs/BONLY/WZ_ANN_BONLY_Data.config /data/hrzhao/MyOutputs/Run2GMCombination/WZ/TRexFitter/FitOutputs/
```


## Run Daniel's Configs 

### Modify the input location in the config files

``` bash 

python Scripts/mod_fit_config.py ~/Projects/ANA-HDBS-2022-12/run2combination/Fitting/configs/Results_WZ_systematics/ /data/hrzhao/Samples/ssWWWZ_run3/inputs_run2_combination/pos_final --new-configdir-path /home/hrzhao/Projects/Run2GMCombination/WZ/TRexFitter/testDanielConfigs/Results_WZ_systematics/ --new-outputdir-path /data/hrzhao/MyOutputs/Run2GMCombination/WZ/TRexFitter/testDanielConfigs/Results_WZ_systematics

```

### Run the configs

``` bash
# BONLY 


```
