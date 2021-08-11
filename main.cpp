#include <iostream>
#include <vector>
#include <cmath>

#define DEBUG false

struct vertCon {
    bool marked;
    std::vector<int>* comp;
    std::vector<int> connectsTo;

    vertCon() {
        comp = nullptr;
        marked = false;
    }
};

int rad(int digit, int pos) {
    int del = (int)pow(10, pos);
    int dif = (int)pow(10, pos - 1);

    if (digit % dif == digit) {
        return -1;
    }

    int res = (digit % del) / dif;
    return res;
}

std::vector<int> radixSort(std::vector<int> input) { 
    std::vector<int> prom(input.size());
    int countArr[11];
    for (int i = 0; i < 11; ++i)
        countArr[i] = 0;

    int digit = 1;

    while (countArr[0] != (int)input.size()) {


        for (int i = 0; i < 11; ++i)
            countArr[i] = 0;

        for (int i = 0; i < (int)input.size(); ++i) {
            countArr[rad(input[i], digit) != -1 ? rad(input[i], digit) + 1 : 0] += 1;
        }

        int k = 0;
        int flp = 0;
        for (int i = 0; i < 11; ++i) {
            k = countArr[i];
            countArr[i] = flp;
            flp = k;
        }
        for (int i = 2; i < 11; ++i) {
            countArr[i] += countArr[i - 1];
        }

        for (int i = 0; i < (int)input.size(); ++i) {
            int rdn = rad(input[i], digit);
            prom[countArr[rdn != -1 ? rdn + 1 : 0]] = input[i];
            countArr[rdn != -1 ? rdn + 1 : (0)] ++;
        }

        input = prom;
        prom = std::vector<int>(input.size());

        digit++;
    }

    return(input);
}

std::vector<int>* passComponent(int start, std::vector<vertCon>& vs, std::vector<int>* constructed, std::vector<int>* prime) {
    
    constructed->push_back(start);

    if (DEBUG) {
        printf("start: %d |\nconnects: ", start);
        for (size_t i = 0; i < vs[start].connectsTo.size(); ++i) {
            printf("%d ", vs[start].connectsTo[i]);
        }
        printf("|\nconstructed: ");
        if (prime) {
            printf("|\nprime: ");
        }
        printf("\n-----------------\n");
    }

    vs[start].marked = true;
    vs[start].comp = constructed;

    for (int i = 0; i < (int)vs[start].connectsTo.size(); ++i) {
        if (!vs[vs[start].connectsTo[i]].marked) {
            prime = passComponent(vs[start].connectsTo[i], vs, constructed, prime);
            
            if (prime) {
                for (size_t i = 0; i < constructed->size(); ++i) {
                    prime->push_back((*constructed)[i]);
                }
            }
        }
        else {
            if (!vs[vs[start].connectsTo[i]].comp) {
                continue;
            }

            if (vs[vs[start].connectsTo[i]].comp == prime) {
                continue;
            }

            if (prime) {
                for (size_t i = 0; i < vs[vs[start].connectsTo[i]].comp->size(); ++i) {
                    prime->push_back((*(vs[vs[start].connectsTo[i]].comp))[i]);
                }
            }
            else {
                if (vs[vs[start].connectsTo[i]].comp != constructed) {
                    prime = vs[vs[start].connectsTo[i]].comp;

                    for (size_t i = 0; i < constructed->size(); ++i) {
                        prime->push_back((*constructed)[i]);
                    }

                    if (DEBUG) {
                        printf("\t|| prime found: ");
                        printf("||\n");
                    }
                }
            }
        }
    }

    return prime;
}

int main() {
    int vertCount, pathCount;
    std::cin >> vertCount >> pathCount;

    std::vector<vertCon> verts(vertCount + 1);

    int vertA, vertB;
    for (int i = 0; i < pathCount; ++i) {
        std::cin >> vertA >> vertB;
        if (vertA > vertB) {
            verts[vertB].connectsTo.push_back(vertA);
        }
        else {
            verts[vertA].connectsTo.push_back(vertB);
        }
    }

    std::vector<std::vector<int>*> components;
    for (int i = 1; i < vertCount + 1; ++i) {

        if (verts[i].marked) {
            continue;
        }

        std::vector<int>* component = new std::vector<int>();
        std::vector<int>* pr = passComponent(i, verts, component, nullptr);

        if (!pr) {
            components.push_back(component);
        }

    }

    for (size_t i = 0; i < components.size(); ++i) {
        std::vector<int> cur = radixSort(*(components[i]));
        for (size_t j = 0; j < cur.size(); ++j) {
            printf("%d ", cur[j]);
        }
        printf("\n");

        delete components[i];
    }
}