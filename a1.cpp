#include <iostream>
#include <fstream>
#include <random>
#include <cmath>
#include <vector>

using namespace std;

struct Krug {
    double x, y, r;
};

double tochnaya_ploshad() {
    return 0.25 * M_PI + 1.25 * asin(0.8) - 1.0;
}

bool proverka_tochki(double x, double y, const Krug& k) {
    double dx = x - k.x;
    double dy = y - k.y;
    return dx*dx + dy*dy <= k.r*k.r;
}

double monte_karlo(const vector<Krug>& krugi, 
                  double x1, double x2, double y1, double y2, 
                  int n) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dist_x(x1, x2);
    uniform_real_distribution<> dist_y(y1, y2);
    
    int popalo = 0;
    for (int i = 0; i < n; i++) {
        double x = dist_x(gen);
        double y = dist_y(gen);
        
        bool vo_vseh = true;
        for (const auto& k : krugi) {
            if (!proverka_tochki(x, y, k)) {
                vo_vseh = false;
                break;
            }
        }
        if (vo_vseh) popalo++;
    }
    
    double pl_pryam = (x2 - x1) * (y2 - y1);
    return (double)popalo / n * pl_pryam;
}

int main() {
    vector<Krug> krugi = {
        {1.0, 1.0, 1.0},
        {1.5, 2.0, sqrt(5.0) / 2.0},
        {2.0, 1.5, sqrt(5.0) / 2.0}
    };
    
    ofstream file1("shirokaya_oblast.csv");
    ofstream file2("uzkaya_oblast.csv");
    
    file1 << "N,Ploshad,Oshibka\n";
    file2 << "N,Ploshad,Oshibka\n";
    
    double tochno = tochnaya_ploshad();
    cout << "Tochnaya ploshad: " << tochno << endl;
    
    for (int N = 100; N <= 100000; N += 500) {
        double pl_shir = monte_karlo(krugi, 0.5, 2.5, 0.5, 2.5, N);
        double osh_shir = abs(pl_shir - tochno) / tochno;
        file1 << N << "," << pl_shir << "," << osh_shir << "\n";
        
        double pl_uzk = monte_karlo(krugi, 1.0, 2.0, 1.0, 2.0, N);
        double osh_uzk = abs(pl_uzk - tochno) / tochno;
        file2 << N << "," << pl_uzk << "," << osh_uzk << "\n";
        
        if (N % 10000 == 0) {
            cout << "N = " << N << ": shirokaya = " << pl_shir 
                 << ", uzkaya = " << pl_uzk << endl;
        }
    }
    
    file1.close();
    file2.close();
    cout << "Resultaty sohraneny v faily" << endl;
    
    return 0;
}