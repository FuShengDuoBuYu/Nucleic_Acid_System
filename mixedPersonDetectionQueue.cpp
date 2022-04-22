#include<queue>
#include<string>
#include<map>
#include "detectPersonInfo.cpp"
#include<vector>
#include<iostream>
#include <sstream>
#include <iomanip>
using namespace std;
const int mixedCubePersonNum = 10;
class mixedPersonDetectionQueue{
    queue<detectPersonInfo> mixedQueue;

    //加入一个混采的人,返回是否成功
    public :
        //查看某个号码的人是否在排队
        string ifContainSomePerson(string personCode){
            string res = "";
            int size = mixedQueue.size();
            for(int i = 0; i < size; i++) { 
                if(mixedQueue.front().getPersonCode()==personCode){
                    res = "lining";
                }
                mixedQueue.push(mixedQueue.front());
                mixedQueue.pop();
            }
            return res;
        }

        bool addPerson(string personCode){
            if(ifContainSomePerson(personCode)!=""){
                return false;
            }
            int originSize = mixedQueue.size();
            mixedQueue.push(detectPersonInfo(personCode));
            return mixedQueue.size() - originSize == 1 ? true : false;
        }

        int getWaitingPersonNum(){
            return mixedQueue.size();
        }

        //返回在队伍最前面的十个人
        vector<detectPersonInfo> detectPerson(){
            vector<detectPersonInfo> res;
            for (int i = 0; i < mixedCubePersonNum;i++){
                detectPersonInfo person = mixedQueue.front();
                res.push_back(person);
                mixedQueue.pop();
            }
            return res;
        }

        //查看里面在排队的人
        void showWaitingPeople(){
            int size = mixedQueue.size();
            for(int i = 0; i < size; i++) { 
                cout << mixedQueue.front().getPersonCode() << endl;
                mixedQueue.push(mixedQueue.front());
                mixedQueue.pop();
            } 
        }
};