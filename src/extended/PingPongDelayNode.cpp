// License: BSD 2 Clause
// Copyright (C) 2015+, The LabSound Authors. All rights reserved.

#include "LabSound/core/AudioNodeInput.h"
#include "LabSound/core/AudioNodeOutput.h"
#include "LabSound/core/AudioProcessor.h"

#include "LabSound/extended/PingPongDelayNode.h"
#include "LabSound/extended/AudioContextLock.h"

#include "internal/AudioBus.h"

#include <algorithm>
#include <WTF/MathExtras.h>

using namespace lab;

namespace lab 
{
    PingPongDelayNode::PingPongDelayNode(float sampleRate, float tempo)
    {
        input = std::make_shared<lab::GainNode>(sampleRate);
        output = std::make_shared<lab::GainNode>(sampleRate);

        leftDelay = std::make_shared<lab::BPMDelay>(sampleRate, tempo);
        rightDelay = std::make_shared<lab::BPMDelay>(sampleRate, tempo);

        splitterGain = std::make_shared<lab::GainNode>(sampleRate);
        wetGain = std::make_shared<lab::GainNode>(sampleRate);
        feedbackGain = std::make_shared<lab::GainNode>(sampleRate);

        merger = std::make_shared<lab::ChannelMergerNode>(sampleRate, 2);
        splitter = std::make_shared<lab::ChannelSplitterNode>(sampleRate, 2);

        SetDelayIndex(TempoSync::TS_8);
        SetFeedback(0.5f);
        SetLevel(0.5f);
    }
    
    PingPongDelayNode::~PingPongDelayNode()
    {

    }

    void PingPongDelayNode::SetTempo(float t)
    { 
        tempo = t; 
        leftDelay->SetTempo(tempo);
        rightDelay->SetTempo(tempo);
    }

    void PingPongDelayNode::SetFeedback(float f) 
    {
        auto clamped = clampTo<float>(f,0.0f, 1.0f);
        feedbackGain->gain()->setValue(clamped);
    }

    void PingPongDelayNode::SetLevel(float f) 
    {
        auto clamped = clampTo<float>(f,0.0f, 1.0f);
        wetGain->gain()->setValue(clamped);
    }

    void PingPongDelayNode::SetDelayIndex(TempoSync value)
    {
        leftDelay->SetDelayIndex(value);
        rightDelay->SetDelayIndex(value);
    }

    void PingPongDelayNode::BuildSubgraph(ContextGraphLock & lock) 
    {
        auto ac = lock.context();

        if (!ac) 
            throw std::invalid_argument("Graph lock could not acquire context");

        // Input into splitter
        //input->connect(ac, splitter.get(), 0, 0);
        ac->connect(splitter, input, 0, 0);

        ac->connect(splitterGain, splitter, 0, 0);
        ac->connect(splitterGain, splitter, 1, 0);

        ac->connect(wetGain, splitterGain, 0, 0);
        splitterGain->gain()->setValue(0.5f);

        ac->connect(leftDelay, wetGain, 0, 0);

        ac->connect(leftDelay, feedbackGain, 0, 0);

        ac->connect(rightDelay, leftDelay, 0, 0);
        ac->connect(feedbackGain, rightDelay, 0, 0);

        ac->connect(merger, leftDelay, 0, 0);
        ac->connect(merger, rightDelay, 0, 1);

        ac->connect(output, merger, 0, 0);

        // Activate with input->output
        ac->connect(output, input, 0, 0);
    }


}
