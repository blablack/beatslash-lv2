#include <gtkmm-2.4/gtkmm.h>


#include <lvtk-1/lvtk/plugin.hpp>
#include <lvtk-1/lvtk/gtkui.hpp>

#include "beatslicer_mono_gui.hpp"
#include "beatslicer_mono.hpp"
#include "dial.hpp"

BeatSlicerMonoGUI::BeatSlicerMonoGUI(const std::string& URI)
{
	EventBox *p_background = manage (new EventBox());
	Gdk::Color* color = new  Gdk::Color();
	color->set_rgb(7710, 8738, 9252);
	p_background->modify_bg(Gtk::STATE_NORMAL, *color);



	VBox *p_mainWidget = manage (new VBox(false, 10));



	Frame *p_beatFrame = manage(new Frame("Beat"));
	VBox *p_beatBox = manage(new VBox(false, 5));
	HBox *p_beatDials = manage(new HBox(true));

	slot<void> p_slotTempo = compose(bind<0>(mem_fun(*this, &BeatSlicerMonoGUI::write_control), p_tempo), mem_fun(*this,  &BeatSlicerMonoGUI::get_tempo));
	m_dialTempo = new LabeledDial("Tempo", p_slotTempo, p_tempo, 40, 320, NORMAL, 1, 0);
	p_beatDials->pack_start(*m_dialTempo);

	slot<void> p_slotBeatSize = compose(bind<0>(mem_fun(*this, &BeatSlicerMonoGUI::write_control), p_sliceSize), mem_fun(*this,  &BeatSlicerMonoGUI::get_sliceSize));
	m_dialSliceSize = new LabeledDial("Slice Size", p_slotBeatSize, p_sliceSize, 0.03125, 0.5, NORMAL, 0.03125, 5);
	p_beatDials->pack_start(*m_dialSliceSize);

	slot<void> p_slotSampleSize = compose(bind<0>(mem_fun(*this, &BeatSlicerMonoGUI::write_control), p_sampleSize), mem_fun(*this,  &BeatSlicerMonoGUI::get_sampleSize));
	m_dialSampleSize = new LabeledDial("Sample Size", p_slotSampleSize, p_sampleSize, 2, 16, NORMAL, 1, 0);
	p_beatDials->pack_start(*m_dialSampleSize);

	p_beatBox->pack_start(*p_beatDials);


	Label *p_labelReverseMode = manage (new Label("Reverse Mode"));
	p_beatBox->pack_start(*p_labelReverseMode);

	m_comboReverseMode = manage (new ComboBoxText());
	m_comboReverseMode->append_text("Disabled");
	m_comboReverseMode->append_text("Random Reverse");
	m_comboReverseMode->append_text("Reverse");

	slot<void> p_sloReverseMode = compose(bind<0> (mem_fun(*this, &BeatSlicerMonoGUI::write_control), p_reverseMode), mem_fun(*m_comboReverseMode, &ComboBoxText::get_active_row_number));
	m_comboReverseMode->signal_changed().connect(p_sloReverseMode);
	p_beatBox->pack_start(*m_comboReverseMode);

	p_beatFrame->add(*p_beatBox);
	p_mainWidget->pack_start(*p_beatFrame);



	Frame *p_envelopeFrame = manage(new Frame("Envelope"));
	HBox *p_envelopeDials = manage(new HBox(true));

	slot<void> p_slotAttack = compose(bind<0>(mem_fun(*this, &BeatSlicerMonoGUI::write_control), p_attack), mem_fun(*this,  &BeatSlicerMonoGUI::get_attack));
	m_dialAttack = new LabeledDial("Attack", p_slotAttack, p_attack, 3, 25, NORMAL, 1, 0);
	p_envelopeDials->pack_start(*m_dialAttack);

	slot<void> p_slotRelease = compose(bind<0>(mem_fun(*this, &BeatSlicerMonoGUI::write_control), p_release), mem_fun(*this,  &BeatSlicerMonoGUI::get_release));
	m_dialRelease = new LabeledDial("Release", p_slotRelease, p_release, 3, 25, NORMAL, 1, 0);
	p_envelopeDials->pack_start(*m_dialRelease);

	p_envelopeFrame->add(*p_envelopeDials);
	p_mainWidget->pack_start(*p_envelopeFrame);



	p_mainWidget->set_size_request(200, 300);

	p_background->add(*p_mainWidget);
	add(*p_background);

	Gtk::manage(p_mainWidget);
}

float BeatSlicerMonoGUI::get_tempo() 		{ return m_dialTempo->get_value(); }
float BeatSlicerMonoGUI::get_sliceSize() 	{ return m_dialSliceSize->get_value(); }
float BeatSlicerMonoGUI::get_sampleSize() 	{ return m_dialSampleSize->get_value(); }
float BeatSlicerMonoGUI::get_attack() 		{ return m_dialAttack->get_value(); }
float BeatSlicerMonoGUI::get_release() 		{ return m_dialRelease->get_value(); }

void BeatSlicerMonoGUI::port_event(uint32_t port, uint32_t buffer_size, uint32_t format, const void* buffer)
{
	if (port == p_reverseMode)
	{
		int p_reverseModeValue = (int) (*static_cast<const float*> (buffer));
		if (p_reverseModeValue >= 0 && p_reverseModeValue <= 5)
		{
			m_comboReverseMode->set_active((int) p_reverseModeValue);
		}
	}
	else	if (port == p_sliceSize)
	{
		m_dialSliceSize->set_value(*static_cast<const float*> (buffer));
	}
	else if (port == p_tempo)
	{
		m_dialTempo->set_value(*static_cast<const float*> (buffer));
	}
	else if (port == p_sampleSize)
	{
		m_dialSampleSize->set_value(*static_cast<const float*> (buffer));
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

static int _ = BeatSlicerMonoGUI::register_class("http://github.com/blablack/beatslash-lv2/beatslicer_mono/gui");
