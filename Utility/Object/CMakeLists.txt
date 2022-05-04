INCLUDE_DIRECTORIES(
        include
)

SET(DEvent_header_shared
        # Reference the current persistent version:
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Object/DHit.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Object/DParticle.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Object/DStep.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Object/SimulatedHit.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Object/CalorimeterHit.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Object/McParticle.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Object/ReconstructedParticle.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Object/DigiForm.hh
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Object/DEvent.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Object/DMagnet.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Object/McPHelper.h
        # baseline1.5 version
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Object/versions/DHit_b1.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Object/versions/DParticle_b1_5.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Object/versions/DStep_b1_5.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Object/versions/SimulatedHit_b1_5.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Object/versions/CalorimeterHit_b1.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Object/versions/McParticle_b1_5.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Object/versions/ReconstructedParticle_b1_5.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Object/versions/DigiForm_b1.hh
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Object/versions/DEvent_b1_5.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Object/versions/DMagnet_b1.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Object/versions/McPHelper_b1_5.h
        )
SET(DEvent_source
        # Reference the current persistent version:
        ${CMAKE_CURRENT_SOURCE_DIR}/src/SimulatedHit.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/CalorimeterHit.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/McParticle.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ReconstructedParticle.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/DigiForm.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/DEvent.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/DMagnet.cpp
        )

root_generate_dictionary(G__DEvent ${DEvent_header_shared} LINKDEF include/Object/DEventLinkDef.h)
add_library(DEvent SHARED ${DEvent_header_shared} ${DEvent_source} G__DEvent.cxx)

install(TARGETS DEvent LIBRARY DESTINATION lib )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libDEvent_rdict.pcm DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libDEvent.rootmap DESTINATION lib)
