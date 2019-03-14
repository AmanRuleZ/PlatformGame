import sys
from PyQt5.QtWidgets import QApplication, QWidget, QMainWindow, QMessageBox, QScrollBar, QInputDialog
from PyQt5.QtGui import QIcon, QPixmap
from PyQt5.uic import loadUi
from PyQt5.QtCore import QSize, Qt
from functools import partial
import os
import numpy
import re
import random
from datetime import datetime

random.seed(datetime.now())


class Example(QMainWindow):
    def __init__(self):
        super().__init__()
        self.background = QWidget(self)
        self.background.setGeometry(30, 20, 609, 321)
        self.background.setAutoFillBackground(True)
        p = self.background.palette()
        p.setColor(self.background.backgroundRole(), Qt.cyan)
        self.background.setPalette(p)
        loadUi("untitled.ui", self)
        self.initUI()

    def initUI(self):
        self.resize(1200, 650)
        self.move(300, 200)
        self.setWindowTitle('Map Editor v0.1')
        self.setWindowIcon(QIcon('web.png'))
        self.actualTexture = 0
        self.mapInfo = numpy.zeros(4, dtype=int) # Wielkość X mapy, wielkość Y mapy, pozycja X portalu, pozycja Y portalu
        self.mtiles = {}
        self.mtilesHp = {}
        self.mpickups = {}
        self.mtraps = {}
        self.menemies = {}
        self.playerPos = {}

        # MENU
        self.flipButton.hide()
        self.hpLabel.hide()
        self.hpEditor.hide()
        self.armorLabel.hide()
        self.armorEditor.hide()
        self.mhpl.hide()  # max hp label
        self.marl.hide()  # max armor label
        self.mhpe.hide()  # max hp editor
        self.mare.hide()  # max armor editor

        self.hpEditor.valueChanged.connect(self.setMaxParameters)
        self.armorEditor.valueChanged.connect(self.setMaxParameters)

        self.playerInfo = numpy.zeros(6, dtype=int)

        # scrolle do przewijania mapy
        self.scrollX = QScrollBar(Qt.Horizontal, self)
        self.scrollX.setGeometry(30, 341, 609, 16)
        self.scrollY = QScrollBar(Qt.Vertical, self)
        self.scrollY.setGeometry(638, 20, 16, 320)
        self.scrollX.valueChanged.connect(self.prepareMap)
        self.scrollY.valueChanged.connect(self.prepareMap)

        self.checkButton.clicked.connect(self.checkInfo)

        self.button = 1

        self.tiles = [self.tile1, self.tile2, self.tile3, self.tile4, self.tile5, self.tile6, self.tile7, self.tile8,
                      self.tile9, self.tile10,
                      self.tile11, self.tile12, self.tile13, self.tile14, self.tile15, self.tile16, self.tile17,
                      self.tile18, self.tile19, self.tile20,
                      self.tile21, self.tile22, self.tile23, self.tile24, self.tile25, self.tile26, self.tile27,
                      self.tile28, self.tile29, self.tile30,
                      self.tile31, self.tile32, self.tile33, self.tile34, self.tile35, self.tile36, self.tile37,
                      self.tile38, self.tile39, self.tile40,
                      self.tile41, self.tile42, self.tile43, self.tile44, self.tile45, self.tile46, self.tile47,
                      self.tile48, self.tile49, self.tile50,
                      self.tile51, self.tile52, self.tile53, self.tile54, self.tile55, self.tile56, self.tile57,
                      self.tile58, self.tile59, self.tile60,
                      self.tile61, self.tile62, self.tile63, self.tile64, self.tile65, self.tile66, self.tile67,
                      self.tile68, self.tile69, self.tile70,
                      self.tile71, self.tile72, self.tile73, self.tile74, self.tile75, self.tile76, self.tile77,
                      self.tile78, self.tile79, self.tile80,
                      self.tile81, self.tile82, self.tile83, self.tile84, self.tile85, self.tile86, self.tile87,
                      self.tile88, self.tile89, self.tile90,
                      self.tile91, self.tile92, self.tile93, self.tile94, self.tile95, self.tile96, self.tile97,
                      self.tile98, self.tile99, self.tile100,
                      self.tile101, self.tile102, self.tile103, self.tile104, self.tile105, self.tile106, self.tile107,
                      self.tile108, self.tile109, self.tile110,
                      self.tile111, self.tile112, self.tile113, self.tile114, self.tile115, self.tile116, self.tile117,
                      self.tile118, self.tile119, self.tile120,
                      self.tile121, self.tile122, self.tile123, self.tile124, self.tile125, self.tile126, self.tile127,
                      self.tile128, self.tile129, self.tile130,
                      self.tile131, self.tile132, self.tile133, self.tile134, self.tile135, self.tile136, self.tile137,
                      self.tile138, self.tile139, self.tile140,
                      self.tile141, self.tile142, self.tile143, self.tile144, self.tile145, self.tile146, self.tile147,
                      self.tile148, self.tile149, self.tile150,
                      self.tile151, self.tile152, self.tile153, self.tile154, self.tile155, self.tile156, self.tile157,
                      self.tile158, self.tile159, self.tile160,
                      self.tile161, self.tile162, self.tile163, self.tile164, self.tile165, self.tile166, self.tile167,
                      self.tile168, self.tile169, self.tile170,
                      self.tile171, self.tile172, self.tile173, self.tile174, self.tile175, self.tile176, self.tile177,
                      self.tile178, self.tile179, self.tile180,
                      self.tile181, self.tile182, self.tile183, self.tile184, self.tile185, self.tile186, self.tile187,
                      self.tile188, self.tile189, self.tile190]

        self.enemies = [self.enemy1, self.enemy2, self.enemy3, self.enemy4, self.enemy5, self.enemy6, self.enemy7,
                        self.enemy8, self.enemy9, self.enemy10,
                        self.enemy11, self.enemy12, self.enemy13, self.enemy14, self.enemy15, self.enemy16,
                        self.enemy17, self.enemy18, self.enemy19, self.enemy20,
                        self.enemy21, self.enemy22, self.enemy23, self.enemy24, self.enemy25, self.enemy26,
                        self.enemy27, self.enemy28, self.enemy29, self.enemy30,
                        self.enemy31, self.enemy32, self.enemy33, self.enemy34, self.enemy35, self.enemy36,
                        self.enemy37, self.enemy38, self.enemy39, self.enemy40,
                        self.enemy41, self.enemy42, self.enemy43, self.enemy44, self.enemy45, self.enemy46,
                        self.enemy47, self.enemy48, self.enemy49, self.enemy50,
                        self.enemy51, self.enemy52, self.enemy53, self.enemy54, self.enemy55, self.enemy56,
                        self.enemy57, self.enemy58, self.enemy59, self.enemy60,
                        self.enemy61, self.enemy62, self.enemy63, self.enemy64, self.enemy65, self.enemy66,
                        self.enemy67, self.enemy68, self.enemy69, self.enemy70,
                        self.enemy71, self.enemy72, self.enemy73, self.enemy74, self.enemy75, self.enemy76,
                        self.enemy77, self.enemy78, self.enemy79, self.enemy80,
                        self.enemy81, self.enemy82, self.enemy83, self.enemy84, self.enemy85, self.enemy86,
                        self.enemy87, self.enemy88, self.enemy89, self.enemy90,
                        self.enemy91, self.enemy92, self.enemy93, self.enemy94, self.enemy95, self.enemy96,
                        self.enemy97, self.enemy98, self.enemy99, self.enemy100,
                        self.enemy101, self.enemy102, self.enemy103, self.enemy104, self.enemy105, self.enemy106,
                        self.enemy107, self.enemy108, self.enemy109, self.enemy110,
                        self.enemy111, self.enemy112, self.enemy113, self.enemy114, self.enemy115, self.enemy116,
                        self.enemy117, self.enemy118, self.enemy119, self.enemy120,
                        self.enemy121, self.enemy122, self.enemy123, self.enemy124, self.enemy125, self.enemy126,
                        self.enemy127, self.enemy128, self.enemy129, self.enemy130,
                        self.enemy131, self.enemy132, self.enemy133, self.enemy134, self.enemy135, self.enemy136,
                        self.enemy137, self.enemy138, self.enemy139, self.enemy140,
                        self.enemy141, self.enemy142, self.enemy143, self.enemy144, self.enemy145, self.enemy146,
                        self.enemy147, self.enemy148, self.enemy149, self.enemy150,
                        self.enemy151, self.enemy152, self.enemy153, self.enemy154, self.enemy155, self.enemy156,
                        self.enemy157, self.enemy158, self.enemy159, self.enemy160,
                        self.enemy161, self.enemy162, self.enemy163, self.enemy164, self.enemy165, self.enemy166,
                        self.enemy167, self.enemy168, self.enemy169, self.enemy170,
                        self.enemy171, self.enemy172, self.enemy173, self.enemy174, self.enemy175, self.enemy176,
                        self.enemy177, self.enemy178, self.enemy179, self.enemy180,
                        self.enemy181, self.enemy182, self.enemy183, self.enemy184, self.enemy185, self.enemy186,
                        self.enemy187, self.enemy188, self.enemy189, self.enemy190]

        self.pickups = [self.pickup1, self.pickup2, self.pickup3, self.pickup4, self.pickup5, self.pickup6,
                        self.pickup7, self.pickup8, self.pickup9, self.pickup10,
                        self.pickup11, self.pickup12, self.pickup13, self.pickup14, self.pickup15, self.pickup16,
                        self.pickup17, self.pickup18, self.pickup19, self.pickup20,
                        self.pickup21, self.pickup22, self.pickup23, self.pickup24, self.pickup25, self.pickup26,
                        self.pickup27, self.pickup28, self.pickup29, self.pickup30,
                        self.pickup31, self.pickup32, self.pickup33, self.pickup34, self.pickup35, self.pickup36,
                        self.pickup37, self.pickup38, self.pickup39, self.pickup40,
                        self.pickup41, self.pickup42, self.pickup43, self.pickup44, self.pickup45, self.pickup46,
                        self.pickup47, self.pickup48, self.pickup49, self.pickup50,
                        self.pickup51, self.pickup52, self.pickup53, self.pickup54, self.pickup55, self.pickup56,
                        self.pickup57, self.pickup58, self.pickup59, self.pickup60,
                        self.pickup61, self.pickup62, self.pickup63, self.pickup64, self.pickup65, self.pickup66,
                        self.pickup67, self.pickup68, self.pickup69, self.pickup70,
                        self.pickup71, self.pickup72, self.pickup73, self.pickup74, self.pickup75, self.pickup76,
                        self.pickup77, self.pickup78, self.pickup79, self.pickup80,
                        self.pickup81, self.pickup82, self.pickup83, self.pickup84, self.pickup85, self.pickup86,
                        self.pickup87, self.pickup88, self.pickup89, self.pickup90,
                        self.pickup91, self.pickup92, self.pickup93, self.pickup94, self.pickup95, self.pickup96,
                        self.pickup97, self.pickup98, self.pickup99, self.pickup100,
                        self.pickup101, self.pickup102, self.pickup103, self.pickup104, self.pickup105, self.pickup106,
                        self.pickup107, self.pickup108, self.pickup109, self.pickup110,
                        self.pickup111, self.pickup112, self.pickup113, self.pickup114, self.pickup115, self.pickup116,
                        self.pickup117, self.pickup118, self.pickup119, self.pickup120,
                        self.pickup121, self.pickup122, self.pickup123, self.pickup124, self.pickup125, self.pickup126,
                        self.pickup127, self.pickup128, self.pickup129, self.pickup130,
                        self.pickup131, self.pickup132, self.pickup133, self.pickup134, self.pickup135, self.pickup136,
                        self.pickup137, self.pickup138, self.pickup139, self.pickup140,
                        self.pickup141, self.pickup142, self.pickup143, self.pickup144, self.pickup145, self.pickup146,
                        self.pickup147, self.pickup148, self.pickup149, self.pickup150,
                        self.pickup151, self.pickup152, self.pickup153, self.pickup154, self.pickup155, self.pickup156,
                        self.pickup157, self.pickup158, self.pickup159, self.pickup160,
                        self.pickup161, self.pickup162, self.pickup163, self.pickup164, self.pickup165, self.pickup166,
                        self.pickup167, self.pickup168, self.pickup169, self.pickup170,
                        self.pickup171, self.pickup172, self.pickup173, self.pickup174, self.pickup175, self.pickup176,
                        self.pickup177, self.pickup178, self.pickup179, self.pickup180,
                        self.pickup181, self.pickup182, self.pickup183, self.pickup184, self.pickup185, self.pickup186,
                        self.pickup187, self.pickup188, self.pickup189, self.pickup190]

        self.traps = [self.trap1, self.trap2, self.trap3, self.trap4, self.trap5, self.trap6, self.trap7, self.trap8,
                      self.trap9, self.trap10,
                      self.trap11, self.trap12, self.trap13, self.trap14, self.trap15, self.trap16, self.trap17,
                      self.trap18, self.trap19, self.trap20,
                      self.trap21, self.trap22, self.trap23, self.trap24, self.trap25, self.trap26, self.trap27,
                      self.trap28, self.trap29, self.trap30,
                      self.trap31, self.trap32, self.trap33, self.trap34, self.trap35, self.trap36, self.trap37,
                      self.trap38, self.trap39, self.trap40,
                      self.trap41, self.trap42, self.trap43, self.trap44, self.trap45, self.trap46, self.trap47,
                      self.trap48, self.trap49, self.trap50,
                      self.trap51, self.trap52, self.trap53, self.trap54, self.trap55, self.trap56, self.trap57,
                      self.trap58, self.trap59, self.trap60,
                      self.trap61, self.trap62, self.trap63, self.trap64, self.trap65, self.trap66, self.trap67,
                      self.trap68, self.trap69, self.trap70,
                      self.trap71, self.trap72, self.trap73, self.trap74, self.trap75, self.trap76, self.trap77,
                      self.trap78, self.trap79, self.trap80,
                      self.trap81, self.trap82, self.trap83, self.trap84, self.trap85, self.trap86, self.trap87,
                      self.trap88, self.trap89, self.trap90,
                      self.trap91, self.trap92, self.trap93, self.trap94, self.trap95, self.trap96, self.trap97,
                      self.trap98, self.trap99, self.trap100,
                      self.trap101, self.trap102, self.trap103, self.trap104, self.trap105, self.trap106, self.trap107,
                      self.trap108, self.trap109, self.trap110,
                      self.trap111, self.trap112, self.trap113, self.trap114, self.trap115, self.trap116, self.trap117,
                      self.trap118, self.trap119, self.trap120,
                      self.trap121, self.trap122, self.trap123, self.trap124, self.trap125, self.trap126, self.trap127,
                      self.trap128, self.trap129, self.trap130,
                      self.trap131, self.trap132, self.trap133, self.trap134, self.trap135, self.trap136, self.trap137,
                      self.trap138, self.trap139, self.trap140,
                      self.trap141, self.trap142, self.trap143, self.trap144, self.trap145, self.trap146, self.trap147,
                      self.trap148, self.trap149, self.trap150,
                      self.trap151, self.trap152, self.trap153, self.trap154, self.trap155, self.trap156, self.trap157,
                      self.trap158, self.trap159, self.trap160,
                      self.trap161, self.trap162, self.trap163, self.trap164, self.trap165, self.trap166, self.trap167,
                      self.trap168, self.trap169, self.trap170,
                      self.trap171, self.trap172, self.trap173, self.trap174, self.trap175, self.trap176, self.trap177,
                      self.trap178, self.trap179, self.trap180,
                      self.trap181, self.trap182, self.trap183, self.trap184, self.trap185, self.trap186, self.trap187,
                      self.trap188, self.trap189, self.trap190]

        self.names = ["Brick", "Box", "AK", "Pistol", "Rock", "Shield", "Heart", "Dog", "Soldier", "Spikes", "Shooter",
                      "Shooter", "Shooter", "Shooter", "Player", "Portal"]

        self.pixmaps = ["./textures/blank.png", "./textures/brick.png", "./textures/box.png", "./textures/ak.png",
                        "./textures/pistol.png", "./textures/rock.png", "./textures/armor.png", "./textures/heart.png",
                        "./textures/dog.png", "./textures/enemy.png",
                        "./textures/spikes.png", "./textures/shooter1.png", "./textures/shooter2.png",
                        "./textures/shooter3.png", "./textures/shooter4.png", "./textures/spawn.png", "./textures/portal.png"]

        self.bpixmaps = ["./textures/brick.png", "./textures/box.png", "./textures/ak.png",
                         "./textures/pistol.png", "./textures/rock.png", "./textures/armor.png", "./textures/heart.png",
                         "./textures/dog.png", "./textures/enemy.png",
                         "./textures/spikes.png", "./textures/shooter1.png", "./textures/shooter2.png",
                         "./textures/shooter3.png", "./textures/shooter4.png", "./textures/player.png", "./textures/portal.png"]

        # obrót shootera
        self.flip = 0
        self.shooterFlip = ["./textures/shooter1.png", "./textures/shooter2.png", "./textures/shooter3.png",
                            "./textures/shooter4.png"]
        self.flipButton.clicked.connect(self.changeShooterFlip)

        self.buttons = [self.l1, self.l2, self.l3, self.l4, self.l5, self.l6, self.l7, self.l8, self.l9, self.l10,
                        self.l11, self.spawnpointButton, self.endMapButton]
        self.l8.setIconSize(QSize(64, 64))
        self.l9.setIconSize(QSize(64, 64))
        self.confirmSizeButton.clicked.connect(self.makeArray)

        self.saveButton.clicked.connect(self.save)

        for button, pixmap, i in zip(self.buttons, self.bpixmaps, range(1, len(self.buttons) + 1)):
            if i < 12:
                button.setIcon(QIcon(QPixmap(pixmap)))
                button.setIconSize(QSize(32, 32))
                if i > 8:
                    button.setIconSize(QSize(64, 64))
                button.clicked.connect(partial(self.makeMenu, i))
            else:
                button.clicked.connect(partial(self.makeMenu, i+3))

        self.makeArray()

        self.show()

    def changeShooterFlip(self):
        self.flip = (self.flip + 1) % 4
        self.makeMenu(11 + self.flip)

    def makeMenu(self, i):
        print(i)
        self.infoLabel.setText('')
        self.actualTexture = i

        self.nameLabel.setText(self.names[i - 1])
        self.podglad.setPixmap(QPixmap(self.pixmaps[i]))

        self.hpLabel.hide()
        self.hpEditor.hide()
        self.flipButton.hide()
        self.armorLabel.hide()
        self.armorEditor.hide()
        self.mhpl.hide()
        self.marl.hide()
        self.mhpe.hide()
        self.mare.hide()
        if i == 2:
            self.hpLabel.show()
            self.hpEditor.show()
        if i == 11:
            self.flip = 0
        if 11<=i<15:
            self.flipButton.show()
        if i == 8 or i == 9:
            self.hpLabel.show()
            self.hpEditor.show()
            self.armorLabel.show()
            self.armorEditor.show()
        if i == 15:
            self.hpLabel.show()
            self.hpEditor.show()
            self.armorLabel.show()
            self.armorEditor.show()
            self.mhpl.show()
            self.marl.show()
            self.mhpe.show()
            self.mare.show()

    def setMaxParameters(self):
        self.mare.setMinimum(self.armorEditor.value())
        self.mhpe.setMinimum(self.hpEditor.value())

    def makeArray(self):
        allOk = 0
        sizeX = self.sizeX.toPlainText()
        sizeY = self.sizeY.toPlainText()
        try:
            self.mapInfo[0] = (int)(sizeX)
            self.mapInfo[1] = (int)(sizeY)
            allOk = 1
        except ValueError:
            msg = QMessageBox(self)
            msg.setIcon(QMessageBox.Information)
            msg.setWindowTitle("Information")
            msg.setText("Entered data must be numbers!")
            msg.show()

        if allOk:
            if self.mapInfo[0] >= 19 and self.mapInfo[1] >= 10:

                self.mtiles = numpy.zeros((self.mapInfo[0], self.mapInfo[1]), dtype=int)
                self.mtilesHp = numpy.zeros((self.mapInfo[0], self.mapInfo[1]), dtype=int)
                self.mpickups = numpy.zeros((self.mapInfo[0], self.mapInfo[1]), dtype=int)
                self.mtraps = numpy.zeros((self.mapInfo[0], self.mapInfo[1]), dtype=int)
                self.menemies = numpy.zeros((self.mapInfo[0], self.mapInfo[1]), dtype=int)
                self.playerPos = numpy.zeros((self.mapInfo[0], self.mapInfo[1]), dtype=int)

                self.scrollX.setMaximum(self.mapInfo[0] - 19)
                self.scrollY.setMaximum(self.mapInfo[1] - 10)

                msg = QMessageBox(self)
                msg.setIcon(QMessageBox.Information)
                msg.setWindowTitle("Information")
                msg.setText("Current dimensions: " + sizeX + "x" + sizeY)
                msg.show()
                self.prepareMap()
            else:
                msg = QMessageBox(self)
                msg.setIcon(QMessageBox.Information)
                msg.setWindowTitle("Information")
                msg.setText("Dimensions must be >= 19x10")
                msg.show()

    def prepareMap(self):
        for i in range(19):
            for j in range(10):
                self.tiles[19 * j + i].setPixmap(
                    QPixmap(self.pixmaps[self.mtiles[i + self.scrollX.value()][j + self.scrollY.value()]]))
                self.pickups[19 * j + i].setPixmap(
                    QPixmap(self.pixmaps[self.mpickups[i + self.scrollX.value()][j + self.scrollY.value()]]))
                self.traps[19 * j + i].setPixmap(
                    QPixmap(self.pixmaps[self.mtraps[i + self.scrollX.value()][j + self.scrollY.value()]]))

                pom = self.menemies[i + self.scrollX.value()][j + self.scrollY.value()] / 100
                self.enemies[19 * j + i].setPixmap(QPixmap(self.pixmaps[int(pom)]).scaled(32,32,Qt.KeepAspectRatio))
                if self.playerPos[i + self.scrollX.value()][j + self.scrollY.value()] > 0:
                    self.enemies[19 * j + i].setPixmap(QPixmap(self.pixmaps[self.playerPos[i + self.scrollX.value()][j + self.scrollY.value()]]))

    def mouseMoveEvent(self, event):
        self.addItem(event)


    def save(self):
        if self.mapInfo[2] == 0 and self.mapInfo[3] == 0:
            msg = QMessageBox(self)
            msg.setIcon(QMessageBox.Information)
            msg.setWindowTitle("Information")
            msg.setText("Set portal(end of the map)!")
            msg.show()
        elif self.playerInfo[3] == 0:
            msg = QMessageBox(self)
            msg.setIcon(QMessageBox.Information)
            msg.setWindowTitle("Information")
            msg.setText("Set spawnpoint!")
            msg.show()
        else:
            path = QInputDialog.getText(self, "Save map", "Please input map name:")
            if path[1]:
                if not os.path.exists("./levels/" + path[0]):
                    os.makedirs("./levels/"+path[0])


                    text = " ".join(str(x) for x in self.mtiles)
                    text = text.translate({ord('['): ''})
                    text = text.translate({ord(']'): ''})
                    text = re.sub('0', '00', text).strip()
                    text = re.sub('1', '01', text).strip()
                    text = re.sub('2', '02', text).strip()
                    text = text.replace("  ", " ")
                    text = text.replace("  ", " ")
                    text = text.replace("  ", " ")
                    text = text.replace("  ", " ")
                    text = text.replace("  ", " ")
                    text = text.replace("\n","")

                    self.saveFile("./levels/" + path[0] + "/map.map", text)

                    text = " ".join(str(x) for x in self.mtilesHp)
                    text = text.translate({ord('['): ''})
                    text = text.translate({ord(']'): ''})
                    text = text.replace("  ", " ")
                    text = text.replace("  ", " ")
                    text = text.replace("  ", " ")
                    text = text.replace("  ", " ")
                    text = text.replace("  ", " ")
                    text = text.replace("\n","")

                    self.saveFile("./levels/" + path[0] + "/map.hp", text)

                    text = " ".join(str(x) for x in self.mtraps)
                    text = text.translate({ord('['): ''})
                    text = text.translate({ord(']'): ''})
                    text = re.sub('10', '1', text).strip()
                    text = re.sub('11', '2', text).strip()
                    text = re.sub('12', '3', text).strip()
                    text = re.sub('13', '4', text).strip()
                    text = re.sub('14', '5', text).strip()
                    text = text.replace("  ", " ")
                    text = text.replace("  ", " ")
                    text = text.replace("  ", " ")
                    text = text.replace("  ", " ")
                    text = text.replace("  ", " ")
                    text = text.replace("\n","")

                    self.saveFile("./levels/" + path[0] + "/map.trap", text)

                    text = " ".join(str(x) for x in self.mpickups)
                    text = text.translate({ord('['): ''})
                    text = text.translate({ord(']'): ''})
                    text = re.sub('7', '1', text).strip()
                    text = re.sub('6', '2', text).strip()
                    text = re.sub('3', '7', text).strip()
                    text = re.sub('5', '3', text).strip()
                    text = re.sub('7', '5', text).strip()
                    text = text.replace("  ", " ")
                    text = text.replace("  ", " ")
                    text = text.replace("  ", " ")
                    text = text.replace("  ", " ")
                    text = text.replace("  ", " ")
                    text = text.replace("\n","")

                    self.saveFile("./levels/" + path[0] + "/map.pck", text)

                    text = " ".join(str(x) for x in self.menemies)
                    #text = re.sub('0', '000', text).strip()
                    text = re.sub('   ', ' ', text).strip()
                    text = re.sub('   ', ' ', text).strip()
                    text = text.translate({ord('['): ''})
                    text = text.translate({ord(']'): ''})
                    text = text.replace("  ", " ")
                    text = text.replace("  ", " ")
                    text = text.replace("  ", " ")
                    text = text.replace("  ", " ")
                    text = text.replace("  ", " ")
                    text = text.replace("\n","")


                    self.saveFile("./levels/" + path[0] + "/map.enm", text)

                    text = " ".join(str(x) for x in self.playerInfo)
                    text = text.replace("  ", " ")
                    text = text.replace("  ", " ")
                    text = text.replace("  ", " ")
                    text = text.replace("  ", " ")
                    text = text.replace("  ", " ")
                    text = text.replace("\n","")

                    self.saveFile("./levels/" + path[0] + "/player.info", text)

                    text = " ".join(str(x) for x in self.mapInfo)
                    text = text.translate({ord('['): ''})
                    text = text.translate({ord(']'): ''})
                    text = text.replace("  ", " ")
                    text = text.replace("  ", " ")
                    text = text.replace("  ", " ")
                    text = text.replace("  ", " ")
                    text = text.replace("  ", " ")
                    text = text.replace("\n","")

                    self.saveFile("./levels/" + path[0] + "/map.info", text)
                    
                    msg = QMessageBox(self)
                    msg.setIcon(QMessageBox.Information)
                    msg.setWindowTitle("Information")
                    msg.setText("Map created successfully!")
                    msg.show()
                else:
                    msg = QMessageBox(self)
                    msg.setIcon(QMessageBox.Information)
                    msg.setWindowTitle("Information")
                    msg.setText("A map with this name exists!")
                    msg.show()

    def saveFile(self, fileName, text):
        f = open(fileName, "a")
        f.write(text)

    def mousePressEvent(self, event):
        if event.button() == 1:
            self.button = 1
        else:
            self.button = 0
        self.addItem(event)

    def addItem(self, event):
        for i in range(19):
            for j in range(10):
                if (i + 1) * 32 + 30 >= event.x() >= i * 32 + 30 and (j + 1) * 32 + 20 >= event.y() >= j * 32 + 20:
                    if (self.button):
                        if 3 > self.actualTexture > 0:
                            self.mtiles[i + self.scrollX.value()][j + self.scrollY.value()] = self.actualTexture
                            if self.actualTexture==2:
                                pom = self.hpEditor.value()/100.0
                                self.mtilesHp[i + self.scrollX.value()][j + self.scrollY.value()] = int(pom)
                        if 8 > self.actualTexture > 2:
                            self.mpickups[i + self.scrollX.value()][j + self.scrollY.value()] = self.actualTexture
                        if 10 > self.actualTexture > 7:
                            self.menemies[i + self.scrollX.value()][
                                j + self.scrollY.value()] = 100 * self.actualTexture + self.hpEditor.value() / 10 + self.armorEditor.value() / 100
                            if self.playerPos[i + self.scrollX.value()][j + self.scrollY.value()] != 0:
                                self.playerPos[i + self.scrollX.value()][j + self.scrollY.value()] = 0
                                self.playerInfo = numpy.zeros(6, dtype=int)
                        if 15 > self.actualTexture > 9:
                            self.mtraps[i + self.scrollX.value()][j + self.scrollY.value()] = self.actualTexture
                        if self.actualTexture == 15:
                            self.playerPos[self.playerInfo[0]][self.playerInfo[1]] = 0
                            self.playerInfo[0] = i + self.scrollX.value()
                            self.playerInfo[1] = j + self.scrollY.value()
                            self.playerInfo[2] = self.hpEditor.value()/100
                            self.playerInfo[3] = self.mhpe.value()/100
                            self.playerInfo[4] = self.armorEditor.value()/100
                            self.playerInfo[5] = self.mare.value()/100
                            self.playerPos[self.playerInfo[0]][self.playerInfo[1]] = 0
                            self.playerPos[i + self.scrollX.value()][j + self.scrollY.value()] = self.actualTexture
                            self.menemies[i + self.scrollX.value()][j + self.scrollY.value()] = 0
                            self.prepareMap()

                        if self.actualTexture == 16:
                            self.playerPos[self.mapInfo[2]][self.mapInfo[3]] = 0
                            self.mapInfo[2] = i + self.scrollX.value()
                            self.mapInfo[3] = j + self.scrollY.value()
                            self.playerPos[i + self.scrollX.value()][j + self.scrollY.value()] = self.actualTexture
                            self.menemies[i + self.scrollX.value()][j + self.scrollY.value()] = 0
                            self.prepareMap()

                        if self.actualTexture == 0:
                            if (self.playerPos[i + self.scrollX.value()][j + self.scrollY.value()]) != 0:
                                text = "HP: " + str(self.playerInfo[2]*100) + "  Max HP: " + str(
                                    self.playerInfo[3]*100) + "\nArmor: " + str(self.playerInfo[4]*100) + "  Max Armor: " + str(
                                    self.playerInfo[5]*100)
                                self.infoLabel.setText(text)
                            if (self.mtilesHp[i + self.scrollX.value()][j + self.scrollY.value()]) != 0:
                                text = "HP: " + str(
                                    self.mtilesHp[i + self.scrollX.value()][j + self.scrollY.value()] * 100)
                                self.infoLabel.setText(text)
                            if (self.menemies[i + self.scrollX.value()][j + self.scrollY.value()]) != 0:
                                pom = str(self.menemies[i + self.scrollX.value()][j + self.scrollY.value()])
                                text = "HP: " + str(int(pom[1]) * 100) + "\nArmor: " + str(int(pom[2]) * 100)
                                self.infoLabel.setText(text)
                    else:
                        self.mtiles[i + self.scrollX.value()][j + self.scrollY.value()] = 0
                        self.mpickups[i + self.scrollX.value()][j + self.scrollY.value()] = 0
                        self.mtraps[i + self.scrollX.value()][j + self.scrollY.value()] = 0
                        self.menemies[i + self.scrollX.value()][j + self.scrollY.value()] = 0
                        if self.playerPos[i + self.scrollX.value()][j + self.scrollY.value()] != 0:
                            self.playerPos[i + self.scrollX.value()][j + self.scrollY.value()] = 0
                            self.playerInfo = numpy.zeros(6, dtype=int)

        self.prepareMap()
        self.update()

    def checkInfo(self):
        self.makeMenu(0)


if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = Example()
    sys.exit(app.exec_())
