#ifndef BEATSLICER_MONO_H
#define BEATSLICER_MONO_H

#include <lvtk-2/lvtk/plugin.hpp>

#include "beatrepeater_mono_ttl.hpp"

using namespace lvtk;
using namespace std;

class BeatRepeaterMono: public Plugin<BeatRepeaterMono>
{
	private:
		double m_rate;

		float* m_envelope;
		float* m_envelopeRec;
		float* m_attackEnv;
		float* m_releaseEnv;

		float* m_sample;
		int m_sampleFullSize;
		bool m_sampleFull;

		bool m_gate;
		bool m_repeating;

		int m_repeatingPosition;
		int m_fadePosition;

		int m_tempo;
		float m_beatSize;

		int m_attack, m_release;

	public:
		BeatRepeaterMono(double rate);
		void run(uint32_t nframes);
};

#endif
