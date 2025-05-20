#ifndef PIPELINE_H
#define PIPELINE_H
#include <iostream>
#include <cmath>
#include <iomanip>
#include <string> 
#include <vector>

extern double density;
extern int temper;
extern double R;
extern double x_a;
extern double x_y;

class PipLineSection{
private:
std::string name;
double press_start;
double volume;
int length;
double diam_1, diam_2, diam_3;
double eque_diametr;
// Переменные, которые зависят от других переменных
double density_relative;
double z;
double p_nk;
double t_nk;
double p_np;
double t_np;
double u_0;
double b_1, b_2, b_3;
double u_1;
double diametr;
double quantity;
double re;
double roughness_tr;
double roughness;
double press_fin;
double roundToNDecimals(double value, int n) {
    double factor = std::pow(10, n); // 10^n
    return value/ factor; // Округление и деление обратно
}
void calculateDerivedValues() {
    density_relative = density / 1.20445;
    z = 1 - (10.2 * (press_start + 0.101325) - 6) * (0.00345 * density_relative - 0.000446) * (1.3 - 0.0144 * (temper - 283.2));
    p_nk = 2.9585 * (1.608 - 0.05994 * density + x_y - 0.392 * x_a);
    t_nk = 88.25 * (0.9915 + 1.759 * density - x_y - 1.681 * x_a);
    p_np = press_start / p_nk;
    t_np = temper / t_nk;
    u_0 = (1.81 + 5.95 * t_np) * std::pow(10, -6);
    b_1 = -0.67 + 2.36 / t_np - 1.93 / std::pow(t_np, 2);
    b_2 = 0.8 - 2.89 / t_np + 2.65 / std::pow(t_np, 2);
    b_3 = -0.1 + 0.354 / t_np - 0.314 / std::pow(t_np, 2);
    u_1 = u_0 * (1 + b_1 * p_np + b_2 * std::pow(p_np, 2) + b_3 * std::pow(p_np, 3));
    diametr = eque_diametr / 1000;
    quantity = volume / std::pow(10, 6) * 24 * 60 * 60;
    re = (17.75 * std::pow(10, 3) * quantity * density_relative) / (diametr * 1000 * u_1);
    roughness_tr = 0.067 * std::pow(158 / re + ((2 * 3 * std::pow(10, -6)) / diametr), 0.2);
    roughness = 1.05 * roughness_tr / std::pow(0.95, 2);
    double denominator = std::pow(3.87, 2) * std::pow(diametr, 5);
    double volume_2 = roughness * density_relative * (double)temper * z * std::pow(volume, 2) * (double)length * std::pow(10, 4);
    double volume_1 = std::pow(press_start * std::pow(10, 6), 2);
    press_fin = std::sqrt(volume_1 - volume_2/denominator);
    press_fin = roundToNDecimals(press_fin, 6);
    }
  
public:
    PipLineSection(std::string name, double press_start, double volume, int length, double diam_1, double diam_2 = 0, 
    double diam_3 = 0);
    double calc_diam(double diam_1,double diam_2,double diam_3);
    double calc_press();
    double get_press_start()const;
    double get_z() const;
    double get_eq_diam()const;
    double get_density_relative()const;
    double get_quantiy()const;
    double get_press_fin()const;
    double get_re()const;
    double get_roughness()const;
    double get_roughness_tr()const;
    double get_volume()const;
    double get_length()const;
    double get_diam_1()const;
    double get_diam_2()const;
    double get_diam_3()const;
    double get_diametr()const;
    std::string get_name()const;
    void show_selection()const;
    void show()const;
};

void start_calculation(std::vector<PipLineSection> sections_main, std::vector<PipLineSection> sections_vilyisk, std::vector<PipLineSection> vilyisk_agrs);
void print_calculation(std::vector<PipLineSection> sections_main, std::vector<PipLineSection> sections_vilyisk, std::vector<PipLineSection> vilyisk_agrs, std::vector<PipLineSection> main_agrs, std::vector<PipLineSection> district_agrs, const std::string& filename);


#endif