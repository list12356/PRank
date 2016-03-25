#include"Model.h"
using namespace std;


int main() {
    Model M;
    M.init("E:/CS/ClionProjects/PRank 1.1/doc/");
    M.calc_all();
    M.print();
    return 0;
}