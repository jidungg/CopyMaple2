// NIFToBinary.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#include "pch.h"
#include "CNifToBinary.h"

int main(int argc, char* argv[])
{
    CNifToBinary modelConverter;
    fs::path path;
    path = fs::current_path();
    cin >> path;

    for (const auto& entry : recursive_directory_iterator(path)) {
        if (entry.path().extension() == ".nif") {
            //cout << entry.path().string() << endl;
            modelConverter.NifToBinary(entry.path().string());
        }
    }


    return 0;
}