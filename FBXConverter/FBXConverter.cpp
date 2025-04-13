// FBXConverter.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#include "pch.h"

#include "FBXToBinary.h"




int main(int argc, char* argv[])
{
	CFbxToBinary modelConverter;
	fs::path srcPath;
	srcPath = fs::current_path();
	cin >> srcPath;
	//fs::path lookUpPath;
	//cin >> lookUpPath;

    for (const auto& entry : recursive_directory_iterator(srcPath)) {
		//FBXConvert 코드
        if (entry.path().extension() == ".fbx") {
            cout << entry.path().string() << "=========================================="<< endl;
			modelConverter.FbxToBinary( entry.path().string());
        }
    
		//Nif복사 코드
		//if (entry.path().extension() == ".nif") {
		//	for (const auto& lookupENtry : recursive_directory_iterator(lookUpPath)) {
		//		if (entry.path().filename() == lookupENtry.path().filename()) {
		//			fs::copy_file(lookupENtry.path(), entry.path(), fs::copy_options::overwrite_existing);
		//			cout << "COpy " << lookupENtry.path() << "\nto" << entry.path() << endl;
		//		}
		//	}
		//}
    }
		

    return 0;
}