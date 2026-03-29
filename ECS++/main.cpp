#include <iostream>
#include <ArrayList.hpp>

int main() {
    ArrayList<int> list;
    for(int i=1; i<=5;i++) {
        list.push_back(i);
    }
    ArrayList<int> double_list = list.map<int>([](int element, int idx){
        return element*2;
    });
    printf("Map: \n");
    double_list.print();
    printf("Reduce: \n");
    auto result = list.reduce<int>([](int acc, int x){
        return acc + x;
    }, 0);
    printf("Result: %d\n",result);
    return 0;
}
