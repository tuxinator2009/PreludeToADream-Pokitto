TEMPLATE = app
TARGET = editor
DESTDIR = ..
CONFIG += debug
#CONFIG += sanitizer sanitize_address
QT += widgets multimedia

# Input
HEADERS += \
  battleanimation.h \
  battleanimationseditor.h \
  choosemapevent.h \
  colorpicker.h \
  configureevent_battle.h \
  configureevent_buffercharacter.h \
  configureevent_buffervalue.h \
  configureevent_changeeventflags.h \
  configureevent_changeeventlocation.h \
  configureevent_changepassability.h \
  configureevent_changesprite.h \
  configureevent_changevariable.h \
  configureevent_giveplayeritem.h \
  configureevent_ifexited.h \
  configureevent_ifhasitem.h \
  configureevent_ifswitch.h \
  configureevent_ifvalue.h \
  configureevent_ifvariable.h \
  configureevent_ifyesno.h \
  configureevent_moveplayer.h \
  configureevent_playmusic.h \
  configureevent_playsoundeffect.h \
  configureevent_shakescreen.h \
  configureevent_showhidescreen.h \
  configureevent_showimage.h \
  configureevent_showmessage.h \
  configureevent_showshopmenu.h \
  configureevent_switchrange.h \
  configureevent_teleport.h \
  eventview.h \
  globals.h \
  image.h \
  imageview.h \
  itemseditor.h \
	mainwindow.h \
	map.h \
	mapevent.h \
	mapeventeditor.h \
	mapflags.h \
	mapspritepicker.h \
	messageseditor.h \
	monsterseditor.h \
	resources.h \
	skillsspellseditor.h \
	spritepicker.h \
	statseditor.h \
	tileset.h \
	tileseteditor.h \
	tilesetview.h \
	xmlParser.h

SOURCES += \
  battleanimation.cpp \
  battleanimationseditor.cpp \
  choosemapevent.cpp \
  colorpicker.cpp \
  configureevent_changeeventlocation.cpp \
  configureevent_changepassability.cpp \
  configureevent_teleport.cpp \
  eventview.cpp \
  globals.cpp \
  image.cpp \
  imageview.cpp \
  itemseditor.cpp \
	main.cpp \
	mainwindow.cpp \
	map.cpp \
	mapevent.cpp \
	mapeventeditor.cpp \
	mapflags.cpp \
	mapspritepicker.cpp \
	messageseditor.cpp \
	monsterseditor.cpp \
	resources.cpp \
	skillsspellseditor.cpp \
	spritepicker.cpp \
	statseditor.cpp \
	tileset.cpp \
	tileseteditor.cpp \
	tilesetview.cpp \
	xmlParser.cpp

FORMS += \
  battleanimationseditor.ui \
  choosemapevent.ui \
  colorpicker.ui \
  configureevent_battle.ui \
  configureevent_buffercharacter.ui \
  configureevent_buffervalue.ui \
  configureevent_changeeventflags.ui \
  configureevent_changeeventlocation.ui \
  configureevent_changepassability.ui \
  configureevent_changesprite.ui \
  configureevent_changevariable.ui \
  configureevent_giveplayeritem.ui \
  configureevent_ifexited.ui \
  configureevent_ifhasitem.ui \
  configureevent_ifswitch.ui \
  configureevent_ifvalue.ui \
  configureevent_ifvariable.ui \
  configureevent_ifyesno.ui \
  configureevent_moveplayer.ui \
  configureevent_playmusic.ui \
  configureevent_playsoundeffect.ui \
  configureevent_shakescreen.ui \
  configureevent_showhidescreen.ui \
  configureevent_showimage.ui \
  configureevent_showmessage.ui \
  configureevent_showshopmenu.ui \
  configureevent_switchrange.ui \
  configureevent_teleport.ui \
  itemseditor.ui \
	mainwindow.ui \
	mapeventeditor.ui \
	mapflags.ui \
	mapspritepicker.ui \
	messageseditor.ui \
	monsterseditor.ui \
	resources.ui \
	skillsspellseditor.ui \
	spritepicker.ui \
	statseditor.ui \
	tileseteditor.ui

RESOURCES += icons.qrc
