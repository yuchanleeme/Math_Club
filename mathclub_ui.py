import sys
from PyQt5.QtWidgets import QApplication, QWidget, QPushButton, QLabel, QBoxLayout
from PyQt5.QtGui import QPixmap
from PyQt5.QtCore import QCoreApplication, Qt


class MyApp(QWidget):
    def __init__(self):
        super().__init__()
        self.Upload_img = QLabel()
        self.initUI()

    def initUI(self):
        # 버튼 만들기, Upload, Run, Symbols, History
        btn1 = QPushButton('Upload', self)
        btn1.move(10, 50)
        btn1.resize(200, 100)

        btn2 = QPushButton('Run', self)
        btn2.move(10, 150)
        btn2.resize(200, 100)

        btn3 = QPushButton('Symbols', self)
        btn3.move(10, 250)
        btn3.resize(200, 100)

        btn4 = QPushButton('History', self)
        btn4.move(10, 350)
        btn4.resize(200, 100)

        # 폰트 생성자, 폰트 크기 설정
        font = self.font()
        font.setPointSize(20)

        # 버튼 폰트 크기 증가
        btn1.setFont(font)
        btn2.setFont(font)
        btn3.setFont(font)
        btn4.setFont(font)

        # Label 만들기 & 위치 설정, Input, Output
        label1 = QLabel('Input', self)
        label2 = QLabel('Output', self)
        label1.setFont(font)
        label2.setFont(font)
        label1.move(350, 30)
        label2.move(600, 30)

        btn1.clicked.connect(self.uploadImg)

        self.setWindowTitle('Math Club') # 창의 제목 설정
        self.move(600, 300)              # 실행창이 화면 어디에 뜰지
        self.resize(800, 500)            # 창의 크기를 정해준다
        self.show()                      # 위젯을 스크린에 보여준다

    def uploadImg(self):
        pixmap = QPixmap()
        pixmap.load("/Users/jason/Desktop/test.jpeg")
        pixmap = pixmap.scaledToWidth(280)  # 사이즈 조정
        self.Upload_img.setPixmap(pixmap)
        layout = QBoxLayout(QBoxLayout.TopToBottom)
        self.setLayout(layout)
        layout.addWidget(self.Upload_img, alignment=Qt.AlignCenter)


if __name__ == '__main__':
   app = QApplication(sys.argv)
   ex = MyApp()
   sys.exit(app.exec_())