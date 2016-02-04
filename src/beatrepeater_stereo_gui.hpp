#ifndef BEATREPEATER_STEREO_GUI_H
#define BEATREPEATER_STEREO_GUI_H

#include <lvtk-2/lvtk/plugin.hpp>
#include <lvtk-2/lvtk/gtkui.hpp>

#include "labeleddial.hpp"

using namespace lvtk;
using namespace sigc;
using namespace Gtk;

class BeatRepeaterStereoGUI: public UI<BeatRepeaterStereoGUI, GtkUI<true>>
{
	public:
		BeatRepeaterStereoGUI(const std::string& URI);
		void port_event(uint32_t port, uint32_t buffer_size, uint32_t format, const void* buffer);

	protected:
		Gtk::CheckButton* m_checkReverse;
		LabeledDial* m_dialBeatSize;
		LabeledDial* m_dialTempo;
		LabeledDial* m_dialAttack;
		LabeledDial* m_dialRelease;
};

#endif
