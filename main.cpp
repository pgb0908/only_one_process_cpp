#include <iostream>
#include <sys/file.h>
#include <unistd.h>
#include <string>
using namespace std;

int main() {

    // pid를 담는 파일 생성
    const char* fileName = "/home/bong/Desktop/work/anyapi/only_one_process.pid";
    FILE* file = fopen(fileName, "w+"); // 무조건 새로 만들기

    if(file){
        std::cout << "file open" << std::endl;
        string str_pid = std::to_string(getpid());

        fwrite(str_pid.c_str(), 1, str_pid.size(), file);
        fflush(file);

    }else{
        std::cout << "can not open file " << std::endl;
    }
    fclose(file);

    int pid_file = open(fileName, O_CREAT | O_RDWR, 0666);  // 0666 : file
    int rc = flock(pid_file, LOCK_EX | LOCK_NB);
    if(rc) {
        if(EWOULDBLOCK == errno){
            // another instance is running
            std::cout << "other program is running" << std::endl;
            exit(EXIT_FAILURE);
        }

    }

    // this is the first instance
    std::cout << "Hello, World!" << std::endl;
    sleep(60);

    return 0;
}
