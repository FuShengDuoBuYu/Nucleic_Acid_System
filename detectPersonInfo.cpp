#ifndef DETECTPERSONINFO_CPP
#define DETECTPERSONINFO_CPP
#include<string>
using namespace std;
class detectPersonInfo{
    private:
        string personCode;
        string detectStatus;
        int detectionCubeCode;

    public:
        string getPersonCode(){
            return this->personCode;
        }
        string getDetectStatus(){
            return this->detectStatus;
        }
        int getDetectionCubeCode(){
            return this->detectionCubeCode;
        }
        detectPersonInfo(string personCode){
            this->personCode = personCode;
            this->detectStatus = "lining";
            this->detectionCubeCode = -1;
        }
        bool changeDetectStatus(string  newDetectStatus){
            this->detectStatus = newDetectStatus;
            return this->detectStatus == newDetectStatus ? true : false;
        }
        bool changeDetectionCubeCode(int detectionCubeCode){
            this->detectionCubeCode = detectionCubeCode;
            return this->detectionCubeCode == detectionCubeCode ? true : false;
        }
};
#endif