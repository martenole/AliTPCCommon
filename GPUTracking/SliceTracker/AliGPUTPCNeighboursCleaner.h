//-*- Mode: C++ -*-
// ************************************************************************
// This file is property of and copyright by the ALICE HLT Project        *
// ALICE Experiment at CERN, All rights reserved.                         *
// See cxx source for full Copyright notice                               *
//                                                                        *
//*************************************************************************

#ifndef ALIHLTTPCCANEIGHBOURSCLEANER_H
#define ALIHLTTPCCANEIGHBOURSCLEANER_H

#include "AliGPUTPCDef.h"
#include "AliGPUGeneralKernels.h"
#include "AliGPUConstantMem.h"

MEM_CLASS_PRE()
class AliGPUTPCTracker;

/**
 * @class AliGPUTPCNeighboursCleaner
 *
 */
class AliGPUTPCNeighboursCleaner
{
public:
	MEM_CLASS_PRE()
	class AliGPUTPCSharedMemory
	{
		friend class AliGPUTPCNeighboursCleaner;

	  public:
#if !defined(GPUCA_GPUCODE)
		AliGPUTPCSharedMemory()
		    : fIRow(0), fIRowUp(0), fIRowDn(0), fNHits(0)
		{
		}
		AliGPUTPCSharedMemory(const AliGPUTPCSharedMemory & /*dummy*/)
		    : fIRow(0), fIRowUp(0), fIRowDn(0), fNHits(0) {}
		AliGPUTPCSharedMemory &operator=(const AliGPUTPCSharedMemory & /*dummy*/) { return *this; }
#endif //!GPUCA_GPUCODE

	  protected:
		int fIRow;   // current row index
		int fIRowUp; // current row index
		int fIRowDn; // current row index
		int fNHits;  // number of hits
	};
    
	typedef GPUconstantref() MEM_CONSTANT(AliGPUTPCTracker) workerType;
	GPUhdi() static AliGPUDataTypes::RecoStep GetRecoStep() {return GPUCA_RECO_STEP::TPCSliceTracking;}
	MEM_TEMPLATE() GPUhdi() static workerType* Worker(MEM_TYPE(AliGPUConstantMem) &workers) {return workers.tpcTrackers;}
	template <int iKernel = 0> GPUd() static void Thread(int /*nBlocks*/, int nThreads, int iBlock, int iThread, GPUsharedref() MEM_LOCAL(AliGPUTPCSharedMemory) &smem, workerType &tracker);
};

#endif //ALIHLTTPCCANEIGHBOURSCLEANER_H
