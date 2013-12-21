#!/usr/bin/env python
import os
import subprocess
import shutil
from waflib import Logs
from waflib.extras import autowaf as autowaf

# Variables for 'waf dist'
APPNAME = 'beatslash-lv2'
VERSION = '1.0.0'

# Mandatory variables
top = '.'
out = 'build'


def options(opt):
    autowaf.set_options(opt)
    opt.load('compiler_cxx')


def configure(conf):
    autowaf.configure(conf)
    autowaf.display_header('beatslash-lv2 Configuration')

    conf.load('compiler_cxx')

    autowaf.check_pkg(conf, 'gtkmm-2.4',  uselib_store='GTKMM',atleast_version='2.24.0')
    autowaf.check_pkg(conf, 'gtk+-2.0', uselib_store='GTK2', atleast_version='2.24.0')
    autowaf.check_pkg(conf, 'cairo', uselib_store='CAIRO', atleast_version='1.0.0')
    autowaf.check_pkg(conf, 'lv2', uselib_store='LV2', atleast_version='1.2.0')
    autowaf.check_pkg(conf, 'lvtk-plugin-1', uselib_store='LVTK_PLUGIN', atleast_version='1.1.1')
    autowaf.check_pkg(conf, 'lvtk-ui-1', uselib_store='LVTK_UI', atleast_version='1.1.1')
    autowaf.check_pkg(conf, 'lvtk-gtkui-1', uselib_store='LVTK_GTKGUI', atleast_version='1.1.1')

    check = 'Extended Initializer Lists'
    conf.check_cxx(msg         = check,
                   define_name = 'WAF_CXX_FEATURE_' + check.replace(' ', '_').upper(),
                   fragment    = '#include <vector>\n'+
                                 'struct foo {\n'+
                                 '  int i;\n'+
                                 '  int j;\n'+
                                 '  int k;\n'+
                                 '};\n'+
                                 'int main() {\n'+
                                 '  std::vector<foo> v(1);\n'+
                                 '  v[0] = {0, 0, 0};\n'+
                                 '  return 0;\n'+
                                 '}',
                   mandatory   = False)

    # Set env['pluginlib_PATTERN']
    pat = conf.env['cxxshlib_PATTERN']
    if pat[0:3] == 'lib':
        pat = pat[3:]
    conf.env['pluginlib_PATTERN'] = pat

    autowaf.display_msg(conf, "LV2 bundle directory", conf.env['LV2DIR'])
    print('')


def build_plugin(bld, bundle, name, source, cxxflags=[], libs=[], add_source=[]):
    penv = bld.env.derive()
    penv['cxxshlib_PATTERN'] = bld.env['pluginlib_PATTERN']
    obj              = bld(features = 'cxx cxxshlib')
    obj.env          = penv
    obj.source       = source + add_source
    obj.includes     = ['.', './src']
    obj.name         = name
    obj.target       = os.path.join(bundle, name)
    if cxxflags != []:
        obj.cxxflags = cxxflags
    if libs != []:
        obj.uselib = libs
    obj.install_path = '${LV2DIR}/' + bundle

    # Install data file
    data_file = '%s.ttl' % name
    bld.install_files('${LV2DIR}/' + bundle, os.path.join(bundle, data_file))


def build_plugin_gui(bld, bundle, name, source, cxxflags=[], libs=[], add_source=[]):
    penv = bld.env.derive()
    penv['cxxshlib_PATTERN'] = bld.env['pluginlib_PATTERN']
    obj              = bld(features = 'cxx cxxshlib')
    obj.env          = penv
    obj.source       = source + add_source
    obj.includes     = ['.', './src']
    obj.name         = name
    obj.target       = os.path.join(bundle, name)
    if cxxflags != []:
        obj.cxxflags = cxxflags
    if libs != []:
        obj.uselib = libs
    obj.install_path = '${LV2DIR}/' + bundle


def build(bld):
    def do_copy(task):
        src = task.inputs[0].abspath()
        tgt = task.outputs[0].abspath()
        return shutil.copy(src, tgt)

    for i in bld.path.ant_glob('beatslash.lv2/*.ttl'):
        bld(rule   = do_copy,
            source = i,
            target = bld.path.get_bld().make_node('beatslash.lv2/%s' % i),
            install_path = '${LV2DIR}/beatslash.lv2')



    plugins = '''
    beatrepeater_mono
    beatrepeater_stereo
    beatslicer_mono
    beatslicer_stereo
    '''.split()

    for i in plugins:
        build_plugin(bld, 'beatslash.lv2', i, ['src/%s.cpp' % i],
                     ['-DPLUGIN_CLASS=%s' % i,
                      '-std=c++11',
                      '-DURI_PREFIX=\"http://github.com/blablack/beatslash-lv2/\"',
                      '-DPLUGIN_URI_SUFFIX="%s"' % i,
                      '-DPLUGIN_HEADER="src/%s.hpp"' % i],
                     ['LV2', 'LVTK_PLUGIN'],
                     ['src/envgen.cpp'])




    plugins_gui = '''
    beatrepeater_mono_gui
    beatrepeater_stereo_gui
    beatslicer_mono_gui
    beatslicer_stereo_gui
    '''.split()

    for i in plugins_gui:
       build_plugin_gui(bld, 'beatslash.lv2', i, ['src/%s.cpp' % i],
                        ['-DPLUGIN_CLASS=%s' % i,
                         '-std=c++11',
                         '-DURI_PREFIX=\"http://github.com/blablack/beatslash-lv2/\"',
                         '-DPLUGIN_URI_SUFFIX="%s"' % i,
                         '-DPLUGIN_HEADER="src/%s.hpp"' % i],
                        ['LV2', 'LVTK_PLUGIN', 'LVTK_GTKGUI', 'GTKMM', 'GTK2', 'CAIRO'],
                        ['src/dial.cpp', 'src/labeleddial.cpp'])



