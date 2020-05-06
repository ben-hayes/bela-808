#include <memory>

#include <Bela.h>
#include <libraries/Scope/Scope.h>

#include "PulseShaper.h"
#include "TriggerCircuit.h"

Scope scope;

std::unique_ptr<TriggerCircuit> triggerCircuit;
std::unique_ptr<PulseShaper> pulseShaper;
PulseShaperComponentValues c;

int counter = 0;

bool setup(BelaContext* context, void* userData)
{
    scope.setup(1, context->audioSampleRate);
    triggerCircuit.reset(new TriggerCircuit(context->audioSampleRate));
    pulseShaper.reset(new PulseShaper(context->audioSampleRate));

    return true;
}

void render(BelaContext* context, void* userData)
{
    for (int n = 0; n < context->audioFrames; n++)
    {
        counter += 1;
        if (counter == context->audioSampleRate)
        {
            triggerCircuit->trigger();
            counter = 0;
        }

        auto out = triggerCircuit->sample();
        out = pulseShaper->process(out);
        scope.log(out);
        for (int channel = 0; channel < context->audioOutChannels; channel++)
        {
            audioWrite(context, n, channel, out);
        }
    }

}

void cleanup(BelaContext* context, void* userData)
{
}
