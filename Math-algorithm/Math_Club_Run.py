import sys
from ctypes import *
from PyQt5.QtWidgets import QApplication, QWidget, QPushButton, QLabel, QBoxLayout, QFileDialog, QScrollArea, \
    QVBoxLayout, QListView, QMessageBox
from PyQt5.QtGui import QPixmap, QStandardItemModel, QStandardItem
from PyQt5.QtCore import QCoreApplication, Qt
# from PyQt5.QtWidgets.QGridLayout import addWidget
from PyQt5 import QtWidgets
from fastai import *
from fastai.vision import *
import sys
from ctypes import cdll
import runningAlgo


class MyApp(QWidget):
    def __init__(self):
        super().__init__()
        self.Upload_img = QLabel()
        self.initUI()
        self.hlabel = []

    def initUI(self):
        # 버튼 만들기, Upload, Run, Symbols, History
        btn1 = QPushButton('Upload', self)
        btn1.move(50, 100)
        btn1.resize(250, 125)

        btn2 = QPushButton('Run', self)
        btn2.move(50, 250)
        btn2.resize(250, 125)

        btn3 = QPushButton('Symbols', self)
        btn3.move(50, 400)
        btn3.resize(250, 125)

        btn4 = QPushButton('History', self)
        btn4.move(50, 550)
        btn4.resize(250, 125)

        # 폰트 생성자, 폰트 크기 설정
        font = self.font()
        font.setPointSize(13)

        # 버튼 폰트 크기 증가
        btn1.setFont(font)
        btn2.setFont(font)
        btn3.setFont(font)
        btn4.setFont(font)

        # Label 만들기 & 위치 설정, Input, Output
        label1 = QLabel('Input', self)
        label1.setFont(font)
        label1.move(500, 60)

        btn1.clicked.connect(self.uploadImg)  # 버튼1, Upload 누를 때 uploadImg 실행
        btn2.clicked.connect(self.run_algo)
        btn3.clicked.connect(self.symbolView)  # 버튼3, Symbols 누를 때 실행
        btn4.clicked.connect(self.historyView)  # 버튼4, History 누를 때 실행

        self.setWindowTitle('Math Club')  # 창의 제목 설정
        self.move(600, 300)  # 실행창이 화면 어디에 뜰지
        self.resize(800, 800)  # 창의 크기를 정해준다
        self.show()  # 위젯을 스크린에 보여준다

    def uploadImg(self):
        self.absPath = ""  # 업로드할 이미지 절대경로 저장
        self.fname = QFileDialog.getOpenFileName(self)  # 폴더 찾기 창을 띄워서 사용할 데이터를 open 하면 절대경로가 저장됨
        self.absPath = self.fname[0]  # 절대경로 세팅
        self.pixmap = QPixmap()
        self.pixmap.load(self.absPath)  # 받아온 절대경로의 데이터를 로드
        self.pixmap = self.pixmap.scaledToWidth(450)  # 사이즈 조정
        self.Upload_img.setPixmap(self.pixmap)
        self.layout = QBoxLayout(QBoxLayout.TopToBottom)
        self.setLayout(self.layout)
        self.layout.addWidget(self.Upload_img, alignment=Qt.AlignRight)

    def symbolView(self):
        self.next = Symbol_Window()  # 새로운 위젯을 띄우기 위해 Second 클래스 연결

    def historyView(self):
        self.next = History_Window(self.hlabel)

    def run_algo(self):
        check_M = False
        runObj = runningAlgo.runningAlgo(self.absPath)
        # Matrix 확인
        if 'M' in runObj.ans:
            check_M = True
        MathCalculator = cdll.LoadLibrary('MathCalculator.dll')
        font2 = self.font()
        font2.setPointSize(13)
        Qbox = QMessageBox(self)
        Qbox.setWindowTitle("Answer")
        Qbox.setFont(font2)
        ans = MathCalculator.calculator(runObj.ans)

        if check_M:
            res = ""
            for number in range(1, 10):
                res += str.format("%d X %d = %d\n" % (ans, number, ans * number))
                Qbox.setText("MTRIX Number : " + str(ans) + "\n" + res)
            self.hlabel.append("MATRIX Number : " + str(ans) + "\n" + res)
        else:
            self.hlabel.append(runObj.ans + " = " + str(ans))
            Qbox.setText("Answer : " + str(ans) + "\n")

        Qbox.move(1000, 600)
        Qbox.resize(600, 600)
        Qbox.show()


class Symbol_Window(QWidget):  # Symbols 버튼을 누르면 새로운 위젯이 열리고 설명이 보인다
    def __init__(self):
        super().__init__()
        self.title = "Symbols Description"  # 위젯 이름 설정
        self.initWindow()

    def initWindow(self):
        scroll = QScrollArea()  # 스크롤 생성
        self.vbox = QVBoxLayout()  # label 담을 박스 생성
        self.widget = QWidget()  # 위젯 생성
        self.mesg = QLabel("----------------사칙연산----------------\n\n"
                           " + : 덧셈\n\t1 + 2 = 3\n\n"
                           " - : 뺄셈\n\t10 - 3 = 7\n\n"
                           " x : 곱셈\n\t4 x 5 = 20\n\n"
                           " / : 나눗셈\n\t10 / 2 = 5\n\n"
                           "\n----------------특수계산----------------\n\n"
                           " G : 최대공약수 구하기\n\t12 G 15 = 3\n\n"
                           " L : 최소공배수 구하기\n\t12 L 15 = 60\n\n"
                           " P : 순열 구하기\n\t5P3 = 60\n\n"
                           " C : 조합 구하기\n\t5C3 = 10\n\n"
                           " R : 소수 (Prime Number) 구하기(범위)\n\t1R10 = 4 (2, 3, 5, 70)\n\n"
                           " M : 매트릭스 출력 (1~9까지 곱한 값 결과 출력)\n\t7 M = 7 X 1 = 7, 7 X 2 = 14 . . .\n\n")

        scroll.setWidget(self.mesg)  # mesg를 담은 스크롤을 위젯으로 만든다
        self.vbox.addWidget(scroll)  # 박스에 위젯을 담는다
        self.setLayout(self.vbox)  # 박스를 레이아웃에 추가

        self.setWindowTitle(self.title)  # 창 제목 세팅
        self.setGeometry(500, 200, 600, 500)
        self.show()


class History_Window(QWidget):  # History 버튼을 누르면 새로운 위젯이 열리고 리스트에 저장된 수식과 결과를 보여준다
    def __init__(self, hlabel):
        super().__init__()
        self.title = "History"  # 위젯 이름 설정
        self.initWindow(hlabel)

    def initWindow(self, hlabel):
        history_list = hlabel
        list_view = QListView(self)  # 리스트뷰 생성
        model = QStandardItemModel()  # 리스트 뷰에 들어갈 모델 생성

        for h in history_list:
            model.appendRow(QStandardItem(h))  # 모델에 한 줄 씩 추가

        list_view.setModel(model)  # 리스트 뷰에 모델 얹기
        list_view.resize(500, 500)

        self.setWindowTitle(self.title)  # 창 제목 세팅
        self.setGeometry(500, 250, 500, 500)
        self.show()


if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = MyApp()
    sys.exit(app.exec_())
