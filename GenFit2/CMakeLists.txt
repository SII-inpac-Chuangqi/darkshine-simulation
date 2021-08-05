
# include(GNUInstallDirs)


# define path to macros and settings
LIST( APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake )
# include the macros and settings
INCLUDE(settings)
INCLUDE(macros)
INCLUDE(genfit)

# Set GENFIT include directories
SET(GF_INC_DIRS
		./core/include/
		./fields/include/
		./finitePlanes/include/
		./fitters/include/
		./measurements/include/
		./trackReps/include/
		./utilities/include/
		${GF_INC_DIRS}
)
#
INCLUDE_DIRECTORIES(
		BEFORE
		${ROOT_INCLUDE_DIRS}
		SYSTEM ${EIGEN3_INCLUDE_DIR}
		${GF_INC_DIRS}
)

# add the list of source code files -- careful, when a new file is added!
AUX_SOURCE_DIRECTORY( ./core/src          library_sources )
AUX_SOURCE_DIRECTORY( ./fields/src        library_sources )
AUX_SOURCE_DIRECTORY( ./finitePlanes/src  library_sources )
AUX_SOURCE_DIRECTORY( ./fitters/src       library_sources )
AUX_SOURCE_DIRECTORY( ./measurements/src  library_sources )
AUX_SOURCE_DIRECTORY( ./trackReps/src     library_sources )
AUX_SOURCE_DIRECTORY( ./utilities/src     library_sources )

# Dictionary generation.  For the time being, we list classes one-by-one.
SET(CORE_DICTIONARY_SOURCES
		${CMAKE_CURRENT_SOURCE_DIR}/core/include/AbsFinitePlane.h
		${CMAKE_CURRENT_SOURCE_DIR}/core/include/AbsFitterInfo.h
		${CMAKE_CURRENT_SOURCE_DIR}/core/include/AbsHMatrix.h
		${CMAKE_CURRENT_SOURCE_DIR}/core/include/AbsTrackRep.h
		${CMAKE_CURRENT_SOURCE_DIR}/core/include/DetPlane.h
		${CMAKE_CURRENT_SOURCE_DIR}/core/include/FitStatus.h
		${CMAKE_CURRENT_SOURCE_DIR}/core/include/IO.h
		${CMAKE_CURRENT_SOURCE_DIR}/core/include/Material.h
		${CMAKE_CURRENT_SOURCE_DIR}/core/include/MeasuredStateOnPlane.h
		${CMAKE_CURRENT_SOURCE_DIR}/core/include/MeasurementFactory.h
		${CMAKE_CURRENT_SOURCE_DIR}/core/include/MeasurementOnPlane.h
		${CMAKE_CURRENT_SOURCE_DIR}/core/include/MeasurementProducer.h
		${CMAKE_CURRENT_SOURCE_DIR}/core/include/StateOnPlane.h
		${CMAKE_CURRENT_SOURCE_DIR}/core/include/AbsMeasurement.h
		${CMAKE_CURRENT_SOURCE_DIR}/core/include/ThinScatterer.h
		${CMAKE_CURRENT_SOURCE_DIR}/core/include/Track.h
		${CMAKE_CURRENT_SOURCE_DIR}/core/include/TrackCand.h
		${CMAKE_CURRENT_SOURCE_DIR}/core/include/TrackCandHit.h
		${CMAKE_CURRENT_SOURCE_DIR}/core/include/TrackPoint.h
		${CMAKE_CURRENT_SOURCE_DIR}/finitePlanes/include/RectangularFinitePlane.h
		${CMAKE_CURRENT_SOURCE_DIR}/fields/include/DMagnet.h
)

ROOT_GENERATE_DICTIONARY_genfit(
		"${CMAKE_SHARED_LIBRARY_PREFIX}genfit2"
		"${CORE_DICTIONARY_SOURCES}"
		"${CMAKE_CURRENT_SOURCE_DIR}/core/include"
		"${CMAKE_CURRENT_SOURCE_DIR}/core/src/coreLinkDef.h"
		"${CMAKE_CURRENT_BINARY_DIR}/coreRootDict.cc"
)
SET(library_sources "${CMAKE_CURRENT_BINARY_DIR}/coreRootDict.cc" "${library_sources}")


SET(FITTERS_DICTIONARY_SOURCES
		${CMAKE_CURRENT_SOURCE_DIR}/fitters/include/AbsKalmanFitter.h
		${CMAKE_CURRENT_SOURCE_DIR}/fitters/include/KalmanFittedStateOnPlane.h
		${CMAKE_CURRENT_SOURCE_DIR}/fitters/include/KalmanFitStatus.h
		${CMAKE_CURRENT_SOURCE_DIR}/fitters/include/ReferenceStateOnPlane.h
		${CMAKE_CURRENT_SOURCE_DIR}/fitters/include/KalmanFitterRefTrack.h
		${CMAKE_CURRENT_SOURCE_DIR}/fitters/include/KalmanFitter.h
		${CMAKE_CURRENT_SOURCE_DIR}/fitters/include/KalmanFitterInfo.h
		${CMAKE_CURRENT_SOURCE_DIR}/fitters/include/DAF.h
)
ROOT_GENERATE_DICTIONARY_genfit(
		"${CMAKE_SHARED_LIBRARY_PREFIX}genfit2"
		"${FITTERS_DICTIONARY_SOURCES}"
		"${CMAKE_CURRENT_SOURCE_DIR}/core/include;${CMAKE_CURRENT_SOURCE_DIR}/fitters/include"
		"${CMAKE_CURRENT_SOURCE_DIR}/fitters/src/fittersLinkDef.h"
		"${CMAKE_CURRENT_BINARY_DIR}/fittersRootDict.cc"
)
SET(library_sources "${CMAKE_CURRENT_BINARY_DIR}/fittersRootDict.cc" "${library_sources}")

SET(MEASUREMENTS_DICTIONARY_SOURCES
		${CMAKE_CURRENT_SOURCE_DIR}/measurements/include/HMatrixPhi.h
		${CMAKE_CURRENT_SOURCE_DIR}/measurements/include/HMatrixU.h
		${CMAKE_CURRENT_SOURCE_DIR}/measurements/include/HMatrixUnit.h
		${CMAKE_CURRENT_SOURCE_DIR}/measurements/include/HMatrixV.h
		${CMAKE_CURRENT_SOURCE_DIR}/measurements/include/HMatrixUV.h
		${CMAKE_CURRENT_SOURCE_DIR}/measurements/include/FullMeasurement.h
		${CMAKE_CURRENT_SOURCE_DIR}/measurements/include/PlanarMeasurement.h
		${CMAKE_CURRENT_SOURCE_DIR}/measurements/include/ProlateSpacepointMeasurement.h
		${CMAKE_CURRENT_SOURCE_DIR}/measurements/include/SpacepointMeasurement.h
		${CMAKE_CURRENT_SOURCE_DIR}/measurements/include/WireMeasurement.h
		${CMAKE_CURRENT_SOURCE_DIR}/measurements/include/WireMeasurementNew.h
		${CMAKE_CURRENT_SOURCE_DIR}/measurements/include/WirePointMeasurement.h
		${CMAKE_CURRENT_SOURCE_DIR}/measurements/include/WireTrackCandHit.h
)
ROOT_GENERATE_DICTIONARY_genfit(
		"${CMAKE_SHARED_LIBRARY_PREFIX}genfit2"
		"${MEASUREMENTS_DICTIONARY_SOURCES}"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/include;${CMAKE_CURRENT_SOURCE_DIR}/measurements/include"
		"${CMAKE_CURRENT_SOURCE_DIR}/measurements/src/measurementsLinkDef.h"
		"${CMAKE_CURRENT_BINARY_DIR}/measurementsRootDict.cc"
)
SET(library_sources "${CMAKE_CURRENT_BINARY_DIR}/measurementsRootDict.cc" "${library_sources}")

SET(TRACKREPS_DICTIONARY_SOURCES
		${CMAKE_CURRENT_SOURCE_DIR}/trackReps/include/RKTrackRep.h
		${CMAKE_CURRENT_SOURCE_DIR}/trackReps/include/MplTrackRep.h
)
ROOT_GENERATE_DICTIONARY_genfit(
		"${CMAKE_SHARED_LIBRARY_PREFIX}genfit2"
		"${TRACKREPS_DICTIONARY_SOURCES}"
		"${CMAKE_CURRENT_SOURCE_DIR}/core/include;${CMAKE_CURRENT_SOURCE_DIR}/trackReps/include"
		"${CMAKE_CURRENT_SOURCE_DIR}/trackReps/src/trackRepsLinkDef.h"
		"${CMAKE_CURRENT_BINARY_DIR}/trackRepsRootDict.cc"
)
SET(library_sources "${CMAKE_CURRENT_BINARY_DIR}/trackRepsRootDict.cc" "${library_sources}")

SET(UTILITIES_DICTIONARY_SOURCES
		${CMAKE_CURRENT_SOURCE_DIR}/utilities/include/HelixTrackModel.h
		${CMAKE_CURRENT_SOURCE_DIR}/utilities/include/MeasurementCreator.h
		${CMAKE_CURRENT_SOURCE_DIR}/utilities/include/mySpacepointDetectorHit.h
		${CMAKE_CURRENT_SOURCE_DIR}/utilities/include/mySpacepointMeasurement.h
)
ROOT_GENERATE_DICTIONARY_genfit(
		"${CMAKE_SHARED_LIBRARY_PREFIX}genfit2"
		"${UTILITIES_DICTIONARY_SOURCES}"
		"${CMAKE_CURRENT_SOURCE_DIR}/core/include;${CMAKE_CURRENT_SOURCE_DIR}/utilities/include;${CMAKE_CURRENT_SOURCE_DIR}/measurements/include"
		"${CMAKE_CURRENT_SOURCE_DIR}/utilities/src/utilitiesLinkDef.h"
		"${CMAKE_CURRENT_BINARY_DIR}/utilitiesRootDict.cc"
)
SET(library_sources "${CMAKE_CURRENT_BINARY_DIR}/utilitiesRootDict.cc" "${library_sources}")

# install
ADD_SHARED_LIBRARY( genfit2 ${library_sources})
INSTALL(TARGETS genfit2 LIBRARY DESTINATION lib)


# TARGET_LINK_LIBRARIES(genfit2 ${ROOT_LIBS})

