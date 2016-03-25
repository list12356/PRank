#include<iostream>
#include<string.h>
#include<fstream>
#include<vector>
#include<cstdlib>
#include<map>
#include<set>
#include<cmath>
#define ERROR 0.001
#define G_S_STOP 0.001

using namespace std;

class Paper
{
public:
    Paper(){}
    vector<int> Authors;
    vector<int> References;
    int time;
    int Venue;
};
class Author
{
public:
    Author(){}
    vector<int> Papers;
};
class Venue
{
public:
    Venue(){}
    vector<int> Papers;
};
class Model
{
public:
    string path;
    float alpha;
    float beta;
    float damping;
    int n_p;
    int n_a;
    int n_v;
    map<string,int> Map_Paper;
    map<string,int> Map_Author;
    map<string,int> Map_Venue;
    vector<vector<pair<int,float>>> N;
    int* N_cite; //citation counts recieved, the row sum of N
    vector<float> v;    //peronalized vector
    vector<float> x_author;
    vector<float> x_venue;
    vector<float> x_article;
    vector<Venue> Venues;
    vector<Author> Authors;
    vector<Paper> Papers;
    Model();
    void init(string set_path);
    void read_paper();
    void read_ref();
    void read_author();
    void calc_v();
    void calc_all();
    void calc_N();
    void calc_G_S();
    void print();
};
