#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

using namespace std;

int N; // sudokuruudukon sivun koko

vector<vector<int> > S; // annettu sudoku

// lista kaikista alueista (region) ja mitä numeroita niissä alueissa on asetettu
// HUOM: idiomaattisempaa olisi käyttää vector<vector<bool> > -tietorakennetta.
// Syy miksi näin ei tehty, oli eräänlainen pienoptimointi, sillä vector<bool>
// on C++:ssa toteutettu bittivektorina, jonka käsittely on hitaampaa kuin
// vector<int>:n.

vector<vector<int> > RE; // "regions"

// lista kaikista riveistä ja niiden numeroista (vastaava kuin RE alueille)
vector<vector<int> > RO; // "rows"

// lista kaikista sarakkeista ja niiden numeroista (vastaa edellä olevia)
vector<vector<int> > CO; // "columns"

// Lista ruuduista (y,x) jotka olivat alkuperäisessä sudokussa tyhjiä ruutuja.
// Tätä rakennetta tarvitaan siksi, ettei jatkuvasti tarvitse rekursiossa
// "matkustaa" tyhjien ruutujen läpi edestakaisin, vaan voidaan suoraan
// käsitellä ruutuja, joilla on merkitystä.
vector<pair<int,int> > next;


// Laskee koordinaattiparille (y,x) sitä vastaavan alueen järjestysnumeron
// HUOM: ei toimi sudokuilla, joiden sivun koko on yli 16, tai alle 4
int region(const pair<int,int>& sq) {
	int c = sq.second;
	int r = sq.first;
	int S = N == 16 ? 4 : (N == 9 ? 3 : 2);
    return r / S * S + c / S;
}

// Voidaanko ruutuun (y,x) laittaa kandidaatti C?

bool valid(const pair<int,int>& sq, int C) {
    return !RE[region(sq)][C] && !RO[sq.first][C] && !CO[sq.second][C];
}

// Laitetaan ruutuun (y,x) kandidaatti C, eli päivitetään rivit, sarakkeet ja
// alueet sen mukaisesti

void set(const pair<int,int>& sq, int C) {
    RE[region(sq)][C] = 1;
    RO[sq.first][C] = 1;
    CO[sq.second][C] = 1;
    S[sq.first][sq.second] = C;
}

// Peruutetaan asetetut arvot, poistetaan kandidaatti itse ruudusta sekä sitä
// vastaavilta riviltä, sarakkeelta ja alueelta

void unset(const pair<int,int>& sq, int C) {
    RE[region(sq)][C] = 0;
    RO[sq.first][C] = 0;
    CO[sq.second][C] = 0;
    S[sq.first][sq.second] = 0;
}

bool halt = false; // onko vastaus löytynyt?

// rekursiivinen funktio sudokun ratkaisemiseen

void solve(int ind) {
	// mikäli indeksi on suurempi kuin tyhjien ruutujen taulukossa (next) on
	// elementtejä, ollaan löydetty hyväksyttävä vastaus, jolloin algoritmin
	// suoritus voidaan lopettaa
    if(ind == next.size()) {
        halt = true;
        return;
    }

    // kokeillaan kaikki mahdolliset arvot tähän ruutuun
    for(int i = 1; i <= N; ++i) {
    	int C = i;
    	// voidaanko C laittaa tähän ruutuun sudokun perussääntöjen mukaisesti?
        if(valid(next[ind],C)) {
        	set(next[ind],C); // asetetaan C ruutuun next[ind]
            // siirrytään rekursiivisesti ratkaisemaan seuraavaa tyhjää ruutua
        	solve(ind+1); 
        	if(halt) return; // jos ratkaisu löytyi, lopeta
        	unset(next[ind],C); // kumoa tehdyt muutokset
        }
    }
}

int main() {
	cin>>N;
	// varataan vektoreille tilaa
	S.resize(N); 
	RE.resize(N+1);
	RO.resize(N+1);
	CO.resize(N+1);
	for(int i = 0; i < N; ++i) {
		// varataan jokaiselle vektorille vektoreiden sisällä tilaa
        S[i].resize(N);
        RE[i].resize(N+1);
        RO[i].resize(N+1);
        CO[i].resize(N+1);
        // luetaan sudokun arvot
        for(int j = 0; j < S[i].size(); ++j) {
            cin>>S[i][j];
        }
    }

    // kerätään tyhjät ruudut

    for(int i = 0; i < N; ++i) {
        for(int j = 0; j < N; ++j) {
            if(!S[i][j]) next.push_back(make_pair(i,j));
        }
    }

    // asetetaan rivi-, sarake- ja aluevektoreiden tilat vastaamaan sudokun
    // alkutilaa

    for(int i = 0; i < N; ++i) {
        for(int j = 0; j < N; ++j) {
            RE[region(make_pair(i,j))][S[i][j]] = 1;
            RO[i][S[i][j]] = 1;
            CO[j][S[i][j]] = 1;
        }
    }

    // ratkaistaan ongelma

    solve(0);

    // tulostetaan vastaus

    for(int i = 0; i < S.size(); ++i) {
        for(int j = 0; j < S[i].size(); ++j) {
            cout<<S[i][j]<<" ";
        }
        cout<<endl;
    }

    return 0;
}
