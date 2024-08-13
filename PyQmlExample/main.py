# main.py
from PyQt5.QtCore import QObject, pyqtSignal, pyqtSlot, QTimer, QDateTime
from PyQt5.QtGui import QGuiApplication
from PyQt5.QtQml import QQmlApplicationEngine, qmlRegisterType

class Backend(QObject):
    mySignal = pyqtSignal(str)
    
    def __init__(self, parent=None):
        QObject.__init__(self)
        self.timer = QTimer(self)
        self.timer.timeout.connect(lambda: self.mySignal.emit(QDateTime.currentDateTime().toString("yyyy-MM-dd hh:mm:ss")))
        self.timer.start(1000)

    @pyqtSlot(str)
    def onButtonClicked(self, text):
        print(text)

if __name__ == "__main__":
    app = QGuiApplication([])
    engine = QQmlApplicationEngine()
    qmlRegisterType(Backend, "Backend", 1, 0, "Backend")
    engine.load("main.qml")
    app.exec_()