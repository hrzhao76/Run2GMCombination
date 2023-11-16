#include <TFile.h>
#include <TH1.h>
#include <TKey.h>
#include <TString.h>
#include <iostream>

std::vector<std::string> split(const std::string &str, char delim) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);

    while (std::getline(tokenStream, token, delim)) {
        tokens.push_back(token);
    }

    return tokens;
}

bool checkIfProcess(TString histName){
    if (histName.Contains("HVT")){
        return false;
    }
    if (histName.Contains("GMMVASR") || histName.Contains("GMMVACR") || histName.Contains("ZZCRJJ") ) {
        return true;
    }
    else
        return false;
}

void processDirectory(TDirectory* inputDir, TDirectory* outputDir) {
    const int nBins_GMMVA = 9; 
    double m_WZ_bin_GMMVA_edges[nBins_GMMVA + 1] = {150, 200, 230, 270, 310, 350, 390, 480, 660, 1200};

    const int nBins_ZZCRJJ = 17;
    double m_WZ_bin_ZZCRJJ_edges[nBins_ZZCRJJ + 1] = {150,200,250,300,350,400,450,500,550,650,750,850,950,1100,1250,1400,1700,3000};

    TIter next(inputDir->GetListOfKeys());
    TKey *key;
    while ((key = (TKey*)next())) {
        TObject *obj = key->ReadObj();

        if (obj->InheritsFrom("TH1")) {
            TH1 *h = (TH1*)obj;
            TString histName = h->GetName();

            if (!checkIfProcess(histName)) {
                std::cout << "Skipping histogram " << h->GetName() << std::endl;
                delete h;
                continue;
            }

            std::vector<std::string> tokens = split(histName.Data(), '_');
            std::string newHistName = tokens[3] + "_" + tokens[0]; 


            if (tokens.size() != 4) {
                for (int i = 4; i < tokens.size(); i++) {
                    newHistName = newHistName + "_" + tokens[i];
                }
            } 
            // std::cout << "newHistName: " << newHistName << std::endl;

            // Change the current directory to output directory for proper saving.
            outputDir->cd();

            // 对 GMMVA 直方图进行 rebin
            if (histName.Contains("GMMVA"))
            {
                TH1 *hNew = h->Rebin(nBins_GMMVA, newHistName.c_str(), m_WZ_bin_GMMVA_edges);
                hNew->Write(); // Write histogram to the current directory.
                std::cout << "Rebinned histogram " << h->GetName() << " with GMMVA binning scheme and saved as " << hNew->GetName() << std::endl;
                delete hNew;
            }
            
            // 对 ZZCRJJ 直方图进行 rebin
            if (histName.Contains("ZZCRJJ")) {
                TH1 *hNew = h->Rebin(nBins_ZZCRJJ, newHistName.c_str(), m_WZ_bin_ZZCRJJ_edges);
                hNew->Write(); // Write histogram to the current directory.
                std::cout << "Rebinned histogram " << h->GetName() << " with ZZCRJJ binning scheme and saved as " << hNew->GetName() << std::endl;
                delete hNew;
            }
        } else if (obj->InheritsFrom("TDirectory")) {
            // Create a similar sub-directory in output file
            outputDir->cd();
            // TDirectory* newOutputDir = outputDir->mkdir(obj->GetName());

            // Recursively process this sub-directory.
            processDirectory((TDirectory*)obj, outputDir);
        }
        delete obj;
    }
}


void makehist_WZ_allmass(
    const char* inputFilepath="/data/hrzhao/Samples/Run2_WZ/Finebin/ExportHistograms.root",
    const char* outputFilepath="/data/hrzhao/MyOutputs/Run2GMCombination/WZ/TRexFitter/FitInputs/rebin_root/hist_WZ_allmass.root"
) {
    TFile* inputFile = TFile::Open(inputFilepath, "READ"); 
    TFile* outputFile = TFile::Open(outputFilepath, "RECREATE");
    
    processDirectory(inputFile, outputFile);

    inputFile->Close();
    outputFile->Close();
    delete inputFile;
    delete outputFile;

    std::cout << "makehist_WZ_allmass finished" << std::endl;
}
