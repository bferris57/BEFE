//----------------------------------------------------------------------
// File: FuelGauge.h - Declarations for the FuelGauge Class
//----------------------------------------------------------------------
//
// This class serves as a UI mechnism to tell the user how far we
// are in performing a specified action.
//
// Basically, the guage is instantiated with an estimate count (could
// be 100%, 100Mb, etc.).
//
// Once instantiated, a call to FuelGuage.Update can be called whenever
// something is accomplished.
//
// When it's time to refresh the display (whatever that is), Update()
// will call Refresh().
//
// So, sub-class FuelGauge and write the constructor and Refresh.  It's
// basically that simple.  Then, in your loop, call Update() whenever
// you can but don't call it too often if you're doing lots of stuff
// because it'll just slow your processing down.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef FUELGAUGE_H // FUELGAUGE_H...
#define FUELGAUGE_H

#include "Primitives.h"
#include "Time.h"

namespace BEFE { // Namespace BEFE...

class FuelGauge { // Class FuelGauge...

  public: Long max;
  public: Long count;
  public: Time start;

  public: virtual ~FuelGauge() {
    Finish();
  };

  public: virtual void Init(Long estimatemax) {
    max   = estimatemax;
    count = 0;
    start.Set();
  };

  public: virtual void Finish() {};

  public: virtual void Update(Int curcount) {

    Time now;
    Long diff;

    if (count == curcount) return;
    now.Set();
    diff = now.Delta(start);
    if (diff < 30) return;
    count = curcount;
    Refresh();
  };

  public: virtual void Refresh() {};

}; // ...Class FuelGauge

} // ...Namespace BEFE

#endif // ...FUELGAUGE_H