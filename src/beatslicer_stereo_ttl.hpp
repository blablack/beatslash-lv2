#ifndef avw_lv__beatslicer_stereo_ttl_hpp
#define avw_lv__beatslicer_stereo_ttl_hpp


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

/* <http://github.com/blablack/beatslash.lv2/beatslicer_stereo> */

static const char p_uri[] = "http://github.com/blablack/beatslash.lv2/beatslicer_stereo";

enum p_port_enum {
  p_inputL,
  p_inputR,
  p_gate,
  p_tempo,
  p_sliceSize,
  p_sampleSize,
  p_attack,
  p_release,
  p_reverseMode,
  p_outputL,
  p_outputR,
  p_n_ports
};

static const peg_data_t p_ports[] = {
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { 0, 1, 0, 0, 0, 0 },
  { 40, 320, 120, 0, 0, 0 },
  { 0.03125, 0.5, 0.25, 0, 0, 0 },
  { 2, 16, 4, 0, 0, 0 },
  { 3, 25, 7, 0, 0, 0 },
  { 3, 25, 7, 0, 0, 0 },
  { 0, 2, 0, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
};


#endif /* avw_lv__beatslicer_stereo_ttl_hpp */
