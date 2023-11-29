#include <iostream>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

unsigned long long count = 0;

void putTile(unsigned long long p, unsigned long long line, unsigned long long column, unsigned long long tileDim, vector<vector<unsigned long long>> &busy) {
    unsigned long long i,j;
    for (i = line ; i < tileDim + line ; i++){
        for (j = column ; j < tileDim + column ; j++){
            busy[i][j] = p;
        }
    }
}

void decreaseTile(unsigned long long p, unsigned long long line, unsigned long long column, unsigned long long tileDim, vector<vector<unsigned long long>> &busy) {
    unsigned long long i,j;
    unsigned long long lastLine = tileDim + line -1;
    unsigned long long lastColumn = tileDim + column -1;
    for (i = line ; i < tileDim + line ; i++){
        for (j = column ; j < tileDim + column ; j++){
            if (i == lastLine || j == lastColumn)
                busy[i][j] = 0;
            else
                busy[i][j] = p;
        }
    }
}

unsigned long long maxTileSize(unsigned long long line, unsigned long long column, unsigned long long cValue, unsigned long long size) {
    unsigned long long length, height; 

    height = size - line;
    length = cValue - column;

    return (height < length) ? height : length;
}

void combinations(unsigned long long line,unsigned long long column, unsigned long long n, vector<unsigned long long> &floor, unsigned long long tileDim, vector<vector<unsigned long long>> &busy){
    unsigned long long i,j,cValue,iTile,newTile;

    bool done = false;
    bool isbusy;

    putTile(1,line,column,tileDim,busy);
    
    for (i=line ; i < n ; i++){
        if (i==n-1 || done){
            if (!done)
                count++;
            tileDim--;
            if (tileDim==0) {
                putTile(0,line,column,tileDim+1,busy);
                break;
            }    
            decreaseTile(1,line,column,tileDim+1,busy);
            i=line;
            done = false;
        }

        cValue = floor[i];
        for (j = 0 ; j < cValue-1; j++){

            if ((i == line && j <= column) || (j >= column-1 && j < column+tileDim && i<=line+tileDim-1))
                j = column+tileDim-1;
    
            else if (busy[i][j]==0){
                newTile = maxTileSize(i, j, cValue, n);

                if (j<column && i<=line+tileDim-1 && column-j<=newTile)
                    newTile = column-j;
                
                if (newTile>1){
                    isbusy = false;
                
                    for (iTile = newTile; iTile>1; iTile--) {
                        if (busy[i][j+iTile-1]>0){
                            if (!isbusy)  
                                isbusy = true;
                            newTile = iTile-1;
                        }
                    }

                    if (newTile>1){
                        combinations(i,j,n,floor,newTile,busy);
                        done = true;
                        break;
                    }
                }
            }
        }                         
    }
} 


int main() {
    unsigned long long n,m,cValue,i,line=0,column=0,tileDim=0;
    vector<unsigned long long> floor;
    bool max = true;

    cin >> n;
    cin >> m;
    
    vector<vector<unsigned long long>> busy(n,vector<unsigned long long>(m));

    for (i=0 ; i<n ; i++){
        cin >> cValue;
        floor.push_back(cValue);
        if (cValue > 0 && max) {
            tileDim = maxTileSize(i, column, cValue, n);
            line=i;
            max = false;
        }
    }
    auto start = high_resolution_clock::now();
    
    if (!max) 
        combinations(line,column,n,floor,tileDim,busy);
        
    
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout << count << endl;
    cout << duration.count() << endl;
    return 0;
}