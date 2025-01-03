/*MBM103  Akýllý Buzdolabý Dönem Projesi*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_NAME_LENGTH 100
#define MAX_DATE_LENGTH 11
#define MAX_RECIPE_LENGHT 300
#define MAX_INGREDIENTS 50

// Malzeme bilgilerini bellekte tutar
typedef struct {
    char malzemeAdi[MAX_NAME_LENGTH];    // Malzeme adi
    int adet;   // Adet
    char son_kullanma_tarihi[MAX_DATE_LENGTH]; // Son kullanma tarihi 
} Item;


// Fonksiyonlar
void malzemeEkle(Item malzemeler[], int *malzemeSayisi);   // Malzeme ekleyen fonksiyon
void malzemeCikar(Item malzemeler[], int *malzemeSayisi);  // Malzeme cikaran fonksiyon
void malzemeleriListele(Item malzemeler[], int malzemeSayisi);  // Malzemeleri listeleyen fonksiyon
int malzemeBul(Item malzemeler[], int malzemeSayisi, const char *malzemeAdi);  // Malzemeleri bulan fonksiyon
void malzemeleriSirala(Item malzemeler[], int malzemeSayisi);  // Malzemeleri siralayan fonksiyon
int tarihiGecmismi(const char *tarih); // Tarih kontrolu yapan fonksiyon
void tarihleriKontrolEt(Item malzemeler[], int malzemeSayisi); // Son kullanma tarihlerini kontrol eden fonksiyon
void tarifOneriSistemi(Item malzemeler[], int malzemeSayisi); // Tarif oneri sistemi

// Tarif yapisini veren fonksiyon
typedef struct {
    char tarifAdi[MAX_NAME_LENGTH];
    char malzemeler[MAX_INGREDIENTS][MAX_NAME_LENGTH];
    int malzemeSayisi;
} Recipe;

// Tarifler
Recipe tarifler[] = {
    {"Menemen", {"yumurta", "domates", "tuz"}, 3},
    {"Omlet", {"yumurta", "sut", "tuz"}, 3},
    {"Makarna", {"makarna", "domates", "tuz"}, 3},
    {"Muhallebi", {"sut", "seker", "un","nisasta" }, 4},
    {"Corba", {"salca", "tereyagi", "sehriye", "su", "tuz"}, 5},
    {"Kofte", {"kiyma", "tuz", "sogan"}, 3}
};
const int tarifSayisi = 6;

// Malzeme ekleyen fonksiyon
void malzemeEkle(Item malzemeler[], int *malzemeSayisi) {
    char malzemeAdi[MAX_NAME_LENGTH], son_kullanma_tarihi[MAX_DATE_LENGTH];
    int adet;
    
    printf("Malzeme adi: ");
    scanf("%s", malzemeAdi);
    printf("Adet: ");
    scanf("%d", &adet);
    printf("Son kullanma tarihi (GG/AA/YYYY): ");
    scanf("%s", son_kullanma_tarihi);

    // Malzemenin listede olup olmadigini kontrol eder
    int indeks = malzemeBul(malzemeler, *malzemeSayisi, malzemeAdi);
    
    if (indeks != -1) {
        // Malzeme var ise miktarini artirir
        malzemeler[indeks].adet += adet;
        // Son kullanma tarihini günceller
        strcpy(malzemeler[indeks].son_kullanma_tarihi, son_kullanma_tarihi);
    } else {
        // Malzeme yok ise yenisini ekler
        strcpy(malzemeler[*malzemeSayisi].malzemeAdi, malzemeAdi);
        malzemeler[*malzemeSayisi].adet = adet;
        strcpy(malzemeler[*malzemeSayisi].son_kullanma_tarihi, son_kullanma_tarihi);
        (*malzemeSayisi)++;
    }

    // Listeyi sýralar
    malzemeleriSirala(malzemeler, *malzemeSayisi);
}

// Malzeme cikaran fonksiyon
void malzemeCikar(Item malzemeler[], int *malzemeSayisi) {
    char malzemeAdi[MAX_NAME_LENGTH];
    int adet;

    printf("Cikarilacak malzeme adi: ");
    scanf("%s", malzemeAdi);
    printf("Cikarilacak adet: ");
    scanf("%d", &adet);

    int indeks = malzemeBul(malzemeler, *malzemeSayisi, malzemeAdi);
    
    if (indeks == -1) {
        printf("Malzeme bulunamadi!\n");
        return;
    }

    if (malzemeler[indeks].adet >= adet) {
        malzemeler[indeks].adet -= adet;
        if (malzemeler[indeks].adet == 0) {
            // Malzeme miktari sifirsa siler
            int i;
            for ( i = indeks; i < (*malzemeSayisi) - 1; i++) {
                malzemeler[i] = malzemeler[i + 1];
            }
            (*malzemeSayisi)--;
        }
    } else {
        printf("Yeterli miktarda malzeme yok!\n");
    }
}

// Malzeme listeleyen fonksiyon
void malzemeleriListele(Item malzemeler[], int malzemeSayisi) {
    if (malzemeSayisi == 0) {
        printf("Malzeme listede yok!\n");
        return;
    }
    
    printf("Malzeme Adi     Adet   Son Kullanma Tarihi\n");
    int i;
    for ( i = 0; i < malzemeSayisi; i++) {
        printf("%-15s %-6d %s\n", malzemeler[i].malzemeAdi, malzemeler[i].adet, malzemeler[i].son_kullanma_tarihi);
    }
}

// Malzeme arayan fonksiyon
int malzemeBul(Item malzemeler[], int malzemeSayisi, const char *malzemeAdi) {
	int i;
    for ( i = 0; i < malzemeSayisi; i++) {
        if (strcmp(malzemeler[i].malzemeAdi, malzemeAdi) == 0) {
            return i;  // Malzeme bulundu
        }
    }
    return -1;  // Malzeme bulunamadi
}

// Listeyi siralayan fonksiyon
void malzemeleriSirala(Item malzemeler[], int malzemeSayisi) {
	int i, j;
    for ( i = 0; i < malzemeSayisi - 1; i++) {
        for ( j = i + 1; j < malzemeSayisi; j++) {
            if (strcmp(malzemeler[i].malzemeAdi, malzemeler[j].malzemeAdi) > 0) {
                // Ýki malzemenin yerlerini degistirir
                Item temp = malzemeler[i];
                malzemeler[i] = malzemeler[j];
                malzemeler[j] = temp;
            }
        }
    }
}

// Tarih kontrolü yapan fonksiyon
int tarihiGecmismi(const char *tarih) {
    time_t simdi;
    struct tm son_kullanma={0};
    int gun, ay, yil;
    
    time(&simdi);
    sscanf(tarih, "%d/%d/%d", &gun, &ay, &yil);
    
    son_kullanma.tm_year = yil - 1900;
    son_kullanma.tm_mon = ay - 1;
    son_kullanma.tm_mday = gun;
    
    time_t son_kullanma_zamani = mktime(&son_kullanma);
    
    return (simdi > son_kullanma_zamani);
}

// Son kullanma tarihlerini kontrol eden fonksiyon
void tarihleriKontrolEt(Item malzemeler[], int malzemeSayisi) {
    int bulundu = 0;
    printf("\nTarihi Gecmis Malzemeler:\n");
    int i;
    for ( i = 0; i < malzemeSayisi; i++) {
        if (tarihiGecmismi(malzemeler[i].son_kullanma_tarihi)) {
            printf("- %s, Son Kullanma Tarihi: %s\n", 
                   malzemeler[i].malzemeAdi, 
                   malzemeler[i].son_kullanma_tarihi);
            bulundu = 1;
        }
    }
    
    if (!bulundu) {
        printf("Tarihi gecmis malzeme bulunmamaktadir.\n");
    }
}

// Tarif öneri sistemi
void tarifOneriSistemi(Item malzemeler[], int malzemeSayisi) {
    printf("\nTarif Onerileri:\n");
    int t;
    for ( t = 0; t < tarifSayisi; t++) {
        int eksikMalzeme = 0;
        int m;
        for ( m = 0; m < tarifler[t].malzemeSayisi; m++) {
            if (malzemeBul(malzemeler, malzemeSayisi, tarifler[t].malzemeler[m]) == -1) {
                eksikMalzeme = 1;
                break;
            }
        }
        
        if (!eksikMalzeme) {
            printf("- %s: Gerekli Malzemeler: ", tarifler[t].tarifAdi);
            int m;
		    for ( m = 0; m < tarifler[t].malzemeSayisi; m++) {
                printf("%s", tarifler[t].malzemeler[m]);
                if (m < tarifler[t].malzemeSayisi - 1) {
                    printf(", ");
                }
            }
            printf("\n");
        }
    }
}

// Ana fonksiyon
int main() {
    Item malzemeler[50];
    int malzemeSayisi = 0;
    int secim;

    while (1) {
        printf("\n***Akilli Buzdolabi Sistemi***\n");
        printf("1. Malzeme Yonetimi\n");
        printf("2. Son Kullanma Tarihi Kontrolu\n");
        printf("3. Tarif Onerisi\n");
        printf("4. Cikis\n");
        printf("Seciminizi yapiniz (1-4): ");
        scanf("%d", &secim);

        switch (secim) {
            case 1: {
                int altSecim;
                printf("\n***Malzeme Yonetimi***\n");
                printf("1. Malzeme Ekle\n");
                printf("2. Malzeme Cikar\n");
                printf("3. Malzeme Listesini Goruntule\n");
                printf("4. Cikis\n");
                printf("Seciminizi yapiniz (1-4): ");
                scanf("%d", &altSecim);
                
                switch (altSecim) {
                    case 1:
                        malzemeEkle(malzemeler, &malzemeSayisi);
                        break;
                    case 2:
                        malzemeCikar(malzemeler, &malzemeSayisi);
                        break;
                    case 3:
                        malzemeleriListele(malzemeler, malzemeSayisi);
                        break;
                    case 4:
                        printf("Cikis yapiliyor!\n");
                        return 0;
                    default:
                        printf("Gecersiz secenek!\n");
                }
                break;
            }
            case 2:
                tarihleriKontrolEt(malzemeler, malzemeSayisi);
                break;
            case 3:
                tarifOneriSistemi(malzemeler, malzemeSayisi);
                break;
            case 4:
                printf("Cikis yapiliyor!\n");
                return 0;
            default:
                printf("Gecersiz secenek lutfen tekrar deneyin.\n");
        }
    }

    return 0;
}
