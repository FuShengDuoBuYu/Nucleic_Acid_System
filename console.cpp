#include <iostream>
#include <string>
#include <fstream>
#include "singlePersonDetectionQueue.cpp"
#include "mixedPersonDetectionQueue.cpp"
#include "detectedPerson.cpp"
using namespace std;
//待单采队列
singlePersonDetectionQueue spdq;
//待混采队列
mixedPersonDetectionQueue mpdq;
//已单采的人
singleDetectedPerson sdp;
//已混采的人
mixedDetectedPerson mdp;

//单采管编号
int singleDetectionCubeCode = 10000;
//混采管编号
int mixedDetectionCubeCode = 00000;
// 根据输入选择功能
int selectFunctModule(){
    cout << "===================================================" << endl;
    cout << "Welcome to the system" << endl;
    cout << "1:Person detection queue" << endl;
    cout << "2:Start detection" << endl;
    cout << "3:Check the queue" << endl;
    cout << "4:Upload the detection tube information" << endl;
    cout << "5:View query results of various personnel" << endl;
    cout << "6:Personal inquiry" << endl;
    cout << "7:exit system" << endl;
    cout << "Please enter the corresponding function number:" << endl;
    char c;
    // 获取功能的选择
    while(1){
        cin >> c;
        int functCode = (int)c - 48;
        if(functCode>0&&functCode<8){
            return functCode;
        }
        else{
            cout << "Your input is invalid" << endl;
        }
    }
}

//添加一个人员到排队
void addPersonDetectQueue(){
    cout << "please select single detect or mixed detect:" << endl;
    cout << "1:single detect" << endl;
    cout << "2:mixed detect" << endl;
    char c;
    int detectTypeCode;
    // 获取功能的选择
    while(1){
        cin >> c;
        detectTypeCode = (int)c - 48;
        cout << "please input your single detect person code:" << endl;
        //获取人员编号
        string personCode;
        cin >> personCode;
        //单采
        if(detectTypeCode==1 && personCode.length()==8){
            //判断结果
            cout << (spdq.addPerson(personCode) == 1 ? "add person successfully!" : "some thing wrong!") << endl;
            break;
        }
        else if(detectTypeCode==2 && personCode.length()==8){
            //判断结果
            cout << (mpdq.addPerson(personCode) == 1 ? "add person successfully!" : "some thing wrong!") << endl;
            break;
        }
        else{
            cout << "Your input is invalid" << endl;
        }
    }
}

//检测
void detect(){
    cout << "Start detecting! please select single detect or mixed detect:" << endl;
    cout << "1:single detect" << endl;
    cout << "2:mixed detect" << endl;
    char c;
    int detectTypeCode;
    // 获取功能的选择
    while(1){
        cin >> c;
        detectTypeCode = (int)c - 48;
        //单采
        if(detectTypeCode==1){
            //单采排队不为空再单采
            if(spdq.getWaitingPersonNum()==0){
                cout << "there is no person waiting for detecting!" << endl;
            }
            else{
                //获取队首的人
                detectPersonInfo person = spdq.detectPerson();
                //加入到检测过的vector记录,管子的流水号加一
                sdp.addPerson(person.getPersonCode(), singleDetectionCubeCode++);
            }
            break;
        }
        else if(detectTypeCode==2){
            cout << "please input your mixed detect person code:" << endl;
            //混采排队人数大于等于10再混采
            if(mpdq.getWaitingPersonNum()<10){
                cout << "there is not enough people waiting for detecting!" << endl;
            }
            else{
                //获取队首前十的人
                vector<detectPersonInfo> persons = mpdq.detectPerson();
                //加入到检测过的vector记录,管子的流水号加一
                for (int i = 0; i < persons.size();i++){
                    mdp.addPerson(persons[i].getPersonCode(), mixedDetectionCubeCode);
                }
                //混采管子流水号加一
                mixedDetectionCubeCode++;
            }
            break;
        }
        else{
            cout << "Your input is invalid" << endl;
        }
    }
}

//查看排队人员
void showWaitingForDetectingPeople(){
    cout << "please select single detect or mixed detect queue to show:" << endl;
    cout << "1:show single detect line" << endl;
    cout << "2:show mixed detect line" << endl;
    char c;
    int detectTypeCode;
    // 获取功能的选择
    while(1){
        cin >> c;
        detectTypeCode = (int)c - 48;
        //单采队伍
        if(detectTypeCode==1){
            cout << "single detect line:" << endl;
            spdq.showWaitingPeople();
            break;
        }
        //混采队伍
        else if(detectTypeCode==2){
            cout << "mixed detect line:" << endl;
            mpdq.showWaitingPeople();
            break;
        }
        else{
            cout << "Your input is invalid" << endl;
        }
    }
}

//个人状态查询
void findPersonStatus(){
    cout << "please input the personCode:" << endl;
    string personCode;
    cin >> personCode;
    //在各个位置查询    
    string personStatus = sdp.ifPersonDetected(personCode) + mdp.ifPersonDetected(personCode)
     + mpdq.ifContainSomePerson(personCode) + spdq.ifContainSomePerson(personCode);
    if(personStatus!=""){
        cout << personCode + " status is " + personStatus << endl;
    }
    else{
        cout << personCode + " status is not detected" << endl;
    }
}

//大类查找
void findDifferentStatusPeople(){
    cout << "please input the status you want to find:" << endl;
    cout << "1:negative" << endl;
    cout << "2:positive" << endl;
    cout << "3:suspicious" << endl;
    cout << "4:closeContacts" << endl;
    cout << "5:subCloseContacts" << endl;
    cout << "6:uploading" << endl;
    cout << "7:lining" << endl;
    cout << "Please enter the corresponding status number:" << endl;
    char c;
    int statusCode;
    // 获取功能的选择
    while(1){
        cin >> c;
        statusCode = (int)c - 48;
        if(statusCode>0&&statusCode<8){
            break;
        }
        else{
            cout << "Your input is invalid" << endl;
        }
    }
    //各类查找
    switch (statusCode){
            // 阴性
            case 1:
                sdp.findPeopleByStatus("negative");
                mdp.findPeopleByStatus("negative");
                break;
            // 确诊
            case 2:
                sdp.findPeopleByStatus("positive");
                mdp.findPeopleByStatus("positive");
                break;
            // 可疑
            case 3:
                sdp.findPeopleByStatus("suspicious");
                mdp.findPeopleByStatus("suspicious");
                break;
            // 密接
            case 4:
                sdp.findPeopleByStatus("closeContacts");
                mdp.findPeopleByStatus("closeContacts");
                break;
            //次密接
            case 5:
                sdp.findPeopleByStatus("subCloseContacts");
                mdp.findPeopleByStatus("subCloseContacts");
                break;
            //等待上传结果
            case 6:
                sdp.findPeopleByStatus("uploading");
                mdp.findPeopleByStatus("uploading");
                break;
            //排队中
            case 7:
                spdq.showWaitingPeople();
                mpdq.showWaitingPeople();
                break;
            default:
                break;
    }
}

//登记检测管信息
void registerDetectionInfo(){
    //只有所有排队的人都做完以后才能出结果
    if(mpdq.getWaitingPersonNum()!=0||spdq.getWaitingPersonNum()!=0){
        cout << "there are still people waiting in line,unable to upload!" << endl;
        return;
    }
    cout << "please input the detectionCubeCode:" << endl;
    int detectionCubeCode;
    cin >> detectionCubeCode;
    cout << "choose the status:" << endl;
    //记录测试结果
    cout << "1:negative" << endl;
    cout << "2:positive" << endl;
    cout << "3:suspicious" << endl;
    cout << "Please enter the corresponding status number:" << endl;
    char c;
    int statusCode;
    // 获取状态的选择
    while(1){
        cin >> c;
        statusCode = (int)c - 48;
        if(statusCode>0&&statusCode<4){
            break;
        }
        else{
            cout << "Your input is invalid" << endl;
        }
    }
    //录入数据并处理
    switch (statusCode){
        //阴性
        case 1:
            detectionCubeCode >= 10000 ? sdp.changePersonStatus("negative",detectionCubeCode,mdp) : mdp.changePersonStatus("negative",detectionCubeCode);
            break;
        //阳性
        case 2:
            detectionCubeCode >= 10000 ? sdp.changePersonStatus("positive",detectionCubeCode,mdp) : mdp.changePersonStatus("positive",detectionCubeCode);
            break;
        //可疑
        case 3:
            detectionCubeCode >= 10000 ? sdp.changePersonStatus("suspicious",detectionCubeCode,mdp) : mdp.changePersonStatus("suspicious",detectionCubeCode);
            break;
    }
}
//字符串分割
void split(const string& s,vector<int>& sv,const char flag = ' ') {
    sv.clear();
    istringstream iss(s);
    string temp;

    while (getline(iss, temp, flag)) {
        sv.push_back(stoi(temp));
    }
    return;
}
//读取预先准备好的文件
void readDataFromFile(string waitingFile,string DetectedFile){
    vector<string> waitingPeople;
    vector<int> twoQueueSize;
    vector<int> twoDetectedSize;
    ifstream waiting(waitingFile);
    ifstream detected(DetectedFile);
    //排队写入
    string s;
    getline(waiting, s);
    // 获取混采和单采的人数
    split(s, twoQueueSize, ' ');
    //加入到排队中去
    for (int i = 0; i < twoQueueSize[0];i++){
        getline(waiting, s);
        mpdq.addPerson(s);
    }
    for (int i = 0; i < twoQueueSize[1];i++){
        getline(waiting, s);
        spdq.addPerson(s);
    }
    //已检测人员的写入
    getline(detected, s);
    // 获取混采和单采已检测的人数
    split(s, twoDetectedSize, ' ');
    cout << '1' << endl;
    //加入到已检测中去
    for (int i = 0; i < twoDetectedSize[0];i++){
        if((i+1)%10==0){
            //排队减十
            vector<detectPersonInfo> persons = mpdq.detectPerson();
            //已检测加十
            for (int i = 0; i < persons.size();i++){
                mdp.addPerson(persons[i].getPersonCode(), mixedDetectionCubeCode++);
            }
        }
    }
    for (int i = 0; i < twoDetectedSize[1];i++){
        //排队减一
        detectPersonInfo person = spdq.detectPerson();
        //已检测加一
        sdp.addPerson(person.getPersonCode(), singleDetectionCubeCode++);
    }
    waiting.close();
    detected.close();
    cout << "try to load some data from loacl files..." << endl;
}
int main(){
    readDataFromFile(".\\waiting_in_line_test.txt", ".\\detected_test.txt");
    int functCode;
    while(1){
        functCode = selectFunctModule();
        switch (functCode){
            // 排队
            case 1:
                addPersonDetectQueue();
                break;
            // 检测
            case 2:
                detect();
                break;
            //查看排队的人员
            case 3:
                showWaitingForDetectingPeople();
                break;
            //登记测试管信息
            case 4:
                registerDetectionInfo();
                break;
            //各类人员查询
            case 5:
                findDifferentStatusPeople();
                break;
            //个人查询
            case 6:
                findPersonStatus();
                break;
            //退出系统
            case 7:
                cout << "Thank you for using! Bye~";
                exit(0);
            default:
                break;
        }
    }
}