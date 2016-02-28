TEMPLATE = app
TARGET = BrainStd

QT += core widgets gui network xml #opengl

HEADERS += \
    source/windows/mainwindow.h \
    source/windows/workspacetab.h \
    source/windows/newblockwindow.h \
    source/windows/controlswidget.h \
    source/windows/blockpropertieswidget.h \
    source/windows/plotwindow.h \
    source/windows/newconnectionwindow.h \
    source/windows/architecturewindow.h \
    source/windows/xmlwindow.h \
    source/windows/experimentwidget.h \
    source/windows/welcomewindow.h \
    source/windows/pythonwidget.h \
    source/windows/aboutwindow.h \
    source/graphs/graphs.h \
    source/graphs/currentgraph.h \
    source/graphs/rasterplot.h \
    source/graphs/basicgraph.h \
    source/graphs/phaseportraitgraph.h \
    source/simulation/synapticmodel.h \
    source/simulation/simulator.h \
    source/xmleditor/xmlhighlighter.h \
    source/xmleditor/highlighter.h \
    source/xmleditor/codeeditor.h \
    source/model/connection.h \
    source/model/block.h \
    source/model/textblock.h \
    source/helper/TransferEntropyCalculator.h \
    source/helper/modelValues.h \
    source/helper/helperClasses.h \
    source/helper/firingrate.h \
    source/simulation/izhikevichneuronLS.h \
    source/simulation/izhikevichneuronSTN.h \
    source/helper/userdata.h \
    source/simulation/tcpip.h \
    source/helper/tcpip_initialization.h \
    source/helper/backendData.h \
    source/graphs/plot2D.h \
    source/windows/stimulationwidget.h \
    source/windows/backendterminal.h \
    source/helper/stimulus.h

SOURCES += \
    source/main.cpp \
    source/windows/mainwindow.cpp \
    source/windows/controlswidget.cpp \
    source/windows/newblockwindow.cpp \
    source/windows/plotwindow.cpp \
    source/windows/newconnectionwindow.cpp \
    source/windows/blockpropertieswidget.cpp \
    source/windows/architecturewindow.cpp \
    source/windows/xmlwindow.cpp \
    source/windows/workspacetab.cpp \
    source/windows/experimentwidget.cpp \
    source/windows/welcomewindow.cpp \
    source/windows/pythonwidget.cpp \
    source/windows/mainwindow_saveload.cpp \
    source/windows/architecturewindow_xmpparser.cpp \
    source/windows/architecturewindow_drawConnections.cpp \
    source/windows/aboutwindow.cpp \
    source/graphs/graphs.cpp \
    source/graphs/currentgraph.cpp \
    source/graphs/rasterplot.cpp \
    source/graphs/basicgraph.cpp \
    source/graphs/phaseportraitgraph.cpp \
    source/simulation/tcpip.cpp \
    source/xmleditor/xmlhighlighter.cpp \
    source/xmleditor/highlighter.cpp \
    source/xmleditor/codeeditor.cpp \
    source/model/block.cpp \
    source/model/textblock.cpp \
    source/helper/TransferEntropyCalculator.cpp \
    source/helper/tcpip_initialization.cpp \
    source/graphs/plot2D.cpp \
    source/windows/stimulationwidget.cpp \
    source/windows/backendterminal.cpp

FORMS += \
    source/windows/mainwindow.ui \
    source/windows/controlswidget.ui \
    source/windows/plotwindow.ui \
    source/windows/newblockwindow.ui \
    source/windows/newconnectionwindow.ui \
    source/windows/blockpropertieswidget.ui \
    source/windows/workspacetab.ui \
    source/windows/architecturewindow.ui \
    source/windows/experimentwidget.ui \
    source/windows/welcomewindow.ui \
    source/windows/pythonwidget.ui \
    source/windows/stimulationwidget.ui \
    source/windows/BackEndTerminalDock.ui \
    source/windows/aboutwindow.ui

RESOURCES += source/icons.qrc

OTHER_FILES += workspace/config.ini

































