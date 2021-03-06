cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

if(ALIGPU_BUILD_TYPE STREQUAL "ALIROOT")
    set(DEFINITIONS ${DEFINITIONS} GPUCA_ALIROOT_LIB)
endif()

if(ALIGPU_BUILD_TYPE STREQUAL "O2")
    set(DEFINITIONS ${DEFINITIONS} GPUCA_O2_LIB GPUCA_TPC_GEOMETRY_O2 HAVE_O2HEADERS)
endif()

if(OpenMP_CXX_FOUND)
    message(STATUS "AliGPU: Using OpenMP: ${OpenMP_CXX_SPEC_DATE}")
    set(DEFINITIONS ${DEFINITIONS} GPUCA_HAVE_OPENMP)
endif()

include_directories(. SliceTracker Merger Base Global TRDTracking ../Common ../TPCFastTransformation Standalone)

if (ENABLE_CUDA OR ENABLE_OPENCL OR ENABLE_HIP)
    if (CMAKE_SYSTEM_NAME MATCHES Darwin)
        message(WARNING "GPU Tracking disabled on MacOS")
    else()
        if(ENABLE_CUDA)
            add_subdirectory(Base/cuda)
        endif()
        if(ENABLE_OPENCL)
            add_subdirectory(Base/opencl)
        endif()
        if (ENABLE_HIP)
            add_subdirectory(Base/hip)
        endif()
    endif()
endif()

if(ALIGPU_BUILD_TYPE STREQUAL "O2")
    set(MODULE O2GPUTracking)
elseif(ALIGPU_BUILD_TYPE STREQUAL "ALIROOT")
    set(MODULE AliGPUTracking)
endif()

include_directories(Standalone/display Standalone/qa)

#SRCs processed by CINT and added to HDRS
set(SRCS
    SliceTracker/AliGPUTPCTrack.cxx
    SliceTracker/AliGPUTPCBaseTrackParam.cxx
    SliceTracker/AliGPUTPCTrackParam.cxx
    SliceTracker/AliGPUTPCTracklet.cxx
    SliceTracker/AliGPUTPCStartHitsFinder.cxx
    SliceTracker/AliGPUTPCStartHitsSorter.cxx
    SliceTracker/AliGPUTPCNeighboursCleaner.cxx
    SliceTracker/AliGPUTPCTracker.cxx
    SliceTracker/AliGPUTPCSliceData.cxx
    SliceTracker/AliGPUTPCSliceOutput.cxx
    SliceTracker/AliGPUTPCTrackletConstructor.cxx
    SliceTracker/AliGPUTPCNeighboursFinder.cxx
    SliceTracker/AliGPUTPCGrid.cxx
    SliceTracker/AliGPUTPCTrackletSelector.cxx
    SliceTracker/AliGPUTPCHitArea.cxx
    SliceTracker/AliGPUTPCClusterData.cxx
    SliceTracker/AliGPUTPCRow.cxx
    Merger/AliGPUTPCGMMerger.cxx
    Merger/AliGPUTPCGMSliceTrack.cxx
    Merger/AliGPUTPCGMTrackParam.cxx
    Merger/AliGPUTPCGMPhysicalTrackModel.cxx
    Merger/AliGPUTPCGMPropagator.cxx
    Merger/AliGPUTPCGMPolynomialField.cxx
    Merger/AliGPUTPCGMPolynomialFieldManager.cxx
    Merger/AliGPUTPCGMMergerGPU.cxx
    TRDTracking/AliGPUTRDTrack.cxx
    TRDTracking/AliGPUTRDTracker.cxx
    TRDTracking/AliGPUTRDTrackletWord.cxx
    TRDTracking/AliGPUTRDTrackerGPU.cxx
    Base/AliGPUParam.cxx
)

set(SRCS_NO_CINT
    Base/AliGPUReconstruction.cxx
    Base/AliGPUReconstructionCPU.cxx
    Base/AliGPUReconstructionConvert.cxx
    Base/AliGPUSettings.cxx
    Base/AliGPUProcessor.cxx
    Base/AliGPUMemoryResource.cxx
    Base/AliGPUGeneralKernels.cxx
    Base/AliGPUReconstructionDeviceBase.cxx
    Global/AliGPUChain.cxx
    Global/AliGPUChainTracking.cxx
    Global/AliGPUChainITS.cxx
    utils/timer.cpp
)

set(SRCS_NO_H
    SliceTracker/AliGPUTPCTrackerDump.cxx
)

#Extra headers to install
set(HDRS
    SliceTracker/AliGPUTPCDef.h
    SliceTracker/AliGPUTPCSettings.h
    SliceTracker/AliGPUTPCGPUConfig.h
    SliceTracker/AliGPUTPCMCInfo.h
    SliceTracker/AliGPUTPCHit.h
    SliceTracker/AliGPUTPCHitId.h
    SliceTracker/AliGPUTPCSliceOutCluster.h
    SliceTracker/AliGPUTPCSliceOutTrack.h
    SliceTracker/AliGPUTPCTrackLinearisation.h
    Merger/AliGPUTPCGMBorderTrack.h
    Merger/AliGPUTPCGMCluster.h
    Merger/AliGPUTPCGMOfflineStatisticalErrors.h
    Merger/AliGPUTPCGMMergedTrack.h
    Merger/AliGPUTPCGMMergedTrackHit.h
    TRDTracking/AliGPUTRDDef.h
    TRDTracking/AliGPUTRDTrackPoint.h
    TRDTracking/AliGPUTRDTrack.h
    TRDTracking/AliGPUTRDTrackData.h
    TRDTracking/AliGPUTRDTrackPoint.h
    TRDTracking/AliGPUTRDTrackletLabels.h
    Base/AliGPUConstantMem.h
    Base/AliGPUOutputControl.h
    Base/AliGPUReconstructionIncludes.h
)
#Not processed by ROOT
set(HDRS2
    SliceTracker/AliGPUTPCGeometry.h
    SliceTracker/AliGPULogging.h
    SliceTracker/ClusterNativeAccessExt.h
    TRDTracking/AliGPUTRDGeometry.h
    TRDTracking/AliGPUTRDTrackerDebug.h
    TRDTracking/AliGPUTRDInterfaces.h
    Standalone/display/AliGPUDisplayConfig.h
    Standalone/qa/AliGPUQAConfig.h
    Base/AliGPUReconstructionKernels.h
    Base/AliGPUReconstructionIncludesITS.h
    Base/AliGPUReconstructionHelpers.h
    Base/AliGPUDataTypes.h
)

#Some extra files / includes / settings needed for the build for AliRoot
if(ALIGPU_BUILD_TYPE STREQUAL "ALIROOT")
    include_directories(SYSTEM ${ROOT_INCLUDE_DIR})
    include_directories(${AliRoot_SOURCE_DIR}/HLT/BASE
        ${AliRoot_SOURCE_DIR}/HLT/BASE/util
        ${AliRoot_SOURCE_DIR}/HLT/TPCLib
        ${AliRoot_SOURCE_DIR}/HLT/TPCLib/transform
        ${AliRoot_SOURCE_DIR}/HLT/TPCLib/comp
        ${AliRoot_SOURCE_DIR}/HLT/TRD
        ${AliRoot_SOURCE_DIR}/TRD/TRDbase
        ${AliRoot_SOURCE_DIR}/STEER/STEERBase
        ${AliRoot_SOURCE_DIR}/STEER/STEER
        ${AliRoot_SOURCE_DIR}/STEER/ESD
        ${AliRoot_SOURCE_DIR}/STEER/CDB
        ${AliRoot_SOURCE_DIR}/TPC/TPCbase
        ${AliRoot_SOURCE_DIR}/TPC/TPCcalib
        ${AliRoot_SOURCE_DIR}/TPC/TPCrec
    )

    set(SRCS ${SRCS}
        Merger/AliGPUTPCGlobalMergerComponent.cxx
        SliceTracker/AliGPUTPCTrackerComponent.cxx
        Merger/AliGPUTPCGMTracksToTPCSeeds.cxx
        DataCompression/AliHLTTPCClusterStatComponent.cxx
        TRDTracking/AliGPUTRDTrackerComponent.cxx
        TRDTracking/AliGPUTRDTrackletReaderComponent.cxx

        Global/AliHLTGPUDumpComponent.cxx
    )
    
    set (SRCS_NO_CINT ${SRCS_NO_CINT}
        ${AliRoot_SOURCE_DIR}/HLT/TPCLib/AliHLTTPCGeometry.cxx
        ${AliRoot_SOURCE_DIR}/HLT/TPCLib/AliHLTTPCLog.cxx
        ${AliRoot_SOURCE_DIR}/HLT/TPCLib/AliHLTTPCDefinitions.cxx
        ${AliRoot_SOURCE_DIR}/HLT/TRD/AliHLTTRDDefinitions.cxx
    )
    
    set (HDRS2 ${HDRS2}
        SliceTracker/AliGPUTPCDefinitions.h
    )
    
    ALICE_UseVc()
else() #if we do not build for AliRoot, for the time being we still need some dummy headers for some definitions
    include_directories(HLTHeaders)
endif()

#Some extra files / includes / settings needed for the build for O2
if(ALIGPU_BUILD_TYPE STREQUAL "O2")
    set(SRCS ${SRCS}
        Interface/AliGPUO2Interface.cxx
    )
    set(SRCS_NO_CINT ${SRCS_NO_CINT}
        Standalone/display/AliGPUDisplayBackend.cpp
    )
    set (HDRS2 ${HDRS2} Interface/AliGPUO2InterfaceConfiguration.h)
endif()

if (GPUCA_EVENT_DISPLAY)
    message(STATUS "Building AliGPU Event Display")
    set(SRCS_NO_CINT ${SRCS_NO_CINT}
        utils/qsem.cpp
        Standalone/display/AliGPUDisplay.cpp
        Standalone/display/AliGPUDisplayBackendGlfw.cpp
    )
    set(SRCS_NO_H ${SRCS_NO_H}
        Standalone/display/AliGPUDisplayQuaternion.cpp
        Standalone/display/AliGPUDisplayInterpolation.cpp
        Standalone/display/AliGPUDisplayKeys.cpp
    )
    set(DEFINITIONS ${DEFINITIONS} BUILD_EVENT_DISPLAY)
else()
    set(HDRS2 ${HDRS2} Standalone/display/AliGPUDisplay.h)
endif()

if (GPUCA_QA)
    message(STATUS "Building AliGPU QA")
    set(SRCS_NO_CINT ${SRCS_NO_CINT} Standalone/qa/AliGPUQA.cpp)
    set(DEFINITIONS ${DEFINITIONS} BUILD_QA)
else()
    set(HDRS2 ${HDRS2} Standalone/qa/AliGPUQA.h)
endif()

if (GPUCA_EVENT_DISPLAY OR GPUCA_QA)
    set(HDRS2 ${HDRS2} Standalone/qconfigoptions.h)
endif()

string(REPLACE ".cxx" ".h" HDRSSRCA "${SRCS}")
string(REPLACE ".cpp" ".h" HDRSSRC "${HDRSSRCA}")
set(HDRS ${HDRS} ${HDRSSRC})
string(REPLACE ".cxx" ".h" HDRSSRCA "${SRCS_NO_CINT}")
string(REPLACE ".cpp" ".h" HDRSSRC "${HDRSSRCA}")
set(HDRS2 ${HDRS2} ${HDRSSRC})
set(SRCS ${SRCS} ${SRCS_NO_CINT} ${SRCS_NO_H})
foreach (def ${DEFINITIONS})
    add_definitions(-D${def})
endforeach()

#Default cmake build script for AliRoot
if(ALIGPU_BUILD_TYPE STREQUAL "ALIROOT")
    # Generate the dictionary
    # It will create G_ARG1.cxx and G_ARG1.h / ARG1 = function first argument
    get_directory_property(incdirs INCLUDE_DIRECTORIES)
    generate_dictionary_flat("${MODULE}" "GPUTrackingLinkDef_AliRoot.h" "${HDRS}" "${incdirs}")

    # Generate the ROOT map
    # Dependecies
    set(LIBDEPS STEERBase STEER ESD CDB HLTbase TPCbase TPCrec TPCcalib TRDbase AliTPCFastTransformation)
    generate_rootmap("${MODULE}" "${LIBDEPS}" "${CMAKE_CURRENT_SOURCE_DIR}/GPUTrackingLinkDef_AliRoot.h")
    # Don't pass Vc to root
    set(LIBDEPS ${LIBDEPS} Vc)

    # Add a library to the project using the specified source files
    add_library_tested(${MODULE} SHARED ${SRCS} G__${MODULE}.cxx)
    target_link_libraries(${MODULE} ${LIBDEPS})

    # Additional compilation flags
    set_target_properties(${MODULE} PROPERTIES COMPILE_FLAGS "")

    # System dependent: Modify the way the library is build
    if(${CMAKE_SYSTEM} MATCHES Darwin)
        set_target_properties(${MODULE} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
    endif(${CMAKE_SYSTEM} MATCHES Darwin)

    # Installation
    install(TARGETS ${MODULE}
            ARCHIVE DESTINATION lib
            LIBRARY DESTINATION lib)

    install(FILES ${HDRS} ${HDRS2} DESTINATION include)
    install(DIRECTORY utils DESTINATION include FILES_MATCHING PATTERN *.h)
endif()

#Default cmake build script for O2
if(ALIGPU_BUILD_TYPE STREQUAL "O2")
    Set(HEADERS ${HDRS})
    Set(LINKDEF GPUTrackingLinkDef_O2.h)
    Set(LIBRARY_NAME ${MODULE})
    set(BUCKET_NAME GPUTracking_bucket)

    O2_GENERATE_LIBRARY()
    install(FILES ${HDRS} ${HDRS2} DESTINATION include/AliGPU)
    install(DIRECTORY utils DESTINATION include/AliGPU FILES_MATCHING PATTERN *.h)

    set(TEST_SRCS
      ctest/testGPUTracking.cxx
    )

    O2_GENERATE_TESTS(
      MODULE_LIBRARY_NAME ${LIBRARY_NAME}
      BUCKET_NAME ${BUCKET_NAME}
      TEST_SRCS ${TEST_SRCS}
    )
    if (OpenMP_CXX_FOUND)
        target_link_libraries(${MODULE} OpenMP::OpenMP_CXX)
    endif()
    if (GPUCA_EVENT_DISPLAY)
        target_link_libraries(${MODULE} -lGL -lGLU -lGLEW)
    endif()
endif()
