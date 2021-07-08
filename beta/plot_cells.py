"""
Provide simple plotting functionality for PhysiCell output results.

Authors:
Randy Heiland (heiland@iu.edu)
Dr. Paul Macklin (macklinp@iu.edu)

--- Versions ---
0.1 - initial version
"""
# https://doc.qt.io/qtforpython/gettingstarted.html

import os
import sys
import getopt
import shutil
from pathlib import Path
import xml.etree.ElementTree as ET  # https://docs.python.org/2/library/xml.etree.elementtree.html
from xml.dom import minidom

from PyQt5 import QtCore, QtGui
from PyQt5.QtWidgets import *

from vis_tab_cells_only import Vis 

def SingleBrowse(self):
        # if len(self.csv) < 2:
    filePath = QFileDialog.getOpenFileName(self,'',".",'*.xml')

        #     if filePath != "" and not filePath in self.csv:
        #         self.csv.append(filePath)
        # print(self.csv)
  
class PhysiCellXMLCreator(QWidget):
    # def __init__(self, parent = None):
    def __init__(self, parent = None):
        super(PhysiCellXMLCreator, self).__init__(parent)

        self.title_prefix = "PhysiCell Visualization"
        self.setWindowTitle(self.title_prefix)

        # Menus
        vlayout = QVBoxLayout(self)
        # vlayout.setContentsMargins(5, 35, 5, 5)
        # menuWidget = QWidget(self.menu())
        # vlayout.addWidget(menuWidget)

        # self.setWindowIcon(self.style().standardIcon(getattr(QStyle, 'SP_DialogNoButton')))
        # self.setWindowIcon(QtGui.QIcon('physicell_logo_25pct.png'))
        # self.grid = QGridLayout()
        # lay.addLayout(self.grid)
        self.setLayout(vlayout)
        # self.setMinimumSize(400, 790)  # width, height (height >= Cell Types|Death params)
        # self.setMinimumSize(400, 500)  # width, height (height >= Cell Types|Death params)
        # self.setMinimumSize(800, 620)  # width, height (height >= Cell Types|Death params)
        # self.setMinimumSize(800, 660)  # width, height (height >= Cell Types|Death params)
        # self.setMinimumSize(800, 800)  # width, height (height >= Cell Types|Death params)
        self.setMinimumSize(700, 770)  # width, height (height >= Cell Types|Death params)
        # self.setMinimumSize(600, 600)  # width, height (height >= Cell Types|Death params)
        # self.resize(400, 790)  # width, height (height >= Cell Types|Death params)

        # self.menubar = QtWidgets.QMenuBar(self)
        # self.file_menu = QtWidgets.QMenu('File')
        # self.file_menu.insertAction("Open")
        # self.menubar.addMenu(self.file_menu)

        # GUI tabs

        # By default, let's startup the app with a default of template2D (a copy)
        # self.new_model_cb()  # default on startup
        # read_file = "../data/subcellular_flat.xml"
        # read_file = "../data/cancer_biorobots_flat.xml"
        # read_file = "../data/pred_prey_flat.xml"

        model_name = "pred_prey_flat"
        model_name = "biorobots_flat"
        model_name = "cancer_biorobots_flat"
        model_name = "test1"
        model_name = "test-gui"
        model_name = "covid19_v5_flat"
        model_name = "template"
        # model_name = "randy_test"  #rwh
        # read_file = "data/" + model_name + ".xml"

        # then what??
        # binDirectory = os.path.realpath(os.path.abspath(__file__))
        binDirectory = os.path.dirname(os.path.abspath(__file__))
        dataDirectory = os.path.join(binDirectory,'..','data')

        # self.tree = ET.parse(self.config_file)
        # self.xml_root = self.tree.getroot()


        #------------------
        tabWidget = QTabWidget()
        self.vis_tab = Vis()
        # self.vis_tab.xml_root = self.xml_root
        tabWidget.addTab(self.vis_tab,"Plot")

        vlayout.addWidget(tabWidget)
        # self.addTab(self.sbml_tab,"SBML")

        tabWidget.setCurrentIndex(0)  # Config (default)


    def menu(self):
        menubar = QMenuBar(self)
        menubar.setNativeMenuBar(False)

        #--------------
        file_menu = menubar.addMenu('&File')

        # # file_menu.addAction("Open", self.open_as_cb, QtGui.QKeySequence('Ctrl+o'))
        # file_menu.addAction("Save", self.save_cb, QtGui.QKeySequence('Ctrl+s'))

        menubar.adjustSize()  # Argh. Otherwise, only 1st menu appears, with ">>" to others!

def main():
    inputfile = ''

    app = QApplication(sys.argv)
    ex = PhysiCellXMLCreator()
    # ex.setGeometry(100,100, 800,600)
    ex.show()
    sys.exit(app.exec_())
	
if __name__ == '__main__':
    main()