#include <iostream>
#include <math.h>
#include <float.h>
#include "JetSubstructureVariables/Calculator.h"

#include "fastjet/ClusterSequence.hh"

using namespace std;
using fastjet::PseudoJet;

Calculator::Calculator(std::string name) : 
  AsgTool(name)
{
  
}

int Calculator::modify(xAOD::JetContainer &jets) const {
  int retval = 0;

  for(size_t iJ=0; iJ < jets.size(); iJ++) {
    retval |= modify(*jets[iJ]);
  }

  return retval;
}

int Calculator::modify(xAOD::Jet &jet) const {
  jet.setAttribute("Tau1", nSubJettiness(jet, 1));
  jet.setAttribute("Tau2", nSubJettiness(jet, 2));
  jet.setAttribute("Tau3", nSubJettiness(jet, 3));

  return 0;
}

double Calculator::nSubJettiness(const xAOD::Jet &jet, unsigned int nSubJets) const {
  xAOD::JetConstituentVector constit = jet.getConstituents();
  vector<fastjet::PseudoJet> constit_pseudojets;

  for(xAOD::JetConstituentVector::iterator it=constit.begin(); it!=constit.end(); it++) {
    constit_pseudojets.push_back(fastjet::PseudoJet(it->px(), it->py(), it->pz(), it->e()));
  }

  float jetRadius = jet.getAttribute<float>("JetRadius");

  return nSubJettiness(constit_pseudojets, nSubJets, jetRadius);
}

double Calculator::nSubJettiness(vector<fastjet::PseudoJet> &constit_pseudojets,
                                 unsigned int nSubJets, float jetRadius) const
{
  double alpha=1;
  if(constit_pseudojets.size() < nSubJets) {
    ATH_MSG_WARNING("We were asked to calculate nSubjettiness, but there are not enough constituents");
    return 0.0;
  }
                 
  fastjet::JetDefinition jet_def = fastjet::JetDefinition(fastjet::kt_algorithm,
                                                          M_PI/2,fastjet::E_scheme,
                                                          fastjet::Best);
  fastjet::ClusterSequence kt_clust_seq(constit_pseudojets, jet_def);
  vector<fastjet::PseudoJet> kt_subjets = kt_clust_seq.exclusive_jets((int)nSubJets);

  double tauNum = 0.0, tauDen = 0.0;

  for(size_t iC=0; iC < constit_pseudojets.size(); iC++) {
    
    double minDR = DBL_MAX;
    
    for(size_t iJ=0; iJ < kt_subjets.size(); iJ++) {
      double dr = sqrt(constit_pseudojets[iC].squared_distance(kt_subjets[iJ]));
      if(dr < minDR)
        minDR = dr;
    }

    tauNum += constit_pseudojets[iC].pt() * pow(minDR, alpha);
    tauDen += constit_pseudojets[iC].pt() * pow(jetRadius, alpha);
  }

  return tauNum/tauDen;
}
