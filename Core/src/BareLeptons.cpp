#include "NeroProducer/Core/interface/BareLeptons.hpp"
#include "NeroProducer/Core/interface/BareFunctions.hpp"

BareLeptons::BareLeptons():BareP4(){
}

BareLeptons::~BareLeptons(){
    BareFunctions::Delete(pdgId);
    BareFunctions::Delete(iso);
    BareFunctions::Delete(selBits);
    BareFunctions::Delete(lepPfPt);
    BareFunctions::Delete(chIso);
    BareFunctions::Delete(nhIso);
    BareFunctions::Delete(phoIso);
    BareFunctions::Delete(puIso);
}

void BareLeptons::init(){
    BareP4::init();

    BareFunctions::New(pdgId);
    BareFunctions::New(iso);
    BareFunctions::New(selBits);
    BareFunctions::New(lepPfPt);
    BareFunctions::New(chIso);
    BareFunctions::New(nhIso);
    BareFunctions::New(phoIso);
    BareFunctions::New(puIso);

}

void BareLeptons::clear(){
    BareP4::clear();

    pdgId -> clear();
    iso -> clear();
    selBits ->clear();
    lepPfPt->clear();    

    chIso->clear();
    nhIso->clear();
    phoIso->clear();
    puIso->clear();
}

void BareLeptons::defineBranches(TTree*t){
    //
    BareP4::defineBranches(t, "lep" );
    //
    t->Branch("lepPdgId","vector<int>",&pdgId);
    //
    t->Branch("lepIso","vector<float>",&iso);
    //
    t->Branch("lepSelBits","vector<unsigned>",&selBits);
    //
    t->Branch("lepPfPt","vector<float>",&lepPfPt);

    // 
    t->Branch("lepChIso","vector<float>",&chIso);
    t->Branch("lepNhIso","vector<float>",&nhIso);
    t->Branch("lepPhoIso","vector<float>",&phoIso);
    t->Branch("lepPuIso","vector<float>",&puIso);
}

void BareLeptons::setBranchAddresses(TTree*t){

    BareP4::setBranchAddresses(t,"lep");

    BareFunctions::SetBranchAddress(t,"lepPdgId"	,&pdgId);
    BareFunctions::SetBranchAddress(t,"lepIso"	,&iso);
    BareFunctions::SetBranchAddress(t,"lepSelBits"	,&selBits);
    BareFunctions::SetBranchAddress(t,"lepPfPt"	,&lepPfPt);

    BareFunctions::SetBranchAddress(t,"lepChIso",&chIso);
    BareFunctions::SetBranchAddress(t,"lepNhIso",&nhIso);
    BareFunctions::SetBranchAddress(t,"lepPhoIso",&phoIso);
    BareFunctions::SetBranchAddress(t,"lepPuIso",&puIso);
}

// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
