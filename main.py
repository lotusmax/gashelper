import sys
import ctypes
from PyQt6.QtWidgets import (
    QApplication, QMainWindow, QWidget, QVBoxLayout, QHBoxLayout,
    QPushButton, QGraphicsView, QGraphicsScene, QLabel, QLineEdit, QMessageBox
)
from PyQt6.QtCore import Qt
from PyQt6.QtGui import QColor, QPen, QBrush, QPainter

class GasPipelineApp(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Газопровод с C++ расчетами")
        self.setGeometry(100, 100, 1000, 600)
        
        # Загрузка C++ библиотеки
        try:
            self.cpp_lib = ctypes.CDLL('./gas_calculations.so')  # Linux
            # Настройка типов для C++ функций
            self.setup_cpp_functions()
        except Exception as e:
            QMessageBox.critical(self, "Ошибка", f"Не удалось загрузить C++ библиотеку: {str(e)}")
            sys.exit(1)
        
        self.init_ui()
    
    def setup_cpp_functions(self):
        # Настройка функции calculate_gas_params
        self.cpp_lib.calculate_gas_params.argtypes = [
            ctypes.c_double,  # temperature (K)
            ctypes.c_double,  # initial_pressure (Pa)
            ctypes.c_double,  # mass_flow (kg/s)
            ctypes.POINTER(ctypes.c_bool),  # valve_states
            ctypes.c_int,     # valve_count
            ctypes.POINTER(ctypes.c_double)  # result array
        ]
        self.cpp_lib.calculate_gas_params.restype = None
    
    def init_ui(self):
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        main_layout = QVBoxLayout()
        
        # Поля ввода
        input_layout = QHBoxLayout()
        self.temp_input = QLineEdit("293.15")  # 20°C в Кельвинах
        self.temp_input.setPlaceholderText("Температура (K)")
        input_layout.addWidget(QLabel("Температура:"))
        input_layout.addWidget(self.temp_input)
        
        self.pressure_input = QLineEdit("101325")  # Па
        self.pressure_input.setPlaceholderText("Начальное давление (Па)")
        input_layout.addWidget(QLabel("Давление:"))
        input_layout.addWidget(self.pressure_input)
        
        self.flow_input = QLineEdit("1.0")  # кг/с
        self.flow_input.setPlaceholderText("Расход газа (кг/с)")
        input_layout.addWidget(QLabel("Расход:"))
        input_layout.addWidget(self.flow_input)
        
        main_layout.addLayout(input_layout)
        
        # Графическая сцена
        self.scene = QGraphicsScene()
        self.view = QGraphicsView(self.scene)
        self.view.setRenderHint(QPainter.RenderHint.Antialiasing)
        main_layout.addWidget(self.view)
        
        # Кнопки управления и отображение результатов
        self.setup_valve_controls(main_layout)
        
        central_widget.setLayout(main_layout)
        self.draw_pipeline()
    
    def setup_valve_controls(self, layout):
        self.valve_states = [True, True, True]
        self.valve_buttons = []
        buttons_layout = QHBoxLayout()
        
        for i in range(3):
            btn = QPushButton(f"Кран {i+1} (открыт)")
            btn.setCheckable(True)
            btn.setChecked(True)
            btn.clicked.connect(lambda _, idx=i: self.toggle_valve(idx))
            self.valve_buttons.append(btn)
            buttons_layout.addWidget(btn)
        
        self.calculate_btn = QPushButton("Рассчитать параметры (C++)")
        self.calculate_btn.clicked.connect(self.calculate_with_cpp)
        buttons_layout.addWidget(self.calculate_btn)
        
        self.result_label = QLabel("Результаты C++ расчетов будут здесь")
        self.result_label.setWordWrap(True)
        
        layout.addLayout(buttons_layout)
        layout.addWidget(self.result_label)
    
    def toggle_valve(self, valve_idx):
        self.valve_states[valve_idx] = not self.valve_states[valve_idx]
        self.valve_buttons[valve_idx].setText(
            f"Кран {valve_idx+1} ({'открыт' if self.valve_states[valve_idx] else 'закрыт'})"
        )
        self.draw_pipeline()
    
    def calculate_with_cpp(self):
        try:
            # Получаем входные параметры
            temperature = float(self.temp_input.text())  # K
            initial_pressure = float(self.pressure_input.text())  # Pa
            mass_flow = float(self.flow_input.text())  # kg/s
            
            # Подготавливаем данные для передачи в C++
            valve_states = (ctypes.c_bool * 3)(*self.valve_states)
            results = (ctypes.c_double * 4)()  # Массив для результатов
            
            # Вызов C++ функции
            self.cpp_lib.calculate_gas_params(
                ctypes.c_double(temperature),
                ctypes.c_double(initial_pressure),
                ctypes.c_double(mass_flow),
                valve_states,
                ctypes.c_int(3),
                results
            )
            
            # Извлекаем результаты
            output_pressure = results[0]
            velocity = results[1]
            pressure_drop = results[2]
            reynolds = results[3]
            
            # Форматируем результаты
            result_text = (
                f"Результаты C++ расчетов:\n"
                f"Давление на выходе: {output_pressure/1000:.2f} кПа\n"
                f"Скорость газа: {velocity:.2f} м/с\n"
                f"Перепад давления: {pressure_drop/1000:.2f} кПа\n"
                f"Число Рейнольдса: {reynolds:.1f}\n"
                f"Состояние кранов: {self.valve_states}"
            )
            
            self.result_label.setText(result_text)
            
        except ValueError as e:
            QMessageBox.warning(self, "Ошибка", f"Некорректные входные данные: {str(e)}")
        except Exception as e:
            QMessageBox.critical(self, "Ошибка", f"Ошибка при вызове C++ кода: {str(e)}")
    
    def draw_pipeline(self):
        self.scene.clear()
        pipe_length = 800
        pipe_thickness = 5
        pipe_spacing = 50
        
        for i in range(3):
            y_pos = 100 + i * (pipe_thickness + pipe_spacing)
            color = QColor(0, 128, 0) if self.valve_states[i] else QColor(255, 0, 0)
            
            # Труба
            self.scene.addRect(
                50, y_pos, pipe_length, pipe_thickness,
                QPen(Qt.GlobalColor.black, 2),
                QBrush(color)
            )
            
            # Задвижка
            self.draw_valve(50, y_pos, pipe_thickness, i)
            
            # Подпись
            self.scene.addText(f"Нитка {i+1}").setPos(10, y_pos)
    
    def draw_valve(self, x, y, thickness, valve_idx):
        valve_width = 30
        valve_height = thickness + 20
        
        # Основание задвижки
        self.scene.addRect(
            x - valve_width//2, y - 10, valve_width, valve_height,
            QPen(Qt.GlobalColor.black, 2),
            QBrush(QColor(200, 200, 200)))
        
        # Ручка
        if self.valve_states[valve_idx]:
            self.scene.addLine(
                x - valve_width//2 + 5, y + thickness//2,
                x + valve_width//2 - 5, y + thickness//2,
                QPen(Qt.GlobalColor.black, 4)
            )
        else:
            self.scene.addLine(
                x, y - 5, x, y + thickness + 5,
                QPen(Qt.GlobalColor.black, 4)
            )
        
        # Индикатор
        indicator_color = QColor(0, 255, 0) if self.valve_states[valve_idx] else QColor(255, 0, 0)
        self.scene.addEllipse(
            x - 5, y + thickness + 15, 10, 10,
            QPen(Qt.GlobalColor.black, 1),
            QBrush(indicator_color))
        
if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = GasPipelineApp()
    window.show()
    sys.exit(app.exec())