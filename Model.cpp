#include"Model.h"


Model::Model()
{
    n_a=0;
    n_v=0;
    n_p=0;
    damping=0;
    alpha=0;
    beta=0;
}

void Model::init(string set_path)
{
    path=set_path;
    ifstream input(path+"global.txt");
    input>>alpha>>beta>>damping;

    read_paper();
    read_ref();
    read_author();

}

void Model::read_paper()
{
    ifstream input(path+"papers.txt");
    string paper_name,venue_name;
    int paper_time;

    cout<<"loading papers...."<<endl;
    while(input>>paper_name>>paper_time>>venue_name)
    {
        Map_Paper.insert(pair<string,int>(paper_name,n_p));
        Map_Venue.insert(pair<string,int>(venue_name,n_v));
        //if(n_p<Map_Paper.size()) //whether there is a new paper
        {
            n_p++;
            Paper *t=new Paper;
            Papers.push_back(*t);
        }
        if(n_v<Map_Venue.size()) //whether there is a new venue
        {
            n_v++;
            Venue *t=new Venue;
            Venues.push_back(*t);
        }
        Papers[Map_Paper[paper_name]].Venue=Map_Venue[venue_name];
        Papers[Map_Paper[paper_name]].time=paper_time;
        Venues[Map_Venue[venue_name]].Papers.push_back(Map_Paper[paper_name]);
    }
    cout<<"papers loading successfully!"<<endl;
}

void Model::read_ref()
{
    ifstream input(path+"references.txt");
    string p,q;                //p cites q
    N.resize(n_p);
    N_cite=new int[n_p];
    memset(N_cite,0,sizeof(int)*n_p);
    cout<<"loading references"<<endl;

    while(input>>p>>q)
    {
        Papers[Map_Paper[p]].References.push_back(Map_Paper[q]);
        N[Map_Paper[p]].push_back(pair<int,float>(Map_Paper[q],1.0));
        N_cite[Map_Paper[q]]++;
    }
    cout<<"references loading successfully"<<endl;
}

void Model::read_author()
{
    ifstream input(path+"authors.txt");
    string author_name,paper_name,no_use;//we do not use affliation

    cout<<"loading authors......"<<endl;
    while(input>>paper_name>>author_name>>no_use) {
        Map_Author.insert(pair<string, int>(author_name, n_a));

        //if there is a new author
        if (n_a < Map_Author.size()) {
            n_a++;
            Author *t = new Author;
            Authors.push_back(*t);
        }
        Papers[Map_Paper[paper_name]].Authors.push_back(Map_Author[author_name]);
        Authors[Map_Author[author_name]].Papers.push_back(Map_Paper[paper_name]);
    }
    cout<<"authors loading successfully"<<endl;
}

void Model::print()
{
    ofstream output(path+"PRank.txt");

    for(map<string,int>::iterator it=Map_Paper.begin();it!=Map_Paper.end();it++)
    {
        output<<it->first<<"\t"<<Papers[it->second].time<<"\t"<<x_article[it->second]<<endl;
}
}
void Model::calc_N()
{
    for(int i=0;i<n_p;i++)
    {
        for(int j=0;j<N[i].size();j++)
        {
            N[i][j].second=-damping*N[i][j].second/N_cite[N[i][j].first];
        }
        N[i].push_back(pair<int,float>(i,1));
    }
}
void Model::calc_all()
{
    //pre-calculate some vector and Matrix
    //initialize x_article
    for(int i=0;i<n_p;i++)
    {
        float tmp=1.0/n_p;
        x_article.push_back(tmp);
    }
    v.resize(n_p);
    x_author.resize(n_a);
    x_venue.resize(n_v);
    calc_N();
    float error=1;
    float prev=x_article[0];
    int count=0;

    while(error>ERROR)
    {
        //error=0;
        calc_v();
        calc_G_S();
        float sum=0;
        for(int i=0;i<n_p;i++) {
            sum+=x_article[i];
        }
        for(int i=0;i<n_p;i++) {
            x_article[i]/=sum;
        }
        error=fabs(x_article[0]-prev)/prev;
        cout<<"iteration: "<<++count<<" error: "<<error<<endl;
        prev=x_article[0];
    }
}
void Model::calc_v()
{
    for(int i=0;i<n_a;i++) {
        x_author[i]=0;
        for (int j = 0; j < Authors[i].Papers.size(); j++) {
            x_author[i] += x_article[Authors[i].Papers[j]];
        }
    }
    for(int i=0;i<n_v;i++) {
        x_venue[i]=0;
        for (int j = 0; j < Venues[i].Papers.size(); j++) {
            x_venue[i] += x_article[Venues[i].Papers[j]];
        }
    }
    for(int i=0;i<n_p;i++)
    {
        v[i]=0;
        for(int j=0;j<Papers[i].Authors.size();j++)
        {
            v[i]+=alpha*x_author[Papers[i].Authors[j]]/Authors[Papers[i].Authors[j]].Papers.size();
        }
        v[i]+=beta*x_venue[Papers[i].Venue]/Venues[Papers[i].Venue].Papers.size();
    }
    /*float sum=0;
    for(int i=0;i<n_p;i++) {
        sum+=v[i];
    }
    for(int i=0;i<n_p;i++) {
        v[i]/=sum;
    }*/
}
void Model::calc_G_S()
{
    float delta=0;
    float error=1;
    int num=0;
    while(error>G_S_STOP)
    //for(int k=0;k<20;k++)
    {
        error=0;
        for (int i = 0; i < n_p; i++) {
            float sum = 0;
            for (int j = 0; j < N[i].size(); j++)
            {
                sum += N[i][j].second * x_article[N[i][j].first];
            }
            delta=  (v[i]*(1-damping) - sum);
            x_article[i] +=delta;
            if(fabs(delta)/x_article[i]>error)
                error=fabs(delta)/x_article[i];
        }
        cout<<"G_S iteration: "<<++num<<"error: "<<error<<endl;
    }
}


