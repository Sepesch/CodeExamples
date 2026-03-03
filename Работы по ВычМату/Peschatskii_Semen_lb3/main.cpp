#include <iostream>
#include <cmath>

using namespace std;

double Round (double X,double Delta)
{
    if (Delta<=1E-9) 
    {
        cerr << "Неверное задание точности округления" << std::endl;
        exit(1);
    }
    if (X>0.0) return (Delta*(long((X/Delta)+0.5)));
    else    return (Delta*(long((X/Delta)-0.5)));
}


double F(double x, double delta)
{
    return Round(exp(x) - acos(sqrt(x)), delta); // Округление до заданной точности
} // функция f (x)



double F1(double x) {
    return exp(x) + 1/(2*sqrt(x-x*x));
} // производная функции f'(x)


double BISECT(double Left, double Right, double Eps, int &N, double delta) {
    double E = fabs(Eps) * 2.0;
    double FLeft = F(Left, delta);
    double FRight = F(Right, delta);
    double X = (Left + Right) / 2.0;
    double Y;
    
    if (FLeft * FRight > 0.0) {
        cerr << "Неверное задание интервала" << endl;
        exit(1);
    }
    if (Eps <= 0.0) {
        cerr << "Неверное задание точности" << endl;
        exit(1);
    }
    
    N = 0;
    if (FLeft == 0.0) return Left;
    if (FRight == 0.0) return Right;
    
    while ((Right - Left) >= E) {
        X = 0.5 * (Right + Left); // вычисление середины отрезка
        Y = F(X, delta);
        if (Y == 0.0) return X;
        if (Y * FLeft < 0.0) {
            Right = X;
        } else {
            Left = X;
            FLeft = Y;
        }
        N++;
    }
    
    return X;
}


int main(){
	int k;
	double a, b, eps, x;
	a = 0; b = 1;
    std::cout << "eps" << "\t" << "delta" << "\t" << "a" << "\t" << "b" << "\t" << "x" << "\t" << "k" << "\t" << "k_t" << "\n";
    double delta = 0.0001;

    for (eps = 0.1; eps >= 0.00001; eps /= 10){
        x = BISECT(a, b, eps, k, delta);
        std::cout << eps << "\t" << delta << "\t" << a << "\t" << b << "\t" << x << "\t" << k << "\t" << (int)round(log2((b - a) / eps)) << "\n";
    }
    
	std::cout << "eps" <<"\t\t" << "delta" << "\t\t" << "a" << "\t\t" << "b" << "\t\t" << "x" << "\t\t" << "V_max" << "\t\t" << "V" << "\t\t" << "C" << "\n";

	for (double delta = 0.1; delta >= 0.000001; delta /= 10) {
		for (eps = 0.1; eps >= 0.000001; eps /= 10) {
			x = BISECT(a, b, eps, k, delta);
			std::cout << eps << "\t\t" << delta << "\t\t" << a << "\t\t" << b << "\t\t" << x << "\t\t" << eps / delta << "\t\t" << 1 / (double)F1(x) << "\t\t" << ((eps / delta) > (1 / F1(x))) << "\n";
		}
	}
	return 0;
}
