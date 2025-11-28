#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// =============================================================
// 1. KHAI BAO CAU TRUC DU LIEU (STRUCTS)
// =============================================================

// Cau truc luu thong tin dia diem
typedef struct {
    char name[50];
    char address[100];
} Location;

// Cau truc luu thong tin mot chuyen xe
typedef struct {
    char tripId[20];
    Location departure;
    Location destination;
    char date[20];
    int totalSeats;
    int bookedSeats;
} Trip;

// Cau truc luu thong tin hanh khach
typedef struct {
    char name[50];
    char phone[15];
} Passenger;

// Cau truc luu thong tin mot ve xe
typedef struct {
    char ticketId[20];
    char tripId[20];
    Passenger passenger;
    int seatNumber;
    double price;
    int paymentStatus; // 1 = da thanh toan, 0 = chua thanh toan
    char date[20];
} Ticket;

// =============================================================
// 2. NGUYEN MAU HAM (FUNCTION PROTOTYPES)
// =============================================================

// Ham tien ich (Helper functions)
void removeNewline(char *str);
int khongChuaSpace(char str[]);
int deTrong(char str[], int size);
int checkDateSimple(char date[]);

// Ham chuc nang chinh
void themChuyen(Trip trips[], int *tripCount);
void updateTrip(Trip trips[], int nTrips);
void buyTicket(Trip trips[], int nTrips, Ticket tickets[], int *nTickets);
void checkTicket(Ticket tickets[], int nTickets);
void listTrips(Trip trips[], int nTrips);
void payTicket(Ticket tickets[], int nTickets);

// =============================================================
// 3. HAM MAIN (CHUONG TRINH CHINH)
// =============================================================

int main() {
    char choose[50];
    int choice;
    Trip trips[100];
    Ticket tickets[500];
    int nTrips = 0, nTickets = 0;
    
    do {
        // Xoa man hinh cho dep (tuy chon)
        // system("cls"); 
        
        printf("\n+------------------------------------------+\n");
        printf("|        TICKET MANAGEMENT SYSTEM          |\n");
        printf("+------------------------------------------+\n");
        printf("|1. Them chuyen xe                         |\n");
        printf("|2. Cap nhat thong tin chuyen xe           |\n");
        printf("|3. Dat ve                                 |\n");
        printf("|4. Kiem tra ve da dat                     |\n");
        printf("|5. Liet ke chuyen xe                      |\n");
        printf("|6. Thanh toan ve                          |\n");
        printf("|7. Quan ly trang thai ve                  |\n");
        printf("|8. Bao cao thong ke va doanh thu          |\n");
        printf("|9. Thoat                                  |\n");
        printf("+------------------------------------------+\n");
        printf("Nhap lua chon (1-9): ");
       
        fgets(choose, sizeof(choose), stdin);
        removeNewline(choose);
        
        if (deTrong(choose, 50)) {
            printf("Ban chua nhap gi ca!\n");
            continue;
        }
        
        int isDigit = 1;
        int i;
        for ( i = 0; i < strlen(choose); i++) {
            if (choose[i] < '0' || choose[i] > '9') {
                isDigit = 0;
                break;
            }
        }
        if (!isDigit) {
            printf("Vui long nhap so tu 1 den 9!\n");
            continue;
        }
        
        choice = atoi(choose);
        if (choice < 1 || choice > 9) {
            printf("Chi nhap tu 1 den 9!\n");
            continue;
        }
        
        // Bat buoc phai them chuyen xe truoc khi lam cac chuc nang khac (tru thoat)
        if (choice != 1 && choice != 9 && nTrips == 0) {
            printf("Chua co du lieu! Vui long them chuyen xe truoc (Chon so 1).\n");
            continue;
        }
       
        switch (choice) {
            case 1: themChuyen(trips, &nTrips); 
                break;
            case 2: updateTrip(trips, nTrips); 
                break;
            case 3: buyTicket(trips, nTrips, tickets, &nTickets); 
                break;
            case 4: checkTicket(tickets, nTickets); 
                break;
            case 5: listTrips(trips, nTrips);   
                break;
            case 6: payTicket(tickets, nTickets);   
                break;
            case 7: printf("Chuc nang chua hoan thanh.\n"); 
                break;
            case 8: printf("Chuc nang chua hoan thanh.\n"); 
                break;
            case 9: printf("Cam on ban da su dung chuong trinh!\n"); 
                break;
        }
    } while (choice != 9);
    
    return 0;
}

// =============================================================
// 4. DINH NGHIA CHI TIET CAC HAM (FUNCTION DEFINITIONS)
// =============================================================

void removeNewline(char *str) {
    str[strcspn(str, "\n")] = 0;
}

int khongChuaSpace(char str[]) {
    int len = strlen(str);
    int i;
    for ( i = 0; i < len; i++) {
        if (isspace(str[i])) return 0;
    }
    return 1;
}

int deTrong(char str[], int size) {
    if (strlen(str) == 0) return 1;
    int i;
    for (i = 0; i < strlen(str); i++) {
        if (!isspace(str[i])) {
            return 0;
        }
    }
    return 1;
}

int checkDateSimple(char date[]) {
    if (strlen(date) != 10) {
        printf("Nhap dung format dd/mm/yyyy!\n");
        return 0;
    }
    if (date[2] != '/' || date[5] != '/') {
        printf("Nhap dung format dd/mm/yyyy!\n");
        return 0;
    }
    return 1;
}

void themChuyen(Trip trips[], int *tripCount) {
    Trip c1;
   
    // Nhap va kiem tra ma chuyen xe
    while (1) {
        printf("Nhap ID xe: ");
        fgets(c1.tripId, sizeof(c1.tripId), stdin);
        removeNewline(c1.tripId);
        if (strlen(c1.tripId) == 0) {
            printf("ID khong duoc de trong!\n");
            continue;
        }
        if (!khongChuaSpace(c1.tripId)) {
            printf("ID khong duoc chua khoang trang!\n");
            continue;
        }
        int isDuplicate = 0;
        int i;
        for ( i = 0; i < *tripCount; i++) {
            if (strcmp(trips[i].tripId, c1.tripId) == 0) {
                printf("ID da ton tai!\n");
                isDuplicate = 1;
                break;
            }
        }
        if (!isDuplicate) break;
    }
   
    // Nhap diem khoi hanh
    while (1) {
        printf("Ten diem khoi hanh: ");
        fgets(c1.departure.name, 50, stdin);
        removeNewline(c1.departure.name);
        if (!deTrong(c1.departure.name, 50)) break;
        else printf("Ten diem khoi hanh khong hop le.\n");
    }
   
    while (1) {
        printf("Dia chi diem khoi hanh: ");
        fgets(c1.departure.address, 100, stdin);
        removeNewline(c1.departure.address);
        if (!deTrong(c1.departure.address, 100)) break;
        else printf("Dia chi diem khoi hanh khong hop le.\n");
    }
   
    // Nhap diem den
    while (1) {
        printf("Ten diem den: ");
        fgets(c1.destination.name, 50, stdin);
        removeNewline(c1.destination.name);
        if (!deTrong(c1.destination.name, 50)) break;
        else printf("Ten diem den khong hop le.\n");
    }
   
    while (1) {
        printf("Dia chi diem den: ");
        fgets(c1.destination.address, 100, stdin);
        removeNewline(c1.destination.address);
        if (!deTrong(c1.destination.address, 100)) break;
        else printf("Dia chi diem den khong hop le.\n");
    }
   
    // Nhap ngay di
    while (1) {
        printf("Ngay di chuyen (dd/mm/yyyy): ");
        fgets(c1.date, 20, stdin);
        removeNewline(c1.date);
        if (strlen(c1.date) == 0) continue;
        
        int valid = 1;
        if (strlen(c1.date) != 10) valid = 0;
        else {
        	int j;
            for ( j = 0; j < 10; j++) {
                if (j == 2 || j == 5) {
                    if (c1.date[j] != '/') valid = 0;
                } else {
                    if (c1.date[j] < '0' || c1.date[j] > '9') valid = 0;
                }
            }
        }
        if (!valid) {
            printf("Ngay khong hop le.\n");
            continue;
        }
        if (checkDateSimple(c1.date)) break;
    }
   
    // Nhap so ghe
    while (1) {
        char buf[20];
        printf("Tong so ghe: ");
        fgets(buf, sizeof(buf), stdin);
        removeNewline(buf);
        if (strlen(buf) == 0) continue;
        
        int hopLe = 1;
        int i;
        for ( i = 0; i < strlen(buf); i++) {
            if (buf[i] < '0' || buf[i] > '9') {
                hopLe = 0;
                break;
            }
        }
        if (!hopLe) {
            printf("Chi duoc nhap so > 0!\n");
            continue;
        }
        c1.totalSeats = atoi(buf);
        if (c1.totalSeats <= 0) {
            printf("So ghe phai > 0!\n");
            continue;
        }
        break;
    }
   
    c1.bookedSeats = 0;
    trips[*tripCount] = c1;
    (*tripCount)++;
    printf("Them chuyen thanh cong!\n");
}

void updateTrip(Trip trips[], int nTrips) {
    char id[20];
    int index = -1;
    
    while (1) {
        printf("Nhap Trip ID can cap nhat: ");
        fgets(id, sizeof(id), stdin);
        removeNewline(id);
        if (deTrong(id, 20)) {
            printf("ID khong duoc de trong!\n");
            continue;
        }
        int i;
        for (i = 0; i < nTrips; i++) {
            if (strcmp(trips[i].tripId, id) == 0) {
                index = i;
                break;
            }
        }
        if (index != -1) break;
        printf("Khong tim thay Trip ID!\n");
    }
   
    int choice;
    char s[150];
    int newSeats;
    
    while (1) {
        printf("\n+------ CAP NHAT TRIP %s --------+\n", trips[index].tripId);
        printf("|1. Ten diem khoi hanh                  |\n");
        printf("|2. Dia chi diem khoi hanh              |\n");
        printf("|3. Ten diem den                        |\n");
        printf("|4. Dia chi diem den                    |\n");
        printf("|5. Ngay                                |\n");
        printf("|6. Tong so ghe                         |\n");
        printf("|0. Thoat                               |\n");
        printf("+---------------------------------------+\n");
        printf("Nhap lua chon (0-6): ");
        
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');
        
        if (choice == 0) break;
        
        switch (choice) {
            case 1:
                printf("Nhap ten diem khoi hanh moi: ");
                fgets(s, sizeof(s), stdin); removeNewline(s);
                if (!deTrong(s, 50)) strcpy(trips[index].departure.name, s);
                break;
            case 2:
                printf("Nhap dia chi diem khoi hanh moi: ");
                fgets(s, sizeof(s), stdin); removeNewline(s);
                if (!deTrong(s, 100)) strcpy(trips[index].departure.address, s);
                break;
            case 3:
                printf("Nhap ten diem den moi: ");
                fgets(s, sizeof(s), stdin); removeNewline(s);
                if (!deTrong(s, 50)) strcpy(trips[index].destination.name, s);
                break;
            case 4:
                printf("Nhap dia chi diem den moi: ");
                fgets(s, sizeof(s), stdin); removeNewline(s);
                if (!deTrong(s, 100)) strcpy(trips[index].destination.address, s);
                break;
            case 5:
                printf("Nhap ngay moi: ");
                fgets(s, sizeof(s), stdin); removeNewline(s);
                if (checkDateSimple(s)) strcpy(trips[index].date, s);
                break;
            case 6:
                printf("Nhap tong so ghe moi: ");
                if (scanf("%d", &newSeats) == 1 && newSeats >= trips[index].bookedSeats) {
                    trips[index].totalSeats = newSeats;
                } else {
                    printf("So ghe khong hop le!\n");
                }
                while(getchar()!='\n');
                break;
        }
        printf("Cap nhat xong!\n");
    }
}

void buyTicket(Trip trips[], int nTrips, Ticket tickets[], int *nTickets) {
    if (nTrips == 0) {
        printf("Khong co chuyen xe!\n");
        return;
    }
    
    char id[20];
    int index = -1;
    
    while (1) {
        printf("Ma chuyen xe: ");
        fgets(id, 20, stdin);
        removeNewline(id);
        if (deTrong(id, 20)) continue;

		int i;
        for ( i = 0; i < nTrips; i++) {
            if (strcmp(trips[i].tripId, id) == 0) {
                index = i;
                break;
            }
        }
        if (index == -1) {
            printf("Khong tim thay chuyen!\n");
            continue;
        }
        if (trips[index].bookedSeats >= trips[index].totalSeats) {
            printf("Het ghe! Khong the dat ve.\n");
            return;
        }
        break;
    }
    
    Ticket tk;
    strcpy(tk.tripId, id);
    
    // Nhap ID ve (can them logic check trung ID ve)
    printf("Nhap ma ve: ");
    fgets(tk.ticketId, 20, stdin);
    removeNewline(tk.ticketId);
    
    printf("Ten khach hang: ");
    fgets(tk.passenger.name, 50, stdin);
    removeNewline(tk.passenger.name);
    
    printf("SDT khach hang: ");
    fgets(tk.passenger.phone, 15, stdin);
    removeNewline(tk.passenger.phone);
    
    strcpy(tk.date, trips[index].date); // Lay ngay cua chuyen xe
    tk.seatNumber = trips[index].bookedSeats + 1;
    tk.price = 100000;
    
    int payChoice;
    while (1) {
        printf("Thanh toan (1. Ngay, 2. Sau): ");
        if (scanf("%d", &payChoice) == 1 && (payChoice == 1 || payChoice == 2)) break;
        while (getchar() != '\n');
    }
    while (getchar() != '\n');
    
    tk.paymentStatus = (payChoice == 1) ? 1 : 0;
       
    tickets[*nTickets] = tk;
    (*nTickets)++;
    trips[index].bookedSeats++;
    
    printf("Dat ve thanh cong! Ghe so %d. Gia: %.0lf\n", tk.seatNumber, tk.price);
}

void checkTicket(Ticket tickets[], int nTickets) {
    if (nTickets == 0) {
        printf("Chua co ve nao!\n");
        return;
    }
    int i;
    for (i = 0; i < nTickets; i++) {
        printf("\n===== VE %d =====\n", i + 1);
        printf("Ticket ID : %s\n", tickets[i].ticketId);
        printf("Trip ID   : %s\n", tickets[i].tripId);
        printf("Khach hang: %s\n", tickets[i].passenger.name);
        printf("SDT       : %s\n", tickets[i].passenger.phone);
        printf("So ghe    : %d\n", tickets[i].seatNumber);
        printf("Trang thai: %s\n", tickets[i].paymentStatus ? "Da thanh toan" : "Chua thanh toan");
    }
}

void listTrips(Trip trips[], int nTrips) {
    if (nTrips == 0) {
        printf("Khong co chuyen xe de hien thi\n");
        return;
    }
    int pageSize = 10;
    int totalPages = (nTrips + pageSize - 1) / pageSize;
    int pageNumber = 1;
    char cmd[10];
    
    while (1) {
        // system("cls");
        int start = (pageNumber - 1) * pageSize;
        int end = start + pageSize;
        if (end > nTrips) end = nTrips;
   
        printf("+--------------------------------------------------------------------------------------+\n");
        printf("|                        DANH SACH CHUYEN XE (Trang %d / %d)                           |\n", pageNumber, totalPages);
        printf("+----------+-------------------------+-------------------------+------------+----------+\n");
        printf("| Trip ID  | Noi khoi hanh           | Noi den                 | Ngay       | Ghe (Dat)|\n");
        printf("+----------+-------------------------+-------------------------+------------+----------+\n");
        
        int i;
        for (i = start; i < end; i++) {
            printf("| %-8s | %-23s | %-23s | %-10s | %d/%d    |\n",
                   trips[i].tripId,
                   trips[i].departure.name,
                   trips[i].destination.name,
                   trips[i].date,
                   trips[i].bookedSeats, trips[i].totalSeats);
        }
        printf("+--------------------------------------------------------------------------------------+\n");
        
        printf("Nhap 1 (Trang sau), 0 (Menu): ");
        fgets(cmd, sizeof(cmd), stdin);
        removeNewline(cmd);
        if (cmd[0] == '1' && pageNumber < totalPages) pageNumber++;
        else if (cmd[0] == '0') break;
        else if (cmd[0] == '1') printf("Da la trang cuoi!\n");
    }
}

void payTicket(Ticket tickets[], int nTickets) {
    char id[20];
    printf("Nhap ma ve can thanh toan: ");
    fgets(id, 20, stdin);
    removeNewline(id);
    
    int found = 0;
    int i;
    for(i = 0; i < nTickets; i++){
        if(strcmp(tickets[i].ticketId, id) == 0){
            found = 1;
            if(tickets[i].paymentStatus == 1){
                printf("Ve nay da thanh toan roi!\n");
            } else {
                tickets[i].paymentStatus = 1;
                printf("Thanh toan thanh cong cho ve %s. So tien: %.0lf\n", id, tickets[i].price);
            }
            break;
        }
    }
    if(!found) printf("Khong tim thay ma ve!\n");
}
