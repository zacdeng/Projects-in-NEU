//
// Created by 2_20173878_邓志豪 on 2020/5/26.
// 排序部分使用插入排序、冒泡排序、选择排序
//

#include <iostream>
using namespace std;
const int nMaxSize = 15;  //线性表初始化长度

typedef struct {
    int nlength = 0;  //表中元素长度
    int nLinearList[nMaxSize];
}Clist;

void BubbleSort(Clist *);
void SelectionSort(Clist *);
void InsertionSort(Clist *);
void Ldisplay(Clist *);
void Linsert(int n, Clist *);
void Ldelete(int n, Clist *);

int main(){
    Clist l1;
    cout << "Please input 10 data (int).\n";
    int storage;
    for(int i=0; i<10; i++){
        scanf("%d",&storage);
        l1.nLinearList[i] = storage;
        l1.nlength++;
    }
    Ldisplay(&l1);

    //冒泡排序
    cout<<"The result of BubbleSort is as follow:\n";
    BubbleSort(&l1);
    Ldisplay(&l1);

    //选择排序
    cout<<"The result of SelectionSort is as follow:\n";
    SelectionSort(&l1);
    Ldisplay(&l1);

    //插入排序
    cout<<"The result of InsertionSort is as follow:\n";
    InsertionSort(&l1);
    Ldisplay(&l1);

    //插入
    cout<<"Input a integer then it will be inserted to the list."<<endl;
    scanf("%d",&storage);
    Linsert(storage,&l1);
    Ldisplay(&l1);

    //删除
    cout<<"Input a integer then it will be deleted from the list."<<endl;
    scanf("%d",&storage);
    Ldelete(storage,&l1);
    Ldisplay(&l1);

    return 0;
}

void BubbleSort(Clist *p){ //冒泡算法——每次排序将最大值沉入底部
    int i,j,tmp;
    for(i=0; i<p->nlength-1; i++){ //i表示已排序的趟数(每过一趟便有一个最大数沉至底部)
        for(j=0 ;j<p->nlength-1-i; j++){
            if(p->nLinearList[j]>p->nLinearList[j+1]){
                tmp = p->nLinearList[j+1];
                p->nLinearList[j+1] =  p->nLinearList[j];
                p->nLinearList[j] = tmp;
            }
        }
    }
}


void SelectionSort(Clist *p){ //选择排序——每次选择最小数放在第一位
    int i,j,k,tmp;
    for(i=0; i<p->nlength-1; i++){
        k=i;
        for(j=i+1; j<p->nlength; j++){
            if(p->nLinearList[j] < p->nLinearList[k]) k=j;
        }
        tmp = p->nLinearList[k];
        p->nLinearList[k] = p->nLinearList[i];
        p->nLinearList[i] = tmp;
    }
}

void InsertionSort(Clist *p){ //插入排序
    int i,j,tmp;
    for(i=1; i<p->nlength; i++){ //默认第一位已经排序好，从第二位开始
        if(p->nLinearList[i] < p->nLinearList[i-1]){ //若元素比已排序元素中的最大值更大，则进行排序
            tmp = p->nLinearList[i];
            for(j=i-1; j>=0 && p->nLinearList[j]>tmp; j--){
                p->nLinearList[j+1] = p->nLinearList[j];  //元素后移腾出tmp的位置
            }
            p->nLinearList[j] = tmp;
        }
    }
}

void Ldisplay(Clist *p){  //循环打印
    cout<<"[";
    for(int i=0; i<p->nlength; i++){
        cout<<p->nLinearList[i]<<" ";
    }
    cout<<"]\n";
}

void Linsert(int n, Clist *p){  //插入后保持有序
    if(p->nlength == nMaxSize) cout<<"The list is full!"<<endl;  //判断线性表是否已满
    else{
        for(int i=p->nlength-1; i>=0; i--){
            if(p->nLinearList[i] >= n) p->nLinearList[i+1]=p->nLinearList[i];
            else{  //找到对应位置插入
                p->nLinearList[i+1] = n;
                break;
            }
        }
    }
    p->nlength = p->nlength+1;
}
void Ldelete(int n, Clist *p){
    int count=0;
    for(int i=0; i<p->nlength; i++){  //第一层循环找到要删除的元素
        count++;
        if(p->nLinearList[i] == n){
            for(int j=i; j<p->nlength; j++){ //第二层循环删除元素（通过将后面的元素前移）
                p->nLinearList[j] = p->nLinearList[j+1];
            }
            break;
        }
    }
    p->nlength = p->nlength-1;
}