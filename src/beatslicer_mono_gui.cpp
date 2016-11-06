#include <gtkmm-2.4/gtkmm.h>

#include <lvtk-1/lvtk/plugin.hpp>
#include <lvtk-1/lvtk/gtkui.hpp>

#include "beatslicer_mono_gui.hpp"
#include "beatslicer_mono.hpp"
#include "dial.hpp"
#include "my_box.hpp"

BeatSlicerMonoGUI::BeatSlicerMonoGUI(const std::string& URI)
{
    EventBox *p_background = manage (new EventBox());
    Gdk::Color* color = new  Gdk::Color();
    color->set_rgb(7710, 8738, 9252);
    p_background->modify_bg(Gtk::STATE_NORMAL, *color);


    VBox *p_mainWidget = manage (new VBox(false));


    MyBox *p_beatBox = manage (new MyBox("Beat", Gtk::Orientation::ORIENTATION_HORIZONTAL));

    m_dialTempo = new LabeledDial("Tempo", p_tempo, 40, 320, NORMAL, 1, 0);
    m_dialTempo->signal_value_changed().connect(compose(bind<0>(mem_fun(*this, &BeatSlicerMonoGUI::write_control), p_tempo), mem_fun(*m_dialTempo, &LabeledDial::get_value)));
    p_beatBox->pack_start(*m_dialTempo);

    m_dialSliceSize = new LabeledDial("Slice Size", p_sliceSize, 0.0078125, 128, MULTIPLIER, 0.0078125, 7);
    m_dialSliceSize->signal_value_changed().connect(compose(bind<0>(mem_fun(*this, &BeatSlicerMonoGUI::write_control), p_sliceSize), mem_fun(*m_dialSliceSize, &LabeledDial::get_value)));
    p_beatBox->pack_start(*m_dialSliceSize);

    m_dialSampleSize = new LabeledDial("Sample Size", p_sampleSize, 2, 16, NORMAL, 1, 0);
    m_dialSampleSize->signal_value_changed().connect(compose(bind<0>(mem_fun(*this, &BeatSlicerMonoGUI::write_control), p_sampleSize), mem_fun(*m_dialSampleSize, &LabeledDial::get_value)));
    p_beatBox->pack_start(*m_dialSampleSize);

    p_mainWidget->pack_start(*p_beatBox);


    MyBox *p_revBox = manage (new MyBox("Reverse Mode", Gtk::Orientation::ORIENTATION_HORIZONTAL));

    m_comboReverseMode = manage (new ComboBoxText());
    m_comboReverseMode->append_text("Disabled");
    m_comboReverseMode->append_text("Random Reverse");
    m_comboReverseMode->append_text("Reverse");
    m_comboReverseMode->signal_changed().connect(compose(bind<0> (mem_fun(*this, &BeatSlicerMonoGUI::write_control), p_reverseMode), mem_fun(*m_comboReverseMode, &ComboBoxText::get_active_row_number)));
    p_revBox->pack_start(*m_comboReverseMode);

    p_mainWidget->pack_start(*p_revBox);


    MyBox *p_envelopeFrame = manage(new MyBox("Envelope", Gtk::Orientation::ORIENTATION_HORIZONTAL));

    m_dialAttack = new LabeledDial("Attack", p_attack, 3, 25, NORMAL, 1, 0);
    m_dialAttack->signal_value_changed().connect(compose(bind<0>(mem_fun(*this, &BeatSlicerMonoGUI::write_control), p_attack), mem_fun(*m_dialAttack,  &LabeledDial::get_value)));
    p_envelopeFrame->pack_start(*m_dialAttack);

    m_dialRelease = new LabeledDial("Release", p_release, 3, 25, NORMAL, 1, 0);
    m_dialRelease->signal_value_changed().connect(compose(bind<0>(mem_fun(*this, &BeatSlicerMonoGUI::write_control), p_release), mem_fun(*m_dialRelease,  &LabeledDial::get_value)));
    p_envelopeFrame->pack_start(*m_dialRelease);

    p_mainWidget->pack_start(*p_envelopeFrame);


    p_background->add(*p_mainWidget);
    add(*p_background);

    Gtk::manage(p_mainWidget);
}

void BeatSlicerMonoGUI::port_event(uint32_t port, uint32_t buffer_size, uint32_t format, const void* buffer)
{
    int p_reverseModeValue;

    switch(port)
    {
    case p_reverseMode:
        p_reverseModeValue = (int) (*static_cast<const float*> (buffer));
        if (p_reverseModeValue >= 0 && p_reverseModeValue <= 5)
            m_comboReverseMode->set_active((int) p_reverseModeValue);
        break;
    case p_sliceSize:
        m_dialSliceSize->set_value(*static_cast<const float*> (buffer));
        break;
    case p_tempo:
        m_dialTempo->set_value(*static_cast<const float*> (buffer));
        break;
    case p_sampleSize:
        m_dialSampleSize->set_value(*static_cast<const float*> (buffer));
        break;
    case p_attack:
        m_dialAttack->set_value(*static_cast<const float*> (buffer));
        break;
    case p_release:
        m_dialRelease->set_value(*static_cast<const float*> (buffer));
        break;
    }
}

static int _ = BeatSlicerMonoGUI::register_class("http://github.com/blablack/beatslash-lv2/beatslicer_mono/gui");
