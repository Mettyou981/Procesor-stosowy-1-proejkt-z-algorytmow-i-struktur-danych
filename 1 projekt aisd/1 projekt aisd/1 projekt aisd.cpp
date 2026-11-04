#include <iostream>
using namespace std;

struct lista {
    char wartosc = '0';
    lista* next;
    lista* prev;
};

struct stos_element {
    lista* lista;
    stos_element* next;
};

void wypiszListe(lista* l) {
    if (l == nullptr) return;
    if (l->wartosc != '\0') cout << l->wartosc;
    wypiszListe(l->next);
}

void wypiszStos(stos_element* stos, int indeks) {
    if (stos == nullptr) return;
    wypiszStos(stos->next, indeks + 1);
    cout << indeks << ": ";
    wypiszListe(stos->lista);
    cout << endl;
}

lista* kopiaListy(lista* l, lista* prev = nullptr) {
    if (l == nullptr) return nullptr;
    lista* nowy = new lista{ l->wartosc, nullptr, prev };
    nowy->next = kopiaListy(l->next, nowy);
    return nowy;
}

void wykonajOperacjeDwukropek(stos_element*& stos) {
    if (stos != nullptr) {
        lista* skopiowana_lista = kopiaListy(stos->lista);
        stos_element* nowy = new stos_element{ skopiowana_lista, stos };
        stos = nowy;
    }
}

void zwolnijListe(lista* l) {
    if (l == nullptr) return;
    zwolnijListe(l->next);
    delete l;
}
void wykonajOperacjeUsunZeStosu(stos_element*& stos) {
    if (stos == nullptr) return;
    lista* l = stos->lista;
    zwolnijListe(l);
    stos_element* do_usuniecia = stos;
    stos = stos->next;
    delete do_usuniecia;
}

void wykonajOperacjeZamienMiejscami(stos_element* stos) {
    if (stos == nullptr || stos->next == nullptr) return;
    lista* temp = stos->lista;
    stos->lista = stos->next->lista;
    stos->next->lista = temp;
}

int odczytajLiczbeZListy(lista* l, int mnoznik = 1) {
    if (l == nullptr) return 0;
    int cyfra = l->wartosc - '0';
    return cyfra * mnoznik + odczytajLiczbeZListy(l->next, mnoznik * 10);
}

stos_element* przesunStos(stos_element* s, int kroki) {
    if (kroki == 0 || s == nullptr) return s;
    return przesunStos(s->next, kroki - 1);
}

void wykonajOperacjeAt(stos_element*& stos) {
    if (stos == nullptr || stos->lista == nullptr) return;

    int A = odczytajLiczbeZListy(stos->lista);

    lista* l = stos->lista;
    zwolnijListe(l);
    stos_element* do_usuniecia = stos;
    stos = stos->next;
    delete do_usuniecia;

    stos_element* temp = przesunStos(stos, A); 

    if (temp != nullptr) {
        lista* kopia = kopiaListy(temp->lista);
        stos_element* nowy = new stos_element{ kopia, stos };
        stos = nowy;
    }
}
void dodajNaPoczatekListy(stos_element* stos, char znak) {
    if (stos == nullptr) return;

    if (stos->lista == nullptr) {
        stos->lista = new lista{ znak, nullptr, nullptr };
    }
    else {
        lista* nowy = new lista{ znak, stos->lista, nullptr };
        stos->lista->prev = nowy;
        stos->lista = nowy;
    }
}
void wykonajOperacjeWypiszIZdejmij(stos_element*& stos) {
    if (stos == nullptr || stos->lista == nullptr) return;

    cout << stos->lista->wartosc;

    lista* l = stos->lista;
    zwolnijListe(l);

    // Usuń element stosu
    stos_element* do_usuniecia = stos;
    stos = stos->next;
    delete do_usuniecia;
}
lista* znajdzOstatni(lista* aktualny) {
    if (aktualny == nullptr || aktualny->next == nullptr) return aktualny;
    return znajdzOstatni(aktualny->next);
}
void sprawdzOstatniMinusRekurencyjnie(lista*& glowa, lista* aktualny) {
    if (aktualny == nullptr) {
        // Lista jest pusta – dodajemy '-' jako pierwszy i jedyny element
        lista* nowy = new lista{ '-', nullptr, nullptr };
        glowa = nowy;
        return;
    }

    if (aktualny->next == nullptr) { // Jesteśmy na ostatnim elemencie
        if (aktualny->wartosc == '-') {
            // Usuwamy '-'
            if (aktualny->prev != nullptr) {
                aktualny->prev->next = nullptr;
            }
            else {
                glowa = nullptr; // To był jedyny element
            }
            delete aktualny;
        }
        else {
            // Dodajemy '-' na koniec
            lista* nowy = new lista{ '-', nullptr, aktualny };
            aktualny->next = nowy;
        }
        return;
    }

    sprawdzOstatniMinusRekurencyjnie(glowa, aktualny->next);
}

void wykonajOperacjeMinus(stos_element*& stos) {
    if (stos == nullptr) return;

    sprawdzOstatniMinusRekurencyjnie(stos->lista, stos->lista);
}
void usunOstatniMinusRekurencyjnie(lista*& glowa, lista* aktualny) {
    if (aktualny == nullptr) return;

    if (aktualny->next == nullptr) { // Jesteśmy na ostatnim elemencie
        if (aktualny->wartosc == '-') {
            if (aktualny->prev != nullptr) {
                aktualny->prev->next = nullptr;
            }
            else {
                glowa = nullptr; // To był jedyny element w liście
            }
            delete aktualny;
        }
        return;
    }

    usunOstatniMinusRekurencyjnie(glowa, aktualny->next);
}

void wykonajOperacjeDaszek(stos_element*& stos) {
    if (stos == nullptr || stos->lista == nullptr) return;

    usunOstatniMinusRekurencyjnie(stos->lista, stos->lista);
}

void dodajCyfreDoListy(lista** nowaLista, lista** ostatni, int cyfra) {
    lista* nowy = new lista{ static_cast<char>('0' + cyfra), nullptr, *ostatni };
    if (*ostatni != nullptr) {
        (*ostatni)->next = nowy;
    }
    *ostatni = nowy;
    if (*nowaLista == nullptr) {
        *nowaLista = nowy;
    }
}

void stworzListeZCyfr(int kodASCII, lista** nowaLista, lista** ostatni) {
    if (kodASCII == 0) return;
    int cyfra = kodASCII % 10;
    dodajCyfreDoListy(nowaLista, ostatni, cyfra);
    stworzListeZCyfr(kodASCII / 10, nowaLista, ostatni);
}

lista* znajdzPoczatekListy(lista* l) {
    if (l == nullptr || l->prev == nullptr) return l;
    return znajdzPoczatekListy(l->prev);
}

void wykonajOperacjeKodASCII(stos_element*& stos) {
    if (stos == nullptr || stos->lista == nullptr) return;

    // Zdejmij listę ze stosu
    lista* listaA = stos->lista;
    stos->lista = nullptr;
    stos_element* doUsuniecia = stos;
    stos = stos->next;
    delete doUsuniecia;

    // Jeśli lista była pusta, nic więcej nie robimy
    if (listaA == nullptr) return;

    // Pobierz pierwszy znak z listy
    char pierwszyZnak = listaA->wartosc;
    int kodASCII = static_cast<unsigned char>(pierwszyZnak);

    // Zwolnij pamięć po zdjętej liście
    zwolnijListe(listaA);

    // Utwórz nową listę z cyframi kodu ASCII
    lista* nowaLista = nullptr;
    lista* ostatni = nullptr;

    if (kodASCII == 0) {
        nowaLista = new lista{ '0', nullptr, nullptr };
    }
    else {
        stworzListeZCyfr(kodASCII, &nowaLista, &ostatni);
        nowaLista = znajdzPoczatekListy(ostatni);
    }

    // Utwórz nowy element stosu z listą cyfr
    stos_element* nowyElement = new stos_element{ nowaLista, stos };
    stos = nowyElement;
}
void obliczLiczbeRekurencyjnie(lista* current, int& liczba) {
    if (current == nullptr) return;

    if (isdigit(current->wartosc)) {
        liczba = liczba * 10 + (current->wartosc - '0');
    }

    obliczLiczbeRekurencyjnie(current->prev, liczba);
}
void wykonajOperacjeZnakASCII(stos_element*& stos) {
    if (stos == nullptr || stos->lista == nullptr) return;

    // Znajdź ostatni element listy rekurencyjnie
    lista* ostatni = znajdzOstatni(stos->lista);

    // Oblicz liczbę rekurencyjnie (idąc od końca)
    int liczba = 0;
    obliczLiczbeRekurencyjnie(ostatni, liczba);

    // Ogranicz do zakresu ASCII
    if (liczba < 0 || liczba > 127) {
        liczba = 0;
    }

    // Zwolnij starą listę rekurencyjnie
    zwolnijListe(stos->lista);

    // Wstaw nową listę z pojedynczym znakiem ASCII
    stos->lista = new lista{ static_cast<char>(liczba), nullptr, nullptr };
}

void wykonajOperacjeDolar(stos_element*& stos) {
    if (stos == nullptr || stos->lista == nullptr) return;

    lista* pierwszy = stos->lista;
    lista* reszta = pierwszy->next;

    // Odłącz pierwszy element
    pierwszy->next = nullptr;
    pierwszy->prev = nullptr;

    // Zmodyfikuj istniejącą listę na stosie
    stos->lista = reszta;
    if (reszta != nullptr) {
        reszta->prev = nullptr;
    }

    // Utwórz nowy element stosu z pierwszym elementem
    stos_element* nowyElement = new stos_element{ pierwszy, stos };
    stos = nowyElement;
}
void znajdzKoniecIDolacz(lista* obecny, lista* listaA) {
    if (obecny->next == nullptr) {
        obecny->next = listaA;
        if (listaA != nullptr) {
            listaA->prev = obecny;
        }
    }
    else {
        znajdzKoniecIDolacz(obecny->next, listaA);
    }
}

void wykonajOperacjeHasz(stos_element*& stos) {
    if (stos == nullptr || stos->next == nullptr) return;

    // Przechowaj wskaźniki do zwolnienia później
    stos_element* elementA = stos;
    lista* listaA = elementA->lista;

    // Przesuń stos do przodu
    stos = stos->next;
    elementA->next = nullptr;

    if (stos->lista == nullptr) {
        stos->lista = listaA;
    }
    else {
        znajdzKoniecIDolacz(stos->lista, listaA);
    }

    // Zwolnij pamięć - tylko element stosu, lista jest teraz częścią innego elementu
    delete elementA;
}



// Cofnij się do poprzedniej cyfry różnej od zera
lista* idzDoPoprzedniejCyfry(lista* l) {
    // Warunek kończący rekurencję
    if (l == nullptr) return nullptr;

    // Sprawdź obecny element
    if (isdigit(l->wartosc) && l->wartosc != '0') {
        return l;  // Znaleziono odpowiednią cyfrę
    }

    // Rekurencyjne przejście do poprzedniego elementu
    return idzDoPoprzedniejCyfry(l->prev);
}
bool czyUjemnaRekurencyjnie(lista* l, bool sprawdzajZnak) {
    // Warunek kończący rekurencję
    if (l == nullptr) return false;

    // Najpierw idziemy do końca listy
    if (!sprawdzajZnak && l->next != nullptr) {
        return czyUjemnaRekurencyjnie(l->next, false);
    }

    // Następnie wracamy sprawdzając znak '-'
    if (l->wartosc == '-') return true;

    // Kontynuujemy przeszukiwanie wstecz
    return czyUjemnaRekurencyjnie(l->prev, true);
}

// Funkcja opakowująca dla wygody użytkowania
bool czyUjemna(lista* l) {
    return czyUjemnaRekurencyjnie(l, false);
}

// Czy lista zawiera tylko zera i/lub minusy
bool tylkoZeraIMinusy(lista* l) {
    // Warunek kończący rekurencję - dotarliśmy do końca listy
    if (l == nullptr) {
        return true;
    }

    // Sprawdzamy bieżący element
    if (l->wartosc >= '0' && l->wartosc <= '9') {
        if (l->wartosc != '0') {
            return false;  // Znaleziono cyfrę różną od '0'
        }
    }
    else if (l->wartosc != '-') {
        return false;  // Znaleziono znak inny niż '-' lub cyfra
    }

    // Rekurencyjne sprawdzenie następnego elementu
    return tylkoZeraIMinusy(l->next);
}
// Pomija zera z lewej strony (czyli z przodu listy)
lista* pominZeraZPrzoduRekurencyjnie(lista* l) {
    // Warunek kończący rekurencję
    if (l == nullptr || l->wartosc != '0') {
        return l;
    }

    // Rekurencyjne pominięcie kolejnego zera
    return pominZeraZPrzoduRekurencyjnie(l->next);
}

// Funkcja opakowująca z referencją
lista* pominZeraZPrzodu(lista*& l) {
    l = pominZeraZPrzoduRekurencyjnie(l);
    return l;
}
int porownajRek(lista* a, lista* b) {
    if ((a == nullptr || tylkoZeraIMinusy(a)) && (b == nullptr || tylkoZeraIMinusy(b))) return 0;
    if (a == nullptr || tylkoZeraIMinusy(a)) return -1;
    if (b == nullptr || tylkoZeraIMinusy(b)) return 1;

    int dalej = porownajRek(a->next, b->next);

    if (dalej != 0) return dalej;

    if (a->wartosc < b->wartosc) return -1;
    if (a->wartosc > b->wartosc) return 1;
    return 0;
}
bool ifyDlaMniejszeNiz(lista*& a, lista*& b)
{
    bool wynik = false;

    if (tylkoZeraIMinusy(a) && tylkoZeraIMinusy(b)) {
        wynik = false;
    }
    else {

        if (czyUjemna(a) == false && czyUjemna(b) == true) {
            wynik = 1;
        }
        else if (czyUjemna(a) == true && czyUjemna(b) == false) {
            wynik = 0;
        }
        else if (czyUjemna(a) == true && czyUjemna(b) == true)
        {
            lista* AbezZer = pominZeraZPrzodu(a);
            lista* BbezZer = pominZeraZPrzodu(b);
            int cmp = porownajRek(AbezZer, BbezZer);
            if (cmp == 1 || cmp == 0) {
                wynik = 0;  
            }
            else if (cmp == -1)
            {
                wynik = 1;
            }

        }
        else if (czyUjemna(a) == false && czyUjemna(b) == false)
        {
            lista* AbezZer = pominZeraZPrzodu(a);
            lista* BbezZer = pominZeraZPrzodu(b);
            int cmp = porownajRek(AbezZer, BbezZer);

            if (cmp == 1) {
                wynik = 1;  // B < A dla dodatnich
            }
            else if (cmp == 0 || cmp == -1)
            {
                wynik = 0;   // B < A dla ujemnych -> B "mniej ujemne" niż A
            }
        }
    }
    return wynik;
}
void wykonajOperacjeMniejszeNiz(stos_element*& stos) {
    if (stos == nullptr || stos->next == nullptr) return;

    lista* A = stos->lista;
    lista* B = stos->next->lista;
    stos_element* elemA = stos;
    stos_element* elemB = stos->next;
    stos = stos->next->next;

    bool wynik = ifyDlaMniejszeNiz(A, B);

    // Zwolnij stare listy i elementy
    zwolnijListe(A);
    delete elemA;

    zwolnijListe(B);
    delete elemB;

    // Dodaj wynik jako '1' lub '0' na stos
    lista* nowaLista = new lista{ wynik ? '1' : '0', nullptr, nullptr };
    stos_element* nowyElement = new stos_element{ nowaLista, stos };
    stos = nowyElement;


}
// Rekurencyjne porównanie dwóch list od końca
bool porownajRekurencyjnie(lista* a, lista* b) {
    a = idzDoPoprzedniejCyfry(a);
    b = idzDoPoprzedniejCyfry(b);

    if (a == nullptr && b == nullptr) return true;
    if (a == nullptr || b == nullptr) return false;
    if (a->wartosc != b->wartosc) return false;

    return porownajRekurencyjnie(a->prev, b->prev);
}
void wykonajOperacjeRowne(stos_element*& stos) {
    if (stos == nullptr || stos->next == nullptr) return;

    lista* A = stos->lista;
    lista* B = stos->next->lista;

    stos_element* elemA = stos;
    stos_element* elemB = stos->next;
    stos = stos->next->next;

    bool wynik = false;

    bool tylkoZeraA = tylkoZeraIMinusy(A);
    bool tylkoZeraB = tylkoZeraIMinusy(B);

    if (tylkoZeraA && tylkoZeraB) {
        wynik = true;
    }
    else if (tylkoZeraA != tylkoZeraB) {
        // Jeden ma tylko zera/minusy, drugi nie — NIE są równe
        wynik = false;
    }
    else if (czyUjemna(A) != czyUjemna(B)) {
        wynik = false;
    }
    else {
        lista* koniecA = znajdzOstatni(A);
        lista* koniecB = znajdzOstatni(B);
        wynik = porownajRekurencyjnie(koniecA, koniecB);
    }

    // Zwolnij pamięć (A i B oraz ich elementy stosu)
    zwolnijListe(A);
    delete elemA;

    zwolnijListe(B);
    delete elemB;

    // Zapisz wynik (1 lub 0) jako lista i wrzuć na stos
    lista* nowaLista = new lista{ wynik ? '1' : '0', nullptr, nullptr };
    stos_element* nowyElement = new stos_element{ nowaLista, stos };
    stos = nowyElement;
}
void wykonajOperacjeNegacjaLogiczna(stos_element*& stos) {
    if (stos == nullptr) return;

    lista* listaA = stos->lista;
    stos_element* elemA = stos;
    stos = stos->next;

    bool warunekPrawda = false;

    if (listaA == nullptr) {
        warunekPrawda = true; // lista pusta
    }
    else if (listaA->next == nullptr && listaA->wartosc == '0') {
        warunekPrawda = true; // lista jednoelementowa z '0'
    }

    // Zwolnij starą listę
    zwolnijListe(listaA);
    delete elemA;

    // Zależnie od warunku, dodajemy '1' lub '0'
    char nowyZnak = warunekPrawda ? '1' : '0';
    lista* nowaLista = new lista{ nowyZnak, nullptr, nullptr };
    stos_element* nowyElement = new stos_element{ nowaLista, stos };
    stos = nowyElement;
}
void dodajCyfre(lista** nowaLista, lista** ostatni, int cyfra) {
    lista* nowy = new lista{ static_cast<char>('0' + cyfra), nullptr, *ostatni };
    if (*ostatni != nullptr) {
        (*ostatni)->next = nowy;
    }
    else {
        *nowaLista = nowy;
    }
    *ostatni = nowy;
}

// Funkcja rekurencyjna tworząca listę z cyfr
void tworzListeZCyfr(int temp, lista** nowaLista, lista** ostatni) {
    if (temp == 0) return;

    int cyfra = temp % 10;
    dodajCyfre(nowaLista, ostatni, cyfra);
    tworzListeZCyfr(temp / 10, nowaLista, ostatni);
}


void wykonajOperacjeTylda(stos_element*& stos, int ip) {
    if (ip < 0) return;

    lista* nowaLista = nullptr;
    lista* ostatni = nullptr;

    if (ip == 0) {
        nowaLista = new lista{ '0', nullptr, nullptr };
    }
    else {
        tworzListeZCyfr(ip, &nowaLista, &ostatni);
    }

    stos_element* nowyElement = new stos_element{ nowaLista, stos };
    stos = nowyElement;
}
bool czyListaJestZero(lista* l) {
    if (l == nullptr) return true;
    if (l->wartosc != '0') return false;
    return czyListaJestZero(l->next);
}

int zamienListeNaLiczbe(lista* l, int mnoznik = 1) {
    if (l == nullptr) return 0;
    if (!isdigit(l->wartosc)) return odczytajLiczbeZListy(l->next, mnoznik * 10);
    int cyfra = l->wartosc - '0';
    return cyfra * mnoznik + odczytajLiczbeZListy(l->next, mnoznik * 10);
}
void wykonajOperacjeSkokWarunkowy(stos_element*& stos, int& ip, int dlugosc_programu) {
    if (stos == nullptr || stos->next == nullptr) return;

    // Zdejmujemy listę A
    lista* listaA = stos->lista;
    stos_element* doUsunieciaA = stos;
    stos = stos->next;
    delete doUsunieciaA;

    // Zdejmujemy listę B
    lista* listaB = stos->lista;
    stos_element* doUsunieciaB = stos;
    stos = stos->next;
    delete doUsunieciaB;

    // Sprawdzenie warunku
    bool warunek = !(tylkoZeraIMinusy(listaB));

    // Jeśli warunek spełniony, ustaw ip
    int nowy_ip = zamienListeNaLiczbe(listaA);
    if (warunek) {
        if (nowy_ip < dlugosc_programu) {
            ip = nowy_ip - 1; // zmniejszamy o 1, bo w pętli i tak będzie zwiększony
        }
    }

    // Zwolnij listy
    zwolnijListe(listaA);
    zwolnijListe(listaB);
}
void dodajCyfreDoWyniku(int cyfra, lista*& wynik, lista*& ostatni) {
    lista* nowy = new lista{ static_cast<char>(cyfra + '0'), nullptr, ostatni };
    if (wynik == nullptr) {
        wynik = nowy;
    }
    else {
        ostatni->next = nowy;
    }
    ostatni = nowy;
}

// Funkcja rekurencyjna wykonująca właściwe dodawanie
void dodajRekurencyjnie(lista* A, lista* B, int przeniesienie, lista*& wynik, lista*& ostatni) {
    // Warunek zakończenia rekurencji
    if (A == nullptr && B == nullptr && przeniesienie == 0) {
        return;
    }

    // Pobierz cyfry (lub 0 jeśli koniec listy)
    int cyfraA = (A != nullptr) ? (A->wartosc - '0') : 0;
    int cyfraB = (B != nullptr) ? (B->wartosc - '0') : 0;

    // Oblicz sumę i nowe przeniesienie
    int suma = cyfraA + cyfraB + przeniesienie;
    int nowePrzeniesienie = suma / 10;
    int cyfraWyniku = suma % 10;

    // Dodaj cyfrę do wyniku
    dodajCyfreDoWyniku(cyfraWyniku, wynik, ostatni);

    // Rekurencyjne wywołanie dla następnych cyfr
    lista* nextA = (A != nullptr) ? A->next : nullptr;
    lista* nextB = (B != nullptr) ? B->next : nullptr;
    dodajRekurencyjnie(nextA, nextB, nowePrzeniesienie, wynik, ostatni);
}

void wykonajOperacjePlusDlaDodatnich(lista* A, lista* B, lista*& wynik) {
    wynik = nullptr; // Inicjalizacja wyniku jako pustej listy
    lista* ostatni = nullptr;
    dodajRekurencyjnie(A, B, 0, wynik, ostatni);
}
void dodajCyfreDoWynikuOdejmowania(int roznica, lista*& wynik, lista*& ostatni) {
    char znak;
    if (roznica == 0) {
        znak = static_cast<char>(roznica);
    }
    else {
        znak = static_cast<char>(roznica + '0');
    }
    lista* nowy = new lista{ znak, nullptr, ostatni };
    if (wynik == nullptr) {
        wynik = nowy;
    }
    else {
        ostatni->next = nowy;
    }
    ostatni = nowy;
}

void odejmijRekurencyjnie(lista* A, lista* B, int przeniesienie, lista*& wynik, lista*& ostatni) {
    // Warunek zakończenia:
    // 1. Gdy obie listy są puste i nie ma przeniesienia
    if (A == nullptr && B == nullptr) {
        if (przeniesienie != 0) {
            // Dodaj przeniesienie jako nową cyfrę (np. gdy wynik jest ujemny)
            dodajCyfreDoWynikuOdejmowania(przeniesienie, wynik, ostatni);
        }
        return;
    }

    // Pobieramy cyfry z A i B, lub 0 jeśli nullptr
    int cyfraA = (A != nullptr) ? (A->wartosc - '0') : 0;
    int cyfraB = (B != nullptr) ? (B->wartosc - '0') : 0;

    // Odejmowanie z uwzględnieniem przeniesienia
    int roznica = cyfraA - cyfraB - przeniesienie;
    int nowePrzeniesienie = 0;

    if (roznica < 0) {
        roznica += 10;
        nowePrzeniesienie = 1;
    }

    // Dodaj cyfrę do wyniku
    dodajCyfreDoWynikuOdejmowania(roznica, wynik, ostatni);

    // Rekurencyjne wywołanie dla następnych cyfr
    lista* nextA = (A != nullptr) ? A->next : nullptr;
    lista* nextB = (B != nullptr) ? B->next : nullptr;
    odejmijRekurencyjnie(nextA, nextB, nowePrzeniesienie, wynik, ostatni);
}


void wykonajOperacjeOdejmowanie(lista* A, lista* B, lista*& wynik) {
    wynik = nullptr;
    lista* ostatni = nullptr;
    odejmijRekurencyjnie(A, B, 0, wynik, ostatni);
}
void wykonajOperacjePlus(stos_element*& stos) {
    if (stos == nullptr || stos->next == nullptr) return;
    lista* A = stos->lista;
    lista* B = stos->next->lista;
    stos_element* elemA = stos;
    stos_element* elemB = stos->next;
    stos = stos->next->next;
    lista* wynik = new lista{ '0', nullptr, nullptr };

    if (tylkoZeraIMinusy(A) && tylkoZeraIMinusy(B)) {
        wynik->wartosc = '0';
    }
    else if (czyUjemna(A) == false && czyUjemna(B) == false) {
        wykonajOperacjePlusDlaDodatnich(A, B, wynik);
    }
    else if (czyUjemna(A) == true && czyUjemna(B) == true)
    {
        sprawdzOstatniMinusRekurencyjnie(A, A);
        sprawdzOstatniMinusRekurencyjnie(B, B);
        wykonajOperacjePlusDlaDodatnich(A, B, wynik);
        sprawdzOstatniMinusRekurencyjnie(wynik, wynik);
    }
    else if (czyUjemna(A) == true && czyUjemna(B) == false)
    {
        sprawdzOstatniMinusRekurencyjnie(A, A);
        if (ifyDlaMniejszeNiz(A, B))
        {
            wykonajOperacjeOdejmowanie(A, B, wynik);
            sprawdzOstatniMinusRekurencyjnie(wynik, wynik);
        }
        else if (ifyDlaMniejszeNiz(B, A))
        {
            wykonajOperacjeOdejmowanie(B, A, wynik);
            pominZeraZPrzodu(wynik);

        }
    }
    else if (czyUjemna(A) == false && czyUjemna(B) == true)
    {
        sprawdzOstatniMinusRekurencyjnie(B, B);
        if (ifyDlaMniejszeNiz(A, B))
        {
            wykonajOperacjeOdejmowanie(A, B, wynik);
            pominZeraZPrzodu(wynik);
        }
        else if (ifyDlaMniejszeNiz(B, A))
        {
            wykonajOperacjeOdejmowanie(B, A, wynik);
            sprawdzOstatniMinusRekurencyjnie(wynik, wynik);
        }

    }

    zwolnijListe(A);
    delete elemA;
    zwolnijListe(B);
    delete elemB;

    stos_element* nowyElement = new stos_element{ wynik, stos };
    stos = nowyElement;
}

void obliczDlugoscRekurencyjnie(const char* str, int& dlugosc) {
    if (str[dlugosc] == '\0') return;
    dlugosc++;
    obliczDlugoscRekurencyjnie(str, dlugosc);
}

void wczytajDaneRekurencyjnie(char* buffer, int& dlugosc) {
    char znak;
    if (!cin.get(znak)) return;
    if (znak == '\n') return;

    buffer[dlugosc++] = znak;
    wczytajDaneRekurencyjnie(buffer, dlugosc);
}


void wyczyscStosRekurencyjnie(stos_element*& stos) {
    if (stos == nullptr) return;
    wykonajOperacjeUsunZeStosu(stos);
    wyczyscStosRekurencyjnie(stos);
}


int main() {
    const int MAX = 20000;
    char program[MAX];
    char wejscie[MAX];
    int dlugosc_programu = 0;
    int dlugosc_wejscia = 0;
    cin >> program >> wejscie;

    
    obliczDlugoscRekurencyjnie(program, dlugosc_programu);
    obliczDlugoscRekurencyjnie(wejscie, dlugosc_wejscia);

    int ip = 0; 
    int input_pos = 0;
    int instruckcja = 0;
    stos_element* stos = nullptr;

    
    wczytajDaneRekurencyjnie(program, dlugosc_programu);

    
    wczytajDaneRekurencyjnie(wejscie, dlugosc_wejscia);

    while (ip < dlugosc_programu) {
        char instr = program[ip];
        /*cout << "===STOS===" << endl;
        cout << "Numer instrukcji: " << instruckcja << endl;
        cout << "Numer wskaznika instukcji: " << ip << endl;
        cout << "Operacja: " << program[ip] << endl;
        cout << "Koda ASCII operacji: " << static_cast<int>(program[ip]) << endl;
        wypiszStos(stos, 0);*/
        switch (instr) {
        case '\'': {
            stos_element* nowy = new stos_element{ nullptr, stos };
            stos = nowy;
            break;
        }
        case ',': {
            wykonajOperacjeUsunZeStosu(stos);
            break;
        }
        case ':': {
            wykonajOperacjeDwukropek(stos);
            break;
        }
        case ';': {
            wykonajOperacjeZamienMiejscami(stos);
            break;
        }
        case '@': {
            wykonajOperacjeAt(stos);
            break;
        }
        case '&': {
            wypiszStos(stos, 0);
            break;
        }
        case '.': {
            if (input_pos < dlugosc_wejscia) {
                dodajNaPoczatekListy(stos, wejscie[input_pos]);
                input_pos++;
            }
            break;
        }
        case '>': {
            wykonajOperacjeWypiszIZdejmij(stos);
            break;
        }
        case '-': {
            wykonajOperacjeMinus(stos);
            break;
        }
        case '^': {
            wykonajOperacjeDaszek(stos);
            break;
        }
        case '[': {
            wykonajOperacjeKodASCII(stos);
            break;
        }
        case ']': {
            wykonajOperacjeZnakASCII(stos);
            break;
        }
        case '$': {
            wykonajOperacjeDolar(stos);
            break;
        }
        case '#': {
            wykonajOperacjeHasz(stos);
            break;
        }
        case'<': {
            wykonajOperacjeMniejszeNiz(stos);
            break;
        }
        case '=': {
            wykonajOperacjeRowne(stos);
            break;
        }
        case '!': {
            wykonajOperacjeNegacjaLogiczna(stos);
            break;
        }
        case '~': {
            wykonajOperacjeTylda(stos, ip);
            break;
        }
        case '?': {
            wykonajOperacjeSkokWarunkowy(stos, ip, dlugosc_programu);
            break;
        }
        case '+': {
            wykonajOperacjePlus(stos);
            break;
        }
        default: {
            if (stos != nullptr) {
                lista* nowy_element = new lista{ instr, stos->lista, nullptr };
                if (stos->lista != nullptr) {
                    stos->lista->prev = nowy_element;
                }
                stos->lista = nowy_element;
            }
            break;
        }
        }
        instruckcja++;
        ip++;
    }

  
    wyczyscStosRekurencyjnie(stos);

    return 0;
}