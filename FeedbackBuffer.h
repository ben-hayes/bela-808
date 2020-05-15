// ===========================================================================
// TR-808 Kick Drum Model
// ECS7012P Music and Audio Programming: Final Project
// Author: Ben Hayes
// File: FeedbackBuffer.h
// Description: The feedback buffer filter connected to the TR-808's
//              bridged T network.
// ===========================================================================
#pragma once
#include "IIRAnalogFilter.h"
#include "TR808Components.h"

class FeedbackBuffer : public IIRAnalogFilter
{
public:
    FeedbackBuffer(int sampleRate, TR808Components& components);

    // Update the feedback coefficient:
    void setFeedbackCoefficient(double k) { this->k = k; }
    double process(double x);

protected:
    double k = 1.0;
    
    TR808Components& components;

    void calculateAnalogCoefficients() override;
};
