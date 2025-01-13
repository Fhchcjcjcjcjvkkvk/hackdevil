import sys
import requests
from io import BytesIO
from PyQt5.QtWidgets import QApplication, QMainWindow, QLabel
from PyQt5.QtGui import QPixmap, QPalette
from PyQt5.QtCore import Qt

class AppWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        # Set window properties
        self.setWindowTitle("PyQt5 App with Web Background")
        self.setGeometry(100, 100, 800, 600)

        # Set background image from web
        self.set_background()

        # Create a simple label
        label = QLabel("Hello, PyQt5!", self)
        label.setStyleSheet("font-size: 24px; color: white;")
        label.setGeometry(300, 250, 200, 50)

    def set_background(self):
        # Fetch the image from the web
        url = "https://raw.githubusercontent.com/Fhchcjcjcjcjvkkvk/hackdevil/main/vortex.png"
        response = requests.get(url)
        if response.status_code == 200:
            # Convert image to QPixmap
            pixmap = QPixmap()
            pixmap.loadFromData(BytesIO(response.content).read())

            # Set the image as the background
            palette = self.palette()
            palette.setBrush(QPalette.Background, pixmap)
            self.setPalette(palette)

if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = AppWindow()
    window.show()
    sys.exit(app.exec_())
