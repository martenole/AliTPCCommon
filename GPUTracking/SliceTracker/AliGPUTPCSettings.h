#ifndef ALIHLTTPCCASETTINGS_H
#define ALIHLTTPCCASETTINGS_H

#if !defined(GPUCA_STANDALONE) && !defined(GPUCA_ALIROOT_LIB) && !defined(GPUCA_O2_LIB)
#error You are using the CA GPU tracking without defining the build type (O2/AliRoot/Standalone). If you are running ROOT macro, please include AliGPUO2Interface.h first!
#endif

#if defined(GPUCA_ALIROOT_LIB) && defined(GPUCA_O2_LIB)
#error Invalid Compile Definitions!
#endif

#define EXTERN_ROW_HITS
#define TRACKLET_SELECTOR_MIN_HITS(QPT) (CAMath::Abs(QPT) > 10 ? 10 : (CAMath::Abs(QPT) > 5 ? 15 : 29)) //Minimum hits should depend on Pt, low Pt tracks can have few hits. 29 Hits default, 15 for < 200 mev, 10 for < 100 mev

#define GLOBAL_TRACKING_RANGE 45					//Number of rows from the upped/lower limit to search for global track candidates in for
#define GLOBAL_TRACKING_Y_RANGE_UPPER_LEFT 0.85		//Inner portion of y-range in slice that is not used in searching for global track candidates
#define GLOBAL_TRACKING_Y_RANGE_LOWER_LEFT 0.85
#define GLOBAL_TRACKING_Y_RANGE_UPPER_RIGHT 0.85
#define GLOBAL_TRACKING_Y_RANGE_LOWER_RIGHT 0.85
#define GLOBAL_TRACKING_MIN_ROWS 10					//Min num of rows an additional global track must span over
#define GLOBAL_TRACKING_MIN_HITS 8					//Min num of hits for an additional global track

//#define MERGE_CE_ROWLIMIT 15						//Distance from first / last row in order to attempt merging accross CE

#define MERGE_LOOPER_QPT_LIMIT 4					//Min Q/Pt to run special looper merging procedure
#define MERGE_HORIZONTAL_DOUBLE_QPT_LIMIT 2			//Min Q/Pt to attempt second horizontal merge between slices after a vertical merge was found

#define GPUCA_Y_FACTOR 4							//Weight of y residual vs z residual in tracklet constructor
#define GPUCA_MAXN 40							//Maximum number of neighbor hits to consider in one row in neightbors finder
#define TRACKLET_CONSTRUCTOR_MAX_ROW_GAP 4			//Maximum number of consecutive rows without hit in track following
#define TRACKLET_CONSTRUCTOR_MAX_ROW_GAP_SEED 2		//Same, but during fit of seed
#define GPUCA_MERGER_MAXN_MISSED_HARD 10			//Hard limit for number of missed rows in fit / propagation
#define GPUCA_MERGER_COV_LIMIT 1000                 //Abort fit when y/z cov exceed the limit
#define MIN_TRACK_PT_DEFAULT 0.010					//Default setting for minimum track Pt at some places

#define MAX_SLICE_NTRACK (2 << 24)					//Maximum number of tracks per slice (limited by track id format)

#define GPUCA_TIMING_SUM 1

#define GPUCA_MAX_SIN_PHI_LOW 0.99f						//Must be preprocessor define because c++ pre 11 cannot use static constexpr for initializes
#define GPUCA_MAX_SIN_PHI 0.999f

#ifdef GPUCA_TPC_GEOMETRY_O2
#define GPUCA_ROW_COUNT 152
#else
#define GPUCA_ROW_COUNT 159
#endif
#define GPUCA_NSLICES 36

//#define GPUCA_MERGER_BY_MC_LABEL
#define REPRODUCIBLE_CLUSTER_SORTING

#ifdef GPUCA_O2_LIB
typedef unsigned int calink;
typedef unsigned int cahit;
#else
typedef unsigned int calink;
typedef unsigned int cahit;
#endif

#ifdef GPUCA_GPUCODE
#define ALIHLTTPCCANEIGHBOURS_FINDER_MAX_NNEIGHUP 6
#else
#define ALIHLTTPCCANEIGHBOURS_FINDER_MAX_NNEIGHUP GPUCA_MAXN
#endif //GPUCA_GPUCODE

//#define GPUCA_FULL_CLUSTERDATA						//Store all cluster information in the cluster data, also those not needed for tracking.
//#define GMPropagatePadRowTime							//Propagate Pad, Row, Time cluster information to GM
//#define GMPropagatorUseFullField						//Use offline magnetic field during GMPropagator prolongation
//#define GPUseStatError								//Use statistical errors from offline in track fit

#endif
