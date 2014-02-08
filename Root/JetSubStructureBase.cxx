#include <iostream>
#include <math.h>
#include <float.h>
#include "JetSubStructure/JetSubStructureBase.h"

#include "fastjet/ClusterSequence.hh"

using namespace std;
using fastjet::PseudoJet;

int JetSubStructureBase::modify(xAOD::JetContainer &jets) const {
  int retval = 0;

  for(size_t iJ=0; iJ < jets.size(); iJ++) {
    retval |= modify(*jets[iJ]);
  }

  return retval;
}

vector<fastjet::PseudoJet> JetSubStructureBase::getConstituentPseudoJets(const xAOD::Jet &jet) const {
  if(jet.getPseudoJet() != NULL) { // Use existing pseudojet if it exists
    return jet.getPseudoJet()->constituents();
  }
  else { // Use JetConstituentVector as fallback
    xAOD::JetConstituentVector constit = jet.getConstituents();
    vector<fastjet::PseudoJet> constit_pseudojets;

    for(xAOD::JetConstituentVector::iterator it=constit.begin(); it!=constit.end(); it++) {
      constit_pseudojets.push_back(fastjet::PseudoJet(it->px(), it->py(), it->pz(), it->e()));
    }

    return constit_pseudojets;
  }
}