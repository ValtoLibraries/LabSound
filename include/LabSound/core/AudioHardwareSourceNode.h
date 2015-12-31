/*
 * Copyright (C) 2012, Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1.  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef AudioHardwareSourceNode_h
#define AudioHardwareSourceNode_h

#include "LabSound/core/AudioSourceNode.h"
#include "LabSound/core/AudioSourceProvider.h"

namespace lab {

class AudioContext;
    
class AudioHardwareSourceNode : public AudioSourceNode, public AudioSourceProviderClient
{

public:

    AudioHardwareSourceNode(AudioSourceProvider*, float sampleRate);
    virtual ~AudioHardwareSourceNode();

    // AudioNode
    virtual void process(ContextRenderLock&, size_t framesToProcess) override;
    virtual void reset(ContextRenderLock&) override;

    // AudioSourceProviderClient
    virtual void setFormat(ContextRenderLock & r, size_t numberOfChannels, float sampleRate) override;

    AudioSourceProvider * audioSourceProvider() const { return m_audioSourceProvider; }

private:

    // As an audio source, we will never propagate silence.
    virtual bool propagatesSilence(double now) const override { return false; }

    AudioSourceProvider * m_audioSourceProvider;

    unsigned m_sourceNumberOfChannels;
};

} // namespace lab

#endif