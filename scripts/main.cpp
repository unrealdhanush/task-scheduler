/*
MCC TASK SCHEDULER ALGORITHM
==================================================================================================================
Instructions to use:
1. There are 5 task graphs that are preloaded in this script, two of which have 10 tasks and the rest have TASK_NUMBER tasks. 
2. Every task graph is different from each other. 
3. There are two execution time tables. One with 10 tasks and the other with 20 tasks. Use the required time tables with the respective task graphs. Make sure the number of tasks are same.
4. The parameters are by default set to run for 20 tasks, in order to run it for 10 tasks, on your IDE and use the 'change all occurences' feature.
*/

#include <iostream>
#include <algorithm>
#include <stack>
#include <vector>
#include <list>
#include <iterator>
#include <chrono>
#include <iomanip>
#include <climits>
#define TASK_NUMBER 10

using namespace std;

void primary_assignment(int tlk[][3],int C[], int remote_execution_time, int n, int k){
    int min_local_time[n];
    for(int i=0; i<n; i++){
        min_local_time[i] = 0;
    }
    for(int i=0; i<n; i++){
        for(int j=0; j<k; j++){
            if(min_local_time[i]>tlk[i][j]){
                min_local_time[i] = tlk[i][j];
            }
        }
    }
    //classify tasks: value of C[i] 1:local 0:cloud
    for(int i=0; i<n; i++){
        if(min_local_time[i] > remote_execution_time){
            C[i] = 1;
        }
        else
            C[i] = 0;
    }
}

void task_prioritzing(int tlk[][3], int pri[], int pri_n[], int task_graph[][TASK_NUMBER], int w[], int C[], int remote_execution_time, int n, int k){
    for(int i=0; i<n; i++){
        if(C[i] == 1){
            w[i] = remote_execution_time;
        }
        else{
            int sum = 0;
            for(int j=0; j<k; j++){
                sum += tlk[i][j];
            }
            w[i] = sum/k;
        }
    }

    pri[n-1] = w[n-1];
    for(int i=n-1; i>=0; i--){
        int max_j = 0;
        for(int j=n-1; j>=0; j--){
            if(task_graph[i][j] == 1 && pri[j] > max_j){
                max_j = pri[j];
            }
        }
        pri[i] = w[i] + max_j;
    }

    vector<pair<int,int>> vect;
    for (int i=0; i<n; i++){
        vect.push_back(make_pair(pri[i],i));
    }
    sort(vect.begin(), vect.end());
    for(int i=0; i<n; i++){
        pri_n[i] = vect[i].second;
    }
    //sort according to priority
}

void execution_unit_selection(int tlk[][3],int pri_n[], int task_graph[][TASK_NUMBER], int C[], int rt_l[], int rt_c[], int rt_ws[], int ft_ws[], int ft_wr[], int ft_l[], int ft[], int core[], int core1[], int n, int k, int t_s, int t_r, int t_c, vector<vector<int>> clist){
    int f_i = pri_n[n-1];//schedule first element
    rt_l[f_i] = 0;
    rt_ws[f_i] = 0;
    ft_ws[f_i] = rt_ws[f_i] + t_s;
    rt_c[f_i] = ft_ws[f_i];
    if(C[f_i] == 1){
        ft_wr[f_i] = rt_c[f_i] + t_c + t_r;
        ft_l[f_i] = 0;
        ft[f_i] = ft_wr[f_i];
        core[3] = ft[f_i];
        clist[0].push_back(f_i);
        core1[f_i] = 3;
    }
    else{
        int min_local_time = INT_MAX;
        int index;
        for(int i=0; i<k; i++){
            if(tlk[f_i][i]<min_local_time){
                min_local_time = tlk[f_i][i];
                index = i;
            }
        }
        ft_l[f_i] = rt_l[f_i] + min_local_time;
        ft_wr[f_i] = rt_c[f_i] + t_c + t_r;
        if(ft_l[f_i] <= ft_wr[f_i]){
            ft[f_i] = ft_l[f_i];
            ft_wr[f_i] = 0;
            core[3] = ft_ws[f_i];
            clist[index+1].push_back(f_i);
            core1[f_i] = index;
        }
        else{
            ft[f_i] = ft_wr[f_i];
            ft_l[f_i] = 0;
            core[index] = ft[f_i];
            clist[0].push_back(f_i);
            core1[f_i] = 3;
        }
    }
    //schedule rest of the elements
    for(int a=n-2; a>=0; a--){
        int i = pri_n[a];
        //cout<<i;
        int max_j_l = 0;
        for(int j=0; j<n; j++){
            if(task_graph[j][i] == 1 && max_j_l < max(ft_l[j],ft_wr[j])){
                max_j_l = max(ft_l[j],ft_wr[j]);
            }
        }
        rt_l[i] = max_j_l;
        int max_j_ws = 0;
        for(int j=0; j<n; j++){
            if(task_graph[j][i] == 1 && max_j_ws < max(ft_l[j],ft_ws[j])){
                max_j_ws = max(ft_l[j],ft_ws[j]);
            }
        }
        rt_ws[i] = max_j_ws;
        ft_ws[i] = max(core[3],rt_ws[i]) + t_s;
        int max_j_c = 0;
        for(int j=0; j<n; j++){
            if(task_graph[j][i] == 1 && max_j_c < ft_wr[j]-t_r){
                max_j_c = ft_wr[j]-t_r;
            }
        }
        rt_c[i] = max(ft_ws[i],max_j_c);
        if(C[i] == 1){
            ft_wr[i] = rt_c[i] + t_c + t_r;
            ft[i] = ft_wr[i];
            ft_l[i] = 0;
            core[3] = ft_ws[i];
            clist[0].push_back(i);
            core1[i] = 3;
        }
        else{
            int rt, index;
            int f = INT_MAX;
            for(int j=0; j<k; j++){
                rt = max(rt_l[i],core[j]);
                if(f > rt + tlk[i][j]){
                    f = rt + tlk[i][j];
                    index = j;
                }
            }
            rt_l[i] = f - tlk[i][index];
            ft_l[i] = f;
            ft_wr[i] = rt_c[i] + t_c + t_r;
            if(ft_l[i] <= ft_wr[i]){
                ft[i] = ft_l[i];
                ft_wr[i] = 0;
                core[index] = ft[i];
                clist[index+1].push_back(i);
                core1[i] = index;
            }
            else{
                ft[i] = ft_wr[i];
                ft_l[i] = 0;
                core[3] = ft[i];
                clist[0].push_back(i);
                core1[i] = 3;
            }
        }
        //cout<<core[2]<<endl;
    }
}

void kernel( vector<vector<int>> clist, int tlk[][3], int task_graph[][TASK_NUMBER],int core1[], int tmax, int t_total, float e_total, int n, int k, int st[], int et[], int E_c, int E_l[][3]){
    int out = 0;
    int count = 0;
    while(out == 0){
        float max_ratio = 0;
        int new_n = 0, new_k = 0, new_index1 = 0, new_index2 = 0, new_t = t_total;
        float new_e = e_total;
        int less_t1 =0, less_t2 = 0;
        int temp_core[TASK_NUMBER], new_st[TASK_NUMBER], new_ft[TASK_NUMBER];;
        for(int i=0; i<n; i++){
            for(int j=0; j<k+1; j++){
                int core2[TASK_NUMBER], core3[4], rt[TASK_NUMBER], rt1[TASK_NUMBER], ft[TASK_NUMBER], ft1[TASK_NUMBER], pushed[TASK_NUMBER];
                vector<vector<int>> tlist(4);
                int index1, index2 = 0;
                for(int i=0; i<TASK_NUMBER; i++){
                    rt[i] = 0;
                    ft[i] = 0;
                    core2[i] = core1[i];
                    ft1[i] = et[i];
                    rt1[i] = st[i];
                    pushed[i] = 0;
                }
                for(int a=0; a<clist.size(); a++){
                    //core2[i] = core1[i];
                    core3[a] = 0;
                    for(int b=0; b<clist[a].size(); b++){
                        tlist[a].push_back(clist[a][b]);
                    }
                }
                int current_core = core1[i];
                for(int a=0; a<tlist[current_core].size(); a++){
                    if(tlist[current_core][a] == i){
                        index1 = a;
                    }
                }
                tlist[current_core].erase(tlist[current_core].begin()+index1);
                //caculate the ready time of target task
                if(j == 3){
                    int max_j_ws = 0;
                    for(int a=0; a<n; a++){
                        if(task_graph[a][i] == 1 && max_j_ws < ft1[a]){
                            max_j_ws = ft1[a];
                        }
                    }
                    rt[i] = max_j_ws;
                }
                else{
                    int max_j_l = 0;
                    for(int a=0; a<n; a++){
                        if(task_graph[a][i] == 1 && max_j_l < ft1[a]){
                            max_j_l = ft1[a];
                        }
                    }
                    rt[i] = max_j_l;
                }
                core2[i] = j;
                //cout<<tlist[j].size()<<" "<<count<<endl;
                if(tlist[j].size() == 0){
                    index2 = 0;
                }
                else if(tlist[j].size() == 1){
                    if(rt1[tlist[j][0]] > rt[i]){
                        index2 = 0;
                    }
                    else{
                        index2 = 1;
                    }
                }
                else{
                    if(rt1[tlist[j][0]] > rt[i]){
                        index2 = 0;
                    }
                    else if(rt1[tlist[j][tlist[j].size()-1]] <= rt[i]){
                        index2 = tlist[j].size();
                    }
                    else{
                        for(int b=0; b<tlist[j].size()-1; b++){
                            if(rt[i]>=rt1[tlist[j][b]] && rt[i]<=rt1[tlist[j][b+1]]){
                                index2 = b+1;
                            }
                        }
                    }
                }
                tlist[j].insert(tlist[j].begin()+index2,i);
                //cout<<index2<<" "<<i<<" "<<j<<endl;
                // initialize ready1 and ready2
                int ready1[TASK_NUMBER], ready2[TASK_NUMBER];
                for(int a=0; a<TASK_NUMBER; a++){
                    ready1[a] = 0;
                }
                for(int a=0; a<TASK_NUMBER; a++){
                    for(int b=0; b<TASK_NUMBER; b++){
                        if(task_graph[a][b] == 1){
                            ready1[b] += 1;
                        }
                    }
                    ready2[a] = 1;
                }
                //cout<<ready2[0]<<" "<<i<<" "<<j<<endl;
                for(int a=0; a<4; a++){
                    if(tlist[a].size()>0){
                        ready2[tlist[a][0]] = 0;
                    }
                }

                /*for(int a=0; a<4; a++){
                    for(int b = 0; b<tlist[a].size(); b++){
                        if(b == 0){
                            ready2[tlist[a][b]] = 0;
                        }
                        else{
                            ready2[tlist[a][b]] = 1;
                        }
                    }
                }*/
                //intialize the stack and implement the first operation
                stack<int> s;
                for(int a=0; a<TASK_NUMBER; a++){
                    if(ready1[a] == 0 && ready2[a] == 0 && pushed[a] == 0){
                        s.push(a);
                        pushed[a] = 1;
                        //cout<<a<<" "<<endl;
                    }
                }
                //cout<<ready2[2]<<" "<<i<<" "<<j<<endl;
                int current1 = s.top();
                s.pop();
                rt[current1] = 0;
                if(core2[current1] == 3){
                    rt[current1] = max(core3[core2[current1]],rt[current1]);
                    ft[current1] = rt[current1] + 5;
                    core3[core2[current1]] = rt[current1] + 3;
                }
                else{
                    rt[current1] = max(core3[core2[current1]],rt[current1]);
                    ft[current1] = rt[current1] + tlk[current1][core2[current1]];
                    core3[core2[current1]] = ft[current1];
                }
                //cout<<rt[current1]<<" "<<ft[current1]<<endl;
                //update ready1 and ready2
                for(int a=0; a<TASK_NUMBER; a++){
                    if(task_graph[current1][a] == 1){
                        ready1[a] -= 1;
                    }
                }
                ready2[current1] = 1;
                //cout<<ready1[5]<<" "<<count<<endl;
                if(tlist[core2[current1]].size()>1){
                    for(int a=1; a<tlist[core2[current1]].size(); a++){
                        if(tlist[core2[current1]][a-1] == current1){
                            ready2[tlist[core2[current1]][a]] = 0;
                            //cout<<tlist[core2[current1]][a]<<" "<<i<<" "<<j<<endl;
                        }
                    }
                }
                //cout<<core2[current1]<<" "<<i<<" "<<j<<endl;
                for(int a=0; a<TASK_NUMBER; a++){
                    if(ready1[a] == 0 && ready2[a] == 0 && pushed[a] == 0){
                        s.push(a);
                        pushed[a] = 1;
                        //cout<<a<<" ";
                    }
                }
                /*for(int a=0; a<TASK_NUMBER;a++){
                    cout<<ready2[a]<<" ";
                }*/
                //cout<<rt[0]<<" "<<ft[0]<<" "<<i<<" "<<j<<endl;
                //cout<<ready2[1]<<ready2[2]<<ready2[3]<<ready2[5]<<" "<<i<<" "<<j<<endl;
                //cout<<ready2[1]<<" "<<i<<" "<<j<<endl;
                //cout<<i<<" "<<j<<endl;
                while(s.size() != 0){
                    int current = s.top();
                    s.pop();
                    //cout<<current<<" ";
                    //caculate ready time of current task
                    if(core2[current] == 3){
                        int max_j_ws1 = 0;
                        for(int a=0; a<n; a++){
                            if(task_graph[a][current] == 1 && max_j_ws1 < ft[a]){
                                max_j_ws1 = ft[a];
                            }
                        }
                        rt[current] = max_j_ws1;
                    }
                    else{
                        int max_j_l1 = 0;
                        for(int a=0; a<n; a++){
                            if(task_graph[a][current] == 1 && max_j_l1 < ft[a]){
                                max_j_l1 = ft[a];
                            }
                        }
                        rt[current] = max_j_l1;
                    }
                    if(core2[current] == 3){
                        rt[current] = max(core3[core2[current]],rt[current]);
                        ft[current] = rt[current] + 5;
                        core3[core2[current]] = rt[current] + 3;
                    }
                    else{
                        rt[current] = max(core3[core2[current]],rt[current]);
                        ft[current] = rt[current] + tlk[current][core2[current]];
                        core3[core2[current]] = ft[current];
                    }
                    //update ready1 and ready2
                    for(int a=0; a<TASK_NUMBER; a++){
                        if(task_graph[current][a] == 1){
                            ready1[a] -= 1;
                        }
                    }
                    ready2[current] = 1;
                    if(tlist[core2[current]].size()>1){
                        for(int a=1; a<tlist[core2[current]].size(); a++){
                            if(tlist[core2[current]][a-1] == current){
                                ready2[tlist[core2[current]][a]] = 0;
                            }
                        }
                    }
                    for(int a=0; a<TASK_NUMBER; a++){
                        if(ready1[a] == 0 && ready2[a] == 0 && pushed[a] == 0){
                            s.push(a);
                            pushed[a] = 1;
                            //cout<<a<<" ";
                        }
                    }
                    //cout<<"s";
                }
                //cout<<endl;
                int current_t = ft[n-1];
                int current_e = 0;
                for(int a=0; a<TASK_NUMBER; a++){
                    if(core2[a] == 3){
                        current_e += E_c;
                    }
                    else{
                        current_e += E_l[a][core2[a]];
                    }
                }
                //cout<<current_t<<" "<<i<<" "<<j<<endl;
                if(current_t <= t_total && current_e < new_e){
                    less_t1 = 1;
                    new_n = i;
                    new_k = j;
                    new_index1 = index1;
                    new_index2 = index2;
                    new_t = current_t;
                    new_e = current_e;
                    //cout<<new_t<<endl;
                    for(int a=0; a<TASK_NUMBER; a++){
                        temp_core[a] = core2[a];
                        new_st[a] = rt[a];
                        new_ft[a] = ft[a];
                     }
                }
                if(current_t > t_total && current_t <= tmax && less_t1 == 0 && current_e < e_total && max_ratio < double((e_total - current_e) / (current_t - t_total))){
                    max_ratio = double((e_total - current_e) / (current_t - t_total));
                    //cout<<max_ratio<<endl;
                    less_t2 = 1;
                    new_n = i;
                    new_k = j;
                    new_index1 = index1;
                    new_index2 = index2;
                    new_t = current_t;
                    new_e = current_e;
                    //cout<<new_t<<endl;
                    for(int a=0; a<TASK_NUMBER; a++){
                        temp_core[a] = core2[a];
                        new_st[a] = rt[a];
                        new_ft[a] = ft[a];
                    }
                }
                //cout<<less_t1<<less_t2<<endl;
                //cout<<new_n<<" "<<new_k<<" "<<i<<" "<<j<<endl;
                //count += 1;
                //cout<<count<<endl;
            }
        }
        if(less_t1 != 1 && less_t2 != 1){
            out = 1;
        }
        else{
            clist[core1[new_n]].erase(clist[core1[new_n]].begin()+new_index1);
            clist[new_k].insert(clist[new_k].begin()+new_index2,new_n);
            t_total = new_t;
            e_total = new_e;
            for(int a=0; a<TASK_NUMBER; a++){
                core1[a] = temp_core[a];
                st[a] = new_st[a];
                et[a] = new_ft[a];
            }
            if(less_t1 != 1 && less_t2 != 1){
                out = 1;
            }
            count += 1;
            cout<<count<<"th step "<<endl;
            cout<<"Current Operation: Insert Task "<<new_n+1<<" to Core "<<new_k+1<<endl;
            cout<<"Current Completion Time: "<<t_total<< "   Current Energy Consumption: "<<e_total<<endl;
        }
    }
    /*for(int i=0; i<TASK_NUMBER; i++){
        cout<<core1[i]<<endl;
    }*/
    cout<<endl;
    cout<<"Best Asssignment"<<endl;
    cout<<"Best Energy Consumption: "<<e_total<<"   Best Completion Time: "<<t_total<<endl;
    for(int i=0; i<clist.size(); i++){
        if(i == 3){
            cout<<"Wireless sending: |";
            for(int j=0; j<clist[i].size(); j++){
                cout<<st[clist[i][j]]<<"<--Task"<<clist[i][j]+1 <<"-->"<< et[clist[i][j]]  - 2<<"|";
            }//2=t_c+t_r
            cout<<endl;
            cout << endl;
            cout<<"Cloud: |";
            for(int j=0; j<clist[i].size(); j++){
                cout<<st[clist[i][j]] +3<<"<--Task"<<clist[i][j] + 1 <<"-->"<<et[clist[i][j]] - 1<<"|";
            }//2=t_c+t_r
            cout<<endl;
            cout<<endl;
            cout<<"Wireless receiving: |";
            for(int j=0; j<clist[i].size(); j++){
                cout<<st[clist[i][j]] + 4<<"<--Task"<<clist[i][j] + 1 <<"-->"<<et[clist[i][j]]<<"|";
            }//2=t_c+t_r
            cout<<endl;
            cout <<endl;
        }
        else{
            cout<<"Core"<<i+1<<": |";
            for(int j=0; j<clist[i].size(); j++){
            cout<<st[clist[i][j]]<<"<--Task"<<clist[i][j] + 1<<"-->"<<et[clist[i][j]]<<"|";
            }
        }
        cout << endl;
        cout << endl;


    }
    /*
    for(int i=0; i<clist.size(); i++){
        if(i == 3){
            cout<<"Cloud: ";
        }
        else{
            cout<<"Core "<<i+1<<": ";
        }
        for(int j=0; j<clist[i].size(); j++){
            cout<<clist[i][j]+1<<" ";
        }
        cout<<endl;
    }
    */
    cout<<endl;
    cout<<"Time Assignment Details"<<endl;
    /*for(int i=0; i<TASK_NUMBER; i++){
        cout<<i+1<<"th task from "<<"start "<<st[i]<<" end "<<et[i]<<" at core "<<core1[i]<<endl;
    }*/
    for(int i=0; i<clist.size(); i++){
        if(i == 3){
            cout<<"Cloud: |";
        }
        else{
            cout<<"Core "<<i+1<<": |";
        }
        for(int j=0; j<clist[i].size(); j++){
            cout<<st[clist[i][j]]<<"<--{Task"<<clist[i][j]+1<<"}-->"<<et[clist[i][j]]<<"|";
        }
        cout<<endl;
    }
}


int main(int argc, char *argv[])
{
    //(1)(2)
    int tlk[][3]={{9,7,5},
                     {8,6,5},
                     {6,5,4},
                     {7,5,3},
                     {5,4,2},
                     {7,6,4},
                     {8,5,3},
                     {6,4,2},
                     {5,3,2},
                     {7,4,2}};
    
    //(3)(4)(5)
    /*int tlk[][3]={   {9,7,5}, // 1
                     {8,6,5}, // 2
                     {6,5,4}, // 3
                     {7,5,3}, // 4
                     {5,4,2}, // 5
                     {7,6,4}, // 6
                     {8,5,3}, // 7
                     {6,4,2}, // 8
                     {5,3,2}, // 9
                     {7,4,2}, // 10
                     {5,3,3}, // 11
                     {6,4,4}, // 12
                     {3,5,4}, // 13
                     {9,8,3}, // 14
                     {8,5,3}, // 15
                     {3,5,2}, // 16
                     {9,8,5}, // 17
                     {6,4,3}, // 18
                     {4,4,2}, // 19
                     {9,6,4}};// 20 */
    //cout<<tlk[2][9];
    
    //(1)
    int task_graph[][10]={{0,1,1,1,1,1,0,0,0,0},
                    {0,0,0,0,0,0,0,1,1,0},
                    {0,0,0,0,0,0,1,0,0,0},
                    {0,0,0,0,0,0,0,1,1,0},
                    {0,0,0,0,0,0,0,0,1,0},
                    {0,0,0,0,0,0,0,1,0,0},
                    {0,0,0,0,0,0,0,0,0,1},
                    {0,0,0,0,0,0,0,0,0,1},
                    {0,0,0,0,0,0,0,0,0,1},
                    {0,0,0,0,0,0,0,0,0,0}};
    
    //(2)
    /*int task_graph[][10]={{0,1,1,1,1,1,0,0,0,0},
                    {0,0,0,0,0,0,1,0,0,0},
                    {0,0,0,0,0,0,1,0,0,0},
                    {0,0,0,0,0,0,0,1,0,0},
                    {0,0,0,0,0,0,0,0,1,0},
                    {0,0,0,0,0,0,0,0,1,0},
                    {0,0,0,0,0,0,0,0,0,1},
                    {0,0,0,0,0,0,0,0,0,1},
                    {0,0,0,0,0,0,0,0,0,1},
                    {0,0,0,0,0,0,0,0,0,0}};*/

    //(3)
    /*int task_graph[][20]={{0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                    {0,0,0,0,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,1,0,0,0,0},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};*/
    
    //(4)
    /*int task_graph[][20]={{0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,1,0,0,0,0},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};*/
    
    //(5)
    /*int task_graph[][20]={{0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,1,0,0,0,0},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};*/
    
    int C[TASK_NUMBER]; //local:0, cloud:1
    int pri[TASK_NUMBER], pri_n[TASK_NUMBER], w[TASK_NUMBER], core[4], core1[TASK_NUMBER];
    int rt_l[TASK_NUMBER], rt_c[TASK_NUMBER], rt_ws[TASK_NUMBER], ft_ws[TASK_NUMBER], ft_wr[TASK_NUMBER], ft_l[TASK_NUMBER], ft[TASK_NUMBER];
    vector<vector<int>> clist(4);
    //list<int> c1, c2, c3, c0;
    //int clist[4][TASK_NUMBER]
    for(int i=0; i<TASK_NUMBER; i++){
        C[i] = 0;
        pri[i] = 0;
        pri_n[i] = 0;
        w[i] = 0;
        rt_l[i] = 0;
        rt_ws[i] = 0;
        rt_c[i] = 0;
        ft_ws[i] = 0;
        ft_wr[i] = 0;
        ft_l[i] = 0;
        ft[i] = 0;
        core1[i] = 0;
    }
    for(int i=0; i<4; i++){
        core[i] = 0;
    }
    int k = 3, n = TASK_NUMBER;
    int t_s = 3, t_c = 1, t_r = 1;
    int remote_execution_time = t_s + t_c + t_r;
    int E_l[TASK_NUMBER][3];
    int pk[] = {1,2,4};
    float ps = 0.5;
    float E_c = ps * t_s;
    for(int i=0; i<TASK_NUMBER; i++){
      for(int j=0; j<3; j++){
          E_l[i][j] = pk[j] * tlk[i][j];
      }
    }
    auto start = chrono::high_resolution_clock::now();
    ios_base::sync_with_stdio(false);
    primary_assignment(tlk,C,remote_execution_time,n,k);
    task_prioritzing(tlk,pri,pri_n,task_graph,w,C,remote_execution_time,n,k);
    /*for(int i=0; i<n; i++){
        cout<<pri_n[i]<<endl;
    }*/
    execution_unit_selection(tlk,pri_n,task_graph,C,rt_l,rt_c,rt_ws,ft_ws,ft_wr,ft_l,ft,core,core1,n,k,t_s,t_r,t_c,clist);
    /*for(int i=0; i<TASK_NUMBER; i++){
        int rt = max(rt_ws[i],rt_l[i]);
        cout<<rt<<" - "<<ft[i]<<endl;
    }*/
    for(int i=0; i<4; i++){
        for(int j=0; j<TASK_NUMBER; j++){
            if(core1[j] == i){
                clist[i].push_back(j);
            }
        }
    }
    float e_total = 0;
    for(int i=0; i<TASK_NUMBER; i++){
        if(core1[i] == 3){
            e_total += E_c;
        }
        else{
            e_total += E_l[i][core1[i]];
        }
    }
    int st[TASK_NUMBER];
    for(int i=0; i<TASK_NUMBER; i++){
        st[i] = max(rt_l[i],rt_ws[i]);
        //cout<<st[i]<<endl;
        //cout<<core1[i]<<" ";
    }
    /*for(int i=0; i<clist.size(); i++){
        for(int j=0; j<clist[i].size(); j++){
            cout<<clist[i][j]<<" ";
        }
        cout<<endl;
    }*/
    //cout<<e_total;
    int tmin = ft[n-1];
    int tmax = 1.5*tmin;

    cout<<"Initial Scheduling (Format: Start Time - Task Number - Finish Time)"<<endl;
    for(int i=0; i<clist.size(); i++){
        if(i == 3){
            cout<<"Wireless sending: |";
            for(int j=0; j<clist[i].size(); j++){
                cout<<st[clist[i][j]]<<"<--Task"<<clist[i][j]+1 <<"-->"<< ft[clist[i][j]]-2<<"|";
            }//2=t_c+t_r
            cout<<endl;
            cout<<"Cloud: |";
            for(int j=0; j<clist[i].size(); j++){
                cout<<st[clist[i][j]]+3<<"<--Task"<<clist[i][j]+1 <<"-->"<<ft[clist[i][j]]-1<<"|";
            }//2=t_c+t_r
            cout<<endl;
            cout<<"Wireless receiving: |";
            for(int j=0; j<clist[i].size(); j++){
                cout<<st[clist[i][j]]+4<<"<--Task"<<clist[i][j]+1 <<"-->"<<ft[clist[i][j]]<<"|";
            }//2=t_c+t_r
            cout<<endl;
        }
        else{
            cout<<"Core "<<i+1<<": |";
            for(int j=0; j<clist[i].size(); j++){
            cout<<st[clist[i][j]]<<"<--Task"<<clist[i][j]+1<<"-->"<<ft[clist[i][j]]<<"|";
            }
            cout<<endl;
        }
        }
        cout << endl;
    /*
    for(int i=0; i<clist.size(); i++){
        if(i == 3){
            cout<<"Cloud: |";
        }
        else{
            cout<<"Core"<<i+1<<": |";
        }
        for(int j=0; j<clist[i].size(); j++){
            cout<<st[clist[i][j]]<<"<--{Task"<<clist[i][j]+1<<"}-->"<<ft[clist[i][j]]<<"|";
        }
        cout<<endl;
    }
    */
    cout<<"Initial Energy Consumption: "<<e_total<<"   Initial Completion Time: "<<tmin<<endl;
    cout<<endl;
    kernel(clist,tlk,task_graph,core1,tmax,tmin,e_total,TASK_NUMBER,3,st,ft,E_c,E_l);
    /*for(int i=0; i<TASK_NUMBER; i++){
        cout<<core1[i]<<" ";
    }*/
    
    auto end = chrono::high_resolution_clock::now();
    double time_taken = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    time_taken *= 1e-9;
    cout << endl << "Time taken by program is : " << fixed << time_taken << setprecision(9);
    cout << " sec" << endl;
    return 0;
}
