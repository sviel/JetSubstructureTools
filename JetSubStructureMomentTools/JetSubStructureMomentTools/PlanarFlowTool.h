#ifndef jetsubstructuremomenttools_planarflowtool_header
#define jetsubstructuremomenttools_planarflowtool_header

#include "JetSubStructureMomentTools/JetSubStructureMomentToolsBase.h"

class PlanarFlowTool :
  public JetSubStructureMomentToolsBase {
    ASG_TOOL_CLASS(PlanarFlowTool, IJetModifier)
    
    public:
      // Constructor and destructor
      PlanarFlowTool(std::string name);

      int modifyJet(xAOD::Jet &jet) const;
};


#endif
