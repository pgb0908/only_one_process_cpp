#include <iostream>
#include <sys/file.h>
#include <unistd.h>
#include <string>
using namespace std;


int lockFile(const std::string& fileName){
    int pid_file = open(fileName.c_str(), O_CREAT | O_RDWR, 0666);  // 0666 : file
    return flock(pid_file, LOCK_EX | LOCK_NB);
}

bool isFileLock(const std::string& fileName){
    int rc = lockFile(fileName);
    if(rc) {
        if(EWOULDBLOCK == errno){
            // 만약 파일이 잠겨있게 되면 errno에 EWOULBLOCK가 설정
            cout << "file is locked" << endl;
            return true;
        }
    }

    cout << "file is not locked" << endl;
    return false;
}

void updatePidFile(const std::string& fileName){
    FILE* file = fopen(fileName.c_str(), "w+"); // 무조건 새로 만들기

    if(file){
        std::cout << "file open" << std::endl;
        string str_pid = std::to_string(getpid());

        fwrite(str_pid.c_str(), 1, str_pid.size(), file);
        fflush(file);

    }else{
        std::cout << "can not open file " << std::endl;
        exit(EXIT_FAILURE);
    }

    fclose(file);
}

int main() {

    // pid를 담는 파일 생성
    std::string fileName = "/home/bong/Desktop/work/anyapi/only_one_process.pid";

    if(access(fileName.c_str(), F_OK))
    {
        cout << "file is not exist" << endl;
        // 파일 생성
        updatePidFile(fileName);
        lockFile(fileName);

    }else{
        cout << "file is already exist" << endl;

        if(isFileLock(fileName)){         // 파일이 lock을 잡고 있는지 확인
            std::cout << "current Process is running [" <<  getpid() << "]" << std::endl;
            std::cout << "another process already running" << std::endl;
            exit(EXIT_FAILURE);
        }else{
            updatePidFile(fileName);
            lockFile(fileName);
        }
    }


    // this is the first instance
    std::cout << "Process is running [" <<  getpid() << "]" << std::endl;
    sleep(120);

    return 0;
}
