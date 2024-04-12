#include <iostream>
#include<string>
#include<fstream>
#include<vector>
using namespace std;

vector<int> chars; 
vector<int> freq;
int liczbaS = 0;

    struct galaz
    { 
       int ojciec;
       int potomek1;
       int potomek2;
     };
    struct galaz drzewo[256]; 



   

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

	liczbaS = ileBajtow;
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

/*nt porownajLiczebnosci(const void* operand1, const void* operand2)
{
	const struct opisSymbol* pointer1 = (const struct opisSymbol*)operand1;
	const struct opisSymbol* pointer2 = (const struct opisSymbol*)operand2;
	if ((pointer1->licznosc) == (pointer2->licznosc)) return 0;
	else if ((pointer1->licznosc) < (pointer2->licznosc))return 1;
	else return -1;
}*/

void zrobDrzewo(string file)
{

	

	liczbaS = chars.size();

	int liczbaSymboli = liczbaS;
	int licznikSymboli = 0;
	int licznikSymboliDodanych = 0;
	int n, k;


	licznikSymboli = liczbaS;
	for (k = 0; k < liczbaSymboli - 1; k++)
	{
		drzewo[k].ojciec = 256 + k;
		drzewo[k].potomek1 = chars[liczbaSymboli - 1 - k];
		drzewo[k].potomek2 = chars[liczbaSymboli - 2 - k];
		chars[liczbaSymboli - 2 - k] = 256 + k;
		freq[liczbaSymboli - 2 - k] = freq[liczbaSymboli - 1 - k] + freq[liczbaSymboli - 2 - k];
		licznikSymboli--;
		//qsort(modelDlaDrzewa, licznikSymboli, sizeof(struct opisSymbol), porownajLiczebnosci);
		licznikSymboliDodanych++;
	}

	string namebase = file.substr(0, file.find("."));
	ofstream plik(namebase + ".drzewo");

	if (!plik.is_open())
	{
		printf("Nie mozna utworzyc pliku: \n", namebase + ".drzewo");
		exit(EXIT_FAILURE);
	};

	printf("Drzewo Huffmana \n");
	for (k = 0; k < licznikSymboliDodanych; k++)
	{
		plik<<drzewo[k].ojciec<<" "<<drzewo[k].potomek1<<" "<<drzewo[k].potomek2<<endl;
		printf("%d %d %d\n ", drzewo[k].ojciec, drzewo[k].potomek1, drzewo[k].potomek2);
	};

	plik.close();
}

    struct slowoDlaSymbolu 
    {   
       int symbol;
       unsigned char slowo[4];
       int liczbaBitow;
    };
    struct slowoDlaSymbolu tablicaKodu[256];


void zapiszTablicaKodu(int liczbaSymboli, string file)
{
	int k, i;
	unsigned char Four[4];

	string namebase = file.substr(0, file.find("."));
	ofstream plik(namebase + ".tabelaKodu");

	if (!plik.is_open())
	{
		printf("Nie mozna utworzyc pliku: \n", namebase + ".tabelaKodu");
		exit(EXIT_FAILURE);
	};

	if (!plik.is_open())
	{
		printf("Nie mozna utworzyc pliku: %s \n", namebase + ".tabelaKodu");
		exit(EXIT_FAILURE);
	}

	cout << "Tablica kodu w zapisie bajtowym do pliku " << namebase << ".tabelaKodu\n";
	for (k = 0; k < liczbaSymboli; ++k)
	{
		for (i = 0; i < 4; ++i)
			Four[i] = tablicaKodu[k].slowo[i];
		plik<<tablicaKodu[k].symbol<<" "<<Four[0]<<" "<<Four[1]<<" "<<Four[2]<<" "<<Four[3]<<" "<<tablicaKodu[k].liczbaBitow<<endl;
		printf(" %d %d  %d  %d %d %d \n", tablicaKodu[k].symbol, Four[0], Four[1], Four[2], Four[3], tablicaKodu[k].liczbaBitow);
	}
	plik.close();
}

void zrobTablicaKodu(string file)
{
	extern struct galaz drzewo[256];
	extern struct slowoDlaSymbolu tablicaKoduFull[512];

	int liczbaSymboli = liczbaS;
	int symbol;
	int n, k, i;
	int indeksOjca, symbolOjca;
	int licznikDrzewa;
	int licznikKodu;
	int dlugoscCiagu = 0;
	int pozycja, ktoryBajt, ktoryBit;
	unsigned char four[4];
	unsigned char bajt, jedynka = 1;


	for (k = 0; k < 512; ++k)
	{
		for (i = 0; i < 4; ++i)
			tablicaKoduFull[k].slowo[i] = 0;
		tablicaKoduFull[k].symbol = 0;
		tablicaKoduFull[k].liczbaBitow = 0;
	}

	licznikDrzewa = liczbaSymboli - 2;
	licznikKodu = 0;
	tablicaKoduFull[licznikKodu].symbol = drzewo[licznikDrzewa].ojciec;

	while (licznikDrzewa + 1)
	{
		symbolOjca = drzewo[licznikDrzewa].ojciec;
		tablicaKoduFull[licznikKodu + 1].symbol = drzewo[licznikDrzewa].potomek1;
		tablicaKoduFull[licznikKodu + 2].symbol = drzewo[licznikDrzewa].potomek2;

		indeksOjca = -1;
		for (i = 0; i < licznikKodu + 1; ++i)
		{
			if ((tablicaKoduFull[i].symbol) == (symbolOjca))
			{
				indeksOjca = i;
				break;
			}
		}
		if (indeksOjca == -1)
		{
			printf("Blad tworzenie tablicy kodu\n");
			exit(EXIT_FAILURE);
		}

		dlugoscCiagu = tablicaKoduFull[indeksOjca].liczbaBitow;

		for (i = 0; i < 4; ++i)
		{
			tablicaKoduFull[licznikKodu + 1].slowo[i] = tablicaKoduFull[indeksOjca].slowo[i];
			tablicaKoduFull[licznikKodu + 2].slowo[i] = tablicaKoduFull[indeksOjca].slowo[i];
		}

		pozycja = dlugoscCiagu;
		ktoryBajt = pozycja / 8;//pozycja= d�ugo�� ci�gu w bitach,bajt ma 8 bit�w
		ktoryBit = pozycja % 8;
		jedynka = 1;
		jedynka = jedynka << 7 - ktoryBit;

		bajt = tablicaKoduFull[indeksOjca].slowo[ktoryBajt];
		bajt = bajt | jedynka;

		tablicaKoduFull[licznikKodu + 1].slowo[ktoryBajt] = bajt;
		tablicaKoduFull[licznikKodu + 1].liczbaBitow = dlugoscCiagu + 1;
		tablicaKoduFull[licznikKodu + 2].liczbaBitow = dlugoscCiagu + 1;
		licznikKodu += 2;
		licznikDrzewa--;
	}

	zapiszTablicaKodu(licznikKodu + 1, file);

		//Eliminacja symboli dodanych
		licznikKodu = 0;
	for (k = 0; k < 256; ++k)
	{
		for (i = 0; i < 4; ++i)
			tablicaKodu[k].slowo[i] = 0;
		tablicaKodu[k].symbol = 0;
		tablicaKodu[k].liczbaBitow = 0;
	}

	for (k = 0; k < 2 * liczbaSymboli - 1; ++k)
	{
		symbol = tablicaKoduFull[k].symbol;
		if (symbol < 256)
		{
			tablicaKodu[licznikKodu].symbol = symbol;
			tablicaKodu[licznikKodu].liczbaBitow = tablicaKoduFull[k].liczbaBitow;
			for (i = 0; i < 4; ++i)
				tablicaKodu[licznikKodu].slowo[i] = tablicaKoduFull[k].slowo[i];
			licznikKodu++;
		}
	}

	zapiszTablicaKodu(liczbaSymboli, file);

}

void printTree(int liczbaSymboli)
{
	extern struct galaz drzewo[256];
	int k;

	printf("Drzewo Huffmana \n");
	for (k = 0; k < liczbaSymboli - 1; k++)
	{
		printf("%d %d %d\n ", drzewo[k].ojciec, drzewo[k].potomek1, drzewo[k].potomek2);
	}
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
	zrobDrzewo(file);
	zrobTablicaKodu(file); 
	printTree(liczbaS); 

	return 0;
}

