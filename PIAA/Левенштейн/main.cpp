
#include <iostream>
#include <vector>
using namespace std;
//глобальная константа, отвечающая за режим дебага
const bool DEBUG = true;
//функция, заполнаяющая матрицу для алгоритма Вагнера-Фишера
void LDTable(string A, string B, int lenA, int lenB, vector<vector<int>> & VFM , vector<vector<int>> & opers, vector<int> &cost){
    //проход по всей матрице VFM
    for(int j=1; j<lenB;j++){
        //заполнение первой строки стоимостью удаления
        VFM[0][j]=VFM[0][j-1]+cost[1];
        opers[0][j]=2;
    }
    int minvfm;
    for(int i=1; i<lenA; i++){
        //заполнение первого столбца стоимостью вставки
        VFM[i][0]=VFM[i-1][0]+cost[2];
        opers[i][0] = 3;
        for(int j=1; j<lenB; j++){
            //переменная для определения минимальной по стоимости операции на данном шаге
            minvfm =  min(min(VFM[i-1][j-1] + cost[0], VFM[i][j-1] + cost[1]), VFM[i-1][j] + cost[2]);
            //проверка на возможность двойной вставки
            if(j>=2 && B[j-1]==B[j-2] && (min(minvfm, VFM[i][j-2] + cost[3]) ==VFM[i][j-2] + cost[3])){
                VFM[i][j] = min(minvfm, VFM[i][j-2] + cost[3]);
                opers[i][j]=4;
                continue;
            }
            //информация о лучшей по стоимости операции, или о совпадении символов, заносится в VFM и opers
            if(A[i-1] != B[j-1]){
                VFM[i][j] = minvfm;
                if(VFM[i][j]==VFM[i-1][j-1] + cost[0]){
                    opers[i][j]=1;
                }
                if(VFM[i][j]==VFM[i][j-1] + cost[1]){
                    opers[i][j]=2;
                }
                if(VFM[i][j]==VFM[i-1][j]+cost[2]){
                     opers[i][j]=3;
                }

            }
                
        else{
                VFM[i][j] = VFM[i-1][j-1];
                opers[i][j]=0;
            }

        }
    }
}
//функция для получения строки операции для оптимального преобразования строк
void getOperations(int lenA, int lenB, vector<vector<int>> opers, string &res){
    //заполнение строки начинается с конца
    int i=lenA-1; int j=lenB-1;
    while(i>0 || j>0){
        if(opers[i][j]==0){
            res.append("M");
            i--;j--;
        }
        if(opers[i][j]==1){
            res.append("R");
            i--;j--;
        }
        if(opers[i][j]==2){
            if(j!=0){res.append("I");
            j--;}
        }
        if(opers[i][j]==3){
            if(i!=0){res.append("D");
            i--;}
        }     
        if(opers[i][j]==4){
            if(j>=2){res.append("T");
                j--;j--;}
        }  
    }
}
void printTable(int lenA, int lenB, vector<vector<int>> mat){
        for(int i=0; i<lenA; i++){
            for(int j=0; j<lenB; j++){
                cout<<mat[i][j]<<" ";
            }
            cout<<endl;
        }
}
int main(){
    //чтение стоимостей операций
    vector<int> cost(3);
    //cin>>cost[0]>>cost[1]>>cost[2]>>cost[3];
    cost[0] = 3; cost[1] = 3; cost[2] = 3; cost[3] = 1;
    //чтение строк A и B
    string A, B;
    cin>>A;
    cin>>B;
    int lenA = A.length()+1;
    int lenB = B.length()+1;
    //построение матриц VFM и opers
    //в матрицу opers записываются операции
    vector<vector<int>> VFM;
    vector<int> tmp(lenB);
    for(int j=0; j<lenA; j++){
        VFM.push_back(tmp);
    }
    vector<vector<int>> opers = VFM;
    for(int i=0; i<lenA; i++){
        for(int j=0; j<lenB; j++){
            VFM.at(i).at(j)=0;
        }
    }
    //заполнение матриц VFM и opers
    LDTable(A, B, lenA, lenB, VFM, opers, cost);
    //получение строки необходимых оперций для преобразования строки A в строку B
    string res;
    getOperations(lenA, lenB, opers, res);
    //вывод матриц VFM и opers если программа в режиме дебага
    if(DEBUG){
        printTable(lenA, lenB, VFM);
        cout<<endl;
        printTable(lenA, lenB, opers);
    }
    //вывод расстояния Левенштейна
    cout<<VFM[lenA-1][lenB-1]<<endl;
    //вывод строки операций и строк A и B
    string ans=res;
    //строку операций необходимо отразить по скольку она восстанавливается в обратон порядке
    for(int i=0; i<size(res); i++){
        ans[i]=res[size(res)-i-1];
    }
    cout<<ans<<endl<<A<<endl<<B;
    return 0;
}