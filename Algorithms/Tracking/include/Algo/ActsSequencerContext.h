//
// Created by xuliang on 2024/3/29.
//

#ifndef TRACKING_ACTSSEQUENCERCONTEXT_H
#define TRACKING_ACTSSEQUENCERCONTEXT_H

#include "Core/AnaData.h"
#include "Algo/ActsMagnet.h"

// Acts Core
#include "Acts/Utilities/Logger.hpp"
#include "ActsFatras/EventData/Hit.hpp"
// Acts misc
#include "ActsExamples/Geometry/CommonGeometry.hpp"
#include "ActsExamples/Detector/TelescopeDetectorWithOptions.hpp"
#include "ActsExamples/EventData/SimHit.hpp"
#include "ActsExamples/Utilities/TracksToTrajectories.hpp"
// Acts IO
#include "ActsExamples/Io/Json/JsonGeometryList.hpp"
#include "ActsExamples/Io/Json/JsonDigitizationConfig.hpp"
#include "ActsExamples/Io/Root/RootTrajectoryStatesWriter.hpp"
// Acts Options
#include "ActsExamples/Options/CommonOptions.hpp"
#include "ActsExamples/Options/CsvOptionsReader.hpp"
#include "ActsExamples/Options/MagneticFieldOptions.hpp"
#include "ActsExamples/Options/TrackFittingOptions.hpp"
#include "ActsExamples/Options/TrackFindingOptions.hpp"
#include "ActsExamples/Options/DigitizationOptions.hpp"
#include "ActsExamples/Options/SpacePointMakerOptions.hpp"
#include "ActsExamples/Options/CsvOptionsWriter.hpp"
#include "ActsExamples/Options/ParticleSmearingOptions.hpp"
#include "ActsExamples/Options/TruthSeedSelectorOptions.hpp"
#include "ActsExamples/Options/ParticleSmearingOptions.hpp"
#include "ActsExamples/Options/TrackFindingOptions.hpp"
// Acts Algorithms
#include "ActsExamples/TruthTracking/TruthSeedSelector.hpp"
#include "ActsExamples/TruthTracking/ParticleSmearing.hpp"
#include "ActsExamples/TrackFinding/SeedingAlgorithm.hpp"
#include "ActsExamples/TrackFinding/TrackParamsEstimationAlgorithm.hpp"
#include "ActsExamples/TrackFinding/TrackFindingAlgorithm.hpp"
#include "ActsExamples/TrackFitting/TrackFittingAlgorithm.hpp"
#include "ActsExamples/Vertexing/AdaptiveMultiVertexFinderAlgorithm.hpp"
#include "ActsExamples/Vertexing/IterativeVertexFinderAlgorithm.hpp"

class ActsSequencerContext {
public:
    ActsSequencerContext() = default;
    ~ActsSequencerContext() = default;
    int setup(const std::vector<std::string>& arguments);
    void setConstantBField(double in) {
        constantBField.at(2) = -in;
    }
public:
    tracking::detector detector_type{};
    bool useDMagnet = false;
    bool truthSmearedSeeded = true;
    double particle_selector_ptmin{};
//    Acts::Logging::Level logLevel = Acts::Logging::DEBUG;
    Acts::Logging::Level logLevel = Acts::Logging::ERROR;
    // Acts whiteboard
    std::vector<std::shared_ptr<ActsExamples::IContextDecorator>> decorators;
    std::unordered_map<std::string, std::string> whiteboardObjectAliases;
    // Acts Sequencer
    std::vector<std::shared_ptr<ActsExamples::SequenceElement>> sequenceElements;
    // Reader Configs
    ActsExamples::CsvSimHitReader::Config sim_hit_reader_cfg_{};
    ActsExamples::CsvParticleReader::Config particle_reader_cfg_{};
    ActsExamples::TracksToTrajectories::Config tracks_to_traj_cfg_{};

    ActsExamples::AdaptiveMultiVertexFinderAlgorithm::Config vertex_finder_cfg_;

private:
    void addElement(const std::shared_ptr<ActsExamples::SequenceElement>& element) {
        assert(std::find(sequenceElements.begin(), sequenceElements.end(), element) == sequenceElements.end() && " don't add twice in sequence.");
        sequenceElements.emplace_back(element);
    }
    void addAlgorithm(const std::shared_ptr<ActsExamples::IAlgorithm>& algorithm) {
        addElement(algorithm);
    }
    int addActsOptions(int argc, char* argv[]);

    void setupSimHitReader();
    void setupParticleReader(); // for truth seeding
    void setupDigitization();
    void setupParticleSelector(); // for truth seeding
    void setupParticleSmearing(); // for truth seeding
    void setupSpacePointMaker();
    void setupSeedingAlgorithm();
    void setupTrackParamsEstimationAlgorithm();
    void setupCKFTrackFinder(std::string& inputInitialTrackParameters);
    void setupKalmanFilter();
    void setupTracksToTrajectories();
    void setupVertexFinder();
//    void setupTrackStatesWriter();
    // geometry setup
    std::array<double, 3> constantBField = {0_T, 0_T, 1.5_T};

    // General Configs
    std::shared_ptr<boost::program_options::options_description> description{};
    std::shared_ptr<boost::program_options::variables_map> variables{};
    std::shared_ptr<ActsExamples::RandomNumbers> rnd;

    // Algorithm Configs
    ActsExamples::SpacePointMaker::Config space_point_cfg_;
    std::shared_ptr<ActsExamples::DigitizationConfig> digi_cfg_;
    ActsExamples::TruthSeedSelector::Config particle_selector_cfg_;
    ActsExamples::ParticleSmearing::Config particle_smearing_cfg_;
    ActsExamples::SeedingAlgorithm::Config seeding_cfg_;
    ActsExamples::TrackParamsEstimationAlgorithm::Config params_estimation_cfg_;
    ActsExamples::TrackFindingAlgorithm::Config track_finding_cfg_;
    ActsExamples::TrackFittingAlgorithm::Config track_fitting_cfg_;

    // Acts detector geometry and decorators
    ActsExamples::TelescopeDetectorWithOptions detector;
    std::pair<std::shared_ptr<const Acts::TrackingGeometry>,
    std::vector<std::shared_ptr<ActsExamples::IContextDecorator>>> geometry;
    std::shared_ptr<const Acts::TrackingGeometry> trackingGeometry;
    std::shared_ptr<Acts::MagneticFieldProvider> magneticField;
};

#endif //TRACKING_ACTSSEQUENCERCONTEXT_H
