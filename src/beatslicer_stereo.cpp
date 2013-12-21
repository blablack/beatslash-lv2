#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include <lvtk-1/lvtk/plugin.hpp>

#include "envgen.hpp"
#include "beatslicer_stereo.hpp"

using namespace lvtk;

BeatSlicerStereo::BeatSlicerStereo(double rate)
: Plugin<BeatSlicerStereo>(p_n_ports)
  {
	m_rate = rate;

	// 60 / tempo * rate * sampleSize
	m_sampleFullSize = int(60 * m_rate * 4 / 120 + 0.5);

	m_gate = false;

	m_attack = -1;
	m_release = -1;
	m_sliceSize = -1;
	m_tempo = -1;
	m_sampleSize = -1;
	m_sampleFull = false;
	m_slicing = false;
  }

void BeatSlicerStereo::run(uint32_t nframes)
{
	int tempo = int(*p(p_tempo) + 0.5);
	if(tempo < 40)
		tempo = 40;

	int sampleSize = int(*p(p_sampleSize));
	if(sampleSize < 2)
		sampleSize = 2;

	float sliceSize = *p(p_sliceSize);
	if(sliceSize < 0.03125)
		sliceSize = 0.03125;

	int attack = int(*p(p_attack));
	if(attack < 3)
		attack = 3;

	int release = int(*p(p_release));
	if(release < 3)
		release  = 3;

	attack = m_rate / 1000 * attack;
	release = m_rate / 1000 * release;

	if(sampleSize != m_sampleSize || tempo != m_tempo || sliceSize != m_sliceSize || attack != m_attack || release != m_release)
	{
		m_attack = attack;
		m_release = release;
		m_sampleSize = sampleSize;
		m_tempo = tempo;
		m_sliceSize = sliceSize;

		m_sampleFullSize = int(60 * m_rate * m_sampleSize / m_tempo + 0.5);
		m_sampleL.clear();
		m_sampleR.clear();
		m_sampleFull = false;
		m_readingSampleSize = int(m_sampleFullSize * m_sliceSize + 0.5);

		m_envelope = gen_full_envelope(m_readingSampleSize, m_attack, m_release);
		m_fadeIn = gen_attack(m_attack);
	}

	for (unsigned int n = 0; n < nframes; n++)
	{

		if (!m_gate && (p(p_gate)[n] > 0.5))
		{
			m_gate = true;
			if(m_sampleFull)
			{
				m_readingSampleL = m_sampleL;
				m_readingSampleR = m_sampleR;
			}
			giveMeReverse(int(*p(p_reverseMode)));
			m_readingPosition = 0;
			m_fadePosition = 0;
			m_fadeOut = gen_release(m_attack);
			m_slicing = true;
		}
		else if(m_gate && (p(p_gate)[n] < 0.5))
		{
			m_gate = false;
			m_fadePosition = 0;
			m_fadeOut = gen_release(m_envelope[m_readingPosition], m_attack);
		}

		m_sampleL.push_back(float(p(p_inputL)[n]));
		m_sampleR.push_back(float(p(p_inputR)[n]));
		while(m_sampleL.size() > m_sampleFullSize)
		{
			m_sampleL.pop_front();
			m_sampleR.pop_front();
			m_sampleFull = true;
		}

		if(m_gate && m_sampleFull)
		{
			if(!m_reverse)
				m_calculatedReadingPosition = m_positionStart + m_readingPosition;
			else
				m_calculatedReadingPosition = m_positionStart - m_readingPosition;

			if(m_fadePosition < m_attack)
			{
				p(p_outputL)[n] = m_readingSampleL[m_calculatedReadingPosition] * m_envelope[m_readingPosition] + p(p_inputL)[n] * m_fadeOut[m_fadePosition];
				p(p_outputR)[n] = m_readingSampleR[m_calculatedReadingPosition] * m_envelope[m_readingPosition] + p(p_inputR)[n] * m_fadeOut[m_fadePosition];
				m_fadePosition++;
			}
			else
			{
				p(p_outputL)[n] = m_readingSampleL[m_calculatedReadingPosition] * m_envelope[m_readingPosition];
				p(p_outputR)[n] = m_readingSampleR[m_calculatedReadingPosition] * m_envelope[m_readingPosition];
			}

			m_readingPosition++;
			if(m_readingPosition > m_readingSampleSize)
			{
				m_readingPosition = 0;
				giveMeReverse(int(*p(p_reverseMode)));
			}
		}
		else
		{
			if(m_slicing && m_sampleFull)
			{
				if(!m_reverse)
					m_calculatedReadingPosition = m_positionStart + m_readingPosition;
				else
					m_calculatedReadingPosition = m_positionStart - m_readingPosition;

				p(p_outputL)[n] = m_readingSampleL[m_calculatedReadingPosition] * m_fadeOut[m_fadePosition] +  p(p_inputL)[n] * m_fadeIn[m_fadePosition];
				p(p_outputR)[n] = m_readingSampleR[m_calculatedReadingPosition] * m_fadeOut[m_fadePosition] +  p(p_inputR)[n] * m_fadeIn[m_fadePosition];

				m_fadePosition++;
				if(m_fadePosition >= m_attack)
					m_slicing = false;

				m_readingPosition++;
				if(m_readingPosition > m_readingSampleSize)
				{
					m_readingPosition = 0;
					giveMeReverse(int(*p(p_reverseMode)));
				}
			}
			else
			{
				p(p_outputL)[n] = p(p_inputL)[n];
				p(p_outputR)[n] = p(p_inputR)[n];
			}
		}
	}
}

void BeatSlicerStereo::giveMeReverse(int ReverseMode)
{
	switch(ReverseMode)
	{
		case 0:
			m_reverse = false;
			break;
		case 1:
			m_reverse = ((rand() % 2) == 0);
			break;
		case 2:
			m_reverse = true;
			break;
		default:
			m_reverse = false;
			break;
	}
	if(m_reverse)
	{
		m_positionStart = (rand() % (int(1 / *p(p_sliceSize)))) * m_readingSampleSize + m_readingSampleSize - 1;
	}
	else
	{
		m_positionStart = (rand() % (int(1 / *p(p_sliceSize)))) * m_readingSampleSize;
	}
}

static int _ = BeatSlicerStereo::register_class("http://github.com/blablack/beatslash-lv2/beatslicer_stereo");

