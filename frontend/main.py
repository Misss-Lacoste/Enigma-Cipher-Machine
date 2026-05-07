import sys
import os

from PyQt5.QtWidgets import (QApplication, QMainWindow, QWidget, QVBoxLayout, QHBoxLayout, QLabel, QPushButton, QTextEdit, QLineEdit, QComboBox,
QGroupBox, QFormLayout, QMessageBox, QTabWidget, QScrollArea, QButtonGroup, QCheckBox)

from PyQt5.QtCore import Qt, QTimer
from PyQt5.QtGui import QFont

from enigma_core import EnigmaBackend

class EnigmaGUI(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Margarita's Enigma Cipher Machine")
        self.resize(1000, 700)
        
        base_dir = os.path.dirname(os.path.abspath(__file__))
        project_dir = os.path.dirname(base_dir)
        exe_path = os.path.join(project_dir, "backend", "enigma_backend.exe")
        
        try:
            self.backend = EnigmaBackend(exe_path)
        except FileNotFoundError as e:
            QMessageBox.critical(self, "Ошибка!", f"Не найден C++ backend .\n\n{str(e)}\n\nНеобходима компиляция.")
            sys.exit(1)

        self.typewriter_timer = QTimer()
        self.typewriter_timer.timeout.connect(self.type_next_char)
        self.typewriter_text = ""
        self.typewriter_index = 0
        self.final_status_msg = ""

        self.init_ui()

    def init_ui(self):
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        main_layout = QVBoxLayout(central_widget)

        #Header
        title_label = QLabel("Шифровальная Машина Энигма")
        title_label.setFont(QFont("Times New Roman", 24))
        title_label.setAlignment(Qt.AlignCenter)
        main_layout.addWidget(title_label)

        #вкладки (как в main.cpp)
        self.tabs = QTabWidget()
        self.tabs.tabBar().setFont(QFont("Times New Roman", 11))
        
        #вкладка_1: ciphering(e/d)
        cipher_tab = self.create_cipher_tab()
        self.tabs.addTab(cipher_tab, "1. Эмуляция работы")
        
        #вкладка_2
        history_tab = self.create_info_tab("enigma_info.txt", "Historical Information")
        self.tabs.addTab(history_tab, "2. Историческая Информация")
        
        #вкладка_3: attack methods
        attacks_tab = self.create_info_tab("crib_attack.txt", "Методы Атак")
        self.tabs.addTab(attacks_tab, "3. Методы Атак")
        
        #вкладка_4: help
        help_tab = self.create_info_tab("help.txt", "Помощь")
        self.tabs.addTab(help_tab, "4. Помощь")
        
        #вкладка_5: crypto
        crypto_tab = self.create_crypto_tab()
        self.tabs.addTab(crypto_tab, "5. Криптоанализ")
        
        main_layout.addWidget(self.tabs)
        
        #состояние
        self.status_label = QLabel("Готово к работе")
        self.status_label.setFont(QFont("calibri", 8))
        self.status_label.setStyleSheet("QLabel { background-color: #f0f0f0; padding: 5px; }")
        main_layout.addWidget(self.status_label)

    def create_cipher_tab(self):
        #1вкладка
        widget = QWidget()
        layout = QHBoxLayout(widget)

        #левая настроечная панель
        left_panel = QVBoxLayout()
        
        #роторы
        rotor_group = QGroupBox("Конфигурация роторов (1-5, различные)")
        rotor_group.setFont(QFont("Times New Roman", 11)) #, QFont.Bold
        rotor_layout = QFormLayout()
        
        self.rotor1 = QComboBox()
        self.rotor1.addItems(["1", "2", "3", "4", "5"])
        self.rotor2 = QComboBox()
        self.rotor2.addItems(["1", "2", "3", "4", "5"])
        self.rotor3 = QComboBox()
        self.rotor3.addItems(["1", "2", "3", "4", "5"])
        #dif rotors
        self.rotor2.setCurrentIndex(1)
        self.rotor3.setCurrentIndex(2)
        
        rotor_layout.addRow("I ротор:", self.rotor1)
        rotor_layout.addRow("II ротор:", self.rotor2)
        rotor_layout.addRow("III ротор:", self.rotor3)
        rotor_group.setLayout(rotor_layout)
        left_panel.addWidget(rotor_group)
        
        #кольца
        ring_group = QGroupBox("Конфигурация колец (буквы от 'A' до 'Z')")
        ring_group.setFont(QFont("Times New Roman", 11)) #, QFont.Bold
        ring_layout = QFormLayout()
        
        self.rings = QLineEdit("ABC") #example
        self.rings.setMaxLength(3)
        self.rings.setFont(QFont("Courier New", 12))
        ring_layout.addRow("Кольца:", self.rings)
        ring_group.setLayout(ring_layout)
        left_panel.addWidget(ring_group)

        stecker_group = QGroupBox("Коммутационная панель")
        stecker_group.setFont(QFont("Times New Roman", 11))
        stecker_layout = QVBoxLayout()

        self.use_stecker = QCheckBox("Здесь можно поставить галочку для исп-я панели")
        self.use_stecker.setFont(QFont("Times New Roman", 11))
        stecker_layout.addWidget(self.use_stecker)

        self.stecker_input = QLineEdit()
        self.stecker_input.setFont(QFont("Courier New", 12))
        self.stecker_input.setPlaceholderText("ML, RI, TA, etc.")
        self.stecker_input.setEnabled(False)
        stecker_layout.addWidget(self.stecker_input)

        stecker_group.setLayout(stecker_layout)
        left_panel.addWidget(stecker_group)

        #поле ввода только при активной галочке
        self.use_stecker.stateChanged.connect(lambda state: self.stecker_input.setEnabled(state == Qt.Checked))
        
        #занимаем все пространство свободное
        left_panel.addStretch()
        layout.addLayout(left_panel, 1)

        #кнопки
        right_panel = QVBoxLayout()
        input_group = QGroupBox("Ввод секретного сообщения")
        input_group.setFont(QFont("Times New Roman", 11))
        input_layout = QVBoxLayout()
        self.input_text = QTextEdit()
        self.input_text.setFont(QFont("Courier New", 12))
        self.input_text.setPlaceholderText("Введите, пожалуйста, Ваш текст...")
        input_layout.addWidget(self.input_text)
        input_group.setLayout(input_layout)
        right_panel.addWidget(input_group, 2)
        
        btn_layout = QHBoxLayout()
        btn_layout.setSpacing(20)
        
        self.btn_encrypt = QPushButton("Зашифровать")
        self.btn_encrypt.setFont(QFont("Times New Roman", 14))
        self.btn_encrypt.setMinimumHeight(60)
        self.btn_encrypt.setStyleSheet("""
            QPushButton {
                background-color: #ffd1dc; 
                color: white; 
                border-radius: 8px;
                border: 2px solid #ffcbdb;
            }
            QPushButton:hover {
                background-color: #ffcbdb;
            }
            QPushButton:pressed {
                background-color: #f995c6;
            }
        """)
        self.btn_encrypt.clicked.connect(self.encrypt)
        
        self.btn_decrypt = QPushButton("Расшифровать")
        self.btn_decrypt.setFont(QFont("Times New Roman", 14))
        self.btn_decrypt.setMinimumHeight(60)
        self.btn_decrypt.setStyleSheet("""
            QPushButton {
                background-color: #CEC4FF; 
                color: white; 
                border-radius: 8px;
                border: 2px solid #c8bdff;
            }
            QPushButton:hover {
                background-color: #c8bdff;
            }
            QPushButton:pressed {
                background-color: #beb3ff;
            }
        """)
        self.btn_decrypt.clicked.connect(self.decrypt)
        
        self.btn_clear = QPushButton("Очистить")
        self.btn_clear.setFont(QFont("Times New Roman", 14)) #, QFont.Bold
        self.btn_clear.setMinimumHeight(60)
        self.btn_clear.setStyleSheet("""
            QPushButton {
                background-color: #dab1da; 
                color: white; 
                border-radius: 8px;
                border: 2px solid #d29dd2;
            }
            QPushButton:hover {
                background-color: #d29dd2;
            }
            QPushButton:pressed {
                background-color: #d095d0;
            }
        """)
        self.btn_clear.clicked.connect(self.clear)
        
        btn_layout.addWidget(self.btn_encrypt)
        btn_layout.addWidget(self.btn_decrypt)
        btn_layout.addWidget(self.btn_clear)
        right_panel.addLayout(btn_layout)
        
        #вывод
        output_group = QGroupBox("Зашифрованное сообщение")
        output_group.setFont(QFont("Times New Roman", 11))
        output_layout = QVBoxLayout()
        self.output_text = QTextEdit()
        self.output_text.setFont(QFont("Courier New", 14))
        self.output_text.setReadOnly(True)
        self.output_text.setStyleSheet("""
            QTextEdit { 
                background-color: #f5e8ec; 
                border: 2px solid #f5e8f2;
                padding: 10px;
            }
        """)
        output_layout.addWidget(self.output_text)
        output_group.setLayout(output_layout)
        right_panel.addWidget(output_group, 2)
        
        layout.addLayout(right_panel, 2)
        return widget

    """def _check_message_validity(self, text):
        invalid_chars = set()
        for char in text:
            if char == ' ':
                continue
            if not ('A' <= char <= 'Z' or 'a' <= char <= 'z'):
                invalid_chars.add(char)
        return len(invalid_chars) == 0, list(invalid_chars)"""

    def encrypt(self):
        self._process_operation("encrypt")

    def decrypt(self):
        self._process_operation("decrypt")

    def _process_operation(self, mode):
        try:
            rotors = [
                int(self.rotor1.currentText()), 
                int(self.rotor2.currentText()), 
                int(self.rotor3.currentText())
            ]
            
            #уникальность роторов
            if len(set(rotors)) != 3:
                QMessageBox.warning(self, "Ошибка конфигурации!", 
                    "Роторы должны быть разными (1-5)!\nПожалуйста, выберете три различных ротора.")
                return

            rings = self.rings.text().upper()
            if len(rings) != 3 or not rings.isalpha():
                QMessageBox.warning(self, "Ошибка конфигурации!", 
                    "Следует ввести ровно 3 кольцевые настройки (буквы латинского алфавита от 'A' до 'Z')!\n"
                    "Символы кириллицы, цифры и иные знаки запрещены.")
                return

            if not all(('A' <= c <= 'Z' or 'a' <= c <= 'z') for c in rings):
                QMessageBox.warning(self, "Ошибка конфигурации!", 
                    "Внимание! Кольцевые настройки должны содеражть только буквы латинского алфавита от 'A' до 'Z'.\n"
                    "Символы кириллицы, цифры и иные знаки запрещены.")
                return

            stecker_active = self.use_stecker.isChecked()
            stecker_raw = self.stecker_input.text().strip()
            stecker_final = ""

            if stecker_active:
                if not stecker_raw:
                    QMessageBox.warning(self, "Ошибка конфигурации!", "Панель активирована, однако пары букв не введены.")
                    return
                
                clean = stecker_raw.replace(" ", "").upper()
                if len(clean) % 2 != 0:
                    QMessageBox.warning(self, "Ошибка конфигурации!", "Количество введенных букв в буквосочетаниях должно быть четным.")
                    return
                if len(clean) > 26:
                    QMessageBox.warning(self, "Ошибка конфигурации!", "Максимальное число допустимых пар букв - 13.")
                    return
                if not all('A' <= c <= 'Z' for c in clean):
                    QMessageBox.warning(self, "Ошибка конфигурации!", "Допустимы только латинские буквы от 'A' до 'Z'")
                    return
                if len(set(clean)) != len(clean):
                    QMessageBox.warning(self, "Ошибка конфигурации!", "Буквы в буквосочетаниях не должны повторяться.")
                    return
                    
                stecker_final = clean

            text = self.input_text.toPlainText()
            if not text.strip():
                QMessageBox.warning(self, "Ошибка ввода!", "Пожалуйста, введите Ваш текст.")
                return

            if not all((c == ' ' or 'A' <= c <= 'Z' or 'a' <= c <= 'z') for c in text):
                QMessageBox.warning(self, "Некорректный ввод!", 
                    "Внимание! Разрешены только символы латинского алфавита и пробелы по желанию.\n"
                    "Символы кириллицы, цифры и иные занки запрещены.")
                return
            
            self.status_label.setText("Обработка...")
            QApplication.processEvents()

            if mode == "encrypt":
                result = self.backend.encrypt(text, rotors, rings, stecker_active, stecker_final)
                self.final_status_msg = "Шифрование выполнено успешно!"
            else:
                result = self.backend.decrypt(text, rotors, rings, stecker_active, stecker_final)
                self.final_status_msg = "Дешифровка выполнена успешно!"

            self.start_typewriter_effect(result)

        except Exception as e:
            self.output_text.setText(f"Error 500: ошибка связи с бэкендом:\n{str(e)}")
            self.status_label.setText("Ошибка!")

    def clear(self):
        self.typewriter_timer.stop()
        self.input_text.clear()
        self.output_text.clear()
        self.status_label.setText("Готово к работе!")

    

    def start_typewriter_effect(self, text):
        self.typewriter_timer.stop()
        self.output_text.clear()
        self.typewriter_text = text
        self.typewriter_index = 0
        self.typewriter_timer.start(50)

    def type_next_char(self):
        if self.typewriter_index < len(self.typewriter_text):
            self.output_text.insertPlainText(self.typewriter_text[self.typewriter_index])
            self.typewriter_index += 1
            
            cursor = self.output_text.textCursor()
            cursor.movePosition(cursor.End)
            self.output_text.setTextCursor(cursor)
        else:
            self.typewriter_timer.stop()
            self.status_label.setText(self.final_status_msg)

    #вкладки с информацией
    def create_info_tab(self, filename, title):
        widget = QWidget()
        layout = QVBoxLayout(widget)
        
        scroll = QScrollArea()
        scroll.setWidgetResizable(True)
        scroll.setStyleSheet("QScrollArea { background-color: white; }")
        
        content_widget = QWidget()
        content_layout = QVBoxLayout(content_widget)
        
        text_edit = QTextEdit()
        text_edit.setReadOnly(True)
        text_edit.setFont(QFont("Courier New", 11))
        text_edit.setStyleSheet("QTextEdit { border: none; background-color: white; padding: 10px; }")
        
        #содержимое файла через бэкенд
        content = self.backend.get_info_file(filename)
        text_edit.setText(content)
        
        content_layout.addWidget(text_edit)
        scroll.setWidget(content_widget)
        layout.addWidget(scroll)
        
        return widget
    
    #вкладка крпитоанализа
    def create_crypto_tab(self):
        widget = QWidget()
        layout = QVBoxLayout(widget)
        
        #подпапки для криптоанализа
        crypto_tabs = QTabWidget()
        crypto_tabs.tabBar().setFont(QFont("Times New Roman", 11))
        
        text_font  = QFont("Times New Roman", 11)
        
        #1.frequency analysis
        tab_freq = QWidget()
        layout_freq = QVBoxLayout(tab_freq)

        lbl2 = QLabel("Введите шифртекст для анализа буквенных частот:")
        lbl2.setFont(text_font)
        layout_freq.addWidget(lbl2)
        
        input_freq = QTextEdit()
        input_freq.setMaximumHeight(150)
        input_freq.setFont(QFont("Courier New", 11))
        input_freq.setPlaceholderText("Введите, пожалуйста, Ваш шифртекст...")
        layout_freq.addWidget(input_freq)
        
        btn_freq = QPushButton("Анализировать частоту букв")
        btn_freq.setFont(QFont("Times New Roman", 11, QFont.Bold))
        btn_freq.setStyleSheet("""
            QPushButton {
                background-color: #e491a6;
                color: white;
                padding: 10px;
                border-radius: 5px;
            }
            QPushButton:hover { 
                background-color: #e0809a; 
            }
            QPushButton:pressed {
                background-color: #dc6586;
            }
        """)
        layout_freq.addWidget(btn_freq)
        
        output_freq = QTextEdit()
        output_freq.setReadOnly(True)
        output_freq.setFont(QFont("Courier New", 11))

        lbl3 = QLabel("Сводка анализа:")
        lbl3.setFont(text_font)
        layout_freq.addWidget(lbl3)

        layout_freq.addWidget(output_freq)
        
        def do_freq():
            res = self.backend.run_crypto_analysis(1, input_freq.toPlainText())
            output_freq.setText(res)
            self.status_label.setText("Частотный анализ завершен!")
        
        btn_freq.clicked.connect(do_freq)
        crypto_tabs.addTab(tab_freq, "1. Частотный Анализ")

        #2.no-self-mapping
        tab_nsm = QWidget()
        layout_nsm = QVBoxLayout(tab_nsm)

        lbl5 = QLabel("В логике 'Энигмы' буква не может шифроваться сама в себя.")
        lbl5.setFont(text_font)
        layout_nsm.addWidget(lbl5)
        
        lbl6 = QLabel("Данное свойство позволяет найти потенциальную 'зацепку' (crib).")
        lbl6.setFont(text_font)
        layout_nsm.addWidget(lbl6)
        
        lbl7 = QLabel("Исходное секретное сообщение:")
        lbl7.setFont(text_font)
        layout_nsm.addWidget(lbl7)

        input_plain = QLineEdit()
        input_plain.setFont(QFont("Courier New", 11))
        input_plain.setMinimumHeight(40)
        input_plain.setPlaceholderText("Введите, пожалуйста, секретное сообщение...")
        layout_nsm.addWidget(input_plain)

        lbl8 = QLabel("Шифртекст:")
        lbl8.setFont(text_font)
        layout_nsm.addWidget(lbl8)
        
        input_cipher = QLineEdit()
        input_cipher.setFont(QFont("Courier New", 11))
        input_cipher.setMinimumHeight(40)
        input_cipher.setPlaceholderText("Введите, пожалуйста, Ваш шифртекст...")
        layout_nsm.addWidget(input_cipher)
        
        btn_nsm = QPushButton("Проверить свойство")
        btn_nsm.setFont(QFont("Times New Roman", 11, QFont.Bold))
        btn_nsm.setStyleSheet("""
            QPushButton {
                background-color: #FF9800;
                color: white;
                padding: 10px;
                border-radius: 5px;
            }
            QPushButton:hover { 
                background-color: #F57C00; 
            }
            QPushButton:pressed {
                background-color: #f57300;
            }
        """)
        layout_nsm.addWidget(btn_nsm)
        
        output_nsm = QTextEdit()
        output_nsm.setReadOnly(True)
        output_nsm.setMaximumHeight(150)
        output_nsm.setFont(QFont("Courier New", 11))

        lbl9 = QLabel("Сводка анализа:")
        #lbl9.setFont(QFont("Courier New", 12)) 
        lbl9.setFont(text_font)
        layout_nsm.addWidget(lbl9)
        
        layout_nsm.addWidget(output_nsm)

        def do_nsm():
            res = self.backend.run_crypto_analysis(2, input_plain.text(), input_cipher.text())
            output_nsm.setText(res)
            self.status_label.setText("Проверка завершена!")

        btn_nsm.clicked.connect(do_nsm)
        crypto_tabs.addTab(tab_nsm, "2. Отсутствие шифрования буквы саму в себя")

        # 3.сrib Positions
        tab_crib = QWidget()
        layout_crib = QVBoxLayout(tab_crib)

        lbl11 = QLabel("Найти возможные позиции, в котрых символы исходного сообщения ('зацепка') могут появиться в шифртексте.")
        lbl11.setFont(text_font)
        layout_crib.addWidget(lbl11)
        
        lbl12 = QLabel("Шифртекст:")
        lbl12.setFont(text_font)
        layout_crib.addWidget(lbl12)
        
        input_ct = QTextEdit()
        input_ct.setMaximumHeight(100)
        input_ct.setFont(QFont("Courier New", 11))
        input_ct.setPlaceholderText("Пожалуйста, введите шифртекст...")
        layout_crib.addWidget(input_ct)
        
        lbl13 = QLabel("Зацепка (известный исходный текст):")
        lbl13.setFont(text_font)
        layout_crib.addWidget(lbl13)
        input_crib = QLineEdit()
        input_crib.setMinimumHeight(40)
        input_crib.setFont(QFont("Courier New", 11))
        input_crib.setPlaceholderText("Введите потенциальную зацепку...")
        layout_crib.addWidget(input_crib)
        
        btn_crib = QPushButton("Найти позиции 'зацепок'")
        btn_crib.setFont(QFont("Times New Roman", 11, QFont.Bold))
        btn_crib.setStyleSheet("""
            QPushButton {
                background-color: #dca1a1;
                color: white;
                padding: 10px;
                border-radius: 5px;
            }
            QPushButton:hover { 
                background-color: #d59090; 
            }
            QPushButton:pressed {
                background-color: #cd7a7a;
            }
        """)
        layout_crib.addWidget(btn_crib)
        
        output_crib = QTextEdit()
        output_crib.setReadOnly(True)
        output_crib.setFont(QFont("Courier New", 11))
        
        lbl14 = QLabel("Сводка анализа:")
        #lbl14.setFont(QFont("Courier New", 12))
        lbl14.setFont(text_font)
        layout_crib.addWidget(lbl14)
        
        layout_crib.addWidget(output_crib)

        def do_crib():
            res = self.backend.run_crypto_analysis(3, input_ct.toPlainText(), input_crib.text())
            output_crib.setText(res)
            self.status_label.setText("Поиск 'зацепок' найден.")

        btn_crib.clicked.connect(do_crib)
        crypto_tabs.addTab(tab_crib, "3. Позиции 'зацепок'")

        layout.addWidget(crypto_tabs)
        return widget

    def closeEvent(self, event):
        self.backend.close()
        event.accept()

if __name__ == "__main__":
    app = QApplication(sys.argv)
    app.setStyle("iOS")
    window = EnigmaGUI()
    window.show()
    sys.exit(app.exec_())