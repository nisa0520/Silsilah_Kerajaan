#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include "Tree.h"

#define CREATOR "Created by Adinda Fauzia Puspita, Muhammad Fikri Nursyabani & Welsya Almaira Indra"

int main() {
    struct nkTree pTree;
    Create_Tree(&pTree);  // Initialize the tree
    nkAddr node, king;
    int year;
    char choose;
    char str[] = "";
    char searchName[20];
    unsigned char choice;

    //printf("Loading tree from file...\n");
    nkAddr loadedRoot = loadTreeFromFile("Silsilah.txt");  // Declare loadedRoot here

    loading_screen();
    system("cls");
    printFromFile("ilustrasi/opening.txt");
    printf("\n\n\t\t");
    printf(CREATOR);
    printf("\n\n\t\t");
    system("Pause");
    system("cls");
    printFromFile("ilustrasi/AturanPenurunanTahta.txt");
    printf("\n\n\t\t");
    system("Pause");
    system("cls");
    printFromFile("ilustrasi/AturanPenambahanAnggota.txt");
    printf("\n\n\t\t");
    system("Pause");
    system("cls");
    
    if (loadedRoot == NULL) {
    	printFromFile("ilustrasi/king.txt");
    	printf("\n");
        InsertKing(&pTree);
    } else {
        pTree.root = loadedRoot;
    }

    do {
        system("CLS");
        king = pTree.root;
        if ((!isEmpty) && (king->info.age >= 80)) {
            deleteNode(&king, &pTree);
        }
        system("CLS");
        printf("\t   Keadaan Pohon Keluarga\n");
        printf("\t   ~~~~~~~~~~~~~~~~~~~~~~\n\n");
        if (isEmpty(pTree)) {
            printf("\tTree kosong");
        } else {
            levelOrderTraversal(pTree.root);
        }
        gotoxy(60, 7);
        printf("    ************************************************\n");
        gotoxy(60, 8);
        printf(" []                                                  []\n");
        gotoxy(60, 9);
        printf(" []   [1] Tambah anggota keluarga kerajaan           []\n");
        gotoxy(60, 10);
        printf(" []   [2] Tambah pasangan                            []\n");
        gotoxy(60, 11);
        printf(" []   [3] Turunkan takhta Raja sekarang              []\n");
        gotoxy(60, 12);
        printf(" []   [4] Matikan/hapus anggota keluarga kerajaan    []\n");
        gotoxy(60, 13);
        printf(" []   [5] Tampilkan informasi keluarga kerajaan      []\n");
        gotoxy(60, 14);
        printf(" []   [6] Tampilkan penerus takhta                   []\n");
        gotoxy(60, 15);
        printf(" []   [7] Print info tree                            []\n");
        gotoxy(60, 16);
        printf(" []   [8] Save                                       []\n");
        gotoxy(60, 17);
        printf(" []   [0] Exit                                       []\n");
        gotoxy(60, 18);
        printf(" []                                                  []\n");
        gotoxy(60, 19);
        printf("  ***************************************************\n");
        gotoxy(74, 20);
        printf("\tPilihan %c ", 175); fflush(stdin); scanf("%d", &choose);
        system("cls");
        switch (choose) {
            case 1:
                system("cls");
                printFromFile("ilustrasi/throne.txt");
                InputMember(&pTree);
                printf("Anggota keluarga kerajaan berhasil ditambahkan.\n");
                break;
            case 2:
                system("cls");
                printFromFile("ilustrasi/Queen.txt");
                InsertVPartner(&pTree);
                printf("Pasangan berhasil ditambahkan.\n");
                break;
            case 3:
                system("cls");
                DescendThrone(&pTree);
                break;
            case 4:
                system("cls");
                DeleteNode(&pTree, pTree.root);
                printf("Anggota keluarga kerajaan berhasil dihapus.\n");
                break;
            case 5:
                printFromFile("ilustrasi/castle.txt");
                printf("\n\t%c Nama: ", 175);
                scanf(" %[^\n]", searchName);
                node = Search(pTree.root, searchName);
                if (node != NULL) {
                    printNodeInfo(node, searchName);
                } else {
                    printf("\n\t[x] %s tidak terdapat pada pohon keluarga.\n", searchName);
                }
                printf("\n\tTekan tombol apapun untuk kembali");
                getch();
                break;
            case 6:
                system("cls");
                nextKing(pTree.root);
                break;
            case 7:
                system("cls");
                printf("\n\t== Informasi Keseluruhan Pohon Keluarga ==\n\n");
                printTree(pTree.root);
                getch();
                break;
            case 8:
                system("cls");
                saveTreeToFile(pTree.root, "Silsilah.txt");
                break;
            case 0:
                system("cls");
                gotoxy(44, 8);
                printf("Sukses Keluar dari Program\n\n");
                system("PAUSE");
                exit(1);
                break;
            default:
                printf("\n\tInput tidak valid");
                printf("\n\tTekan tombol apapun untuk kembali");
                getch();
                break;
        }
    } while (choose != 0);
    exit(1);

    return 0;
}

