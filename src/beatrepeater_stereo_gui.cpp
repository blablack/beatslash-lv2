#include <gtkmm-2.4/gtkmm.h>


#include <lvtk-1/lvtk/plugin.hpp>
#include <lvtk-1/lvtk/gtkui.hpp>

#include "beatrepeater_stereo_gui.hpp"
#include "beatrepeater_stereo.hpp"
#include "dial.hpp"

BeatRepeaterStereoGUI::BeatRepeaterStereoGUI(const std::string& URI)
{
	EventBox *p_background = manage (new EventBox());
	Gdk::Color* color = new  Gdk::Color();
	color->set_rgb(7710, 8738, 9252);
	p_background->modify_bg(Gtk::STATE_NORMAL, *color);



	VBox *p_mainWidget = manage (new VBox(false, 10));



	Frame *p_beatFrame = manage(new Frame("Beat"));
	VBox *p_beatBox = manage(new VBox(false, 5));
	HBox *p_beatDials = manage(new HBox(true));

	m_dialBeatSize = new LabeledDial("Beat Size", p_beatSize, 0.03125, 32, NORMAL, 0.03125, 5);
    m_dialBeatSize->signal_value_changed().connect(compose(bind<0>(mem_fun(*this, &BeatRepeaterStereoGUI::write_control), p_beatSize), mem_fun(*m_dialBeatSize,  &LabeledDial::get_value)));
    p_beatDials->pack_start(*m_dialBeatSize);

    m_dialTempo = new LabeledDial("Tempo", p_tempo, 40, 320, NORMAL, 1, 0);
    m_dialTempo->signal_value_changed().connect(compose(bind<0>(mem_fun(*this, &BeatRepeaterStereoGUI::write_control), p_tempo), mem_fun(*m_dialTempo,  &LabeledDial::get_value)));
    p_beatDials->pack_start(*m_dialTempo);

    p_beatBox->pack_start(*p_beatDials);

    m_checkReverse = manage(new CheckButton("Reverse"));
    m_checkReverse->signal_toggled().connect(compose(bind<0>(mem_fun(*this, &BeatRepeaterStereoGUI::write_control), p_reverse), mem_fun(*m_checkReverse, &CheckButton::get_active)));
    p_beatBox->pack_start(*m_checkReverse);

    p_beatFrame->add(*p_beatBox);
    p_mainWidget->pack_start(*p_beatFrame);



    Frame *p_envelopeFrame = manage(new Frame("Envelope"));
    HBox *p_envelopeDials = manage(new HBox(true));

    m_dialAttack = new LabeledDial("Attack", p_attack, 3, 25, NORMAL, 1, 0);
    m_dialAttack->signal_value_changed().connect(compose(bind<0>(mem_fun(*this, &BeatRepeaterStereoGUI::write_control), p_attack), mem_fun(*m_dialAttack,  &LabeledDial::get_value)));
    p_envelopeDials->pack_start(*m_dialAttack);

    m_dialRelease = new LabeledDial("Release", p_release, 3, 25, NORMAL, 1, 0);
    m_dialRelease->signal_value_changed().connect(compose(bind<0>(mem_fun(*this, &BeatRepeaterStereoGUI::write_control), p_release), mem_fun(*m_dialRelease,  &LabeledDial::get_value)));
    p_envelopeDials->pack_start(*m_dialRelease);

	p_envelopeFrame->add(*p_envelopeDials);
	p_mainWidget->pack_start(*p_envelopeFrame);



	p_mainWidget->set_size_request(160, 260);

	p_background->add(*p_mainWidget);
	add(*p_background);

	Gtk::manage(p_mainWidget);
}

void BeatRepeaterStereoGUI::port_event(uint32_t port, uint32_t buffer_size, uint32_t format, const void* buffer)
{
	   switch(port)
    {
    case p_reverse:
        m_checkReverse->set_active(*static_cast<const float*> (buffer) == 1);
        break;
    case p_tempo:
        m_dialTempo->set_value(*static_cast<const float*> (buffer));
        break;
    case p_beatSize:
        m_dialBeatSize->set_value(*static_cast<const float*> (buffer));
        break;
    case p_attack:
        m_dialAttack->set_value(*static_cast<const float*> (buffer));
        break;
    case p_release:
        m_dialRelease->set_value(*static_cast<const float*> (buffer));
        break;
    }
}

static int _ = BeatRepeaterStereoGUI::register_class("http://github.com/blablack/beatslash-lv2/beatrepeater_stereo/gui");
