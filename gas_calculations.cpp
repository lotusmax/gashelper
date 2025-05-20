#include <iostream>
#include <cmath>
#include "pipeline.h"

extern "C" {
    void calculate_gas_params(
        double temperature,      // K
        double initial_pressure, // Pa
        double mass_flow,        // kg/s
        bool* valve_states,      // массив состояний кранов
        int valve_count,         // количество кранов
                // диаметры труб 
        double* results          // массив для результатов [output_pressure, velocity, pressure_drop, reynolds]
    ) {
        // Физические константы
        const double R = 8.314;       // Универсальная газовая постоянная [Дж/(моль·K)]
        const double M = 0.029;       // Молярная масса метана [кг/моль]
        const double R_specific = R / M;  // Удельная газовая постоянная
        int* diam_piplane = new int[3](); 
        // Параметры трубы
        const double D = 0.5;         // Диаметр трубы [м]
        const double A = M_PI * D * D / 4.0;  // Площадь сечения [м²]
        const double L = 100.0;       // Длина трубы [м]
        
        // Вязкость метана при ~20°C [Па·с]
        const double mu = 1.1e-5;
        
        // Рассчитываем количество открытых кранов
        int open_valves = 0;
        for (int i = 0; i < valve_count; ++i) {
            if (valve_states[i]) {
                open_valves++;
            }
        }
        if (valve_states[0]){diam_piplane[0] = 512;}
        if (valve_states[1]){diam_piplane[1] = 515;}
        if (valve_states[2]){diam_piplane[2] = 703;}
        if (open_valves == 0) {
            std::cerr << "Все краны закрыты!" << std::endl;
            results[0] = 0.0;  // output_pressure
            results[1] = 0.0;  // velocity
            results[2] = 0.0;  // pressure_drop
            results[3] = 0.0;  // reynolds
            return;
        }
        
        PipLineSection first_section("0-28", initial_pressure, 8000, 28000, diam_piplane[0], diam_piplane[1], diam_piplane[2]); 
        
        
        // Плотность газа (уравнение состояния идеального газа)
        double rho = initial_pressure / (R_specific * temperature);
        
        // Эффективная площадь (учитывает открытые краны)
        double A_eff = A * open_valves / valve_count;
        
        // Скорость потока
        double velocity = mass_flow / (rho * A_eff);
        
        // Число Рейнольдса
        double Re = first_section.get_re();
        
        // Коэффициент трения
        double f = (Re > 4000) ? 0.316 / pow(Re, 0.25) : 64.0 / Re;
        
        // Перепад давления (формула Дарси-Вейсбаха)
        double delta_P = f * (L / D) * (rho * velocity * velocity) / 2.0;
        
        // Давление на выходе
        double output_pressure = first_section.calc_press();
        
        // Записываем результаты
        results[0] = output_pressure;
        results[1] = velocity;
        results[2] = delta_P;
        results[3] = Re;
        
        // Вывод в консоль для отладки
        std::cout << "C++ расчет:" << std::endl;
        std::cout << "  Температура: " << temperature << " K" << std::endl;
        std::cout << "  Открыто кранов: " << open_valves << "/" << valve_count << std::endl;
        std::cout << "  Давление на выходе: " << output_pressure << " Pa" << std::endl;
        std::cout << "  Скорость газа: " << velocity << " м/с" << std::endl;
        std::cout << "  Перепад давления: " << delta_P << " Pa" << std::endl;
        std::cout << "  Число Рейнольдса: " << Re << std::endl;
        delete[] diam_piplane;
    }
}