#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include "stdbool.h"

#define MALE 1
#define FEMALE 0

/***************************/
/*      Type data          */
/***************************/

typedef char infoType[50];
typedef struct kingdomTree *nkAddr;

typedef struct {
    infoType name;
    int age;
    boolean gender;
} identity;

typedef struct kingdomTree{
    nkAddr fs;       // First Son
    nkAddr nb;       // Next Brother
    nkAddr parent;
    identity info;
    identity partner;  // Directly include partner information
    char predecessor[100];  // Name of the predecessor (previous king/queen)
} nkTreeNode;

typedef struct nkTree {
    nkAddr root;
} nkTree;

struct Queue {
    int front, rear, size;
    nkAddr *arr;
};

/***************************/
/*  S P E S I F I K A S I  */
/***************************/

/*********** PROTOTYPE ****************/
/**** Predikat untuk test keadaan Tree  ****/

boolean isEmpty(struct nkTree X);
/* Mengirimkan true jika Isi_Tree KOSONG */

/**** Konstruktor/Kreator Tree Kosong ****/

void Create_Tree(struct nkTree *X);
/* Membuat tree kosong */

/**** Manajemen Memory ****/

nkAddr CreateNode(nkAddr parent, infoType name, int age, boolean gender);
/* Alokasi node */

/**** Pencarian sebuah node Tree ****/

nkAddr Search(nkAddr root, infoType src);
/* Mencari apakah ada node dengan infoType src */
/* Jika ada, mengirimkan address node tsb */
/* Jika tidak ada, mengirimkan NULL */

/**** Penambahan Node ****/

void InsertKing(struct nkTree *pTree);
/* IS: pTree kosong */
/* FS: Meminta input identitas node dari user, mengalokasi node dan menjadikannya root tree */
/* Menambahkan node raja/ratu */

void InsertVPartner(struct nkTree *myTree);
/* Menambahkan node partner */

void InputMember();
/* IS: data subvar newNode belum diketahui */
/* FS: data subvar newNode terinput oleh user */

/**** PRIMITIF BERDASARKAN ALAMAT ****/
/**** Penambahan node berdasarkan alamat ****/

void InsertPartner(nkAddr familyMember, infoType name, int age, boolean gender);
/* IS: familyMember sembarang, partner info sudah diketahui */
/* FS: Menambahkan partner pada node familyMember */

void InsertNode(struct nkTree *treeRoot, nkAddr newNode);
/* IS: treeRoot sembarang, newNode sudah dialokasi */
/* FS: Menambahkan node ber-address newNode pada tree */

/**** Penghapusan sebuah node ****/

nkAddr FindParent(nkAddr root, nkAddr node);
/* IS: pointer nkAddr root dan pointer nkAddr node yang akan dicari induknya */
/* FS: pointer nkAddr yang menunjuk pada node induk dari node target. Jika tidak ditemukan, akan mengembalikan NULL */

nkAddr FindSuccessor(nkAddr root, nkAddr node);
/* IS: pointer nkAddr root dan pointer nkAddr node yang akan dicari penggantinya */
/* FS: pointer nkAddr yang menunjuk pada node pengganti. Jika tidak ditemukan pengganti, akan mengembalikan NULL */

void DeleteNode(struct nkTree *tree, nkAddr root);
/* IS: node member dengan info sembarang tersedia/tidak tersedia */
/* FS: apabila node member dengan info sembarang tersedia, maka akan terhapus. apabila tidak tersedia, muncul output "nama anggota tidak tersedia" */

void deleteNode(nkAddr *pDel, struct nkTree *pTree);

/**** PROSES SEMUA NODE TREE  ****/

//void printTree(nkAddr node, char tab[]);
/* Menampilkan tree pada program secara preorder */

struct Queue *initQueue(int size);

void enQueue(struct Queue *queue, nkAddr node);

nkAddr deQueue(struct Queue *queue);

void printTreeInfo(nkAddr node, int index);

// Fungsi untuk memulai pencetakan pohon
void printTree(nkAddr root);

void levelOrderTraversal(nkAddr root);

void nextKing(nkAddr root);

int GetNodeDegree(nkAddr node);
/* Mengirimkan nilai degree dari suatu node */

void printNodeInfo(nkAddr node, infoType name);
/* IS: node sembarang, name sama dengan node->info.name atau node->partner.name */
/* FS: Menampilkan nama, umur, jenis kelamin, orang tua, saudara, dan anak dari node jika atribut tersebut ada */

void gotoxy(int x, int y);
/* Memindahkan kursor ke posisi tertentu di layar */

void loading_screen();
/* Menampilkan tulisan load */

void printFromFile(const char* location);
/* Menampilkan teks pada file yang terdapat pada location */

void TimeSkip(nkAddr node, int year);
/* Rekursif untuk menambahkan node->info.age dengan year pada setiap node */

void ProceedTimeSkip(nkAddr *node, int year);
/* Process eksekusi dan passing address kepada node untuk menambahkan node->info.age dengan year */

void DeleteRoot(struct nkTree *pTree);
nkAddr FindFirstSon(nkAddr root);
void DescendThrone(struct nkTree *pTree);

int hitungAge(int hari, int bulan, int tahun);
bool isTglValid(char *tanggal, bool *isFormatMMDDYYYY);

void saveTreeToFile(nkAddr root, const char* filename);

nkAddr loadTreeFromFile(const char* filename);

#endif
