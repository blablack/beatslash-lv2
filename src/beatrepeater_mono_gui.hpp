#ifndef BEATREPEATER_MONO_GUI_H
#define BEATREPEATER_MONO_GUI_H

#include <lvtk-1/lvtk/plugin.hpp>
#include <lvtk-1/lvtk/gtkui.hpp>

#include "labeleddial.hpp"

using namespace lvtk;
using namespace sigc;
using namespace Gtk;

class BeatRepeaterMonoGUI: public UI<BeatRepeaterMonoGUI, GtkUI<true>>
{
	public:
		BeatRepeaterMonoGUI(const std::string& URI);
		void port_event(uint32_t port, uint32_t buffer_size, uint32_t format, const void* buffer);

	protected:
		Gtk::CheckButton* m_checkReverse;
		LabeledDial* m_dialBeatSize;
		LabeledDial* m_dialTempo;
		LabeledDial* m_dialAttack;
		LabeledDial* m_dialRelease;


		float get_beatSize();
		float get_tempo();
		float get_attack();
		float get_release();
};

#endif
