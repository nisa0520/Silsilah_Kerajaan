#include "Tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <stdbool.h>

 // Deklarasi variabel pointer memesan memori kosong dengan ukuran yang ditentukan sesuai kebutuhan
char tglInput[11]; // Alokasikan memori untuk menyimpan tanggal
int hari, bulan, tahun;
bool isFormatMMDDYYYY = true;

void Create_Tree(struct nkTree *X) { // Create Non Binary Tree
    (*X).root = NULL;
}

boolean isEmpty(struct nkTree X) {
    return (X.root == NULL);
}

nkAddr CreateNode(nkAddr parent, infoType name, int age, boolean gender) {
    nkAddr newNode = (nkAddr)malloc(sizeof(nkTreeNode));
    if (newNode != NULL) {
        newNode->fs = NULL;
        newNode->nb = NULL;
        newNode->parent = parent;
        strcpy(newNode->info.name, name);
        newNode->info.age = age;
        newNode->info.gender = gender;
        strcpy(newNode->partner.name, "");  // Initialize partner name to empty
        newNode->partner.age = 0;
        newNode->partner.gender = 0;
        strcpy(newNode->predecessor, "");  // Initialize predecessor to empty
    }
    return newNode;
}

void DeallocNode(nkAddr *Node) {
    if (Node && *Node) {
        free(*Node);
        *Node = NULL;
    }
}

nkAddr Search(nkAddr root, infoType src) {
    if (root != NULL) {
        if (strcmp(root->info.name, src) == 0) {
            return root;
        }
        if (root->partner.name[0] != '\0' && strcmp(root->partner.name, src) == 0) {
            return root;
        }
        nkAddr searchNode = Search(root->fs, src);
        if (searchNode == NULL) {
            return Search(root->nb, src);
        } else {
            return searchNode;
        }
    }
    return NULL;
}

// Fungsi untuk memvalidasi tanggal yang diinputkan, berlaku untuk tanggal Check In ataupun tanggal Check Out
bool isTglValid(char *tanggal, bool *isFormatMMDDYYYY) { 
	// Deklrasi Variabel
	int i;
	bool *valid = (bool*)malloc(sizeof(bool));
	int bln31[] = {1,3,5,7,8,10,12};
	int bln30[] = {4,6,9,11};
    char delim[] = "/";    
    
    bool hariValid = true;
    bool bulanValid = true;
    bool tahunValid = true;    
	
	bool bulan31 = false;
    bool bulan30 = false;
    bool bulanKabisat = false;
    
	char originalTanggal[100];
	strcpy(originalTanggal, tanggal); // Menyimpan nilai asli tanggal
    
    // Mendeklarasikan variabel untuk menyimpan informasi tanggal dan waktu
    time_t now;
    struct tm *local;

    // Mendapatkan waktu saat ini
    now = time(NULL);

    // Mengkonversi waktu saat ini ke waktu lokal
    local = localtime(&now);
    
    // Memeriksa apakah bulan valid
    if (bulan < 1 || bulan > 12) {
        *isFormatMMDDYYYY = false; // Menandai kemungkinan format DD/MM/YYYY
        return false;
    }
    
    // Simpan waktu saat ini ke variabel
    int tahunSkrg = local->tm_year + 1900;
    
    // Memisahkan hari bulan tahun dari parameter
    int hari = atoi(strtok(originalTanggal, delim));
    int bulan = atoi(strtok(NULL, delim));
    int tahun = atoi(strtok(NULL, delim));
    

    // Cek apakah bulan yang diinputkan memiliki 31 hari
    for (i = 0; i < sizeof(bln31) / sizeof(bln31[0]); i++) {
        if (bulan == bln31[i]) {
            bulan31 = true;
            break; // Keluar dari loop setelah menemukan kecocokan
        }
    }
    
    // Cek apakah bulan yang diinputkan memiliki 30 hari
    if(!bulan31){
    	for (i = 0; i < sizeof(bln30) / sizeof(bln30[0]); i++) {
	        if (bulan == bln30[i]) {
	            bulan30 = true;
	            break; // Keluar dari loop setelah menemukan kecocokan
	        }
	    }
	}
	
	// Cek jika bulan februari apakah dai tahun kabisat atau bukan
	else{
		if(tahun % 4 == 0 && tahun % 100 != 0){
			bulanKabisat = true;
		}
	}
	
	//mengecek apakah tanggal valid atau tidak, syaratnya tidak bisa menginput sebelum tanggal hari ini dan hanya bisa memesan maksmial satu tahun kedepan
    if(tahun < tahunSkrg || tahun > tahunSkrg+1){
    	tahunValid = false;
	}
    else if(bulan>12 || bulan <1){
    	bulanValid = false;
	}
	else {
		if(bulan31){
			if(hari > 31){ // kalo bulan 31 apakah tanggal lebih dari 31
				hariValid = false;
			}
		}
		else if(bulan30){
			if(hari > 30){
				hariValid = false;
			}
		}
		else{ // untuk bulan februari
			if(bulanKabisat){
				if(hari > 29){ // jika februari di tahun kabisat
					hariValid = false;
				}
			}
			else{
				if(hari >28){ // jika februari bukan di tahun kabisat
					hariValid = false;
				}
			}
		}
	}
	
	// Mengambil kesimpulan apakah tanggal yang diinputkan sesuai syarat atau tidak dengan menggunakan AND
	*valid = tahunValid && bulanValid && hariValid;
	return valid;
}

// Fungsi untuk menghitung umur dalam tahun
int hitungAge(int tahun, int bulan, int hari) {
    // Mendapatkan tanggal hari ini
    time_t t = time(NULL);
    struct tm today = *localtime(&t);

    int age = today.tm_year + 1900 - tahun;

    // Koreksi jika bulan lahir dan hari lahir belum dilewati tahun ini
    if ((today.tm_mon + 1 < bulan) || (today.tm_mon + 1 == bulan && today.tm_mday < hari)) {
        age -= 1;
    }

    return age;
}

void InsertKing(struct nkTree *pTree) {
    nkAddr king;
    infoType name;
    int age, temp;
    boolean gender;
    
    printf("\n\tMasukan Identitas Raja / Ratu:\n");
    do {
    printf("\n\t%c Nama: ", 175);
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0; // Menghapus newline di akhir input nama
    if (strspn(name, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ") != strlen(name)) {
            printf("\t[x] INPUT TIDAK VALID! Nama hanya boleh berisi huruf. [x]\n");
        } else {
            break;
        }
    }while (1);
    
    do {
        printf("\n\t%c Pilih jenis kelamin\n", 175);
        printf("\t  0. Wanita\n");
        printf("\t  1. Pria\n");
        printf("\t  Pilihan: ");
        scanf(" %d", &temp);
        if (temp != 0 && temp != 1) {
            printf("\t[x] INPUT TIDAK VALID ! [x]\n");
        } else {
            gender = temp;
            break;
        }
    } while (1);
    
    //getchar(); // Menghapus newline yang tersisa di buffer
    
	do {
        printf("\n\t» Masukkan tanggal lahir (MM/DD/YYYY): ");
        scanf("%s", tglInput);
        
        // Memeriksa dan memparsing input menggunakan sscanf
        if (sscanf(tglInput, "%2d/%2d/%4d", &bulan, &hari, &tahun) == 3) {
            isFormatMMDDYYYY = true;
            if (isTglValid(tglInput, &isFormatMMDDYYYY)) {
                age = hitungAge(tahun, bulan, hari);

                if (age < 50 || age >= 80) {
                    printf("\t[x] Umur tidak dalam rentang 50-79 tahun! [x]\n");
                } else {
                    break;
                }
            } else {
                if (!isFormatMMDDYYYY) {
                    printf("\n\t[x] Tanggal yang anda masukkan mungkin dalam format DD/MM/YYYY. Silakan coba lagi. [x]\n");
                } else {
                    printf("\n\t[x] Tanggal yang anda masukkan tidak valid. Silakan coba lagi. [x]\n");
                }
            }
        } else {
            printf("\n\t[x] Format tanggal yang anda masukkan tidak valid. Silakan coba lagi. [x]\n");
        }
        
    } while (age < 50 || age >= 80);

    king = CreateNode(NULL, name, age, gender);
    InsertNode(pTree, king);
    printf("\n\t[o] Raja/ratu berhasil ditambahkan [o]");
    getch();
}

void InsertVPartner(struct nkTree *pTree) {
    nkAddr srcNode;
    boolean gender;
    infoType name, partnerName;
    int age;

    printf("\n\n\tMasukan 'q' untuk kembali\n");
    printf("\tUmur minimal untuk menikah adalah 18 tahun\n");
    do {
        printf("\n\t%c Nama anggota keluarga yang akan menikah: ", 175);
//        scanf(" %[^\n]", name);
//        if (strcmp(name, "q") == 0) {
//            return;
//        }
		fflush(stdin);
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = 0; // Menghapus newline di akhir input nama
        if (strcmp(name, "q") == 0) {
            return;
        }
        if (strspn(name, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ") != strlen(name)) {
            printf("\t[x] INPUT TIDAK VALID! Nama hanya boleh berisi huruf. [x]\n");
            continue;
        }
        srcNode = Search((*pTree).root, name);
        if (srcNode == NULL) {
            printf("\t[x] Anggota keluarga tidak ditemukan [x]\n");
        } else if (srcNode->partner.name[0] != '\0') {
            printf("\t[x] Anggota keluarga tersebut sudah memiliki pasangan [x]\n");
        } else if (srcNode->info.age < 18) {
            printf("\t[x] Anggota keluarga tersebut masih dibawah umur [x]\n");
        } else {
            break;
        }
    } while (1);

    gender = srcNode->info.gender == 0 ? MALE : FEMALE;

    do {
        printf("\n\t%c Masukan nama pasangan: ", 175);
        fgets(partnerName, sizeof(partnerName), stdin);
        partnerName[strcspn(partnerName, "\n")] = 0; // Menghapus newline di akhir input nama
        if (strcmp(partnerName, "q") == 0) {
            return;
        }
        if (strspn(partnerName, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ") != strlen(partnerName)) {
            printf("\t[x] INPUT TIDAK VALID! Nama hanya boleh berisi huruf. [x]\n");
            continue;
        }
//        scanf(" %[^\n]", partnerName);
        
        if (Search((*pTree).root, partnerName) != NULL) {
            printf("\t[x] Nama orang tersebut sudah ada pada pohon keluarga [x]\n");
        } else {
            break;
        }
    } while (1);
    
	do {
        printf("\n\t» Masukkan tanggal lahir (MM/DD/YYYY): ");
        scanf("%s", tglInput);
        
        // Memeriksa dan memparsing input menggunakan sscanf
        if (sscanf(tglInput, "%2d/%2d/%4d", &bulan, &hari, &tahun) == 3) {
            isFormatMMDDYYYY = true;
            if (isTglValid(tglInput, &isFormatMMDDYYYY)) {
                age = hitungAge(tahun, bulan, hari);

                if (age < 18) {
                    printf("\t[x] Pasangan masih di bawah umur! [x]\n");
                } else {
                	break;
				}
            } else {
                if (!isFormatMMDDYYYY) {
                    printf("\n\t[x] Tanggal yang anda masukkan mungkin dalam format DD/MM/YYYY. Silakan coba lagi. [x]\n");
                } else {
                    printf("\n\t[x] Tanggal yang anda masukkan tidak valid. Silakan coba lagi. [x]\n");
                }
            }
        } else {
            printf("\n\t[x] Format tanggal yang anda masukkan tidak valid. Silakan coba lagi. [x]\n");
        }
        
    } while (1);

    // Directly assign partner info to the srcNode's partner field
    strcpy(srcNode->partner.name, partnerName);
    srcNode->partner.age = age;
    srcNode->partner.gender = gender;
    
    system("cls");
    printFromFile("ilustrasi/wedding.txt");
    printf("\n\tPress any key to continue . . . ");
    getch();
}

void InputMember(struct nkTree *pTree) {
    nkAddr parentNode, newNode;
    int temp;
    int age;
    boolean gender;
    infoType name, parentName;

    printf("\n\tMasukan 'q' untuk kembali\n");
    do {
        printf("\n\t%c Nama orang tua: ", 175);
//        scanf(" %[^\n]", parentName);
//        if (strcmp(parentName, "q") == 0) {
//            return;
//        }
		fflush(stdin);
        fgets(parentName, sizeof(parentName), stdin);
        parentName[strcspn(parentName, "\n")] = 0; // Menghapus newline di akhir input nama
        if (strcmp(parentName, "q") == 0) {
            return;
        }
        if (strspn(parentName, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ") != strlen(parentName)) {
            printf("\t[x] INPUT TIDAK VALID! Nama hanya boleh berisi huruf. [x]\n");
            continue;
        }
        parentNode = Search((*pTree).root, parentName);
        if (parentNode == NULL) {
            printf("\t[x] Nama orang tua tidak ditemukan [x]\n");
        } else if (parentNode->partner.name[0] == '\0') {
            printf("\t[x] Orang tersebut tidak memiliki pasangan [x]\n");
        } else {
            break;
        }
    } while (1);

    do {
        printf("\n\t%c Masukan nama anak: ", 175);
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = 0; // Menghapus newline di akhir input nama
        if (strspn(name, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ") != strlen(name)) {
            printf("\t[x] INPUT TIDAK VALID! Nama hanya boleh berisi huruf. [x]\n");
            continue;
        }
        if (Search((*pTree).root, name) != NULL) {
            printf("\t[x] Nama orang tersebut sudah ada pada pohon keluarga [x]\n");
        } else {
            break;
        }
    } while (1);
    
//        scanf(" %[^\n]", name);
//        if (Search((*pTree).root, name) != NULL) {
//            printf("\t[x] Nama orang tersebut sudah ada pada pohon keluarga [x]\n");
//        } else {
//            break;
//        }
//    } while (1);
    
    do {
        printf("\n\t%c Pilih jenis kelamin\n", 175);
        printf("\t  0. Wanita\n");
        printf("\t  1. Pria\n");
        printf("\t  Pilihan: ");
        scanf(" %d", &temp);
        if (temp != 0 && temp != 1) {
            printf("\t[x] INPUT TIDAK VALID ! [x]\n");
        } else {
            gender = temp;
            break;
        }
    } while (1);

    printf("\n\tUmur anak minimal 19 tahun lebih muda dari kedua orang tua\n");

	do {
        printf("\n\t» Masukkan tanggal lahir (MM/DD/YYYY): ");
        scanf("%s", tglInput);
        
        // Memeriksa dan memparsing input menggunakan sscanf
        if (sscanf(tglInput, "%2d/%2d/%4d", &bulan, &hari, &tahun) == 3) {
            isFormatMMDDYYYY = true;
            if (isTglValid(tglInput, &isFormatMMDDYYYY)) {
                age = hitungAge(tahun, bulan, hari);
                
                if (age >= 1 && age <= parentNode->info.age - 19 && age <= parentNode->partner.age - 19) {
					break;                
                } else {
                	printf("\n\tUmur minimal 19 tahun lebih muda dari kedua orang tua\n");
				}
            } else {
                if (!isFormatMMDDYYYY) {
                    printf("\n\t[x] Tanggal yang anda masukkan mungkin dalam format DD/MM/YYYY. Silakan coba lagi. [x]\n");
                } else {
                    printf("\n\t[x] Tanggal yang anda masukkan tidak valid. Silakan coba lagi. [x]\n");
                }
            }
        } else {
            printf("\n\t[x] Format tanggal yang anda masukkan tidak valid. Silakan coba lagi. [x]\n");
        }
        
    } while (1);

    newNode = CreateNode(parentNode, name, age, gender);
    InsertNode(pTree, newNode);
    printf("\n\t[o] Anggota keluarga berhasil ditambahkan [o]");
    getch();
}

void InsertNode(struct nkTree *treeRoot, nkAddr newNode) {
    if (newNode->parent == NULL) {
        treeRoot->root = newNode;
        return;
    }

    nkAddr temp = newNode->parent;
    if (temp->fs == NULL) {
        temp->fs = newNode;
        return;
    }

    temp = temp->fs;
    while (temp->nb != NULL) {
        temp = temp->nb;
    }

    temp->nb = newNode;
}

void InsertPartner(nkAddr familyMember, infoType name, int age, boolean gender) {
    if (familyMember != NULL) {
        strcpy(familyMember->partner.name, name);
        familyMember->partner.age = age;
        familyMember->partner.gender = gender;
    }
}

nkAddr FindParent(nkAddr root, nkAddr node) {
    if (root == NULL || root == node) {
        return NULL;
    }

    nkAddr child = root->fs;
    while (child != NULL) {
        if (child == node) {
            return root;
        }

        nkAddr parent = FindParent(child, node);
        if (parent != NULL) {
            return parent;
        }
        child = child->nb;
    }
    return NULL;
}

void ReadNode(nkAddr root, char name[]) {
    nkAddr searchNode = Search(root, name);
    if (searchNode == NULL) {
        printf("\n\t[x] Data tidak ditemukan [x]\n");
        return;
    }

    printf("\n\tInformasi Keluarga:\n");
    printf("\tNama: %s\n", searchNode->info.name);
    printf("\tUsia: %d\n", searchNode->info.age);
    printf("\tJenis Kelamin: %s\n", searchNode->info.gender ? "Pria" : "Wanita");

    if (searchNode->partner.name[0] != '\0') {
        printf("\n\tNama Pasangan: %s\n", searchNode->partner.name);
        printf("\tUsia: %d\n", searchNode->partner.age);
        printf("\tJenis Kelamin: %s\n", searchNode->partner.gender ? "Pria" : "Wanita");
    } else {
        printf("\n\tBelum memiliki pasangan\n");
    }

    if (searchNode->parent != NULL) {
        printf("\n\tNama Orang Tua: %s\n", searchNode->parent->info.name);
    } else {
        printf("\n\tOrang tua tidak ditemukan\n");
    }

    if (searchNode->fs != NULL) {
        printf("\n\tAnak-anak:\n");
        nkAddr child = searchNode->fs;
        while (child != NULL) {
            printf("\t  - Nama: %s\n", child->info.name);
            child = child->nb;
        }
    } else {
        printf("\n\tBelum memiliki anak\n");
    }
    getch();
}

// Fungsi untuk menurunkan tahta dengan menghapus root dan mengganti dengan first son root sebelumnya
void DescendThrone(struct nkTree *tree) {
    if (tree->root == NULL) {
        printf("Tidak ada raja atau ratu yang saat ini bertakhta.\n");
        return;
    }

    nkAddr oldKing = tree->root;
    nkAddr newKing = oldKing->fs; // Biasanya anak pertama yang naik tahta

    if (newKing == NULL) {
        printf("Tidak ada pewaris tahta.\n");
        return;
    }

    // Simpan nama raja/ratu sekarang sebagai pendahulu
    strcpy(newKing->predecessor, oldKing->info.name);
    if (oldKing->partner.name[0] != '\0') {
        strcat(newKing->predecessor, " dan ");
        strcat(newKing->predecessor, oldKing->partner.name);
    }

    // Update root ke pewaris
    tree->root = newKing;

    // Deallocate old king node
    //DeallocNode(&oldKing);

    printf("Tahta telah diturunkan kepada %s, menggantikan %s.\n", newKing->info.name, newKing->predecessor);
}

void DeleteNode(struct nkTree *tree, nkAddr root) {
    infoType target;
    char targetName[100];
    struct nkTree pTree;
    
    printf("Ketik nama anggota yang akan dihapus: ");
    scanf("%s", targetName);
    
    nkAddr parent = NULL;
    nkAddr node = Search(root, targetName);
    
    // Jika node target tidak ditemukan, return
    if (node == NULL) {
        printf("Node dengan nama %s tidak ditemukan.\n", targetName);
        return;
    }

    // Jika target adalah partner dari node
    if (strcmp(node->partner.name, targetName) == 0) {
        printf("Menghapus partner: %s\n", node->partner.name);
        // Hapus partner
        memset(&node->partner, 0, sizeof(node->partner));
        return;
    }

    // Temukan node parent dari node target
    if (node != root) {
        parent = FindParent(root, node);
    }

    // Jika node target tidak memiliki child node
    if (node->fs == NULL && node->nb == NULL) {
        // Jika node target adalah root node
        if (node == root) {
            DescendThrone(tree);
            return;
        }
        // Jika node target adalah child node
        if (node == parent->fs) {
            parent->fs = NULL;
        } else {
            parent->nb = NULL;
        }
        DeallocNode(&node);
        return;
    }

    // Jika node target hanya memiliki satu child node
    if (node->fs != NULL && node->nb == NULL) {
        // Jika node target adalah root node
        if (node == root) {
            DescendThrone(tree);
            return;
        }
        // Jika node target adalah child node
        if (node == parent->fs) {
            parent->fs = node->fs;
        } else {
            parent->nb = node->fs;
        }
        DeallocNode(&node);
        return;
    }
    if (node->fs == NULL && node->nb != NULL) {
        // Jika node target adalah root node
        if (node == root) {
            DescendThrone(tree);
            return;
        }
        // Jika node target adalah child node
        if (node == parent->fs) {
            parent->fs = node->nb;
        } else {
            parent->nb = node->nb;
        }
        DeallocNode(&node);
        return;
    }

    // Jika node target memiliki dua child nodes
    nkAddr successor = FindSuccessor(root, node);
    nkAddr successorParent = FindParent(root, successor);
    
	// Copy info successor ke node target
    node->info = successor->info;
    
	// Hapus node successor dari tree
    if (successor == successorParent->fs) {
        successorParent->fs = successor->nb;
    } else {
        nkAddr temp = successor->nb;
        successorParent->nb = temp;
    }
    DeallocNode(&successor);
}

void enqueue(nkAddr queue[], int *rear, nkAddr node) {
    if (node != NULL) {
        queue[(*rear)++] = node;
    }
}

void nextKing(nkAddr root) {
    if (root == NULL) {
        printf("\n[x] Belum ada silsilah [x]\n\n");
        system("Pause");
        return;
    }
    gotoxy(38, 4); printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"); 
    gotoxy(38, 5); printf("        Penerus Takhta        \n"); 
    gotoxy(38, 6); printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"); 

    nkAddr queue[100]; // Simplified queue for BFS traversal
    int front = 0, rear = 0;

    enqueue(queue, &rear, root->fs);

    int x = 40, y = 8, i = 1;

    while (front < rear) {
        nkAddr current = queue[front++];
        
        if (current != NULL) {
            gotoxy(x, y++); printf("%d. %s\n", i++, current->info.name);
			
			enqueue(queue, &rear, current->fs);
			
            nkAddr sibling = current->fs != NULL ? current->fs->nb : NULL;
            while (sibling != NULL) {
                enqueue(queue, &rear, sibling);
                sibling = sibling->nb;
            }
        }
        enqueue(queue, &rear, current->nb);
    }
    printf("\n");
    system("Pause");
}

void gotoxy(int X, int y) {
	COORD coord;
	coord.X = X;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void levelOrderTraversal(nkAddr root) {
    if (root == NULL) return;

    // Inisialisasi queue
    struct Queue *queue = initQueue(1000);
    
    // Tambahkan root ke antrian
    enQueue(queue, root);
    
    // Inisialisasi variabel level
    int level = 1;
    
    while (queue->front != queue->rear) {
        // Cetak level
        printf("\t\tGenerasi %d :\n", level);
        int i;
        // Tambahkan semua node pada level ini ke antrian
        int levelSize = queue->rear - queue->front; // levelSize diisi oleh ukuran queue
        for (i = 0; i < levelSize; i++) {
            nkAddr node = deQueue(queue);
            if (node->partner.name[0] == '\0') {
                printf("[%s] -> (%d) (%s) x [Belum ada partner]\n", node->info.name, node->info.age, node->info.gender ? "P" : "W");
            } else { // Jika sudah memiliki pasangan maka tampilkan beserta pasangannya
                printf("[%s] -> (%d) (%s) x [%s] -> (%d) (%s)\n", node->info.name, node->info.age, node->info.gender ? "P" : "W", node->partner.name, node->partner.age, node->partner.gender ? "P" : "W");
            }
            
            // Tambahkan semua anak dari node ke antrian
            nkAddr child = node->fs;
            while (child != NULL) {
                enQueue(queue, child);
                child = child->nb;
            }
        
        }
        printf("\n");    
        // Naikkan level
        level++;
    }
}

void printFromFile(const char* location){
	FILE *read;
	char c;

	read=fopen(location, "rt");
	while((c=fgetc(read))!=EOF){
		printf("%c", c);
	}

	fclose(read);
}

void printNodeInfo(nkAddr node, infoType name) {
    nkAddr temp;
    int count = 1;

    // Check if the current node or its partner is the one we are looking for
    boolean isPartner = (node->partner.name[0] != '\0' && strcmp(node->partner.name, name) == 0);
    boolean isNode = strcmp(node->info.name, name) == 0;

    // Printing partner info if matched
    if (isPartner) {
        printf("\nInformasi untuk %s:\n", name);
        printf("\tUmur: %d tahun\n", node->partner.age);
        printf("\tJenis Kelamin: %s\n", node->partner.gender ? "Pria" : "Wanita");
        printf("\tPasangan: %s\n", node->info.name);
    }

    // Printing node info if matched
    if (isNode) {
        printf("\nInformasi untuk %s:\n", name);
        printf("\tUmur: %d tahun\n", node->info.age);
        printf("\tJenis Kelamin: %s\n", node->info.gender ? "Pria" : "Wanita");

        // Display partner information if exists
        if (node->partner.name[0] != '\0') {
            printf("\tPasangan: %s\n", node->partner.name);
        }

        // Display predecessors (parents)
        if (node->parent != NULL && node->parent->partner.name[0] != '\0') {
            printf("\tPredecessor: %s dan %s\n", node->parent->info.name, node->parent->partner.name);
        } else if (node->parent != NULL) {
            printf("\tPredecessor: %s\n", node->parent->info.name);
        } else {
            printf("\t%s adalah pendiri dinasti (tidak memiliki predecessor)\n", node->info.name);
        }

        // Display siblings if any
        if (node->parent != NULL && node->parent->fs != NULL) {
            printf("\tSaudara/Saudari: \n");
            temp = node->parent->fs;
            while (temp != NULL) {
                if (strcmp(node->info.name, temp->info.name) != 0) {
                    printf("\t\t%d. %s\n", count++, temp->info.name);
                }
                temp = temp->nb;
            }
        }

        // Display successors (children)
        if (node->fs != NULL) {
            printf("\tSuccessor: \n");
            temp = node->fs;
            count = 1;
            while (temp != NULL) {
                printf("\t\t%d. %s\n", count++, temp->info.name);
                temp = temp->nb;
            }
        }
    }
    
}

nkAddr FindSuccessor(nkAddr root, nkAddr node) {
    nkAddr current = node->fs;
    while (current != NULL && current->nb != NULL) {
        current = current->nb;
    }
    return current;
}

struct Queue *initQueue(int size) {
    struct Queue *queue = (struct Queue*)malloc(sizeof(struct Queue));
    queue->front = 0;
    queue->rear = 0;
    queue->size = size;
    queue->arr = (nkAddr*)malloc(size * sizeof(nkAddr));
    return queue;
}

void enQueue(struct Queue *queue, nkAddr node) {
    if (queue->rear == queue->size - 1) {
        printf("[x] Queue is full [x]");
        return;
    }
    queue->arr[queue->rear] = node;
    queue->rear++;
}

nkAddr deQueue(struct Queue *queue) {
    if (queue->front == queue->rear) {
        printf("[x] Queue is empty [x]");
        return NULL;
    }
    nkAddr node = queue->arr[queue->front];
    queue->front++;
    return node;
}

void loading_screen() {
	int i;
	gotoxy(50, 10); printf("Loading...");
	gotoxy(50, 12);
	for (i = 0; i <= 17; i++) {
		Sleep(90);
		printf("%c", 177);
	}
	printf("\n\n");
	system("pause");
	system("cls");
}

void printTreeInfo(nkAddr node, int index) {
    if (node == NULL) return;

    // Print current node information
    printf("--> Generasi ke - %d\n", index + 1);
    printf("----------------------------------\n");
    if (index == 0) {
        if (node->info.gender == 1 && node->partner.name[0] != '\0') {
			printf("Raja %s x Ratu %s\n", node->info.name, node->partner.name);       	
		} else if (node->info.gender == 0 && node->partner.name[0] != '\0') {
			printf("Ratu %s x Prince %s\n", node->info.name, node->partner.name);
		} else if (node->info.gender == 0 && node->partner.name[0] == '\0') {
			printf("Ratu %s\n", node->info.name);
		} else if (node->info.gender == 1 && node->partner.name[0] == '\0') {
			printf("Raja %s\n", node->info.name);
		}
    } else {
    	if (node->info.gender == 1 && node->partner.name[0] != '\0') {
			printf("Prince %s x Princess %s\n", node->info.name, node->partner.name);       	
		} else if (node->info.gender == 0 && node->partner.name[0] != '\0') {
			printf("Princess %s x Prince %s\n", node->info.name, node->partner.name);
		} else if (node->info.gender == 0 && node->partner.name[0] == '\0') {
			printf("Princess %s\n", node->info.name);
		} else if (node->info.gender == 1 && node->partner.name[0] == '\0') {
			printf("Prince %s\n", node->info.name);
		}
    }
    
    if (node->parent != NULL) {
        if (node->parent->info.gender == 1 && node->parent->partner.name != NULL) {
        	printf("Orangtua : Raja %s x Ratu %s\n ", node->parent->info.name, node->parent->partner.name);
		} else if (node->parent->info.gender == 0 && node->parent->partner.name != NULL) {
			printf("Orangtua : Ratu %s x Prince %s\n ", node->parent->info.name, node->parent->partner.name);
		} else if (node->parent->info.gender == 0 && node->parent->partner.name == NULL) {
			printf("Orangtua : Ratu %s \n ", node->parent->info.name);
		} else if (node->parent->info.gender == 1 && node->parent->partner.name == NULL) {
			printf("Orangtua : Raja %s \n ", node->parent->info.name);
		}
    }

    // Iterate through all children
    nkAddr child = node->fs;
    int childIndex = 1;
    while (child != NULL) {
    	if (child->info.gender == 0) {
    		printf("Anak ke %d : Princess %s\n", childIndex, child->info.name);
		} else {
			printf("Anak ke %d : Prince %s\n", childIndex, child->info.name);
		}
        child = child->nb;
        childIndex++;
    }
    printf("----------------------------------\n");
}

void printTree(nkAddr root) {
    if (root == NULL) {
        printf("Pohon kosong.\n");
        return;
    }

    struct Queue* q = initQueue(100);  // Initialize queue with an arbitrary size
    enQueue(q, root);

    int generation = 0;
    while (q->front != q->rear) {  // While the queue is not empty
        int levelSize = q->rear - q->front;
        int i;
		for (i = 0; i < levelSize; i++) {
            nkAddr node = deQueue(q);
            printTreeInfo(node, generation);

            // Enqueue the first son (fs) and all next brothers (nb)
            nkAddr child = node->fs;
            while (child != NULL) {
                enQueue(q, child);
                child = child->nb;
            }
        }
        generation++;
    }

    // Cleanup
    free(q->arr);
    free(q);
}

void saveTreeToFile(nkAddr root, const char* filename) {
    if (root == NULL) return;

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    struct Queue* q = initQueue(100);  // Initialize queue with an arbitrary size
    enQueue(q, root);

    while (q->front != q->rear) {  // While the queue is not empty
        nkAddr node = deQueue(q);

        const char* parentName = (node == root) ? "empty" : node->parent->info.name;
        const char* partnerName = (node->partner.name[0] != '\0') ? node->partner.name : "empty";

        char partnerAgeStr[10];
        if (node->partner.name[0] != '\0') {
            sprintf(partnerAgeStr, "%d", node->partner.age);
        } else {
            strcpy(partnerAgeStr, "empty");
        }

        char partnerGenderStr[10];
        if (node->partner.name[0] != '\0') {
            sprintf(partnerGenderStr, "%d", node->partner.gender);
        } else {
            strcpy(partnerGenderStr, "empty");
        }

        // Write node information to file
        fprintf(file, "\"%s\",%d,%d,\"%s\",%s,%s,\"%s\",\"%s\",\"%s\"\n",
                node->info.name, node->info.age, node->info.gender,
                partnerName, 
                partnerAgeStr, partnerGenderStr,
                node->fs ? node->fs->info.name : "empty",  // Check if fs exists
                node->nb ? node->nb->info.name : "empty",  // Check if nb exists
                parentName);

        // Enqueue the first son (fs) and all next brothers (nb)
        nkAddr child = node->fs;
        while (child != NULL) {
            enQueue(q, child);
            child = child->nb;
        }
    }

    // Cleanup
    fclose(file);
    free(q->arr);
    free(q);
}

//void removeNonAsciiAndTrim(char* str) {
//    char* dst = str;
//    char* src; 
//    for (src = str; *src != '\0'; ++src) {
//        if (isascii((unsigned char)*src) && !isspace((unsigned char)*src)) {
//            *dst++ = *src;
//        }
//    }
//    *dst = '\0';
//}

nkAddr loadTreeFromFile(const char* filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return NULL;
    }

    struct nkTree pTree;
    pTree.root = NULL;
    char line[256];

    while (fgets(line, sizeof(line), file)) {
        // Hilangkan newline di akhir baris
        line[strcspn(line, "\n")] = 0;

        if (strlen(line) == 0) {
            continue; // Skip empty lines
        }

        char *name = strtok(line, "\",");
        char *ageStr = strtok(NULL, ",");
        char *genderStr = strtok(NULL, ",");
        char *partnerName = strtok(NULL, "\",");
        char *partnerAgeStr = strtok(NULL, ",");
        char *partnerGenderStr = strtok(NULL, ",");
        char *fsToken = strtok(NULL, "\",");
        char *nbToken = strtok(NULL, "\",");
        char *parentName = strtok(NULL, "\",\n");

        if (name == NULL || ageStr == NULL || genderStr == NULL || parentName == NULL) {
            printf("Invalid data format, skipping line\n");
            continue;
        }

        int age = atoi(ageStr);
        int gender = atoi(genderStr);
        int partnerAge = partnerAgeStr ? atoi(partnerAgeStr) : 0;
        int partnerGender = partnerGenderStr ? atoi(partnerGenderStr) : 0;

        if (strcmp(parentName, "empty") == 0) {
            // Create the king/ruler of the tree
            nkAddr king = CreateNode(NULL, name, age, gender);
            if (strcmp(partnerName, "empty") != 0) {
                InsertPartner(king, partnerName, partnerAge, partnerGender);
            }
            InsertNode(&pTree, king);
        } else {
            // Create other family members
            nkAddr parentNode = Search(pTree.root, parentName);
            if (parentNode != NULL) {
                nkAddr newNode = CreateNode(parentNode, name, age, gender);
                InsertNode(&pTree, newNode);
                if (strcmp(partnerName, "empty") != 0) {
                    InsertPartner(newNode, partnerName, partnerAge, partnerGender);
                }
            } else {
                printf("Parent not found for: %s\n", name);
            }
        }	       
    }

    fclose(file);
    return pTree.root;
}

