#include <iostream>
#include<string>
#include<fstream>
#include<vector>
using namespace std;

vector<int> chars; 
vector<int> freq;



   

void ileBajtow(string file){

	int ileBajtow = 0;

	ifstream plik(file, ios::binary);
	if (plik.is_open()) {
		plik.seekg(ios::beg, ios::end);
		int rozmiar = plik.tellg();
		ileBajtow = rozmiar;
		string namebase = file.substr(0, file.find("."));
		ofstream plik2(namebase + ".ileBajtow");
		if (plik2.is_open()) {
			plik2<<rozmiar;
			plik2.close();
		}
		else {
			cout<<"Nie udalo sie otworzyc pliku\n";
		}
		plik.close();
	}
	else {
		cout<<"Nie udalo sie otworzyc pliku\n";
	}

	cout<<"Z pliku "<<file<<" odczytano "<<ileBajtow<<" bajtow\n";
	cout<<"Zapisano do pliku "<<file.substr(0, file.length()-4)<<".ileBajtow\n";


}

void model(string file) {
	ifstream plik(file, ios::binary);
	 
	if (plik.is_open()) {
		string namebase = file.substr(0, file.find("."));
		

		//counting chars
		char c;
		while (plik.get(c)) {
			int i = 0;
			for (i = 0; i < chars.size(); i++) {
				if (chars[i] == c) {
					freq[i]++;
					break;
				}
			}
			if (i == chars.size()) {
				chars.push_back(c);
				freq.push_back(1);
			}
		}
		//writing to file
		ofstream plik(namebase + ".model");
		if (plik.is_open()) {
			for (int i = 0; i < chars.size(); i++) {
				plik<<chars[i]<<" "<<freq[i]<<endl;
			}
			plik.close();
		}
		else {
			cout<<"Nie udalo sie otworzyc pliku\n";
		}
	
		plik.close();
	}
	else {
		cout<<"Nie udalo sie otworzyc pliku\n";
	}

	cout << "Model nieposortowany zrodla\n";
	for (int i = 0; i < chars.size(); i++) {
		cout << i << " indeks tablicy: " << chars[i] << " liczebnosc " << freq[i] << endl;
	}
	cout << "Model Shannona nieposortowany zapisano do pliku " << file.substr(0, file.length() - 4) << ".model\n";
}

void modelSort(string file) {
	
	for (int i = 0; i < chars.size(); i++) {
		for (int j = 0; j < chars.size() - 1; j++) {
			if (freq[j] < freq[j + 1]) {
				int temp = freq[j];
				freq[j] = freq[j+1];
				freq[j+1] = temp;
				int temp2 = chars[j];
				chars[j] = chars[j+1];
				chars[j+1] = temp2;
			}
		}
	}
	string namebase = file.substr(0, file.find("."));

	ofstream plik(namebase + ".modelSort");
	if (plik.is_open()) {
		for (int i = 0; i < chars.size(); i++) {
			plik<<chars[i]<<" "<<freq[i]<<endl;
		}
		plik.close();
	}
	else {
		cout<<"Nie udalo sie otworzyc pliku\n";
	}

	cout << "Model posortowany zrodla\n";
	for (int i = 0; i < chars.size(); i++) {
		cout << i << " indeks tablicy: " << chars[i] << " liczebnosc " << freq[i] << endl;
	}
	cout << "Model Shannona posortowany zapisano do pliku " << file.substr(0, file.length() - 4) << ".modelSort\n";

}




int main(int argc, char* argv[])
{
	string file;

	if (argc == 2) {
		file = argv[1];
	}
	else {
		cout << " " << argc << endl;
		cout<<"Dopuszczalna liczba argumentow to 1\n";
		return 1;
	}

	ileBajtow(file);
	model(file);
	modelSort(file);

	return 0;
}

