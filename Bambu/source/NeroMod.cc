#include "NeroProducer/Bambu/interface/NeroMod.h"
#include "NeroProducer/Bambu/interface/TriggerFiller.h"

#include <exception>

ClassImp(mithep::NeroMod)

mithep::NeroMod::NeroMod(char const* _name/* = "NeroMod"*/, char const* _title/* = "Nero-Bambu interface"*/) :
  BaseMod(_name, _title)
{
}

mithep::NeroMod::~NeroMod()
{
}

void
mithep::NeroMod::SlaveBegin()
{
  auto* outputFile = TFile::Open(fileName_, "recreate");
  auto* dir = outputFile->mkdir("nero");
  dir->cd();
  eventsTree_ = new TTree("events", "events");
  allTree_ = new TTree("all", "all");
  hXsec_ = new TH1F("xSec", "xSec", 20, -0.5, 19.5);
  hXsec_->Sumw2();

  tag_.Write();
  head_.Write();
  info_.Write();

  TString triggerNames;
  if (filler_[nero::kTrigger]) {
    auto& filler(*static_cast<nero::TriggerFiller*>(filler_[nero::kTrigger]));
    for (auto& n : filler.triggerNames())
      triggerNames += n + ",";
  }
  TNamed("triggerNames", triggerNames.Data()).Write();

  nero::BaseFiller::ProductGetter getter([this](char const* _name)->TObject* {
      return this->GetObject<TObject>(_name);
    });

  for (unsigned iC(0); iC != nero::nCollections; ++iC) {
    if (!filler_[iC])
      continue;

    if (printLevel_ > 0)
      std::cout << "initializing " << filler_[iC]->getObject()->name() << std::endl;

    filler_[iC]->initialize();
    filler_[iC]->setProductGetter(getter);

    if (iC < nero::nEventObjects)
      filler_[iC]->getObject()->defineBranches(eventsTree_);
    else
      filler_[iC]->getObject()->defineBranches(allTree_);
  }
}

void
mithep::NeroMod::SlaveTerminate()
{
  TFile* outputFile = eventsTree_->GetCurrentFile();
  outputFile->cd();
  outputFile->Write();
  delete outputFile;

  eventsTree_ = allTree_ = 0;
  hXsec_ = 0;
}

void
mithep::NeroMod::Process()
{
  for (auto* filler : filler_) {
    if (filler) {
      if (printLevel_ > 1)
        std::cout << "filling " << filler->getObject()->name() << std::endl;

      filler->getObject()->clear();
      try {
        filler->fill();
      }
      catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        AbortAnalysis();
      }

      if (printLevel_ > 1)
        std::cout << "filled " << filler->getObject()->name() << std::endl;
    }
  }

  eventsTree_->Fill();
  allTree_->Fill();
}
