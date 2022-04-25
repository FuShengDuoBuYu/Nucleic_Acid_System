#include<vector>
#include<string>
#include "detectPersonInfo.cpp"
#include<iostream>
#include <sstream>
#include <iomanip>
using namespace std;
class mixedDetectedPerson{
    private:
        friend class singleDetectedPerson;
        vector<detectPersonInfo> mixedVector;
    public :
        //加入一个单采完成的人,返回是否成功
        bool addPerson(string personCode,int detectionCubeCode){
            //加入这个人
            mixedVector.push_back(detectPersonInfo(personCode));
            //修改对应的状态
            bool flag1 = mixedVector[mixedVector.size() - 1].changeDetectionCubeCode(detectionCubeCode);
            bool flag2 = mixedVector[mixedVector.size() - 1].changeDetectStatus("uploading");
            return flag1 && flag2;
        }

        //查看某个人是否已经采集
        string ifPersonDetected(string personCode){
            for (int i = 0; i < mixedVector.size();i++){
                if(mixedVector[i].getPersonCode()==personCode){
                    return mixedVector[i].getDetectStatus();
                }
            }
            return "";
        }
        
        //查找某个已采集的状态的某些人
        void findPeopleByStatus(string status){
            for (int i = 0; i < mixedVector.size();i++){
                if(mixedVector[i].getDetectStatus()==status){
                    cout << mixedVector[i].getPersonCode() << endl;
                }
            }
        }

        //修改检测后的状态
        void changePersonStatus(string status,int detectionCubeCode){
            if(status=="negative"){
                cout << mixedVector[2].getDetectionCubeCode() << endl;
                cout << mixedVector[3].getDetectionCubeCode() << endl;
                //管子对应的10个人的状态转为阴
                for (int i = 0; i < mixedVector.size();i++){
                    if(mixedVector[i].getDetectionCubeCode()==detectionCubeCode){
                        mixedVector[i].changeDetectStatus("negative");
                    }
                }
            }
            if(status=="positive"){
                //管子对应的10人的状态转为可疑
                for (int i = 0; i < mixedVector.size();i++){
                    if(mixedVector[i].getDetectionCubeCode()==detectionCubeCode){
                        mixedVector[i].changeDetectStatus("suspicious");
                    }
                }
                //
            }
            if(status=="suspicious"){
                for (int i = 0; i < mixedVector.size();i++){
                    if(mixedVector[i].getDetectionCubeCode()==detectionCubeCode){
                        mixedVector[i].changeDetectStatus("suspicious");
                    }
                }
            }
        }
};

class singleDetectedPerson{
    friend class mixedDetectedPerson;
    vector<detectPersonInfo> singleVector;
    public :
        //加入一个单采完成的人,返回是否成功
        bool addPerson(string personCode,int detectionCubeCode){
            //加入这个人
            singleVector.push_back(detectPersonInfo(personCode));
            //修改对应的状态
            bool flag1 = singleVector[singleVector.size() - 1].changeDetectionCubeCode(detectionCubeCode);
            bool flag2 = singleVector[singleVector.size() - 1].changeDetectStatus("uploading");
            return flag1 && flag2;
        }
        
        //查看某个人是否已经采集
        string ifPersonDetected(string personCode){
            for (int i = 0; i < singleVector.size();i++){
                if(singleVector[i].getPersonCode()==personCode){
                    return singleVector[i].getDetectStatus();
                }
            }
            return "";
        }

        //查找某个已采集的状态的某些人
        void findPeopleByStatus(string status){
            for (int i = 0; i < singleVector.size();i++){
                if(singleVector[i].getDetectStatus()==status){
                    cout << singleVector[i].getPersonCode() << endl;
                }
            }
        }

        //修改检测后的状态
        void changePersonStatus(string status,int detectionCubeCode,mixedDetectedPerson &mixedDetectedPerson){
            if(status=="negative"){
                //管子对应的人的状态不是密接才是阴
                for (int i = 0; i < singleVector.size();i++){
                    if(singleVector[i].getDetectionCubeCode()==detectionCubeCode){
                        if(singleVector[i].getDetectStatus()!="closeContacts")
                            singleVector[i].changeDetectStatus("negative");
                        else{
                            cout << "this person is closeContact!" << endl;
                        }
                    }
                }
            }
            if(status=="positive"){
                int positiveIndex = -1;
                //先标记为阳性
                for (int i = 0; i < singleVector.size();i++){
                    if(singleVector[i].getDetectionCubeCode()==detectionCubeCode){
                        singleVector[i].changeDetectStatus("positive");
                        positiveIndex = i;
                    }
                }
                //同栋楼标为密接,也即前三位相同为密接,阳性本人不是密接
                string buildingNum = singleVector[positiveIndex].getPersonCode().substr(0,3);
                for (int i = 0; i < singleVector.size();i++){
                    if(singleVector[i].getPersonCode().substr(0,3)==buildingNum){
                        if(i!=positiveIndex){
                            singleVector[i].changeDetectStatus("closeContacts");
                        }
                    }
                }
                //混检也是一样
                for (int i = 0; i < mixedDetectedPerson.mixedVector.size();i++){
                    if(mixedDetectedPerson.mixedVector[i].getPersonCode().substr(0,3)==buildingNum){
                        mixedDetectedPerson.mixedVector[i].changeDetectStatus("closeContacts");
                    }
                }
                //排队在他前面的十人不为阳性时才为密接
                if(positiveIndex>=10){
                    for (int i = 0; i < 10;i++){
                        if(singleVector[positiveIndex - i].getDetectStatus()!="positive")
                            singleVector[positiveIndex - i].changeDetectStatus("closeContacts");
                    }
                }
                else{
                    for (int i = 0; i < positiveIndex;i++){
                        if(singleVector[i].getDetectStatus()!="positive")
                            singleVector[i].changeDetectStatus("closeContacts");
                    }
                }
                //排队后面的人全是密接
                for (int i = positiveIndex+1; i < singleVector.size();i++){
                    singleVector[i].changeDetectStatus("closeContacts");
                }
                //所有密接的同一楼栋的人员为次密接
                //获取所有密接的楼栋
                vector<string> closeContactsBuildingNums;
                for (int i = 0; i < singleVector.size();i++){
                    if(singleVector[i].getDetectStatus()=="closeContacts"){
                        closeContactsBuildingNums.push_back(singleVector[i].getPersonCode().substr(0,3));
                    }
                }
                for (int i = 0; i < mixedDetectedPerson.mixedVector.size();i++){
                    if(mixedDetectedPerson.mixedVector[i].getDetectStatus()=="closeContacts"){
                        closeContactsBuildingNums.push_back(mixedDetectedPerson.mixedVector[i].getPersonCode().substr(0,3));
                    }
                }
                //遍历所有的人员,如果在这些密接的楼栋且状态为阴性,状态改为为次密接
                for (int i = 0; i < singleVector.size();i++){
                    for (int j = 0; j < closeContactsBuildingNums.size();j++){
                        if(closeContactsBuildingNums[j]==singleVector[i].getPersonCode().substr(0,3)){
                            if(singleVector[i].getDetectStatus()=="negative"){
                                singleVector[i].changeDetectStatus("subCloseContacts");
                            }
                            break;
                        }
                    }
                }
                for (int i = 0; i < mixedDetectedPerson.mixedVector.size();i++){
                    for (int j = 0; j < closeContactsBuildingNums.size();j++){
                        if(closeContactsBuildingNums[j]==mixedDetectedPerson.mixedVector[i].getPersonCode().substr(0,3)){
                            if(mixedDetectedPerson.mixedVector[i].getDetectStatus()=="negative"){
                                mixedDetectedPerson.mixedVector[i].changeDetectStatus("subCloseContacts");
                            }
                            break;
                        }
                    }
                }
                //确保阳的人是阳而不是其他状态
                singleVector[positiveIndex].changeDetectStatus("positive");
            }
            if(status=="suspicious"){
                //管子对应的人的状态转为可疑
                for (int i = 0; i < singleVector.size();i++){
                    if(singleVector[i].getDetectionCubeCode()==detectionCubeCode){
                        singleVector[i].changeDetectStatus("suspicious");
                    }
                }
            }
        }
};