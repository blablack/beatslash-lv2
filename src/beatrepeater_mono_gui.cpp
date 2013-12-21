#include <gtkmm-2.4/gtkmm.h>


#include <lvtk-1/lvtk/plugin.hpp>
#include <lvtk-1/lvtk/gtkui.hpp>

#include "beatrepeater_mono_gui.hpp"
#include "beatrepeater_mono.hpp"
#include "dial.hpp"

BeatRepeaterMonoGUI::BeatRepeaterMonoGUI(const std::string& URI)
{
	EventBox *p_background = manage (new EventBox());
	Gdk::Color* color = new  Gdk::Color();
	color->set_rgb(7710, 8738, 9252);
	p_background->modify_bg(Gtk::STATE_NORMAL, *color);



	VBox *p_mainWidget = manage (new VBox(false, 10));



	Frame *p_beatFrame = manage(new Frame("Beat"));
	VBox *p_beatBox = manage(new VBox(false, 5));
	HBox *p_beatDials = manage(new HBox(true));

	slot<void> p_slotBeatSize = compose(bind<0>(mem_fun(*this, &BeatRepeaterMonoGUI::write_control), p_beatSize), mem_fun(*this,  &BeatRepeaterMonoGUI::get_beatSize));
	m_dialBeatSize = new LabeledDial("Beat Size", p_slotBeatSize, p_beatSize, 0.03125, 32, NORMAL, 0.03125, 5);
	p_beatDials->pack_start(*m_dialBeatSize);

	slot<void> p_slotTempo = compose(bind<0>(mem_fun(*this, &BeatRepeaterMonoGUI::write_control), p_tempo), mem_fun(*this,  &BeatRepeaterMonoGUI::get_tempo));
	m_dialTempo = new LabeledDial("Tempo", p_slotTempo, p_tempo, 40, 320, NORMAL, 1, 0);
	p_beatDials->pack_start(*m_dialTempo);

	p_beatBox->pack_start(*p_beatDials);

	m_checkReverse = manage(new CheckButton("Reverse"));
	slot<void> p_slotReverse= compose(bind<0>(mem_fun(*this, &BeatRepeaterMonoGUI::write_control), p_reverse), mem_fun(*m_checkReverse, &CheckButton::get_active));
	m_checkReverse->signal_toggled().connect(p_slotReverse);
	p_beatBox->pack_start(*m_checkReverse);

	p_beatFrame->add(*p_beatBox);
	p_mainWidget->pack_start(*p_beatFrame);



	Frame *p_envelopeFrame = manage(new Frame("Envelope"));
	HBox *p_envelopeDials = manage(new HBox(true));

	slot<void> p_slotAttack = compose(bind<0>(mem_fun(*this, &BeatRepeaterMonoGUI::write_control), p_attack), mem_fun(*this,  &BeatRepeaterMonoGUI::get_attack));
	m_dialAttack = new LabeledDial("Attack", p_slotAttack, p_attack, 3, 25, NORMAL, 1, 0);
	p_envelopeDials->pack_start(*m_dialAttack);

	slot<void> p_slotRelease = compose(bind<0>(mem_fun(*this, &BeatRepeaterMonoGUI::write_control), p_release), mem_fun(*this,  &BeatRepeaterMonoGUI::get_release));
	m_dialRelease = new LabeledDial("Release", p_slotRelease, p_release, 3, 25, NORMAL, 1, 0);
	p_envelopeDials->pack_start(*m_dialRelease);

	p_envelopeFrame->add(*p_envelopeDials);
	p_mainWidget->pack_start(*p_envelopeFrame);



	p_mainWidget->set_size_request(160, 260);

	p_background->add(*p_mainWidget);
	add(*p_background);

	Gtk::manage(p_mainWidget);
}

float BeatRepeaterMonoGUI::get_beatSize() 	{ return m_dialBeatSize->get_value(); }
float BeatRepeaterMonoGUI::get_tempo()		{ return m_dialTempo->get_value(); }
float BeatRepeaterMonoGUI::get_attack()		{ return m_dialAttack->get_value(); }
float BeatRepeaterMonoGUI::get_release()	{ return m_dialRelease->get_value(); }

void BeatRepeaterMonoGUI::port_event(uint32_t port, uint32_t buffer_size, uint32_t format, const void* buffer)
{
	if (port == p_reverse)
	{
		m_checkReverse->set_active(*static_cast<const float*> (buffer) == 1);
	}
	else if (port == p_tempo)
	{
		m_dialTempo->set_value(*static_cast<const float*> (buffer));
	}
	else if (port == p_beatSize)
	{
		m_dialBeatSize->set_value(*static_cast<const float*> (buffer));
	}
	else if (port == p_attack)
	{
		m_dialAttack->set_value(*static_cast<const float*> (buffer));
	}
	else if (port == p_release)
	{
		m_dialRelease->set_value(*static_cast<const float*> (buffer));
	}
}

static int _ = BeatRepeaterMonoGUI::register_class("http://github.com/blablack/beatslash-lv2/beatrepeater_mono/gui");
