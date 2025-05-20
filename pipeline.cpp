#include "pipeline.h"
#include <fstream>

double density = 0.71;
int temper = 273;
double R = 8.3144598;
double x_a = 0.005487;
double x_y = 0.0003784;


PipLineSection::PipLineSection(std::string name, double press_start, double volume, int length, double diam_1, double diam_2, 
    double diam_3):name(name), press_start(press_start), volume(volume), length(length), diam_1(diam_1),diam_2(diam_2), diam_3(diam_3){
        eque_diametr = calc_diam(diam_1, diam_2, diam_3);
        calculateDerivedValues(); 
    }
double PipLineSection::calc_diam(double d1, double d2, double d3){
    double de = std::pow(std::pow(d1, 2.6) + std::pow(d2, 2.6) + std::pow(d3, 2.6), 1.0 / 2.6);
    return de;
};
double PipLineSection::calc_press(){
    return press_start;
};
double PipLineSection::get_z()const{return z;};
double PipLineSection::get_density_relative()const{return density_relative;};
double PipLineSection::get_eq_diam()const{return eque_diametr;};
double PipLineSection::get_quantiy()const{return quantity;};
double PipLineSection::get_press_fin()const{return press_fin;};
double PipLineSection::get_re()const{return re;};
double PipLineSection::get_volume()const{return volume;};
double PipLineSection::get_length()const{return length;};
double PipLineSection::get_diam_1()const{return diam_1;};
double PipLineSection::get_diam_2()const{return diam_2;};
double PipLineSection::get_diam_3()const{return diam_3;};
double PipLineSection::get_roughness()const{return roughness;};
double PipLineSection::get_roughness_tr()const{return roughness_tr;};
double PipLineSection::get_diametr()const{return diametr;}
double PipLineSection::get_press_start()const{return press_start;}
std::string PipLineSection::get_name()const{return name;}
void PipLineSection::show_selection()const{
    std::cout << "Name: " << name << std::endl;
    std::cout << "Press start: " << press_start << std::endl;
    std::cout << "Volume: " << volume << std::endl;
    std::cout << "Length: " << length << std::endl;
    std::cout << "Diametr 1: " << diam_1 << std::endl;
    std::cout << "Diametr 2: " << diam_2 << std::endl;
    std::cout << "Diametr 3: " << diam_3 << std::endl;
    std::cout << "Diametr equal: " << eque_diametr << std::endl;
    std::cout << "Roughness: " << roughness << std::endl;
    std::cout << "Roughness trans: " << roughness_tr << std::endl;
    std::cout << "Density relative: " << density_relative << std::endl;
    std::cout << "Z: " << z << std::endl;
    std::cout << "Quantity: " << quantity << std::endl;
    std::cout << "Press fin: " << press_fin << std::endl;
    std::cout << "Re: " << re << std::endl;

}
void PipLineSection::show()const{
    std::cout << "Name: " << name << std::endl;
    //std::cout << "Length: " << length << " metrs" << std::endl;
    //std::cout << "Press start: " << press_start << " MPa"<< std::endl;
    //std::cout << "Diametr 1: " << diam_1 << " mm."<<" Diametr 2: " << diam_2 << " mm."<<" Diametr 3: " << diam_3 << " mm."<<std::endl;
    //std::cout << "Diametr equal: " << eque_diametr << " mm."<<std::endl;
    std::cout << "Quantity: " << quantity << " millions m3 per day"<<std::endl;
    std::cout << "Press fin: " << press_fin << " MPa"<<std::endl;
}

void start_calculation(std::vector<PipLineSection> sections_main, std::vector<PipLineSection> sections_vilyisk, std::vector<PipLineSection> vilyisk_agrs){
    for (const auto& section : sections_main) {
        section.show(); // Вывод информации о текущей секции
        if (section.get_name() == "0-28") {
            for (const auto& viluysk_section : sections_vilyisk) {
                std::cout << std::endl;
                viluysk_section.show();
                if(viluysk_section.get_name() == "0-21"){
                    std::cout << std::endl;
                    vilyisk_agrs[0].show();
                    std::cout << std::endl;
                    vilyisk_agrs[1].show();
                }
                else if(viluysk_section.get_name() == "44-61"){
                    std::cout << std::endl;
                    vilyisk_agrs[2].show();
                }
                else if(viluysk_section.get_name() == "0-21.1"){
                    std::cout << std::endl;
                    vilyisk_agrs[3].show();
                }
                else if(viluysk_section.get_name() == "39-48.2"){
                    std::cout << std::endl;
                    vilyisk_agrs[4].show();
                }
                else if(viluysk_section.get_name() == "51-63.8"){
                    std::cout << std::endl;
                    vilyisk_agrs[5].show();
                }
                else if(viluysk_section.get_name() == "63.8-71"){
                    std::cout << std::endl;
                    vilyisk_agrs[6].show();
                }
            }
        }
        std::cout << std::endl; // Пустая строка для разделения
    }
}

void print_section(std::ostream& stream, std::vector<PipLineSection> sections, int n){
    stream<<std::endl;
    stream<<"            Участок - "<<sections[n].get_name()<<std::endl;
    stream<<"Давление в начале участка - "<<sections[n].get_press_start()<<" MPa"<<std::endl;
    stream<<"Диаметры ниток: 1 нитка - "<<sections[n].get_diam_1()<<" 2 нитка - "<<sections[n].get_diam_2()<<" 3 нитка - "<<sections[n].get_diam_3()<<std::endl;
    stream<<"Протяженность - "<<sections[n].get_length()<<" км."<<std::endl;
    stream<<"Расход по участку - "<<sections[n].get_quantiy()<<" млн. м3. в сутки"<<std::endl;
    stream<<"Давление в конце участка - "<<sections[n].get_press_fin()<<" MPa"<<std::endl;
}

void print_section(std::ostream& stream, std::vector<PipLineSection> sections, int n, int m){
    stream<<std::endl;
    stream<<"            Участок - "<<sections[n].get_name()<<", "<<sections[m].get_name()<<std::endl;
    stream<<"Давление в начале участка - "<<sections[n].get_press_start()<<" MPa"<<std::endl;
    stream<<"Диаметры ниток: 1 нитка - "<<sections[n].get_diam_1()<<" 2 нитка - "<<sections[n].get_diam_2()<<" 3 нитка - "<<sections[n].get_diam_3()<<std::endl;
    stream<<"Протяженность - "<<sections[n].get_length() + sections[m].get_length()<<" км."<<std::endl;
    stream<<"Расход по участку - "<<sections[m].get_quantiy()<<" млн. м3. в сутки"<<std::endl;
    stream<<"Давление в конце участка - "<<sections[m].get_press_fin()<<" MPa"<<std::endl;
}
void print_section(std::ostream& stream, std::vector<PipLineSection> sections, int n, int m, int i){
    stream<<std::endl;
    stream<<"            Участок - "<<sections[n].get_name()<<", "<<sections[m].get_name()<<", "<<sections[i].get_name()<<std::endl;
    stream<<"Давление в начале участка - "<<sections[n].get_press_start()<<" MPa"<<std::endl;
    stream<<"Диаметры ниток: 1 нитка - "<<sections[n].get_diam_1()<<" 2 нитка - "<<sections[n].get_diam_2()<<" 3 нитка - "<<sections[n].get_diam_3()<<std::endl;
    stream<<"Протяженность - "<<sections[n].get_length() + sections[m].get_length() + sections[i].get_length()<<" км."<<std::endl;
    stream<<"Расход по участку - "<<sections[i].get_quantiy()<<" млн. м3. в сутки"<<std::endl;
    stream<<"Давление в конце участка - "<<sections[i].get_press_fin()<<" MPa"<<std::endl;
}

void print_section_retract(std::ostream& stream, std::vector<PipLineSection> sections, int n){
    stream<<std::endl;
    stream<<"                                        Участок - "<<sections[n].get_name()<<std::endl;
    stream<<"                            Давление в начале участка - "<<sections[n].get_press_start()<<" MPa"<<std::endl;
    stream<<"                            Диаметры ниток: 1 нитка - "<<sections[n].get_diam_1()<<" 2 нитка - "<<sections[n].get_diam_2()<<" 3 нитка - "<<sections[n].get_diam_3()<<std::endl;
    stream<<"                            Протяженность - "<<sections[n].get_length()<<" км."<<std::endl;
    stream<<"                            Расход по участку - "<<sections[n].get_quantiy()<<" млн. м3. в сутки"<<std::endl;
    stream<<"                            Давление в конце участка - "<<sections[n].get_press_fin()<<" MPa"<<std::endl;
}

void print_section_retract(std::ostream& stream, std::vector<PipLineSection> sections, int n, int m){
    stream<<std::endl;
    stream<<"                                        Участок - "<<sections[n].get_name()<<", "<<sections[m].get_name()<<std::endl;
    stream<<"                            Давление в начале участка - "<<sections[n].get_press_start()<<" MPa"<<std::endl;
    stream<<"                            Диаметры ниток: 1 нитка - "<<sections[n].get_diam_1()<<" 2 нитка - "<<sections[n].get_diam_2()<<" 3 нитка - "<<sections[n].get_diam_3()<<std::endl;
    stream<<"                            Протяженность - "<<sections[n].get_length() + sections[m].get_length()<<" км."<<std::endl;
    stream<<"                            Расход по участку - "<<sections[m].get_quantiy()<<" млн. м3. в сутки"<<std::endl;
    stream<<"                            Давление в конце участка - "<<sections[m].get_press_fin()<<" MPa"<<std::endl;
}
void print_section_retract(std::ostream& stream, std::vector<PipLineSection> sections, int n, int m, int i){
    stream<<std::endl;
    stream<<"                                        Участок - "<<sections[n].get_name()<<", "<<sections[m].get_name()<<", "<<sections[i].get_name()<<std::endl;
    stream<<"                            Давление в начале участка - "<<sections[n].get_press_start()<<" MPa"<<std::endl;
    stream<<"                            Диаметры ниток: 1 нитка - "<<sections[n].get_diam_1()<<" 2 нитка - "<<sections[n].get_diam_2()<<" 3 нитка - "<<sections[n].get_diam_3()<<std::endl;
    stream<<"                            Протяженность - "<<sections[n].get_length() + sections[m].get_length() + sections[i].get_length()<<" км."<<std::endl;
    stream<<"                            Расход по участку - "<<sections[i].get_quantiy()<<" млн. м3. в сутки"<<std::endl;
    stream<<"                            Давление в конце участка - "<<sections[i].get_press_fin()<<" MPa"<<std::endl;
}

void print_section_retract(std::ostream& stream, std::vector<PipLineSection> sections, int n, int m, int i, int j){
    stream<<std::endl;
    stream<<"                                        Участок - "<<sections[n].get_name()<<", "<<sections[m].get_name()<<", "<<sections[i].get_name()<<", "<<sections[j].get_name()<<std::endl;
    stream<<"                            Давление в начале участка - "<<sections[n].get_press_start()<<" MPa"<<std::endl;
    stream<<"                            Диаметры ниток: 1 нитка - "<<sections[n].get_diam_1()<<" 2 нитка - "<<sections[n].get_diam_2()<<" 3 нитка - "<<sections[n].get_diam_3()<<std::endl;
    stream<<"                            Протяженность - "<<sections[n].get_length() + sections[m].get_length() + sections[i].get_length() + sections[j].get_length()<<" км."<<std::endl;
    stream<<"                            Расход по участку - "<<sections[j].get_quantiy()<<" млн. м3. в сутки"<<std::endl;
    stream<<"                            Давление в конце участка - "<<sections[j].get_press_fin()<<" MPa"<<std::endl;
}

void print_calculation(std::vector<PipLineSection> sections_main, std::vector<PipLineSection> sections_vilyisk, std::vector<PipLineSection> vilyisk_agrs, std::vector<PipLineSection> main_agrs, std::vector<PipLineSection> dirict_agrs, const std::string& filename){
    std::ofstream outFile(filename); // Открываем файл для записи
    if (!outFile.is_open()) {
        std::cerr << "Ошибка открытия файла: " << filename << std::endl;
        return;
    }

    
    outFile<<std::endl;
    outFile<<"                МАГИСТРАЛЬНЫЙ ГАЗОПРОВОД СВГКМ - МАСТАХ"<<std::endl;
    print_section(outFile, sections_main, 0);
    outFile<<std::endl;
    outFile<<"                                        ГАЗОПРОВОД ОТВОД ВИЛЮЙСК - ВЕРХНЕВИЛЮЙСК"<<std::endl;
    print_section_retract(outFile, sections_vilyisk, 0);
    print_section_retract(outFile, vilyisk_agrs, 0);
    print_section_retract(outFile, vilyisk_agrs, 1);    
    print_section_retract(outFile, sections_vilyisk, 1, 2);
    print_section_retract(outFile, vilyisk_agrs, 3);
    print_section_retract(outFile, sections_vilyisk, 3);
    print_section_retract(outFile, sections_vilyisk, 4);
    print_section_retract(outFile, vilyisk_agrs, 4);
    print_section_retract(outFile, sections_vilyisk, 5, 6, 7, 8);
    print_section_retract(outFile, vilyisk_agrs, 5);
    print_section_retract(outFile, sections_vilyisk, 9, 10);
    print_section_retract(outFile, vilyisk_agrs, 6);
    print_section_retract(outFile, vilyisk_agrs, 7);
    print_section_retract(outFile, sections_vilyisk, 11);
    print_section(outFile, sections_main, 1, 2, 3);
    print_section_retract(outFile, main_agrs, 0);
    print_section_retract(outFile, main_agrs, 1);
    print_section_retract(outFile, main_agrs, 2);
    print_section(outFile, sections_main, 4, 5);
    print_section_retract(outFile, main_agrs, 3);
    print_section_retract(outFile, main_agrs, 4);
    outFile<<std::endl;
    outFile<<"                МАГИСТРАЛЬНЫЙ ГАЗОПРОВОД МАСТАХ - БЕРГЕ"<<std::endl;
    print_section(outFile, sections_main, 6);
    print_section(outFile, sections_main, 7, 8);
    print_section_retract(outFile, main_agrs, 5);
    print_section(outFile, sections_main, 9);
    print_section(outFile, sections_main, 10, 11, 12);
    print_section_retract(outFile, main_agrs, 6);
    print_section_retract(outFile, main_agrs, 7);
    print_section_retract(outFile, main_agrs, 8);
    print_section_retract(outFile, main_agrs, 9);
    print_section(outFile, sections_main, 13);
    print_section_retract(outFile, main_agrs, 10);
    print_section(outFile, sections_main, 14);
    print_section(outFile, sections_main, 15);
    print_section_retract(outFile, main_agrs, 11);
    outFile<<std::endl;
    outFile<<"                МАГИСТРАЛЬНЫЙ ГАЗОПРОВОД БЕРГЕ - ЯКУТСК"<<std::endl;
    print_section(outFile, sections_main, 16);
    print_section_retract(outFile, main_agrs, 12);
    print_section(outFile, sections_main, 17);
    outFile<<std::endl;
    outFile<<"                                        ГАЗОПРОВОД ОТВОД 134 км. - БЕРДИГЕСТЯХ"<<std::endl;
    print_section_retract(outFile, main_agrs, 13);
    print_section_retract(outFile, main_agrs, 14);
    print_section_retract(outFile, main_agrs, 15);
    print_section_retract(outFile, main_agrs, 16);
    print_section_retract(outFile, main_agrs, 17);
    print_section(outFile, sections_main, 18);
    print_section(outFile, sections_main, 19, 20);
    print_section_retract(outFile, main_agrs, 18);
    print_section(outFile, sections_main, 21, 22, 23);
    outFile<<std::endl;
    outFile<<"                                        ГАЗОПРОВОД ОТВОД 199 км. - НАМЦЫ - ХАТЫРЫК"<<std::endl;
    print_section_retract(outFile, main_agrs, 19);
    print_section_retract(outFile, main_agrs, 20);
    print_section_retract(outFile, main_agrs, 21);
    print_section_retract(outFile, main_agrs, 22);
    outFile<<std::endl;
    outFile<<"                                        ГАЗОПРОВОД ОТВОД ТААС-ТАХ"<<std::endl;
    print_section_retract(outFile, main_agrs, 23);
    print_section(outFile, sections_main, 24);
    print_section(outFile, sections_main, 25);
    print_section(outFile, sections_main, 26, 27);
    outFile<<std::endl;
    outFile<<"                                        ГАЗОПРОВОД ОТВОД ПОКРОВСК"<<std::endl;
    print_section_retract(outFile, main_agrs, 24);
    print_section_retract(outFile, main_agrs, 25);
    print_section_retract(outFile, main_agrs, 26);
    print_section_retract(outFile, main_agrs, 27);
    outFile<<std::endl;
    outFile<<"                                        ГАЗОПРОВОД ОТВОД ЗАРЕЧЬЕ"<<std::endl;
    for (int i =0; i < 19; i++){
        print_section_retract(outFile, dirict_agrs, i);
    }
}