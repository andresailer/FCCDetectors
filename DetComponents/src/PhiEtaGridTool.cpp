#include "PhiEtaGridTool.h"

// FCCSW
#include "DetSegmentation/PhiEtaGrid.h"

// DD4hep
#include "DDSegmentation/Segmentation.h"

DECLARE_TOOL_FACTORY(PhiEtaGridTool)

PhiEtaGridTool::PhiEtaGridTool(const std::string& aType, const std::string& aName, const IInterface* aParent) :
GaudiTool(aType, aName, aParent),
  m_segmentation(nullptr) {
  declareInterface<ISegmentationTool>(this);
  declareProperty("bitfield", m_decoderString);
  declareProperty("cellSizeEta", m_cellSizeEta = 1.);
  declareProperty("cellNumPhi", m_cellNumPhi = 100);
}

PhiEtaGridTool::~PhiEtaGridTool() {}

StatusCode PhiEtaGridTool::initialize() {
  if(GaudiTool::initialize().isFailure()) {
    return StatusCode::FAILURE;
  }
  // create segmentation
  m_segmentation = std::unique_ptr<DD4hep::DDSegmentation::Segmentation>(
    new DD4hep::DDSegmentation::PhiEtaGrid(m_decoderString));
  std::vector<DD4hep::DDSegmentation::SegmentationParameter*> params = m_segmentation->parameters();
  auto itParam = std::find_if(params.begin(), params.end(),
    [](DD4hep::DDSegmentation::SegmentationParameter* par){return bool(par->name()=="grid_size_eta");});
  (*itParam)->setValue(std::to_string(m_cellSizeEta));
  itParam = std::find_if(params.begin(), params.end(),
    [](DD4hep::DDSegmentation::SegmentationParameter* par){return bool(par->name()=="phi_bins");});
  (*itParam)->setValue(std::to_string(m_cellNumPhi));
  m_segmentation->setParameters(params);

  info()<<endmsg<<"Segmentation "<<m_segmentation->type()<<endmsg;
  info()<<"cell size in eta:  "<<m_cellSizeEta<<endmsg;
  info()<<"number of cells in phi:  "<<m_cellNumPhi<<endmsg;
  return StatusCode::SUCCESS;
}

StatusCode PhiEtaGridTool::finalize() {
  return GaudiTool::finalize();
}

std::unique_ptr<DD4hep::DDSegmentation::Segmentation> PhiEtaGridTool::segmentation() {
  return std::move(m_segmentation);
}
