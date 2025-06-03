#include <iostream>
#include <cmath>
#include <iomanip>

const double g = 9.81;
const double PI = 3.14159265358979323846;
const double EPS = 1e-6;

double calculate_range(double v0, double alpha) {
    return (v0 * v0 * sin(2 * alpha)) / g;
}

double find_min_v0(double alpha, double x_target) {
    double left = 0.0;
    double right = 1e6;

    if (sin(2 * alpha) < EPS) {
        std::cerr << "Ошибка: угол α недопустим (sin(2α) ≈ 0)\n";
        return -1.0;
    }

    for (int i = 0; i < 100; ++i) {
        double mid = (left + right) / 2;
        double x_mid = calculate_range(mid, alpha);

        if (x_mid >= x_target - EPS) {
            right = mid;
        } else {
            left = mid;
        }
    }

    return right;
}

double calculate_v0_analytical(double alpha, double x_target) {
    if (sin(2 * alpha) < EPS) {
        return -1.0;
    }
    return sqrt((g * x_target) / sin(2 * alpha));
}

int main() {
    double alpha_deg, x1, x2;
    std::cout << "Введите угол α (в градусах, 0 < α < 90): ";
    std::cin >> alpha_deg;
    std::cout << "Введите x1 и x2 (x1 < x2): ";
    std::cin >> x1 >> x2;

    double alpha_rad = alpha_deg * PI / 180.0;

    if (alpha_deg <= 0 || alpha_deg >= 90) {
        std::cerr << "Ошибка: угол α должен быть в интервале (0, 90)\n";
        return 1;
    }

    double v0_binary = find_min_v0(alpha_rad, (x1 + x2) / 2);
    double x_binary = calculate_range(v0_binary, alpha_rad);

    double v0_analytical = calculate_v0_analytical(alpha_rad, (x1 + x2) / 2);
    double x_analytical = calculate_range(v0_analytical, alpha_rad);

    std::cout << "\nРезультаты:\n";
    std::cout << "----------------------------------------\n";
    std::cout << "Метод бинарного поиска:\n";
    std::cout << "  Скорость v0: " << v0_binary << " м/с\n";
    std::cout << "  Дальность полёта: " << x_binary << " м\n";
    std::cout << "----------------------------------------\n";
    std::cout << "Аналитическое решение:\n";
    std::cout << "  Скорость v0: " << v0_analytical << " м/с\n";
    std::cout << "  Дальность полёта: " << x_analytical << " м\n";
    std::cout << "----------------------------------------\n";

    if (x_binary < x1 - EPS || x_binary > x2 + EPS) {
        std::cout << "Предупреждение: снаряд не попал в заданный отрезок!\n";
    }

    return 0;
}