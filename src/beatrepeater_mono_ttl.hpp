#ifndef avw_lv__beatrepeater_mono_ttl_hpp
#define avw_lv__beatrepeater_mono_ttl_hpp


#ifndef PEG_STRUCT
#define PEG_STRUCT
typedef struct {
  float min;
  float max;
  float default_value;
  char toggled;
  char integer;
  char logarithmic;
} peg_data_t;
#endif

/* <http://github.com/blablack/beatslash.lv2/beatrepeater_mono> */

static const char p_uri[] = "http://github.com/blablack/beatslash.lv2/beatrepeater_mono";

enum p_port_enum {
  p_input,
  p_gate,
  p_tempo,
  p_beatSize,
  p_attack,
  p_release,
  p_reverse,
  p_output,
  p_n_ports
};

static const peg_data_t p_ports[] = {
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { 0, 1, 0, 0, 0, 0 },
  { 40, 320, 120, 0, 0, 0 },
  { 0.03125, 32, 1, 0, 0, 0 },
  { 3, 25, 7, 0, 0, 0 },
  { 3, 25, 7, 0, 0, 0 },
  { 0, 1, 0, 1, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
};


#endif /* avw_lv__beatrepeater_mono_ttl_hpp */
