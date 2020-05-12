#include <memory>

#include <Bela.h>
#include <libraries/Scope/Scope.h>

#include "BridgedTNetwork.h"
#include "EnvelopeGenerator.h"
#include "FeedbackBuffer.h"
#include "PulseShaper.h"
#include "RetriggeringPulse.h"
#include "ToneControl.h"
#include "TriggerCircuit.h"

Scope scope;

std::unique_ptr<TriggerCircuit> triggerCircuit;
std::unique_ptr<PulseShaper> pulseShaper;
std::unique_ptr<EnvelopeGenerator> envelopeGenerator;
std::unique_ptr<RetriggeringPulse> retriggeringPulse;
std::unique_ptr<BridgedTNetwork> bridgedTNetwork;
std::unique_ptr<FeedbackBuffer> feedbackBuffer;
std::unique_ptr<ToneControl> toneControl;

std::unique_ptr<TR808Components> c;

double lastFb = 1.0;
double feedbackPot = 0.9999;
double tonePot = 0.7;
double levelPot = 0.8;

int counter = 20000;

bool setup(BelaContext* context, void* userData)
{
    scope.setup(6, context->audioSampleRate);

    c.reset(new TR808Components());

    triggerCircuit.reset(new TriggerCircuit(context->audioSampleRate));
    pulseShaper.reset(new PulseShaper(context->audioSampleRate, *c));
    envelopeGenerator.reset(new EnvelopeGenerator(context->audioSampleRate));
    retriggeringPulse.reset(new RetriggeringPulse(context->audioSampleRate, *c));
    bridgedTNetwork.reset(new BridgedTNetwork(context->audioSampleRate, *c));
    feedbackBuffer.reset(new FeedbackBuffer(context->audioSampleRate, *c));
    toneControl.reset(new ToneControl(context->audioSampleRate, *c));


    return true;
}

void render(BelaContext* context, void* userData)
{
    for (int n = 0; n < context->audioFrames; n++)
    {
        counter += 1;
        if (counter == 44100)
        {
            triggerCircuit->trigger(5);
            counter = 0;
        }

        auto trig = triggerCircuit->sample();
        auto env = envelopeGenerator->process(trig);
        auto rePulse = retriggeringPulse->process(env);
        auto pulse = pulseShaper->process(trig);
        auto bridgedTOut = bridgedTNetwork->process(pulse, rePulse, lastFb);
        lastFb = feedbackBuffer->process(bridgedTOut, feedbackPot);
        auto out = toneControl->process(bridgedTOut, tonePot, levelPot);

        scope.log(trig, env, rePulse, pulse, out, lastFb);

        for (int channel = 0; channel < context->audioOutChannels; channel++)
        {
            audioWrite(context, n, channel, out * 3.33);
        }
    }

}

void cleanup(BelaContext* context, void* userData)
{
}
