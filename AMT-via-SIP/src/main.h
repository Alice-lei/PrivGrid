#include "share.h"
#include "mulgate.h"
#include "network.h"
#include "triplescheck.h"
#include "plainmatrix.h"

using namespace std;
class Lstm
{
public:
    Lstm();
    void triplesGen(int epochsT, int epochsP);
};

Lstm::Lstm()
{
}

//三元组生成
void Lstm::triplesGen(int epochsT, int epochsP)
{
    Triples triples;
    cout << "\nTriples generat start!" << endl;
    triples.triplesGen(epochsT, epochsP);
    cout << "\nTriples generat done!" << endl;
}
