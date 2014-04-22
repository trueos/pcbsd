# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'wol.ui'
#
# Created: Wed Aug 19 19:54:17 2009
#      by: PyQt4 UI code generator 4.4.4
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

class Ui_WolWindow(object):
    def setupUi(self, WolWindow):
        WolWindow.setObjectName("WolWindow")
        WolWindow.resize(310, 206)
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap("wol.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        WolWindow.setWindowIcon(icon)
        self.centralwidget = QtGui.QWidget(WolWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.frame = QtGui.QFrame(self.centralwidget)
        self.frame.setGeometry(QtCore.QRect(10, 0, 291, 151))
        self.frame.setFrameShape(QtGui.QFrame.StyledPanel)
        self.frame.setFrameShadow(QtGui.QFrame.Raised)
        self.frame.setObjectName("frame")
        self.formLayoutWidget = QtGui.QWidget(self.frame)
        self.formLayoutWidget.setGeometry(QtCore.QRect(10, 10, 231, 101))
        self.formLayoutWidget.setObjectName("formLayoutWidget")
        self.formLayout = QtGui.QFormLayout(self.formLayoutWidget)
        self.formLayout.setObjectName("formLayout")
        self.mac_label = QtGui.QLabel(self.formLayoutWidget)
        self.mac_label.setObjectName("mac_label")
        self.formLayout.setWidget(0, QtGui.QFormLayout.LabelRole, self.mac_label)
        self.mac_lineEdit = QtGui.QLineEdit(self.formLayoutWidget)
        self.mac_lineEdit.setMaxLength(17)
        self.mac_lineEdit.setObjectName("mac_lineEdit")
        self.formLayout.setWidget(0, QtGui.QFormLayout.FieldRole, self.mac_lineEdit)
        self.ip_lineEdit = QtGui.QLineEdit(self.formLayoutWidget)
        self.ip_lineEdit.setMaxLength(15)
        self.ip_lineEdit.setObjectName("ip_lineEdit")
        self.formLayout.setWidget(1, QtGui.QFormLayout.FieldRole, self.ip_lineEdit)
        self.ip_label = QtGui.QLabel(self.formLayoutWidget)
        self.ip_label.setObjectName("ip_label")
        self.formLayout.setWidget(1, QtGui.QFormLayout.LabelRole, self.ip_label)
        self.port_label = QtGui.QLabel(self.formLayoutWidget)
        self.port_label.setObjectName("port_label")
        self.formLayout.setWidget(2, QtGui.QFormLayout.LabelRole, self.port_label)
        self.port_lineEdit = QtGui.QLineEdit(self.formLayoutWidget)
        self.port_lineEdit.setMaxLength(5)
        self.port_lineEdit.setObjectName("port_lineEdit")
        self.formLayout.setWidget(2, QtGui.QFormLayout.FieldRole, self.port_lineEdit)
        self.send_button = QtGui.QPushButton(self.frame)
        self.send_button.setGeometry(QtCore.QRect(180, 120, 61, 28))
        self.send_button.setObjectName("send_button")
        WolWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtGui.QMenuBar(WolWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 310, 27))
        self.menubar.setDefaultUp(False)
        self.menubar.setObjectName("menubar")
        self.menuFile = QtGui.QMenu(self.menubar)
        self.menuFile.setObjectName("menuFile")
        self.menuHelp = QtGui.QMenu(self.menubar)
        self.menuHelp.setObjectName("menuHelp")
        WolWindow.setMenuBar(self.menubar)
        self.statusbar = QtGui.QStatusBar(WolWindow)
        self.statusbar.setObjectName("statusbar")
        WolWindow.setStatusBar(self.statusbar)
        self.actionQuit = QtGui.QAction(WolWindow)
        self.actionQuit.setObjectName("actionQuit")
        self.actionOpen = QtGui.QAction(WolWindow)
        self.actionOpen.setObjectName("actionOpen")
        self.actionAbout = QtGui.QAction(WolWindow)
        self.actionAbout.setObjectName("actionAbout")
        self.menuFile.addAction(self.actionOpen)
        self.menuFile.addAction(self.actionQuit)
        self.menuHelp.addAction(self.actionAbout)
        self.menubar.addAction(self.menuFile.menuAction())
        self.menubar.addAction(self.menuHelp.menuAction())

        self.retranslateUi(WolWindow)
        QtCore.QObject.connect(self.actionQuit, QtCore.SIGNAL("activated()"), WolWindow.close)
        QtCore.QMetaObject.connectSlotsByName(WolWindow)

    def retranslateUi(self, WolWindow):
        WolWindow.setWindowTitle(QtGui.QApplication.translate("WolWindow", "WOL GUI", None, QtGui.QApplication.UnicodeUTF8))
        self.mac_label.setText(QtGui.QApplication.translate("WolWindow", "MAC", None, QtGui.QApplication.UnicodeUTF8))
        self.mac_lineEdit.setToolTip(QtGui.QApplication.translate("WolWindow", "MAC address of the host to wake up", None, QtGui.QApplication.UnicodeUTF8))
        self.ip_lineEdit.setToolTip(QtGui.QApplication.translate("WolWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'Bitstream Vera Sans\'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">IP address to send the <span style=\" font-style:italic;\">Magic Packet</span> to</p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
        self.ip_label.setText(QtGui.QApplication.translate("WolWindow", "IP", None, QtGui.QApplication.UnicodeUTF8))
        self.port_label.setText(QtGui.QApplication.translate("WolWindow", "Port", None, QtGui.QApplication.UnicodeUTF8))
        self.send_button.setText(QtGui.QApplication.translate("WolWindow", "Send", None, QtGui.QApplication.UnicodeUTF8))
        self.menuFile.setTitle(QtGui.QApplication.translate("WolWindow", "File", None, QtGui.QApplication.UnicodeUTF8))
        self.menuHelp.setTitle(QtGui.QApplication.translate("WolWindow", "Help", None, QtGui.QApplication.UnicodeUTF8))
        self.actionQuit.setText(QtGui.QApplication.translate("WolWindow", "Quit", None, QtGui.QApplication.UnicodeUTF8))
        self.actionOpen.setText(QtGui.QApplication.translate("WolWindow", "Open", None, QtGui.QApplication.UnicodeUTF8))
        self.actionOpen.setToolTip(QtGui.QApplication.translate("WolWindow", "Read a list of host to wake up", None, QtGui.QApplication.UnicodeUTF8))
        self.actionAbout.setText(QtGui.QApplication.translate("WolWindow", "About", None, QtGui.QApplication.UnicodeUTF8))

