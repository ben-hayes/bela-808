#include <memory>

#include <Bela.h>
#include <libraries/Gui/Gui.h>
#include <libraries/GuiController/GuiController.h>
#include <libraries/Scope/Scope.h>

#include "BridgedTNetwork.h"
#include "EnvelopeGenerator.h"
#include "PulseShaper.h"
#include "RetriggeringPulse.h"
#include "ToneControl.h"
#include "TriggerCircuit.h"

Scope scope;
Gui gui;
GuiController guiController;

// Component struct:
std::unique_ptr<TR808Components> c;

// Units of the 808 kick model:
std::unique_ptr<TriggerCircuit> triggerCircuit;
std::unique_ptr<PulseShaper> pulseShaper;
std::unique_ptr<EnvelopeGenerator> envelopeGenerator;
std::unique_ptr<RetriggeringPulse> retriggeringPulse;
std::unique_ptr<BridgedTNetwork> bridgedTNetwork;
std::unique_ptr<ToneControl> toneControl;


double feedbackPot = 0.9999;
double tonePot = 0.7;
double levelPot = 0.8;

int samplesElapsedSinceTrigger = 0;
int triggerInterval = 44100;
int triggerVoltage = 4;

bool setup(BelaContext* context, void* userData)
{
    // Create GUI sliders:
    gui.setup(context->projectName);
    guiController.setup(&gui, "Controller");
    guiController.addSlider("Decay", 0.6, 0.0, 1.0, 0.01);
    guiController.addSlider("Tone", 0.6, 0.0, 1.0, 0.01);
    guiController.addSlider("Level", 0.6, 0.0, 1.0, 0.01);
    guiController.addSlider("Trigger Interval (s)", 1.0, 0.1, 3.0, 0.01);
    guiController.addSlider("Trigger Accent (V)", 4.0, 4.0, 14.0, 1.0);

    // Initialise scope:
    scope.setup(5, context->audioSampleRate);

    // Initialise components struct:
    c.reset(new TR808Components());

    // Initialise all model units:
    triggerCircuit.reset(new TriggerCircuit(context->audioSampleRate));
    pulseShaper.reset(new PulseShaper(context->audioSampleRate, *c));
    envelopeGenerator.reset(new EnvelopeGenerator(context->audioSampleRate));
    retriggeringPulse.reset(new RetriggeringPulse(context->audioSampleRate, *c));
    bridgedTNetwork.reset(new BridgedTNetwork(context->audioSampleRate, *c));
    toneControl.reset(new ToneControl(context->audioSampleRate, *c));

    return true;
}

void render(BelaContext* context, void* userData)
{
    // Pull our control values from our GUI sliders. No need to do this more
    // than once per block. Smoothing could be implemented here in future to
    // deal with some of the audio clicks when changing values, but for now
    // this is fine:
    feedbackPot = pow(guiController.getSliderValue(0), 0.5);
    tonePot = 1.0 - guiController.getSliderValue(1);
    levelPot = 1.0 - guiController.getSliderValue(2);
    triggerInterval = context->audioSampleRate * guiController.getSliderValue(3);
    triggerVoltage = guiController.getSliderValue(4);

    for (int n = 0; n < context->audioFrames; n++)
    {
        // Repeating trigger:
        samplesElapsedSinceTrigger += 1;
        if (samplesElapsedSinceTrigger >= triggerInterval)
        {
            triggerCircuit->trigger(triggerVoltage);
            samplesElapsedSinceTrigger = 0;
        }


        // Create trigger signal
        auto trig = triggerCircuit->sample();
        // Generate shaped pulse from trigger signal
        auto pulse = pulseShaper->process(trig);

        // Also generate pitch envelope from trigger signal
        auto env = envelopeGenerator->process(trig);
        // And retriggering pulse:
        auto rePulse = retriggeringPulse->process(env);

        // Pass all inputs through bridged T network
        auto bridgedTOut = bridgedTNetwork->process(pulse, rePulse, feedbackPot);

        // Run generated signal through tone control
        auto out = toneControl->process(bridgedTOut, tonePot, levelPot);

        // Plot interesting values on scope
        scope.log(trig, env, rePulse, pulse, out);

        for (int channel = 0; channel < context->audioOutChannels; channel++)
        {
            audioWrite(context, n, channel, out);
        }
    }

}

void cleanup(BelaContext* context, void* userData)
{
}
