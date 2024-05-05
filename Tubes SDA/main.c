
#include <stdio.h>
#include <conio.h>
#include <windows.h>

#include "royalTree.h"

#define CREATOR "Created by Adinda Fauzia Puspita, Muhammad Fikri Nursyabani & Welsya Almaira Indra"

int main(){
	struct nkTree pTree;
	Create_Tree(&pTree);
	nkAddr node, king;
	int year;
	char choose;
	char str[]="";
	char searchName[20];
	unsigned char choice;
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
	printFromFile("ilustrasi/king.txt");
	printf("\n");
	InsertKing(&pTree);
	do{
		system("CLS");
		king = pTree.root;
		if((!isEmpty) && (king->info.age >= 80)){
            deleteNode(&king, &pTree);
		}
		system("CLS");
		printf("\t   Keadaan Pohon Keluarga\n");
		printf("\t   ~~~~~~~~~~~~~~~~~~~~~~\n\n");
		if (isEmpty(pTree)){
			printf("\tTree kosong");
		}
		else{
//           printTree(pTree.root,str);
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
		printf(" []   [3] Hapus/turunkan takhta Raja sekarang        []\n");
		gotoxy(60, 12);
		printf(" []   [4] Matikan/hapus anggota keluarga kerajaan    []\n");
		gotoxy(60, 13);
		printf(" []   [5] Tampilkan informasi keluarga kerajaan      []\n");
		gotoxy(60, 14);
		printf(" []   [6] Tampilkan penerus takhta                   []\n");
		gotoxy(60, 15);
		printf(" []   [0] Exit                                       []\n");
		gotoxy(60, 16);
	    printf(" []                                                  []\n");
	    gotoxy(60, 17);
        printf("  ***************************************************\n");
        gotoxy(74, 18);
		printf("\tPilihan %c ", 175); fflush(stdin); scanf("%d", &choose);
		system("cls");
		switch(choose){
			case 1:
				system("cls");
				printFromFile("ilustrasi/throne.txt");
				InputMember(&pTree);
				break;
			case 2:
				system("cls");
				printFromFile("ilustrasi/Queen.txt");
				InsertVPartner(&pTree);
				break;
			case 3:
				system("cls");
				DescendThrone(&pTree);
				break;
			case 4:
				system("cls");
				DeleteNode(pTree.root);
				break;
			case 5:
				printFromFile("ilustrasi/castle.txt");
				printf("\n\t%c Nama: ", 175);
            	scanf(" %[^\n]", &searchName);
            	node = Search(pTree.root, searchName);
            	if(node!=NULL){
            		printNodeInfo(node, searchName);
            		printf("\n\tTekan tombol apapun untuk kembali");
					getch();
				}else{
					printf("\n\t[x] %s tidak terdapat pada pohon keluarga");
					printf("\n\tTekan tombol apapun untuk kembali");
					getch();
				}
				break;
			case 6:
				system("cls");
				nextKing(pTree.root);
				break;	
			case 0:
    			system("CLS");
    			gotoxy(44, 8); printf("Sukses Keluar dari Program\n\n");
    			system("PAUSE");
    			exit(1);
    			break;
    		default:
				printf("\n\tInput tidak valid");
				printf("\n\tTekan tombol apapun untuk kembali");
				getch();
				break;
		}
	}while(choose!=0);
		exit(1);	
		
return 0;
}


