🚨 This project is archived and no longer maintained. 🚨

Thank you for your past support. Feel free to explore the existing codebase and documentation. For any questions, consider checking the existing issues, but note that responses may be delayed.

If you find this project useful and wish to continue its development, please fork it with proper attribution and adherence to the license.

beatslash-lv2
=============

beatslash-lv2 is a set of LV2 plugins to mangle, slash, repeat and do much more with your beats.
They are meant to be used live, but it is up to your imagination what to do!

The set contains:
- Beat Repeater
- Beat Slicer

More information can be found here:
[http://objectivewave.wordpress.com/beatslash-lv2](http://objectivewave.wordpress.com/beatslash-lv2)

DEPENDENCIES
------------

	libgtkmm-2.4-dev
	lvtk-dev

INSTALL
-------

	$  ./waf configure
	$  ./waf 
	$  ./waf install


RUNNING
-------

After the INSTALL step any LV2 host should automatically find the plugins and add them to the list of loadable ones.

Ingen is the recommended host.
More information about Ingen here: [http://drobilla.net/software/ingen/](http://drobilla.net/software/ingen/)


BUG REPORTING
-------------
Please use the issue tracker on github to report bugs:
[http://github.com/blablack/beatslash-lv2/issues](http://github.com/blablack/beatslash-lv2/issues)



