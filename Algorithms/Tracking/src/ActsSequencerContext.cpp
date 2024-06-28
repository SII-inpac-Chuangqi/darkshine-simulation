//
// Created by xuliang on 2024/3/29.
//

#include "Algo/ActsSequencerContext.h"

#include "ActsExamples/EventData/MeasurementCalibration.hpp"

int ActsSequencerContext::addActsOptions(int argc, char* argv[]) {
    description = std::make_shared<boost::program_options::options_description>(ActsExamples::Options::makeDefaultOptions());
    ActsExamples::Options::addSequencerOptions(*description);
    ActsExamples::Options::addRandomNumbersOptions(*description);
    ActsExamples::Options::addGeometryOptions(*description);
    ActsExamples::Options::addMaterialOptions(*description);
    ActsExamples::Options::addInputOptions(*description);
    ActsExamples::Options::addOutputOptions(*description,
                                            ActsExamples::OutputFormat::Csv | ActsExamples::OutputFormat::DirectoryOnly);
    detector.addOptions(*description);
    ActsExamples::Options::addMagneticFieldOptions(*description);
    ActsExamples::Options::addFittingOptions(*description);
    ActsExamples::Options::addTrackFindingOptions(*description);
    ActsExamples::Options::addDigitizationOptions(*description);
    ActsExamples::Options::addParticleSmearingOptions(*description);
    ActsExamples::Options::addSpacePointMakerOptions(*description);
    ActsExamples::Options::addCsvWriterOptions(*description);

    variables = std::make_shared<boost::program_options::variables_map>(ActsExamples::Options::parse(*description, argc, argv));
    if (variables->empty()) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

void ActsSequencerContext::setupSimHitReader() {
    sim_hit_reader_cfg_ = ActsExamples::CsvSimHitReader::Config(ActsExamples::Options::readCsvSimHitReaderConfig(*variables));
    sim_hit_reader_cfg_.inputStem = "hits"; // since the implementation is overrride, this cfg is not used, just placeholder
    sim_hit_reader_cfg_.outputSimHits = "hits";
}

void ActsSequencerContext::setupParticleReader() {
    particle_reader_cfg_ = ActsExamples::Options::readCsvParticleReaderConfig(*variables);
    particle_reader_cfg_.inputStem = "particles_initial";
    particle_reader_cfg_.outputParticles = "particles_initial";
}

void ActsSequencerContext::setupDigitization() {
    rnd = std::make_shared<ActsExamples::RandomNumbers>(
            ActsExamples::Options::readRandomNumbersConfig(*variables));
    nlohmann::json j;
    j["acts-geometry-hierarchy-map"]["format-version"] = 0;
    j["acts-geometry-hierarchy-map"]["value-identifier"] = "digitization-configuration";
    nlohmann::json smearingDetails0 = {{"index", 0}, {"mean", 0.0}, {"stddev", 0.025}, {"type", "Gauss"}};
    nlohmann::json smearingDetails1 = {{"index", 1}, {"mean", 0.0}, {"stddev", 0.1}, {"type", "Gauss"}};
    nlohmann::json smearingArray = {smearingDetails0, smearingDetails1};
    nlohmann::json valueObject;
    valueObject["smearing"] = smearingArray;
    nlohmann::json entryObject;
    entryObject["volume"] = 1;
    entryObject["value"] = valueObject;
    j["entries"] = {entryObject};

    digi_cfg_ = std::make_shared<ActsExamples::DigitizationConfig>(
            (*variables).at("digi-merge").as<bool>(),
            (*variables).at("digi-merge-nsigma").as<double>(),
            (*variables).at("digi-merge-common-corner").as<bool>(),
            Acts::GeometryHierarchyMapJsonConverter<ActsExamples::DigiComponentsConfig>("digitization-configuration").fromJson(j)
    );
    //   Common options for digitization
    digi_cfg_->inputSimHits = sim_hit_reader_cfg_.outputSimHits;
    digi_cfg_->randomNumbers = rnd;
    digi_cfg_->trackingGeometry = trackingGeometry;
    digi_cfg_->outputSourceLinks = "sourcelinks";
    digi_cfg_->outputMeasurements = "measurements";
    digi_cfg_->outputClusters = "clusters";
    digi_cfg_->outputMeasurementParticlesMap = "measurement_particles_map";
    digi_cfg_->outputMeasurementSimHitsMap = "measurement_simhits_map";
    addAlgorithm(std::make_shared<ActsExamples::DigitizationAlgorithm>(*digi_cfg_, logLevel));
}

void ActsSequencerContext::setupParticleSelector() {
//    particle_selector_cfg_ = ActsExamples::Options::readTruthSeedSelectorConfig(*variables);
    particle_selector_cfg_.inputParticles = particle_reader_cfg_.outputParticles;
    particle_selector_cfg_.inputMeasurementParticlesMap = digi_cfg_->outputMeasurementParticlesMap;
    particle_selector_cfg_.outputParticles = "particles_selected";
    particle_selector_cfg_.ptMin = particle_selector_ptmin;
    particle_selector_cfg_.etaMin = -7.0;
    particle_selector_cfg_.etaMax = 7.0;
    particle_selector_cfg_.rhoMin = - std::numeric_limits<double>::max();
    particle_selector_cfg_.rhoMax = std::numeric_limits<double>::max();
    particle_selector_cfg_.zMin = - std::numeric_limits<double>::max();
    particle_selector_cfg_.zMax = std::numeric_limits<double>::max();
    addAlgorithm(std::make_shared<ActsExamples::TruthSeedSelector>(particle_selector_cfg_, logLevel));
}

void ActsSequencerContext::setupParticleSmearing() {
    particle_smearing_cfg_ = ActsExamples::Options::readParticleSmearingOptions(*variables);
    particle_smearing_cfg_.inputParticles = particle_selector_cfg_.outputParticles;
    particle_smearing_cfg_.outputTrackParameters = "smearedparameters";
    particle_smearing_cfg_.randomNumbers = rnd;
    // configs
    particle_smearing_cfg_.sigmaD0 = 10 * Acts::UnitConstants::um;
    particle_smearing_cfg_.sigmaD0PtA = 1 * Acts::UnitConstants::um;
    particle_smearing_cfg_.sigmaD0PtB = 1 * Acts::UnitConstants::um;
    particle_smearing_cfg_.sigmaZ0 = 10 * Acts::UnitConstants::um;
    particle_smearing_cfg_.sigmaZ0PtA = 1 * Acts::UnitConstants::um;
    particle_smearing_cfg_.sigmaZ0PtB = 1 * Acts::UnitConstants::um;
    particle_smearing_cfg_.sigmaT0 = 0;
    particle_smearing_cfg_.sigmaPhi = 0.005 * Acts::UnitConstants::degree;
    particle_smearing_cfg_.sigmaTheta = 0.005 * Acts::UnitConstants::degree;
    particle_smearing_cfg_.sigmaPRel = 0.02; // relative momentum resolution
    addAlgorithm(std::make_shared<ActsExamples::ParticleSmearing>(particle_smearing_cfg_, logLevel));
}

void ActsSequencerContext::setupSpacePointMaker() {
    Acts::GeometryIdentifier geoId;
    geoId.setVolume(1);
    std::vector<Acts::GeometryIdentifier> geoIdList;
    geoIdList.emplace_back(geoId);
    space_point_cfg_.inputSourceLinks  = "sourcelinks";
    space_point_cfg_.inputMeasurements = "measurements";
    space_point_cfg_.trackingGeometry = trackingGeometry;
    space_point_cfg_.geometrySelection = geoIdList;
    space_point_cfg_.outputSpacePoints = "spacepoints";
    addAlgorithm(std::make_shared<ActsExamples::SpacePointMaker>(space_point_cfg_, logLevel));
}

void ActsSequencerContext::setupSeedingAlgorithm() {
    // The Default SeedingAlgorithm
    seeding_cfg_.inputSpacePoints = {
            space_point_cfg_.outputSpacePoints,
    };
    seeding_cfg_.outputSeeds = "seeds";

    seeding_cfg_.gridConfig.rMax = 200._mm;
    seeding_cfg_.seedFinderConfig.rMax = seeding_cfg_.gridConfig.rMax;

    seeding_cfg_.seedFinderConfig.rMinMiddle = 0;

    seeding_cfg_.seedFilterConfig.deltaRMin = 1_mm;
    seeding_cfg_.seedFinderConfig.deltaRMin =
            seeding_cfg_.seedFilterConfig.deltaRMin;

    seeding_cfg_.gridConfig.deltaRMax = 60._mm;
    seeding_cfg_.seedFinderConfig.deltaRMax = seeding_cfg_.gridConfig.deltaRMax;

    seeding_cfg_.seedFinderConfig.collisionRegionMin = -250_mm;
    seeding_cfg_.seedFinderConfig.collisionRegionMax = 250._mm;

    seeding_cfg_.gridConfig.zMin = -2000._mm;
    seeding_cfg_.gridConfig.zMax = 2000._mm;
    seeding_cfg_.seedFinderConfig.zMin = seeding_cfg_.gridConfig.zMin;
    seeding_cfg_.seedFinderConfig.zMax = seeding_cfg_.gridConfig.zMax;

    seeding_cfg_.seedFilterConfig.maxSeedsPerSpM = 1;
    seeding_cfg_.seedFinderConfig.maxSeedsPerSpM =
            seeding_cfg_.seedFilterConfig.maxSeedsPerSpM;

    seeding_cfg_.gridConfig.cotThetaMax = 7.40627;  // 2.7 eta
    seeding_cfg_.seedFinderConfig.cotThetaMax =
            seeding_cfg_.gridConfig.cotThetaMax;

    seeding_cfg_.seedFinderConfig.sigmaScattering = 50;
    seeding_cfg_.seedFinderConfig.radLengthPerSeed = 0.1;

    seeding_cfg_.gridConfig.minPt = 500._MeV;
    seeding_cfg_.seedFinderConfig.minPt = seeding_cfg_.gridConfig.minPt;

    seeding_cfg_.gridOptions.bFieldInZ = 1.5_T;

    seeding_cfg_.seedFinderOptions.bFieldInZ = seeding_cfg_.gridOptions.bFieldInZ;
    seeding_cfg_.seedFinderOptions.beamPos = {0_mm, 0_mm};

    seeding_cfg_.seedFinderConfig.impactMax = 3._mm;
    addAlgorithm(std::make_shared<ActsExamples::SeedingAlgorithm>(seeding_cfg_, logLevel));
}

void ActsSequencerContext::setupTrackParamsEstimationAlgorithm() {
    params_estimation_cfg_.inputSeeds = seeding_cfg_.outputSeeds;
    params_estimation_cfg_.outputTrackParameters = "estimatedparameters";
    params_estimation_cfg_.trackingGeometry = trackingGeometry;
    params_estimation_cfg_.magneticField = magneticField;
    params_estimation_cfg_.bFieldMin = 1.0_T; // The minimum magnetic field to trigger the track parameters estimation
    params_estimation_cfg_.initialSigmas = {30._um, 30._um,  0.005_rad,
                                            0.005_rad, 0.01 / 1._GeV, 1._ps};
    params_estimation_cfg_.initialVarInflation = {1, 1, 1, 1, 1, 1};
    params_estimation_cfg_.particleHypothesis = Acts::ParticleHypothesis::electron();
}

void ActsSequencerContext::setupCKFTrackFinder(std::string& inputInitialTrackParameters) {
    track_finding_cfg_ = ActsExamples::Options::readTrackFindingConfig(*variables);
    track_finding_cfg_.inputMeasurements = digi_cfg_->outputMeasurements;
    track_finding_cfg_.inputSourceLinks = digi_cfg_->outputSourceLinks;
    track_finding_cfg_.inputInitialTrackParameters = inputInitialTrackParameters;
    track_finding_cfg_.outputTracks = "tracks_found";
    track_finding_cfg_.computeSharedHits = true;
    track_finding_cfg_.findTracks = ActsExamples::TrackFindingAlgorithm::makeTrackFinderFunction(
            trackingGeometry, magneticField,
            *Acts::getDefaultLogger("TrackFinder", logLevel));
    addAlgorithm(std::make_shared<ActsExamples::TrackFindingAlgorithm>(track_finding_cfg_, logLevel));
}

void ActsSequencerContext::setupKalmanFilter()
{
    const double reverse_filtering_mom_threshold = 0.0;
    const bool multiple_scattering_correction = true;
    const bool energy_loss_correction = true;
    const bool free_to_bound_correction = false;

    track_fitting_cfg_.inputMeasurements = digi_cfg_->outputMeasurements;
    track_fitting_cfg_.inputSourceLinks = digi_cfg_->outputSourceLinks;
    track_fitting_cfg_.inputProtoTracks = track_finding_cfg_.outputTracks;
    track_fitting_cfg_.inputInitialTrackParameters = particle_smearing_cfg_.outputTrackParameters;
    track_fitting_cfg_.outputTracks = "tracks_fit";
    track_fitting_cfg_.fit = ActsExamples::makeKalmanFitterFunction(trackingGeometry, magneticField,
                                                                    multiple_scattering_correction,
                                                                    energy_loss_correction,
                                                                    reverse_filtering_mom_threshold,
                                                                    Acts::FreeToBoundCorrection(free_to_bound_correction));
    track_fitting_cfg_.calibrator = std::make_shared<ActsExamples::PassThroughCalibrator>();
    addAlgorithm(std::make_shared<ActsExamples::TrackFittingAlgorithm>(track_fitting_cfg_, logLevel));
}

void ActsSequencerContext::setupTracksToTrajectories() {
    //tracks_to_traj_cfg_.inputTracks = track_fitting_cfg_.outputTracks;
    tracks_to_traj_cfg_.inputTracks = track_finding_cfg_.outputTracks;
    tracks_to_traj_cfg_.outputTrajectories = "trajectories";
    addAlgorithm(std::make_shared<ActsExamples::TracksToTrajectories>(tracks_to_traj_cfg_, logLevel));
}

void ActsSequencerContext::setupVertexFinder() {
//    vertex_finder_cfg_.inputTrackParameters = track_finding_cfg_.outputTracks;
    vertex_finder_cfg_.inputTrajectories = tracks_to_traj_cfg_.outputTrajectories;
    vertex_finder_cfg_.outputProtoVertices = "amvf_protovertices";
    vertex_finder_cfg_.outputVertices = "amvf_fittedVertices";
    vertex_finder_cfg_.bField = magneticField;
    addAlgorithm(std::make_shared<ActsExamples::AdaptiveMultiVertexFinderAlgorithm>(vertex_finder_cfg_, logLevel));
}

int ActsSequencerContext::setup(const std::vector<std::string>& arguments) {
    // setup and parse options
    std::vector<char*> argv_vec;
    for (const auto &arg: arguments) {
        argv_vec.emplace_back(const_cast<char*> (arg.c_str()));
    }
    int argc = (int)argv_vec.size();
    auto argv = argv_vec.data();
    addActsOptions(argc, argv);

    // Setup the detector geometry
    geometry = ActsExamples::Geometry::build(*variables, detector);
    trackingGeometry = geometry.first;
    // Add context decorators
    for (const auto& cdr : geometry.second) {
        decorators.emplace_back(cdr);
    }

    // Set up the magnetic field
    if (useDMagnet) {
        magneticField = std::make_shared<DMagnetField>();
        Acts::Vector3 origin;
        std::cout << "[INFO] ==> magnet (T) in ACTS reference frame: \n"
                  << std::dynamic_pointer_cast<DMagnetField>(magneticField)->getField(origin)/Acts::UnitConstants::T << std::endl;
    } else {
        magneticField = ActsExamples::Options::readMagneticField(*variables);
    }

    std::string outputTrackParameters;
    setupSimHitReader();
    setupDigitization();
    if (truthSmearedSeeded) {
        setupParticleReader();
        setupParticleSelector();
        setupParticleSmearing();
        outputTrackParameters = particle_smearing_cfg_.outputTrackParameters;
    } else {
        setupSpacePointMaker();
        setupSeedingAlgorithm();
        setupTrackParamsEstimationAlgorithm();
        outputTrackParameters = params_estimation_cfg_.outputTrackParameters;
    }
    setupCKFTrackFinder(outputTrackParameters);
    //setupKalmanFilter();
    setupTracksToTrajectories();
    setupVertexFinder();
//    setupTrackStatesWriter();

    return EXIT_SUCCESS;
}
