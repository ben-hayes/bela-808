#include <memory>

#include <Bela.h>
#include <libraries/Scope/Scope.h>

#include "BridgedTNetwork.h"
#include "FeedbackBuffer.h"
#include "PulseShaper.h"
#include "TriggerCircuit.h"

Scope scope;

std::unique_ptr<FeedbackBuffer> feedbackBuffer;
std::unique_ptr<BridgedTNetwork> bridgedTNetwork;
std::unique_ptr<BridgedTNetwork2> bridgedTNetwork2;
std::unique_ptr<TriggerCircuit> triggerCircuit;
std::unique_ptr<PulseShaper> pulseShaper;
TR808Components c;

double lastFb = 1.0;

int counter = 20000;

bool setup(BelaContext* context, void* userData)
{
    scope.setup(1, context->audioSampleRate);
    triggerCircuit.reset(new TriggerCircuit(context->audioSampleRate));
    pulseShaper.reset(new PulseShaper(context->audioSampleRate, c));
    bridgedTNetwork.reset(new BridgedTNetwork(context->audioSampleRate, c));
    bridgedTNetwork2.reset(new BridgedTNetwork2(context->audioSampleRate, c));
    feedbackBuffer.reset(new FeedbackBuffer(context->audioSampleRate, c));

    return true;
}

void render(BelaContext* context, void* userData)
{
    for (int n = 0; n < context->audioFrames; n++)
    {
        counter += 1;
        if (counter == 20001)
        {
            triggerCircuit->trigger();
            counter = 0;
        }

        auto out = triggerCircuit->sample();
        out = pulseShaper->process(out);
        out = bridgedTNetwork->process(out);
        out = out * lastFb;
        scope.log(out);
        //lastFb = bridgedTNetwork2->process(feedbackBuffer->process(out, 0.01f));

        for (int channel = 0; channel < context->audioOutChannels; channel++)
        {
        }
    }

}

void cleanup(BelaContext* context, void* userData)
{
}
