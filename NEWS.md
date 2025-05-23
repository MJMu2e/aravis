Development release 0.9.1
=========================

  * gv_interface: discovery interface selection (Yuto)
  * system: add a way to select an interface (Emmanuel)
  * chunk_parser: support for value selector (Emmanuel)
  * camera: replace ctrea_strem_full by stream_create, break API (Emmanuel)
  * gvcp, gvsp: make packet parsing more robust (Emmanue)
  * gv_stream: fix uninitialized data access (Emmanuel)
  * tests: fix unaligned memory access (Matwey)
  * introspection: annotation fixes (Jared, Emmanuel)
  * fake_camera: fix trailer packet content (Michail)

Development release 0.9.0
=========================

  * v4l2, USB3Vision: native buffer allocation (Emmanuel)
  * GenTL producer: experimental implementation (Václav)
  * GenTL consumer: experimental implementation (Xiaoqiang)
  * v4l2 interface: experimental implementation (Emmanuel)
  * USB3Vision: make maximum transfer size configurable (Emmanuel)
  * gst: add gamma property (WhaSukGO)

Stable release 0.8.33
=====================

  * viewer: ignore exposure time feature representation (Emmanuel)

Stable release 0.8.32
=====================

  * viewer: workaround for Blackfly Mono16 pixel endianness issue (Berke)
  * gst: disconnect camera in stop method #907 (Emmanuel)
  * gst: stream object accessor (Marko)
  * gst: camera change signal (Marko)
  * gst: stream change signal (Marko)
  * gst: allow software trigger (WhaSukGo)
  * gvsp: fix chunk detection (Marco)
  * gv_device: crach fix on finalization (Marco)
  * Fix device_write_memory rust binding (Jacob)
  * introspection: add generic feature value accessors (Marko)
  * chore: fix libxml2 deprecation warnings (Emmanuel)
  * chore: appstream fixes (Chiara)

Stable release 0.8.31
=====================

  * viewer: better handling of exposure and gain setting (Momoko)
  * genicam: genicam URL stored as DomDocument URL property (Emmanuel)
  * genicam: fix arv_device_get_feature_access_mode return value (Marco)
  * camera: set/dup_register helper (Emmanuel)
  * camera: frame_rate setting improvements (Marco)
  * usb3vision: handle GenDC payload (Momoko)
  * gigevision: set a lower boundary to minimum socket size in auto mode (Emmanuel)
  * build: fix a libxml2 API breakage (Russel, Marco)

Stable release 0.8.30
=====================

  * gv: add EVK Helios and Automation Technology C6 devices to the legacy mode list (Václav, anathesys)
  * gv: don't ignore POINTTOPOINT interfaces, allows for direct connection to
    devices through VPNs (James)

Stable release 0.8.29
=====================

  * usb3vision: fix callbacks triggering in async mode (Emmanuel)
  * usb3vision: fix underrun counting (Emmanuel)

Stable release 0.8.28
=====================

  * usb3vision: make async libusb default (Emmanuel)
  * usb3vision: improve CPU use when no buffer is available for the stream thread (Emmanuel)
  * usb3vision: don't wait forever for USB transfer completion (Emmanuel)
  * usb3vision: enable stream after the receiving stream thread is started (Emmanuel)
  * viewer: avoid buffer exhaustion if GStreamer pipeline is late (Emmanuel)
  * build: fix clang 32 bits compilation (Nath)

Stable release 0.8.27
=====================

  * camera: add vendor specific quirks to frame rate (durnezj)
  * camera: multiple tap helpers #745 (Brian)
  * gv: start packet timeout after the first non leader packet #746 (Brian)
  * gv: auto packet size fix (Emmanuel)
  * gv: add more camera to the legacy endianness mechanism exception list (Emmanuel)
  * u3v: fix u3vcp_capability register size (Emmanuel)
  * chore: compilation warning fixes (Marco, Emmanuel)
  * ci: pipeline babysitting (Emmanuel)
  * doc: link fix (Henrique)

Stable release 0.8.26
=====================

  * viewer: fix pixel format selection #765 (Emmanuel)

Stable release 0.8.25
=====================

  * viewer: add component selection support (Emmanuel)
  * buffer: add component id getter (Emmanuel)
  * buffer: fix multipart informations (Emmanuel)

Stable release 0.8.24
=====================

  * gv: fix multipart data accessor (Emmanuel)
  * gv: improve error handling of network API (Emmanuel)

Stable release 0.8.23
=====================

  * python: fix crash in stream callback #714 (Cyril)
  * gv: multipart support (Emmanuel)
  * gv: chunk data payload support (Emmanuel)
  * gv: use the trailer packet for expected data size calculation (Emmanuel)
  * gv: reset SCPx register on stream finalization
  * gv: API to allow discovery ack broadcast (Emmanuel)
  * gv: add more legacy endianness entries (Tomohisa, Radim, Emmanuel)
  * gv: fix infinite loop in auto packet size (Emmanuel)
  * gv: don't use source port in socket packet mode (Emmanuel)
  * genicam: AccessMode improvements (Emmanuel)
  * camera: add a Component API (Emmanuel)
  * buffer: part access API (Emmanuel)
  * cameratest: allow to set features from a string (Emmanuel)
  * ci: windows build fixes (Momoko, Emmanuel)
  * doc: improvements (Masahiro, Emmanuel)

Stable release 0.8.22
=====================

  * camera: new network command for IP settings (Romain, Masahiro)
  * camera: don't fail if TriggerSelector is not available (Romain)
  * camera: make region setting work even without Offset features (Emmanuel)
  * camera: allow frame rate setting during acquisition on more devices #683 (Emmanuel)
  * gstplugin: allow low frame rate with external triggers (Edgar)
  * genicam: list for forced legacy mode (Romain)
  * genicam: access mode check policy (Emmanuel)
  * genicam: fix inifinite loop during availability check #678 (Emmanuel)
  * u3v: use libusb device-left event for control lost detection (Emmanuel)
  * genicam: fix information loss #656 (Emmanuel)
  * fake_camera: add TriggerSoftware support (Romain)
  * build: appdata fix (Chiara)
  * build: disable documentation by default (Emmanuel)
  * tests: fix ChunkFloat test on big endian platform #670 (Emmanuel)
  * introspection: fix user_data_destroy_func annotations (Thomas)
  * misc: memory leak and warning fixes (Emmanuel)

Stable release 0.8.21
=====================

  * build: FreeBSD support #620 (Rim)
  * gigevision: performance improvement of standard socket method #617 (Emilio)
  * usb3vision: prevent USB command mix up #622 (Edgar)
  * usb3vision: fix PacketResend size field in extended ID mode #642 (Alexander)
  * genicam: allow more Category child nodes #632 (Emmanuel)
  * genicam: AccessMode fixes #634 (Emmanuel)
  * genicam: fix pIsLocked when target is a Boolean node #638 (Emmanuel)
  * doc: port to gi-docgen #621 (Emmanuel)
  * git: default branch is main now (Emmanuel)

Stable release 0.8.20
=====================

  * camera: make set_frame_rate more robust for Basler cameras (Thies)
  * misc: add runtime version API (Emmanuel)
  * misc: ignore leading spaces in genicam data url (Emmanuel)
  * misc: add get_device_manufacturer_info API (Emmanuel)
  * qa: fix incorrect memory access (Jose)
  * gstreamer: allow to select asynchronous USB mode (Christian)
  * windows: fix thread priority helpers (Garfeng)
  * meson: fix compilation using meson 0.60 (Emmanuel)
  * doc: ubuntu 20.04 build do cimprovements (Brian)

Stable release 0.8.19
=====================

  * camera: black level support (Brian)
  * camera: GainAbs support for gain setting (Brian)
  * camera: FrameBurstStart support for trigger configuration (Emmanuel)
  * viewer: minor layout fixes (Emmanuel)
  * ci: MSVC improvements (Siim)

Stable release 0.8.18
=====================

  * usb3vision: fix header inclusion when USB is disabled (Marc)
  * tests: register cache test (Emmanuel)

Stable release 0.8.17
=====================

  * windows: Microsoft Visual C++ support (Siim)
  * usb3vision: async mode (Takuro, Constantine)
  * usb3vision: use GUID for device disambiguation (Takuro)
  * test: extend test coverage in arv-test (Emmanuel)
  * viewer: in-app notification for save error (Emmanuel)
  * camera: make set_trigger more robust (Emmanuel)
  * tool: add glob pattern based device and feature selection (Emmanuel)

Stable release 0.8.16
=====================

  * ci: use Github actions for linux and macOS (Emmanuel)
  * ci: minGW support (Václav)
  * gigevision: use proper broadcast addresses during discovery (Václav)
  * camera: accessor to float feature increment (Emmanuel)
  * camera: make set_trigger more robust (Emmanuel)
  * camera: fallback to Continuous mode if SingleFrame is not available
    (Emmanuel)
  * viewer: allow to save a snapshot as png or jpeg image (Emmanuel)
  * viewer: display all pixel formats in selector (Emmanuel)
  * simulator: now works on Windows and macOS (Václav, Emmanuel)
  * debug: fix output on Windows (Václav)
  * doc: Windows build documentation (Václav)
  * tests: new arv-test application for automated testing (Emmanuel)

Stable release 0.8.15
=====================

  * gigevision: only disable packet resend after a packet unavailable error
  * gigevision: add a new packet timeout for first packet resend request
  * doc: improve GvStream property documentation
  * code cleanup

Stable release 0.8.14
=====================

  * debug: fix debug timestamp on older platforms (Emmanuel)

Stable release 0.8.13
=====================

  * viewer: fix incorrect bandwith and frame rate computation

Stable release 0.8.12
=====================

  * usb3vision: initialize stream infos

Stable release 0.8.11
=====================

  * stream: add an extensible info API (Emmanuel)
  * gvstream: improve packet resend request behaviour in case of unordered gvsp
    packets (Emmanuel)
  * gvstream: wait for thread setup during ArvGvStream initialization (Emmanuel)
  * gigevision: use MAC as serial number fallback (Emmanuel)
  * genicam: fix arv_exposure_mode_to_string (Martin)
  * usb3vision: improve error reporting in case of libsub error (Emmanuel)
  * usb3vision: automatically detach kernel driver (Emmanuel)
  * cameratest: add a test duration parameter (Emmanuel)
  * misc: rename internal ArvStatistic to ArvHistogram (Emmanuel)

Stable release 0.8.10
=====================

  * arv-tool: make device enumeration faster (Emmanuel)
  * debug: rework debug log levels (Emmanuel)
  * fakegvcamera: improve streaming reliability - partly fix #499 (Emmanuel)
  * gvstream: fix use after reference release - fix #504 (Emmanuel)
  * genicam: String node support - fix #507 (Emmanuel)

Stable release 0.8.9
====================

  * windows: build fix (Emmanuel)

Stable release 0.8.8
====================

  * arv-camera-test: promote to installed application (Emmanuel)
  * applications: fix policy parameter consistency (Emmanuel)
  * debug: timestamped and modernized output (Emmanuel)
  * genicam: add a range check debug mode (Emmanuel)
  * gvdevice: faster finalization (Emmanuel)
  * camera: new DeviceSerialNumber getter (Emmanuel)

Stable release 0.8.7
====================

  * camera: ignore Acquisitiontart feature setting failure (Emmanuel)
  * camera: fix trigger setting for Basler cameras (Casperoo)
  * camera: add set_exposure API (Emmanuel)
  * gigevision: Windows support (Václav)
  * usb3vision: better error packet handling (Emmanuel)
  * genicam: allow get/set float from an int node (Emmanuel)
  * genicam: allow multiple pIndex property nodes (Emmanuel)

Stable release 0.8.6
====================

  * camera: handle GainRaw also as a float feature
  * camera: add arv_camera_new_with_device()
  * camera: ignore error on TriggerSelector and TriggerMode setting in arv_camera_set_trigger()
  * gigevision: fail quicker if a device is not found at ArvGvDevice instantiation
  * fakegvcamera: fix interface selection
  * genicam: implement optional range check for integer and float node values, as a runtime option
  * genicam: fix min/max computation for StructEntry and MaskedIntReg nodes
  * gstplugin: don't fail camera init if gain or exposure features are not available

Stable release 0.8.5
====================

  * macOS: build fix
  * travis:enable macOS

Stable release 0.8.4
====================

  * all: s/adjustement/adjustment/

Stable release 0.8.3
====================

  * gigevision: automatically adjust packet size if needed (Emmanuel)
  * gstreamer: don't try to set frame rate if feature is not available (Emmanuel)
  * genicam: fix pVariable name with dot (Arrigo)
  * genicam: fix parsing of genicam data url (Emmanuel)
  * buffer: add arv_buffer_set_frame_id API (Russel)
  * usb3vision: add Dahua Technology USB id (H.F)
  * build: preparatory work for windows compilation (Eudoxos)

Stable release 0.8.2
====================

  * gvdevice: fix timeout race (casperoo)
  * fakecamera: implement Mono16 pixel format (Hinko)
  * Enable ppc64le in CI pipeline (nagesh)

Stable release 0.8.1
====================

  * build: add more compilation warnings and fix them (Emmanuel)

Stable release 0.8.0
====================

  * gigevision: ExtendedIds support (Hendrick, Emmanuel)
  * gigevision: add get_control_access API (casperoo)
  * genicam: implement proper AccessMode and ImposedAccessMode support (Siim)
  * genicam: add or extend support for Representation, Unit, DisplayNotation and DisplayPrecision proerties (Siim)
  * genicam: extend GcRregisterDescriptionNode API (Siim)
  * genicam: improve String register (Siim)
  * genicam: implement arv_gc_feature_get_name_space() (Siim)
  * gst-plugin: don't shadow GstBaseSrc num-buffers property (Marko)
  * usb3vision: add Daheng Imaging descriptors (Jakob)

Unstable release 0.7.5
======================

  * gst-plugin: error handling and lock fixes (Marko)
  * build: fix when aravis is used as a subproject (Rihards)
  * build: fix viewer build without libusb (Guillaume)
  * fake camera: implement bayer pixel formats (Bernardo)
  * gcregister: don't try to read WO registers (Stefan)
  * viewer: fix buffer leak (Emmanuel)
  * gvstream: correctly handle resend request limit (Emmanuel)
  * stream: device reference leak fix (Emmanuel)

Unstable release 0.7.4
======================

  * camera/device/stream: add an error parameter to object intantiation functions (Emmanuel)
  * camera: fix node type mismatch error in set_frame_rate (Arkadiusz)
  * gigevision: ignore duplicated packets (Joris)
  * build: make build of tests optional (Edgar)

Unstable release 0.7.3
======================

  * all: use gobject macros for class declarations (Emmanuel)
  * camera: add a GError parameter to most functions (Emmanuel)
  * python: add python tests in test suite (Emmanuel)
  * usb3vision: improve reliability of camera connection (Dmitry)
  * introspection: fix PixefFormat type (Léo, Maarten)

Unstable release 0.7.2
======================

  * gigevision: support for ImageExtendedChunkPayload (Nathan)
  * chunkparser: add a GError parameter to the getters (Emmanuel)
  * chunkparser: add a boolean accessor (Emmanuel)
  * arvtool: new `values` command that show the values of all available features (Emmanuel)
  * gcport: don't try to read a register when the port is an event (Emmanuel)
  * genicam: pSelect support (Emmanuel)
  * genicam: remove value_type property, replaced by ARV_IS_GC_(FLOAT|INTEGER`BOOLEAN|STRING|ENUMERATION) (Emmanuel)
  * genicam: simplify read/write feature values as/from string (Emmanuel)
  * genicam: fix min/max of non 64 bit integers
  * genicam: Float and Integer now get their min/max also from pValue (Emmanuel)
  * gigevision: correctly detect access denied errors

Unstable release 0.7.1
======================

  * build: installation fixes (Emmanuel, Rafael)

Unstable release 0.7.0
======================

  * build: aravis is now built using meson (and only meson)
  * camera: make set_region more robust (Aleksandr)
  * usb3vision: fix an align assertion (Johannes)
  * fakecamera: improve discovery (Russell)
  * fakecamera: add RGB8 pixel format support (Russell)
  * fakecamera: allow to instantiate several gvfakecamera (Emmanuel)
  * gigevision: pending ack fix (Emmanuel)
  * gigevision: better handling of GVCP error packets (Emmanuel)
  * gstplugin: packet size parameter (Emmanuel)
  * viewer: show stream statistics (Emmanuel)
  * genicam: fix register cache (Emmanuel)
  * camera: move feature read/write API with status here from ArvDevice (Emmanuel)
  * device: add a GError parameter to feature read/write functions (Emmanuel)
  * gvdevice: use Inc in auto packet size function (Emmanuel)
  * genicam: implementation of `<Inc>` support (Emmanuel)
  * viewer: icon refresh (Emmanuel)

Release 0.6.3
=============

  * device: fix get_status return value (crabsmaps)
  * gigevision: add more pixel format enums (emmanuel)

Release 0.6.2
=============

  * stream: add stop/start thread API  (Werner, Emmanuel)
  * gigevision: allow to discover more devices (Michael)
  * gigevision: stop stream thread quicker (Emmanuel)
  * genicam: add <Register> element support (Nathan)
  * genicam: let float node point to integer node (Emmanuel)
  * usb3vision: sanity checks during device initialization (crabsmaps)

Release 0.6.1
=============

  * gigevision: auto-packet size negociation improvements (Nils, Edgar)
  * gigevision: interface addess assignment improvement (Nils)
  * usb3vision: memory leak fixes (wuyuanyi)
  * usb3vision: payload size computation fixes (Johannes)

Release 0.6.0
=============

  Stable release.

  * camera: avoid clashes in device id generation
  * genicam: signedness and endianness related fix

Release 0.5.13
==============

  * usb3vision: chunk data support (Emmanuel)
  * camera: matrix vision device support (Nils)
  * camera: PointGrey / FLIR renaming support (Nils)
  * i18n: czech and slovak translations (Multiflexi)
  * build: remove libcap-ng dependency (Nils)

Release 0.5.12
==============

  * gigevision: disable GVSP extended id mode
  * genicam: fix for signedness of register nodes
  * genicam: add support for optional ROUND parameter (Aleksandr)
  * add man page (Chiara)

Release 0.5.11
==============

  * camera: ensure binning feature is present before using it (thecaptury)
  * viewer: don't overwrite buffers still used by gstreamer (Tim)
  * usb3vision: improve streaming reliability (Tim, Emmanuel)
  * buid instruction update (Raphie, Rehno)
  * translation updates (Rafael)

Release 0.5.10
==============

  * camera: fix for PointGrey cmeras using AcquisitionFrameRateEnabled (Emmanuel)
  * genicam: correctly implement integer mode of IntSwissKnife (Emmanuel)
  * genicam: implementtion of pIndex, ValueIndexed, ValueDefault (Emmanuel)
  * genicam: add Expression and Constant support to Converter and IntConverter (Emmanuel)
  * evaluator: implement ROUND function (Emmanuel)
  * evaluator: fix '~' precedence and associativity (Emmanuel)
  * gigevision: protocol fixes (Emmanuel)
  * gigevision: improve connection stability under high pressure (Michael)
  * gigevision: avoid using the same port twice (Emmanuel)
  * gvdevice: implement packet-resend-ration limit (Emmanuel)
  * usb3vision: transfer size configuration fixes (Konstantin)
  * build: improve OSX and Windows support (Emmanuel)
  * build: improve build outside of source directory (Janito)
  * viewer: improve Pixel format to GstCaps conversion (Emmanuel)
  * viewer: disable gtkglsink for now to avoid a crash when switching views (Emmanuel)
  * QA: extended code coverage (Emmanuel)
  * QA: make travis build on OSX (Emmanuel)

Release 0.5.9
=============

  * gige/usb3vision: device detection fixes (Nils)
  * stream: allow to handle circular buffer from callback (Nils)
  * build: OSX documentation fix (Mark)
  * viewer: latency improvements (Emmanuel)
  * viewer: show protocol in camera list view (Emmanuel)
  * gigevision: faster connection to devices (Emmanuel)
  * gigevision: PENDING ACK support (Emmanuel)
  * gige/usb3vision: use vendor aliases for default device id (Emmanuel)

Release 0.5.8
=============

  * buffer: save local system time in buffer metadate (Konstantin)
  * usb3vision: better handling of unexpected answers (Emmanuel)
  * stream: fix for self joining receiving thread issue (Michael)
  * fake: new class for instantiation of fake cameras (Emmanuel)
  * tests: new fakegv test that exercizes GigEVision protocol (Emmanuel
  * build system update (code-coverage, address sanitizer and valgrind support) (Emmanuel)
  * miscellaneous fixes for bug discovered vie the update build system (Emmanuel)

Release 0.5.7
=============

  * usb3vision: Ximea camera support (Steve)
  * usb3vision: TheImagingSource camera support (emmanuel)
  * usb3vision: Bandwith control (Steve)
  * usb3vision: USB endpoint detection improvments (Steve, Emmanuel)
  * fake_camera: miscellaneous fixes (Michael)
  * genicam: raw data support (Steve)
  * camera: multiframe acquisition mode (Lee)
  * camera: extension of the trigger API (Steve)
  * gigevision: instantiation by IP address or hostname (Blaz, Arkadiusz)
  * build: fix compilation with old glib (Blaz)
  * build: intructions and compilation fix for OSX (Christoffer, Fernando)
  * build: Travis and Coverity support on github (Emmanuel)

Release 0.5.6
=============

  * genicam: implement Expression and Constant nodes for [Int]SwissKnife
  * API: add ARAVIS_CHECK_VERSION macro
  * API: forbid direct inclusion of headers other than arv.h
  * usb3vision: PENDING_ACK support

Release 0.5.5
=============

  * build: fix library detection needed for packet socket support (Hubert)
  * gigevision: new API for automatic stream packet size (Emmanuel)

Release 0.5.4
=============

  * gigevision: support of packet socket in stream thread (Emmanuel)

Release 0.5.3
=============

  * usb3vision: fix USB3 device scan (Emmanuel)

Release 0.5.2
=============

  * viewer: only show compatbile pixel format in selector
  * viewer: fix binning setting on some cameras
  * viewer: add a fallback to autovideosink if gtk[gl]sink are not available
  * viewer: fix video position issue
  * usb3vision: avoid unnecessary memory during video data transfer
  * usb3vision: better handling of device disconnection
  * gigevision: compile time selection of fast heartbeat mode (Michele)
  * camera: helper function for single image acquisition

Release 0.5.1
=============

  * viewer: new design with support for region settings
  * usb3vision improvements
  * camera: improve Basler support for exposure and frame rate
  * camera: add support for x/y_offset and binning bounds
  * interface: add vendor, model and serial getters
  * genicam: implement min/max support for Converter elements

Release 0.5.0
=============

  * usb3vision initial support

Release 0.4.0
=============

  * buffer: add a timestamp setter
  * interface: don't touch device list on open device
  * viewer: fix appdata file

Release 0.3.8
=============

  * genicam: add bigendian support (Emmanuel)
  * camera: RICOH camera support (Anil)
  * build fixes (Emmanuel)
  * dox fixes (Jarek, Emmanuel)
  * translation updates (Jordi, Jiri, Anders, Μύρων, Pedro, Cédric, Samir, Necdet)

Release 0.3.7
=============

  * genicam: fix accuracy of division of integers (Emmanuel)
  * new arv_make_high_priority and arv_make_realtime API (Emmanuel)
  * viewer: make stream thread realtime if possible (Emmanuel)
  * camera: add GigEVision specific API for packet delay, packet size and stream selection (Emmanuel)
  * gst_pugins: add a number of buffers property (Patrick)
  * build fixes (Patrick)
  * translation updates (Tiago, Muhammet)

Release 0.3.6
=============

  * build: add missing test commands in configure.ac (Adrian)
  * camera: new abort_acquisition function (Davide)
  * gv_stream: missing frame detection fix (Davide)
  * buffer: user_data and frame_id accessors
  * chunk_parser: bug fixes
  * viewer: prevent use of broken coglsink from autovideosink
  * translations: updates (Matej, Balázs, Andika, Daniel, Rafael)

Release 0.3.5
=============

  * chunk_parser: new API for chunk data support
  * buffer: make internal data private and add accessors

Release 0.3.4
=============

  * viewer: add an appdata file
  * misc: compilation warning fixes

Release 0.3.3
=============

  * genicam: improve loading of genicam data from local files (Peter)
  * build: dependency check improvements
  * build: miscellaneous compilation fixes (Patrick)
  * gst_pugins_1.0: bayer format fix (Steve)

Release 0.3.2
=============

  * gst_plugin: new plugin based on gstreamer 1.0, in addition to the existing 0.10 version.
  * gst_plugin: add packet-resend property.
  * viewer: port to gstreamer 1.0, and drop support for gtk+ 2.0.
  * viewer: fix video output for camera with buggy time stamping.
  * camera: fix frame rate setting for Point Grey devices.

Release 0.3.1
=============

  * gv_interface: fix device discovery by using 255.255.255.255 as broadcast address (Csaba).
  * gv_interface: don't crash if there is no INET interface (Toby).
  * camera: make ArvCamera introspection friendly (John).
  * evaluator: fix comparison when one of the operand is a double.
  * python: don't crash on access to buffer data.
  * build: zlib check.
  * build: make configure pass even if gobject introspection is not available.
  * updated translations (Yuri).

Release 0.3.0
=============

  * updated translations (Marek, Matej, Martin, Daniel, Joe, Kenneth, Rūdolfs, Andika, Balázs, Piotr, Christian, Мирослав)
  * make framerate work for The Imaging Source cameras (Edgar)
  * fix exposure setting on Point Grey Blackfly cameras
  * added frame rate support for The Imaging Source(TIS) cameras (Edgar)
  * gv_interface: strip control character from device id (Csaba)
  * gst_plugin: make the plugin more useful for a use inside an application (Edgar)
  * gst_plugin: honor do-timestamp property (Csaba)
  * arv-tool: fix IntReg display and don't display min/max if they are no specified
  * gv_device: don't send packet resend requests when this feature is not available
  * gv_device: implement register workaround for schema < 1.1.0
  * python: crash fixes (Olivier)
  * viewer: fix an issue when aravis buffer row stride is not a multiple of 4
  * viewer: improve framerate entry
  * viewer: translation update
  * compilation warning suppression
  * suppress glib deprecation warnings
  * add a define for BAYER_GR_12_PACKED pixel format

Release 0.2.0
=============

Stable release.

  * camera: add support for BAYER_GB_8 images used by ImageingSource DFK 23G445 cameras (Matthias)
  * camera: fix consistency of order of pixel format display names (Jure)
  * GV: fix parsing of hexadecimal address/length with 0x prefix in Genicam file URL
  * build: fix parallel compilation (make -j)

Release 0.1.15
==============

  * library: don't distribute arvconfig.h (Nial bug report)
  * viewer: desktop file and application icon
  * viewer: rotate and flip button
  * viewer: file save notification
  * viewer: use autovideosink instead of xvimagesink, for systems without XV extension
  * camera: pixel format API improvments (Jure)
  * system: disable the fake camera by default (Kai bug report)
  * genicam: fix "remaining operands" issue in evaluator (Tom bug report)
  * genicam: fix unzipping of genicam data on not supporting unaligned memory access (Nial)
  * GV stream: use machine time for buffer timestamp if GevTimestampTickFrequency register does not exist
  * gstreamer plugin: add auto gain, auto exposure, x and y offset properties (Philipp)

Release 0.1.14
==============

API breaks:

  arv_stream_timed_pop_buffer is now arv_stream_timeout_pop_buffer
  arv_camera_(get/set)_gain use a double value for gain
  arv_camera_get_gain_bounds also use doubles

Changes:

  * genicam: preliminary error handling
  * viewer: internationalization support
  * viewer: widget are now insensitive if feature is not available
  * viewer: fix gtk requirement
  * viewer: add optional support for gtk3
  * viewer: fix flicker on window resize
  * camera: use Gain feature instead of GainRaw when available
  * genicam: fix formula evaluation when they contain entities

Release 0.1.13
==============

  This release hopefufly adds support for cameras depending on IntSwissKnife
  for the computation of register addresses, like JAI cameras. It also allows to
  manually tweak the PacketSize parameter.

  * genicam: add support for SwissKnife in register address.
  * GV device: don't force the packet size.
  * GV device: export interface and device IP addresses (Luca).
  * tests: more unit tests.
  * build: don't require a C++ compiler by default.

Release 0.1.12
==============

  API warning: arv_stream_pop_buffer is now blocking. Use arv_stream_try_pop_buffer
  for non blocking behaviour.

  * genicam: support for StructReg StructEntry
  * stream: new pop_buffer / try_pop_buffer functions, consistent with glib async_queue API.
  * camera: new get/set_trigger_source functions.
  * tool: support for wildcard in camera name.
  * tool: ability to perform the same command on several cameras.
  * interface: new get_device_physical_id function.
  * genicam: rewrite on top of a DOM API.

Release 0.1.11
==============

  * viewer: add a snapshot button.
  * documentation: more information on GigE Vision protocol.
  * all: memory leak fixes.

Release 0.1.10
==============

  This release should fix the lost of connection, at least better than
  the last release. It also allows to instantiate a GigE Vision device
  using its MAC address.

  * device: new "control-lost" signal.
  * GV device: allow use of MAC address for device instantiation.
  * GV device: retry gvcp command if wrong packet is received.
  * GV device: try harder to get control register value in heartbeat thread.
  * GVCP: don't use 0 for packet counter value.

Release 0.1.9
=============

  This release fixes the lost of the connection to the device after
  about 65600 seconds.

  * GV device: don't compare guint32 and guint16.

Release 0.1.8
=============

  In this release, arv-show-devices is renamed to arv-tool, and gain the capability
  to read/write and list device features.

  * Build: fix headers for C++ compatibility (Andre Gaschler)
  * Genicam: partly implement isAvailable and isImplemented.

Release 0.1.7
=============

  More work on GV stream reception thread.

  * GV Stream: handle error GVSP packets.
  * GV Stream: always set packet size.
  * GV Stream: check for control access before creating stream object.
  * GV Stream: check the number of available stream channels.
  * GVCP: more sanity checks on ack packets.
  * GV Device: reliably check for control access.
  * Fake GV Camera: make it work with viewer.
  * Debug: allow different debug levels for each debug category.


Release 0.1.6
=============

  Work on GV stream reception reliability, with improved packet resend mechanism.

  * Camera: software trigger support
  * GV Stream: rewrite of the receiving thread
  * Introspection: annotation fixes
  * Build: make stable release parallel installable

Release 0.1.5
=============

  Release mostly focused on color support improvements.

  * GstAravis: switch to negociated width, height, framerate and pixel format
  * Evaluator: fix support of variable name containing digits
  * Buffer: add a per buffer user data
  * Stream: add a timed buffer pop function
  * Stream: switch to gobject properties for packet resend and socket buffer size
  * Camera: more bounds functions

Release 0.1.4
=============

  * Viewer: support for some color pixel formats
  * Viewer: support for auto gain and auto exposure
  * Utils: Genicam xml data dump capability added to arv-show-devices
  * Camera API: Fix exposure setting for Ace Basler cameras
  * Genicam: Fix caching of registers on read

Release 0.1.3
=============

  * Simple viewer based on ArvCamera API (requires gtk and gstreamer)
  * Add a "new-buffer" signal to ArvStream
  * Fix stream IP address setting for JAI Pulnix cameras (Tom Cobb)
  * Fix use or Aravis from a C++ application (Tom Cobb)
  * Fix division of integers in ArvEvaluator when asked for float result (Tom Cobb)
  * Add an API for retrieving the genicam data (Tom Cobb)
  * Fix minimum offset of ROI (Tom Cobb)
  * Fake camera can now simulate gain and exposure setting

Release 0.1.2
=============

  * Add exposure and gain settings to the gstreamer source element
  * fix exposure setting in ArvCamera for Basler cameras
  * gather running statistics for the GV devices
  * fix GV stream fixed buffer size
  * add a new arv-show-devices utility
  * make API more consistent with the Genicam standard

Release 0.1.1
=============

  * Basic ethernet camera simulator
  * Allow detection of ethernet cameras on lo, for the GV camera simulator
  * Fix in gvcp support code
  * More unit tests
  * Add support for <Group> GENICAM element
  * Memory leak fixes
  * Record statistics on buffer reception times
  * Improve compilation on 64 bit platforms

Release 0.1.0
=============

Initial release.
