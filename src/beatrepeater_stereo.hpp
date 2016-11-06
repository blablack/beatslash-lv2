#ifndef BEATSLICER_STEREO_H
#define BEATSLICER_STEREO_H

#include <lvtk-1/lvtk/plugin.hpp>

#include "beatrepeater_stereo_ttl.hpp"

using namespace lvtk;
using namespace std;

class BeatRepeaterStereo: public Plugin<BeatRepeaterStereo>
{
	private:
		double m_rate;

		float* m_envelope;
		float* m_envelopeRec;
		float* m_attackEnv;
		float* m_releaseEnv;

		float* m_sampleL;
		float* m_sampleR;
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
		BeatRepeaterStereo(double rate);
		void run(uint32_t nframes);
};

#endif
