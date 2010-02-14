//-*- Mode: C++ -*-
// ************************************************************************
// This file is property of and copyright by the ALICE HLT Project        *
// ALICE Experiment at CERN, All rights reserved.                         *
// See cxx source for full Copyright notice                               *
//                                                                        *
//*************************************************************************


#ifndef ALIHLTTPCCASLICEOUTPUT_H
#define ALIHLTTPCCASLICEOUTPUT_H

#include "AliHLTTPCCADef.h"
#include <cstdlib>
#include "AliHLTTPCCASliceTrack.h"
#include "AliHLTTPCCACompressedInputData.h"
//Obsolete
#include "AliHLTTPCCAOutTrack.h"

/**
 * @class AliHLTTPCCASliceOutput
 *
 * AliHLTTPCCASliceOutput class is used to store the output of AliHLTTPCCATracker{Component}
 * and transport the output to AliHLTTPCCAGBMerger{Component}
 *
 * The class contains all the necessary information about TPC tracks, reconstructed in one slice.
 * This includes the reconstructed track parameters and some compressed information
 * about the assigned clusters: clusterId, position and amplitude.
 *
 */
class AliHLTTPCCASliceOutput
{
  public:

	struct outputControlStruct
	{
		outputControlStruct() : fObsoleteOutput( 1 ), fDefaultOutput( 1 ), fOutputPtr( NULL ), fOutputMaxSize ( 0 ), fEndOfSpace(0) {}
		int fObsoleteOutput;	//Enable Obsolete Output
		int fDefaultOutput;		//Enable Default Output
		char* fOutputPtr;		//Pointer to Output Space, NULL to allocate output space
	        int fOutputMaxSize;		//Max Size of Output Data if Pointer to output space is given
   	        bool fEndOfSpace; // end of space flag 
	};

    GPUhd() int NTracks()                    const { return fNTracks;              }
    GPUhd() int NTrackClusters()             const { return fNTrackClusters;       }

    GPUhd() const AliHLTTPCCASliceTrack &Track( int i ) const { return TracksP()[i]; }
    GPUhd() unsigned char   ClusterRow     ( int i )  const { return ClusterRowP()[i]; }
    GPUhd()  int   ClusterId     ( int i )  const { return ClusterIdP()[i]; }
    GPUhd() const AliHLTTPCCACompressedCluster &ClusterPackedXYZ ( int i )  const { return ClusterPackedXYZP()[i]; }

    GPUhd() static int EstimateSize( int nOfTracks, int nOfTrackClusters );
    void SetPointers(int nTracks = -1, int nTrackClusters = -1, const outputControlStruct* outputControl = NULL);
	static void Allocate(AliHLTTPCCASliceOutput* &ptrOutput, int nTracks, int nTrackHits, outputControlStruct* outputControl);

    GPUhd() void SetNTracks       ( int v )  { fNTracks = v;        }
    GPUhd() void SetNTrackClusters( int v )  { fNTrackClusters = v; }

    GPUhd() void SetTrack( int i, const AliHLTTPCCASliceTrack &v ) {  TracksP()[i] = v; }
    GPUhd() void SetClusterRow( int i, unsigned char v ) {  ClusterRowP()[i] = v; }
    GPUhd() void SetClusterId( int i, int v ) {  ClusterIdP()[i] = v; }
    GPUhd() void SetClusterPackedXYZ( int i, AliHLTTPCCACompressedCluster v ) {  ClusterPackedXYZP()[i] = v; }

    GPUhd() size_t OutputMemorySize() const { return(fMemorySize); }

	//Obsolete Output

    GPUhd()  int NOutTracks() const { return(fNOutTracks); }
	GPUhd()  void SetNOutTracks(int val) { fNOutTracks = val; }
    GPUhd()  AliHLTTPCCAOutTrack *OutTracks() const { return  fOutTracks; }
    GPUhd()  const AliHLTTPCCAOutTrack &OutTrack( int index ) const { return fOutTracks[index]; }
    GPUhd()  int NOutTrackHits() const { return  fNOutTrackHits; }
	GPUhd()  void SetNOutTrackHits(int val) { fNOutTrackHits = val; }
    GPUhd()  void SetOutTrackHit(int n, int val) { fOutTrackHits[n] = val; }
    GPUhd()  int OutTrackHit( int i ) const { return  fOutTrackHits[i]; }

  private:
    AliHLTTPCCASliceOutput()
        : fNTracks( 0 ), fNTrackClusters( 0 ), fTracksOffset( 0 ),  fClusterIdOffset( 0 ), fClusterRowOffset( 0 ), fClusterPackedXYZOffset( 0 ),
		fMemorySize( 0 ), fNOutTracks(0), fNOutTrackHits(0), fOutTracks(0), fOutTrackHits(0) {}

	~AliHLTTPCCASliceOutput() {}
    const AliHLTTPCCASliceOutput& operator=( const AliHLTTPCCASliceOutput& ) const { return *this; }
    AliHLTTPCCASliceOutput( const AliHLTTPCCASliceOutput& );

	GPUh() void SetMemorySize(size_t val) { fMemorySize = val; }

  GPUh() AliHLTTPCCASliceTrack *TracksP() const { return (AliHLTTPCCASliceTrack*)(fMemory+fTracksOffset); }
  GPUh() int *ClusterIdP() const { return (int*)(fMemory+fClusterIdOffset); }
  GPUh() UChar_t *ClusterRowP() const { return (UChar_t *)(fMemory+fClusterRowOffset); }
  GPUh() AliHLTTPCCACompressedCluster *ClusterPackedXYZP() const { return (AliHLTTPCCACompressedCluster *)(fMemory+fClusterPackedXYZOffset); }

    int fNTracks;                   // number of reconstructed tracks
    int fNTrackClusters;            // total number of track clusters
    int fTracksOffset; // pointer to reconstructed tracks
    int fClusterIdOffset;              // pointer to cluster Id's ( packed slice, patch, cluster )
    int fClusterRowOffset;     // pointer to cluster row numbers
    int fClusterPackedXYZOffset;    // pointer to cluster coordinates 
	size_t fMemorySize;				// Amount of memory really used

    // obsolete output

	int fNOutTracks;
	int fNOutTrackHits;
    AliHLTTPCCAOutTrack *fOutTracks; // output array of the reconstructed tracks
    int *fOutTrackHits;  // output array of ID's of the reconstructed hits

	//Must be last element of this class, user has to make sure to allocate anough memory consecutive to class memory!
	//This way the whole Slice Output is one consecutive Memory Segment
    char fMemory[1]; // the memory where the pointers above point into

};

#endif //ALIHLTTPCCASLICEOUTPUT_H
