CONFIG -= \
	qt

QMAKE_CXXFLAGS += \
	-pthread \
	-std=c++11

LIBS += \
	-pthread \
	-lssl \
	-lcrypto

SOURCES += \
    main.cpp

DEPENDPATH += \
	$$PWD/../../lnetlib \
	$$PWD/../../lnetlib/lnetlib \
	$$PWD/../../lnetlib/lnetlib/asio \
	$$PWD/../../lnetlib/lnetlib/asio/detail \
	$$PWD/../../lnetlib/lnetlib/asio/generic \
	$$PWD/../../lnetlib/lnetlib/asio/impl \
	$$PWD/../../lnetlib/lnetlib/asio/ip \
	$$PWD/../../lnetlib/lnetlib/asio/local \
	$$PWD/../../lnetlib/lnetlib/asio/posix \
	$$PWD/../../lnetlib/lnetlib/asio/ssl \
	$$PWD/../../lnetlib/lnetlib/asio/windows \
	$$PWD/../../lnetlib/lnetlib/asio/detail/impl \
	$$PWD/../../lnetlib/lnetlib/asio/generic/detail \
	$$PWD/../../lnetlib/lnetlib/asio/ip/detail \
	$$PWD/../../lnetlib/lnetlib/asio/ip/impl \
	$$PWD/../../lnetlib/lnetlib/asio/local/detail \
	$$PWD/../../lnetlib/lnetlib/asio/ssl/detail \
	$$PWD/../../lnetlib/lnetlib/asio/ssl/impl \
	$$PWD/../../lnetlib/lnetlib/asio/ssl/old \
	$$PWD/../../lnetlib/lnetlib/asio/ssl/old/detail

INCLUDEPATH += \
	$$PWD/../../lnetlib \
	$$PWD/../../lnetlib/lnetlib \
	$$PWD/../../lnetlib/lnetlib/asio \
	$$PWD/../../lnetlib/lnetlib/asio/detail \
	$$PWD/../../lnetlib/lnetlib/asio/impl \
	$$PWD/../../lnetlib/lnetlib/asio/detail/impl \
	$$PWD/../../lnetlib/lnetlib/asio/ip \
	$$PWD/../../lnetlib/lnetlib/asio/ip/impl \
	$$PWD/../../lnetlib/lnetlib/asio/ip/detail \
	$$PWD/../../lnetlib/lnetlib/asio/generic/detail \
	$$PWD/../../lnetlib/lnetlib/asio/local/detail

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../lnetlib/ -llnetlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../lnetlib/ -llnetlibd
else:unix: LIBS += -L$$OUT_PWD/../../lnetlib/ -llnetlib

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lnetlib/liblnetlib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lnetlib/liblnetlibd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lnetlib/lnetlib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lnetlib/lnetlibd.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../lnetlib/liblnetlib.a
