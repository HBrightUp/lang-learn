#include <iostream>
#include<set>
#include<map>
#include<algorithm>
#include <sys/time.h>

using namespace std;

struct stMapData
{
    int nSocket;
    string str;
};

typedef map<uint, stMapData>  MAPDATA;
void CalOnline(const set<uint>& setData, const map<uint, stMapData>& mapData, vector<uint>& vc );


int main()
{

    set<uint>  DataSet;

    for(uint  i = 0; i <10000; i = i + 5)
    {
        DataSet.insert(i);
    }

      set<uint>  DataSet10;
       set<uint>  Ret;



    for(uint i = 0; i < 10000000;  ++i)
    {
       DataSet10.insert(i);
    }

    struct timeval start, end;
    gettimeofday( &start, NULL );

     set_intersection(DataSet.begin(),DataSet.end(), DataSet10.begin(), DataSet10.end(), inserter(Ret,Ret.begin()));

    gettimeofday( &end, NULL );
    int timeuse = 1000000 * ( end.tv_sec - start.tv_sec ) + end.tv_usec - start.tv_usec;
    printf("time: %d us\n", timeuse);





    return 0;
}

void CalOnline(const set<uint>& setData, const map<uint, stMapData>& mapData, vector<uint>& vc )
{


}
