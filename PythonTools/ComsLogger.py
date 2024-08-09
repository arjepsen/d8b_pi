import sys
import csv
import threading
import serial
import datetime
from PyQt5.QtWidgets import QApplication, QWidget, QVBoxLayout, QPushButton, QTableWidget, QTableWidgetItem
from PyQt5.QtCore import Qt, QThread, pyqtSignal

class SerialReader(QThread):
    message_received = pyqtSignal(str, str, str)
    MAX_MESSAGE_LENGTH = 100  # Set a maximum message length

    def __init__(self, port_name, port_label):
        super().__init__()
        self.port_name = port_name
        self.port_label = port_label
        self.serial_port = None
        self.running = False

    def run(self):
        self.serial_port = serial.Serial(self.port_name, 115200, timeout=0.1)
        self.running = True
        buffer = ''
        timestamp = ''
        while self.running:
            try:
                if self.serial_port.in_waiting > 0:
                    char = self.serial_port.read().decode('utf-8', errors='replace')
                    if not buffer:
                        timestamp = datetime.datetime.now().strftime('%M:%S:%f')[:-3]
                    buffer += char

                    # Check if the message length exceeds the maximum length
                    if len(buffer) >= self.MAX_MESSAGE_LENGTH:
                        self.message_received.emit(timestamp, self.port_label, buffer.strip())
                        buffer = ''
                else:
                    if buffer:
                        self.message_received.emit(timestamp, self.port_label, buffer.strip())
                        buffer = ''
            except Exception as e:
                print(f"Error reading from {self.port_name}: {e}")
        self.serial_port.close()

    def stop(self):
        self.running = False

class SerialMonitor(QWidget):
    def __init__(self):
        super().__init__()
        self.init_ui()
        self.threads = []
        self.port_labels = ["To Brain", "From Brain", "To DSP", "From DSP"]
        self.port_names = ["COM3", "COM11", "COM10", "COM6"]
        self.log_file = open("serial_log.csv", "w", newline='', encoding='utf-8')
        self.csv_writer = csv.writer(self.log_file)
        self.csv_writer.writerow(["Timestamp", "To Brain", "From Brain", "To DSP", "From DSP"])

    def init_ui(self):
        layout = QVBoxLayout()

        self.start_button = QPushButton('Start')
        self.start_button.clicked.connect(self.start_threads)
        layout.addWidget(self.start_button)

        self.table = QTableWidget()
        self.table.setColumnCount(5)
        self.table.setHorizontalHeaderLabels(["Timestamp", "To Brain", "From Brain", "To DSP", "From DSP"])
        layout.addWidget(self.table)

        self.setLayout(layout)
        self.setWindowTitle('Serial Monitor')
        self.resize(600, 400)

    def start_threads(self):
        for port_name, port_label in zip(self.port_names, self.port_labels):
            thread = SerialReader(port_name, port_label)
            thread.message_received.connect(self.display_message)
            self.threads.append(thread)
            thread.start()

    def display_message(self, timestamp, port_label, message):
        row_position = self.table.rowCount()
        self.table.insertRow(row_position)
        
        self.table.setItem(row_position, 0, QTableWidgetItem(timestamp))
        
        col_position = self.port_labels.index(port_label) + 1
        self.table.setItem(row_position, col_position, QTableWidgetItem(message))

        log_row = [""] * 5
        log_row[0] = timestamp
        log_row[col_position] = message
        self.csv_writer.writerow(log_row)

    def closeEvent(self, event):
        for thread in self.threads:
            thread.stop()
            thread.wait()
        self.log_file.close()
        event.accept()

if __name__ == '__main__':
    app = QApplication(sys.argv)
    monitor = SerialMonitor()
    monitor.show()
    sys.exit(app.exec_())
