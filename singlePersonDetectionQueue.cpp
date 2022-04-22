#include<queue>
#include<string>
#include<map>
#include "detectPersonInfo.cpp"
#include<iostream>
#include <sstream>
#include <iomanip>

using namespace std;
class singlePersonDetectionQueue{
    queue<detectPersonInfo> singleQueue;
    public :
        //查看某个号码的人是否在排队
        string ifContainSomePerson(string personCode){
            string res = "";
            int size = singleQueue.size();
            for(int i = 0; i < size; i++) { 
                if(singleQueue.front().getPersonCode()==personCode){
                    res = "lining";
                }
                singleQueue.push(singleQueue.front());
                singleQueue.pop();
            }
            return res;
        }

        //加入一个单采的人,返回是否成功
        bool addPerson(string personCode){
            if(ifContainSomePerson(personCode)!=""){
                return false;
            }
            int originSize = singleQueue.size();
            singleQueue.push(detectPersonInfo(personCode));
            return singleQueue.size() - originSize == 1 ? true : false;
        }

        //获取还有几个人在排队中
        int getWaitingPersonNum(){
            return singleQueue.size();
        }

        //单采一个人
        detectPersonInfo detectPerson(){
            detectPersonInfo res = singleQueue.front();
            singleQueue.pop();
            return res;
        }

        //查看在单采排队的人
        void showWaitingPeople(){
            int size = singleQueue.size();
            for(int i = 0; i < size; i++) {
                cout << singleQueue.front().getPersonCode() << endl;
                singleQueue.push(singleQueue.front());
                singleQueue.pop();
            } 
        }
};