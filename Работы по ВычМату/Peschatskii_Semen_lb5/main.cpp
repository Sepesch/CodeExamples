#include <iostream>
#include <cmath>

using namespace std;

double Func(double x){
    if (x< 0.0 || x > 1.0){
        cerr<<"x должен быть в диапазоне [0, 1]"<< endl;
        exit(1);
    }
    return exp(x) - acos(sqrt(x));
}
double FuncDer(double x){
    if (x< 0.0 || x > 1.0){
        cerr<<"x должен быть в диапазоне [0, 1]"<< endl;
        exit(1);
    }
    double tmp = x*(1-x);
    return exp(x) + 1/(2*sqrt(tmp));
}
double NewtonMethod(double init, double eps, int &iter) {
    int max_iter = 1000;
    double x = init;
    double xPrev;

    while (fabs(x - xPrev)>eps) {
        xPrev = x;
        x = xPrev - Func(xPrev)/FuncDer(xPrev);
        iter++;
        if(iter >= max_iter){
            cout << "Достигнуто максимальное число итераций (" << max_iter << ")\n";
            break; 
        }
    }

    return x;
}
int main(){
    int i;
    int iter=0;
    double eps;
    cin>>i;
    cout.precision(i);
    eps = pow(10, -i);
    cout<<NewtonMethod(0.16, eps, iter)<<"\t"<<iter<<endl;
    return 0;
}