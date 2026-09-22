#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ADI 50
#define MAX_TEL 15
#define MAX_ADRES 100
#define MAX_TARIH 20

// Yemek, Sipari� ve Gelir Yap�lar�
struct Yemek {
    int yemekID;
    char yemekAdi[MAX_ADI];
    float fiyat;
    int hazirlamaSure;
    struct Yemek *sonraki;
};

struct Siparis {
    int siparisID;
    int yemekID;
    float fiyat;
    char musteriAdi[MAX_ADI];
    char musteriTel[MAX_TEL];
    char musteriAdres[MAX_ADRES];
    struct Siparis *sonraki;
};

struct Gelir {
    int siparisID;
    float odemeTutar;
    char siparisTarih[MAX_TARIH];
    struct Gelir *sonraki;
};

// Men�, Sipari� ve Gelirlerin ba�lang��lar�
struct Yemek *menuBas = NULL;
struct Siparis *siparisBas = NULL;
struct Gelir *gelirBas = NULL;

void menuDosyayaYaz(void);
void siparisDosyayaYaz(void);
void gelirDosyayaYaz(void);

// Men�ye yemek ekleme fonksiyonu
void yemekEkle() {
    struct Yemek *yeniYemek = (struct Yemek *)malloc(sizeof(struct Yemek));
    if (yeniYemek == NULL) { printf("Bellek ayirma hatasi.\n"); return; }
    printf("Yemek ID: ");
    scanf("%d", &yeniYemek->yemekID);
    printf("Yemek Adi: ");
    scanf("%49s", yeniYemek->yemekAdi);
    printf("Fiyat: ");
    scanf("%f", &yeniYemek->fiyat);
    printf("Hazirlama Sure (dakika): ");
    scanf("%d", &yeniYemek->hazirlamaSure);

    yeniYemek->sonraki = menuBas;
    menuBas = yeniYemek;
    menuDosyayaYaz();
    printf("Yemek menuye eklendi.\n");
}

// Men�den yemek silme fonksiyonu
void yemekSil() {
    int yemekID;
    printf("Silmek istediginiz yemek ID'si: ");
    scanf("%d", &yemekID);

    struct Yemek *gecici = menuBas, *onceki = NULL;
    while (gecici != NULL) {
        if (gecici->yemekID == yemekID) {
            if (onceki == NULL) {
                menuBas = gecici->sonraki;
            } else {
                onceki->sonraki = gecici->sonraki;
            }
            free(gecici);
            printf("Yemek silindi.\n");
            return;
        }
        onceki = gecici;
        gecici = gecici->sonraki;
    }
    printf("Yemek bulunamadi.\n");
}

// Men�deki yemekleri listeleme fonksiyonu
void yemekListele() {
    struct Yemek *gecici = menuBas;
    if (gecici == NULL) {
        printf("Menude yemek bulunmuyor.\n");
        return;
    }
    while (gecici != NULL) {
        printf("Yemek ID: %d, Adi: %s, Fiyat: %.2f, Hazirlama Sure: %d dakika\n",
               gecici->yemekID, gecici->yemekAdi, gecici->fiyat, gecici->hazirlamaSure);
        gecici = gecici->sonraki;
    }
}

// Sipari� ekleme fonksiyonu
void siparisEkle() {
    struct Siparis *yeniSiparis = (struct Siparis *)malloc(sizeof(struct Siparis));
    if (yeniSiparis == NULL) { printf("Bellek ayirma hatasi.\n"); return; }
    printf("Siparis ID: ");
    scanf("%d", &yeniSiparis->siparisID);
    printf("Yemek ID: ");
    scanf("%d", &yeniSiparis->yemekID);
    printf("Musteri Adi: ");
    scanf("%49s", yeniSiparis->musteriAdi);
    printf("Musteri Telefonu: ");
    scanf("%14s", yeniSiparis->musteriTel);
    printf("Musteri Adresi: ");
    scanf("%99s", yeniSiparis->musteriAdres);

   struct Yemek *fiyatAl = menuBas;
   if (fiyatAl == NULL) {
       printf("Menude yemek bulunmuyor.\n");
       return;
   }
   if (fiyatAl == NULL) { free(yeniSiparis); printf("Yemek ID bulunamadi.\n"); return; }
   while (fiyatAl != NULL) {
		if (fiyatAl->yemekID == yeniSiparis->yemekID){
			yeniSiparis->fiyat = fiyatAl->fiyat;
   			break;
		}
        fiyatAl = fiyatAl->sonraki;
   }
    
    yeniSiparis->sonraki = siparisBas;
    siparisBas = yeniSiparis;
    siparisDosyayaYaz();
    printf("Siparis alindi ve kuyruga eklendi.\n");
}

// Sipari�lerin haz�rlanmas� ve teslim edilmesi
void siparisTeslimEt() {
    if (siparisBas == NULL) {
        printf("Teslim edilecek siparis yok.\n");
        return;
    }
    struct Siparis *siparis = siparisBas;
    siparisBas = siparisBas->sonraki;

    printf("Siparis ID: %d teslim ediliyor.\n", siparis->siparisID);

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
	char tarih[11];
    sprintf(tarih, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    
    struct Gelir *yeniGelir = (struct Gelir *)malloc(sizeof(struct Gelir));
    yeniGelir->siparisID = siparis->siparisID;
    yeniGelir->odemeTutar = siparis->fiyat; // �deme tutar�n� burada varsay�lan bir de�erle al�yoruz.
    strcpy(yeniGelir->siparisTarih, tarih);
    yeniGelir->sonraki = gelirBas;
    gelirBas = yeniGelir;
}

// Gelir raporu g�sterme fonksiyonu
void gelirRaporu() {
    struct Gelir *gecici = gelirBas;
    float toplamGelir = 0.0;
    while (gecici != NULL) {
        toplamGelir += gecici->odemeTutar;
        gecici = gecici->sonraki;
    }
    printf("Toplam Gelir: %.2f\n", toplamGelir);
    gelirDosyayaYaz();
}

// Men� bilgilerini dosyadan oku fonksiyonu
void menuDosyadanOku() {
    FILE *dosya = fopen("data/menu.txt", "r");
    if (dosya == NULL) {
        printf("Menu dosyasi acilamadi.\n");
        return;
    }

    struct Yemek *gecici, *son = NULL;
    while (!feof(dosya)) {
        gecici = (struct Yemek *)malloc(sizeof(struct Yemek));
        if (fscanf(dosya, "%d %s %f %d", &gecici->yemekID, gecici->yemekAdi, &gecici->fiyat, &gecici->hazirlamaSure) != 4) {
            free(gecici);
            break;
        }
        gecici->sonraki = NULL;

        if (menuBas == NULL) {
            menuBas = gecici;
            son = gecici;
        } else {
            son->sonraki = gecici;
            son = gecici;
        }
    }
    fclose(dosya);
    printf("Menu dosyadan okundu.\n");
}

// Men� bilgilerini dosyaya yazma fonksiyonu
void menuDosyayaYaz() {
    FILE *dosya = fopen("data/menu.txt", "w");
    if (dosya == NULL) { printf("Menu dosyasi acilamadi.\n"); return; }
    //if (dosya == NULL) {
    //    printf("Menu dosyasi acilamadi.\n");
    //    return;
    //}
    struct Yemek *gecici = menuBas;
    while (gecici != NULL) {
        fprintf(dosya, "%d %s %.2f %d\n", gecici->yemekID, gecici->yemekAdi, gecici->fiyat, gecici->hazirlamaSure);
        gecici = gecici->sonraki;
    }
    fclose(dosya);
    printf("Menu dosyaya yazildi.\n");
}

// Sipari� bilgilerini dosyadan okuma fonksiyonu
void siparisDosyadanOku() {
    FILE *dosya = fopen("data/siparisler.txt", "r");
    if (dosya == NULL) {
        printf("Siparis dosyasi acilamadi.\n");
        return;
    }

    struct Siparis *gecici, *son = NULL;
    while (!feof(dosya)) {
        gecici = (struct Siparis *)malloc(sizeof(struct Siparis));
        if (fscanf(dosya, "%d %d %f %s %s %s", &gecici->siparisID, &gecici->yemekID, &gecici->fiyat, gecici->musteriAdi, gecici->musteriTel, gecici->musteriAdres) != 6) {
            free(gecici);
            break;
        }
        gecici->sonraki = NULL;

        if (siparisBas == NULL) {
            siparisBas = gecici;
            son = gecici;
        } else {
            son->sonraki = gecici;
            son = gecici;
        }
    }
    fclose(dosya);
    printf("Siparisler dosyadan okundu.\n");
}

// Sipari� bilgilerini dosyaya yazma fonksiyonu
void siparisDosyayaYaz() {
    FILE *dosya = fopen("data/siparisler.txt", "w");
    if (dosya == NULL) { printf("Siparis dosyasi acilamadi.\n"); return; }
    //if (dosya == NULL) {
    //    printf("Siparis dosyasi acilamadi.\n");
    //    return;
    //}
    struct Siparis *gecici = siparisBas;
    while (gecici != NULL) {
        fprintf(dosya, "%d %d %.2f %s %s %s\n", gecici->siparisID, gecici->yemekID, gecici->fiyat, gecici->musteriAdi, gecici->musteriTel, gecici->musteriAdres);
		gecici = gecici->sonraki;
    }

    fclose(dosya);
    printf("Siparisler dosyaya yazildi.\n");
}


// Gelir bilgilerini dosyadan okuma fonksiyonu
void gelirDosyadanOku() {
    FILE *dosya = fopen("data/gelirler.txt", "r");
    if (dosya == NULL) {
        printf("Gelir dosyasi acilamadi.\n");
        return;
    }

    struct Gelir *gecici, *son = NULL;
    while (!feof(dosya)) {
        gecici = (struct Gelir *)malloc(sizeof(struct Gelir));
        if (fscanf(dosya, "%d %f %s", &gecici->siparisID, &gecici->odemeTutar, gecici->siparisTarih) != 3) {
            free(gecici);
            break;
        }
        gecici->sonraki = NULL;

        if (gelirBas == NULL) {
            gelirBas = gecici;
            son = gecici;
        } else {
            son->sonraki = gecici;
            son = gecici;
        }
    }
    fclose(dosya);
    printf("Gelirler dosyadan okundu.\n");
}

// Gelir bilgilerini dosyaya yazma fonksiyonu
void gelirDosyayaYaz() {
    FILE *dosya = fopen("data/gelirler.txt", "w");
    if (dosya == NULL) { printf("Gelir dosyasi acilamadi.\n"); return; }
    //if (dosya == NULL) {
    //    printf("Gelir dosyasi acilamadi.\n");
    //    return;
    //}
    struct Gelir *gecici = gelirBas;
    while (gecici != NULL) {
        fprintf(dosya, "%d %.2f %s\n", gecici->siparisID, gecici->odemeTutar, gecici->siparisTarih);
        gecici = gecici->sonraki;
    }
    fclose(dosya);
    printf("Gelirler dosyaya yazildi.\n");
}


// Program�n ana fonksiyonu
int main() {
	menuDosyadanOku();
	siparisDosyadanOku();
	gelirDosyadanOku();
	
    int secim;
    //dosyadanYukle();

    do {
        printf("\nYemek Siparis ve Dagitim Sistemi\n");
        printf("1. Yemek Ekle\n");
        printf("2. Yemek Sil\n");
        printf("3. Menu Listele\n");
        printf("4. Siparis Ekle\n");
        printf("5. Siparis Teslim Et\n");
        printf("6. Gelir Raporu\n");
        printf("7. Cikis\n\n");
        printf("Seciminizi yapin: ");
        scanf("%d", &secim);

        switch (secim) {
            case 1: yemekEkle(); break;
            case 2: yemekSil(); break;
            case 3: yemekListele(); break;
            case 4: siparisEkle(); break;
            case 5: siparisTeslimEt(); break;
            case 6: gelirRaporu(); break;
            case 7: printf("Cikis yapiliyor...\n"); break;
            default: printf("Gecersiz secim.\n"); break;
        }
    } while (secim != 7);

    return 0;
}

