
#include <iostream> 
#include<bits/stdc++.h>
#include <fstream> 

using namespace std;
int main() 
{ 
	// Creation of ofstream class object 
	ofstream fout; 
	string line; 
	fout.open("sample.txt"); 

	// Execute a loop If file successfully opened 
	while (fout) { 
		getline(cin, line); 

		// Press -1 to exit  
		if (line == "-1") 
			break; 

		// Writing line in file 
		fout << line << endl; 
	} 
	fout.close(); 

	// ifstream class object created to read the file 
	ifstream fin; 
	fin.open("sample.txt"); 

	// Execute a loop until EOF (End of File) 
	while (fin) { 
		getline(fin, line);  
		cout << line << endl; 
	}  
	fin.close(); 

	return 0; 
} 
//PRESS -1 TO EXIT WRITING IN FILE
