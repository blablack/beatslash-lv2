#ifndef BEATSLICER_MONO_GUI_H
#define BEATSLICER_MONO_GUI_H

#include <lvtk-1/lvtk/plugin.hpp>
#include <lvtk-1/lvtk/gtkui.hpp>

#include "labeleddial.hpp"

using namespace lvtk;
using namespace sigc;
using namespace Gtk;

class BeatSlicerMonoGUI: public UI<BeatSlicerMonoGUI, GtkUI<true>>
{
	public:
		BeatSlicerMonoGUI(const std::string& URI);
		void port_event(uint32_t port, uint32_t buffer_size, uint32_t format, const void* buffer);

	protected:
		LabeledDial* m_dialTempo;
		LabeledDial* m_dialSliceSize;
		LabeledDial* m_dialSampleSize;
		Gtk::ComboBoxText* m_comboReverseMode;
		LabeledDial* m_dialAttack;
		LabeledDial* m_dialRelease;


		float get_tempo();
		float get_sliceSize();
		float get_sampleSize();
		float get_attack();
		float get_release();
};

#endif
