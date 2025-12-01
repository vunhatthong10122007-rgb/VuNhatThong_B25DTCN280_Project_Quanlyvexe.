#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


typedef struct {
    char name[50];
    char address[100];
} Location;

typedef struct {
    char tripId[20];
    Location departure;
    Location destination;
    char date[20];
    int totalSeats;
    int bookedSeats;
} Trip;

typedef struct {
    char name[50];
    char phone[15];
} Passenger;

typedef struct {
    char ticketId[20];
    char tripId[20];
    Passenger passenger;
    int seatNumber;
    double price;
    int paymentStatus; /* 1 = da thanh toan, 0 = chua thanh toan */
    int status;        /* 0 = hoat dong, 2 = da huy, 3 = bi khoa */
    char date[20];
} Ticket;


/* Ham tien ich */
void removeNewline(char *str);
int khongChuaSpace(char str[]);
int deTrong(char str[], int size);
int checkDateSimple(char date[]);
int findTripIndex(Trip trips[], int nTrips, const char *tripId);
int findTicketIndex(Ticket tickets[], int nTickets, const char *ticketId);

/* Ham chuc nang chinh */
void themChuyen(Trip trips[], int *tripCount);
void updateTrip(Trip trips[], int nTrips);
void buyTicket(Trip trips[], int nTrips, Ticket tickets[], int *nTickets);
void checkTicket(Ticket tickets[], int nTickets);
void listTrips(Trip trips[], int nTrips);
void payTicket(Ticket tickets[], int nTickets);
void manageTicketStatus(Ticket tickets[], int nTickets, Trip trips[], int nTrips);

/* Ham bao cao */
void reportMenu(Ticket tickets[], int nTickets, Trip trips[], int nTrips);
void reportRevenue(Ticket tickets[], int nTickets);
void reportByTrip(Ticket tickets[], int nTickets, Trip trips[], int nTrips);
void reportByDate(Ticket tickets[], int nTickets);


int main() {
    char choose[50];
    int choice;
    Trip trips[100];
    Ticket tickets[500];
    int nTrips = 0, nTickets = 0;
    int i, isDigit;
    
    do {
        printf("\n+------------------------------------------+\n");
        printf("|           TICKET MANAGEMENT              |\n");
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
        
        isDigit = 1;
        for (i = 0; i < (int)strlen(choose); i++) {
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
        
        if (choice != 1 && choice != 9 && nTrips == 0) {
            printf("Chua co du lieu! Vui long them chuyen xe truoc (Chon so 1).\n");
            continue;
        }
       
        switch (choice) {
            case 1: 
                themChuyen(trips, &nTrips); 
                break;
            case 2: 
                updateTrip(trips, nTrips); 
                break;
            case 3: 
                buyTicket(trips, nTrips, tickets, &nTickets); 
                break;
            case 4: 
                checkTicket(tickets, nTickets); 
                break;
            case 5: 
                listTrips(trips, nTrips);   
                break;
            case 6: 
                payTicket(tickets, nTickets);   
                break;
            case 7:
                manageTicketStatus(tickets, nTickets, trips, nTrips);
                break;
            case 8: 
                reportMenu(tickets, nTickets, trips, nTrips);
                break;
            case 9: 
                printf("Cam on ban da su dung chuong trinh!\n"); 
                break;
        }
    } while (choice != 9);
    
    return 0;
}


void removeNewline(char *str) {
    str[strcspn(str, "\n")] = 0;
}

int khongChuaSpace(char str[]) {
    int len = strlen(str);
    int i;
    for (i = 0; i < len; i++) {
        if (isspace((unsigned char)str[i])) return 0;
    }
    return 1;
}

int deTrong(char str[], int size) {
    int i;
    if (strlen(str) == 0) return 1;
    for (i = 0; i < (int)strlen(str); i++) {
        if (!isspace((unsigned char)str[i])) {
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

int findTripIndex(Trip trips[], int nTrips, const char *tripId) {
    int i;
    for (i = 0; i < nTrips; i++) {
        if (strcmp(trips[i].tripId, tripId) == 0) {
            return i;
        }
    }
    return -1;
}

int findTicketIndex(Ticket tickets[], int nTickets, const char *ticketId) {
    int i;
    for (i = 0; i < nTickets; i++) {
        if (strcmp(tickets[i].ticketId, ticketId) == 0) {
            return i;
        }
    }
    return -1;
}

int isValidDate(const char *date) {
    if (strlen(date) != 10) return 0;
    if (date[2] != '/' || date[5] != '/') return 0;

    int dd = (date[0]-'0')*10 + (date[1]-'0');
    int mm = (date[3]-'0')*10 + (date[4]-'0');
    int yyyy = (date[6]-'0')*1000 + (date[7]-'0')*100 + (date[8]-'0')*10 + (date[9]-'0');

    if (dd < 1 || dd > 31) return 0;
    if (mm < 1 || mm > 12) return 0;
    if (yyyy < 1900) return 0;

    // check s? ngay h?p l? theo thang
    int maxDays[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    // N?m nhu?n
    if ((yyyy%4==0 && yyyy%100!=0) || (yyyy%400==0)) maxDays[2]=29;

    if (dd > maxDays[mm]) return 0;

    return 1;
}



void themChuyen(Trip trips[], int *tripCount) {
    Trip c1;
    int i, j, isDuplicate, valid, hopLe;
    char buf[20];
   
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
        isDuplicate = 0;
        for (i = 0; i < *tripCount; i++) {
            if (strcmp(trips[i].tripId, c1.tripId) == 0) {
                printf("ID da ton tai!\n");
                isDuplicate = 1;
                break;
            }
        }
        if (!isDuplicate) break;
    }
   
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
   
    while (1) {
	    printf("Ngay di chuyen (dd/mm/yyyy): ");
	    fgets(c1.date, 20, stdin);
	    removeNewline(c1.date);
	    if (isValidDate(c1.date)) break;
	    printf("Ngay khong hop le. Nhap lai!\n");
	}

   
    while (1) {
        printf("Tong so ghe: ");
        fgets(buf, sizeof(buf), stdin);
        removeNewline(buf);
        if (strlen(buf) == 0) continue;
        
        hopLe = 1;
        for (i = 0; i < (int)strlen(buf); i++) {
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
    int i, choice, newSeats;
    char s[150];
    char choiceStr[20];
    
    while (1) {
        printf("Nhap Trip ID can cap nhat: ");
        fgets(id, sizeof(id), stdin);
        removeNewline(id);
        if (deTrong(id, 20)) {
            printf("ID khong duoc de trong!\n");
            continue;
        }
        index = findTripIndex(trips, nTrips, id);
        if (index != -1) break;
        printf("Khong tim thay Trip ID!\n");
    }
    
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
        
        fgets(choiceStr, sizeof(choiceStr), stdin);
        removeNewline(choiceStr);
        if (deTrong(choiceStr, sizeof(choiceStr))) continue;
        choice = atoi(choiceStr);
        
        if (choice == 0) break;
        
        switch (choice) {
            case 1:
                printf("Nhap ten diem khoi hanh moi: ");
                fgets(s, sizeof(s), stdin);
                removeNewline(s);
                if (!deTrong(s, 50)) strcpy(trips[index].departure.name, s);
                break;
            case 2:
                printf("Nhap dia chi diem khoi hanh moi: ");
                fgets(s, sizeof(s), stdin);
                removeNewline(s);
                if (!deTrong(s, 100)) strcpy(trips[index].departure.address, s);
                break;
            case 3:
                printf("Nhap ten diem den moi: ");
                fgets(s, sizeof(s), stdin);
                removeNewline(s);
                if (!deTrong(s, 50)) strcpy(trips[index].destination.name, s);
                break;
            case 4:
                printf("Nhap dia chi diem den moi: ");
                fgets(s, sizeof(s), stdin);
                removeNewline(s);
                if (!deTrong(s, 100)) strcpy(trips[index].destination.address, s);
                break;
           case 5: {
			    char tmpDate[20];
			    printf("Nhap ngay (dd/mm/yyyy): ");
			    fgets(tmpDate, sizeof(tmpDate), stdin);
			    removeNewline(tmpDate);
			    while (!isValidDate(tmpDate)) {
			        printf("Ngay khong hop le! Vui long nhap dung dinh dang dd/mm/yyyy: ");
			        fgets(tmpDate, sizeof(tmpDate), stdin);
			        removeNewline(tmpDate);
			    }
			    strcpy(trips[index].date, tmpDate);
			    break;
			}

            case 6:
                printf("Nhap tong so ghe moi: ");
                fgets(s, sizeof(s), stdin);
                removeNewline(s);
                newSeats = atoi(s);
                if (newSeats >= trips[index].bookedSeats) {
                    trips[index].totalSeats = newSeats;
                } else {
                    printf("So ghe khong duoc nho hon so ghe da dat (%d)!\n", trips[index].bookedSeats);
                }
                break;
            default:
                printf("Lua chon khong hop le!\n");
                continue;
        }
        printf("Cap nhat xong!\n");
    }
}

void buyTicket(Trip trips[], int nTrips, Ticket tickets[], int *nTickets) {
    char id[20];
    int index = -1;
    int i;
    Ticket tk;
    char payStr[20];
    int payChoice;
    
    if (nTrips == 0) {
        printf("Khong co chuyen xe!\n");
        return;
    }
    
    while (1) {
        printf("Ma chuyen xe: ");
        fgets(id, 20, stdin);
        removeNewline(id);
        if (deTrong(id, 20)) continue;

        index = findTripIndex(trips, nTrips, id);
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
    
    strcpy(tk.tripId, id);
    
    /* Nhap ma ve - kiem tra trung */
    while (1) {
        printf("Nhap ma ve: ");
        fgets(tk.ticketId, 20, stdin);
        removeNewline(tk.ticketId);
        if (strlen(tk.ticketId) == 0) {
            printf("Ma ve khong duoc de trong!\n");
            continue;
        }
        if (findTicketIndex(tickets, *nTickets, tk.ticketId) != -1) {
            printf("Ma ve da ton tai! Nhap ma khac.\n");
            continue;
        }
        break;
    }
    
    while (1) {
        printf("Ten khach hang: ");
        fgets(tk.passenger.name, 50, stdin);
        removeNewline(tk.passenger.name);
        if (!deTrong(tk.passenger.name, 50)) break;
        printf("Ten khong hop le!\n");
    }
    
    while (1) {
        printf("SDT khach hang: ");
        fgets(tk.passenger.phone, 15, stdin);
        removeNewline(tk.passenger.phone);
        if (!deTrong(tk.passenger.phone, 15)) break;
        printf("SDT khong hop le!\n");
    }
    
    strcpy(tk.date, trips[index].date);
    tk.seatNumber = trips[index].bookedSeats + 1;
    tk.price = 100000;
    tk.status = 0; /* Hoat dong */
    
    while (1) {
        printf("Thanh toan (1. Ngay, 2. Sau): ");
        fgets(payStr, sizeof(payStr), stdin);
        removeNewline(payStr);
        payChoice = atoi(payStr);
        if (payChoice == 1 || payChoice == 2) break;
        printf("Vui long nhap 1 hoac 2!\n");
    }
    
    tk.paymentStatus = (payChoice == 1) ? 1 : 0;
       
    tickets[*nTickets] = tk;
    (*nTickets)++;
    trips[index].bookedSeats++;
    
    printf("Dat ve thanh cong! Ghe so %d. Gia: %.0lf\n", tk.seatNumber, tk.price);
}

void checkTicket(Ticket tickets[], int nTickets) {
    int i;
    const char *statusStr;
    
    if (nTickets == 0) {
        printf("Chua co ve nao!\n");
        return;
    }
    
    for (i = 0; i < nTickets; i++) {
        switch (tickets[i].status) {
            case 0: statusStr = "Hoat dong"; break;
            case 2: statusStr = "Da huy"; break;
            case 3: statusStr = "Bi khoa"; break;
            default: statusStr = "Khong xac dinh";
        }
        
        printf("\n===== VE %d =====\n", i + 1);
        printf("Ticket ID : %s\n", tickets[i].ticketId);
        printf("Trip ID   : %s\n", tickets[i].tripId);
        printf("Khach hang: %s\n", tickets[i].passenger.name);
        printf("SDT       : %s\n", tickets[i].passenger.phone);
        printf("So ghe    : %d\n", tickets[i].seatNumber);
        printf("Ngay      : %s\n", tickets[i].date);
        printf("Gia       : %.0lf\n", tickets[i].price);
        printf("Thanh toan: %s\n", tickets[i].paymentStatus ? "Da thanh toan" : "Chua thanh toan");
        printf("Trang thai: %s\n", statusStr);
    }
}

void listTrips(Trip trips[], int nTrips) {
    int pageSize, totalPages, pageNumber, start, end, i;
    char cmd[10];
    
    if (nTrips == 0) {
        printf("Khong co chuyen xe de hien thi\n");
        return;
    }
    
    pageSize = 10;
    totalPages = (nTrips + pageSize - 1) / pageSize;
    pageNumber = 1;
    
    while (1) {
        start = (pageNumber - 1) * pageSize;
        end = start + pageSize;
        if (end > nTrips) end = nTrips;
   
        printf("+--------------------------------------------------------------------------------------+\n");
        printf("|                        DANH SACH CHUYEN XE (Trang %d / %d)                           |\n", pageNumber, totalPages);
        printf("+----------+-------------------------+-------------------------+------------+----------+\n");
        printf("| Trip ID  | Noi khoi hanh           | Noi den                 | Ngay       | Ghe (Dat)|\n");
        printf("+----------+-------------------------+-------------------------+------------+----------+\n");
        
        for (i = start; i < end; i++) {
            printf("| %-8s | %-23s | %-23s | %-10s | %3d/%-3d  |\n",
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
    int found, i;
    
    printf("Nhap ma ve can thanh toan: ");
    fgets(id, 20, stdin);
    removeNewline(id);
    
    found = 0;
    for (i = 0; i < nTickets; i++) {
        if (strcmp(tickets[i].ticketId, id) == 0) {
            found = 1;
            if (tickets[i].status == 2) {
                printf("Khong the thanh toan ve da bi huy!\n");
            } else if (tickets[i].status == 3) {
                printf("Khong the thanh toan ve dang bi khoa!\n");
            } else if (tickets[i].paymentStatus == 1) {
                printf("Ve nay da thanh toan roi!\n");
            } else {
                tickets[i].paymentStatus = 1;
                printf("Thanh toan thanh cong cho ve %s. So tien: %.0lf\n", id, tickets[i].price);
            }
            break;
        }
    }
    if (!found) printf("Khong tim thay ma ve!\n");
}

void manageTicketStatus(Ticket tickets[], int nTickets, Trip trips[], int nTrips) {
    char id[20];
    int index, tripIndex, choice;
    char choiceStr[20];
    
    if (nTickets == 0) {
        printf("Khong co ve nao trong he thong!\n");
        return;
    }

    printf("Nhap Ticket ID can quan ly: ");
    fgets(id, sizeof(id), stdin);
    removeNewline(id);

    index = findTicketIndex(tickets, nTickets, id);
    if (index == -1) {
        printf("Khong tim thay ve!\n");
        return;
    }

    tripIndex = findTripIndex(trips, nTrips, tickets[index].tripId);

    printf("\n+================================+\n");
    printf("|   QUAN LY TRANG THAI VE        |\n");
    printf("+================================+\n");
    printf("| Ma ve    : %-20s|\n", tickets[index].ticketId);
    printf("| Trang thai hien tai: ");
    switch (tickets[index].status) {
        case 0: printf("Hoat dong     |\n"); break;
        case 2: printf("Da huy        |\n"); break;
        case 3: printf("Bi khoa       |\n"); break;
        default: printf("Khong xac dinh|\n");
    }
    printf("+--------------------------------+\n");
    printf("| 1. Huy ve                      |\n");
    printf("| 2. Mo lai ve (neu da huy)      |\n");
    printf("| 3. Khoa ve                     |\n");
    printf("| 4. Mo khoa ve                  |\n");
    printf("| 0. Thoat                       |\n");
    printf("+--------------------------------+\n");
    printf("Chon: ");
    
    fgets(choiceStr, sizeof(choiceStr), stdin);
    removeNewline(choiceStr);
    choice = atoi(choiceStr);

    switch (choice) {
        case 1: /* Huy ve */
            if (tickets[index].status == 2) {
                printf("Ve nay da bi huy tu truoc!\n");
            } else if (tickets[index].status == 3) {
                printf("Ve dang bi khoa, khong the huy. Mo khoa truoc!\n");
            } else {
                tickets[index].status = 2;
                if (tripIndex != -1 && trips[tripIndex].bookedSeats > 0) {
                    trips[tripIndex].bookedSeats--;
                }
                /* Hoan tien neu da thanh toan */
                if (tickets[index].paymentStatus == 1) {
                    printf("Ve da thanh toan. Hoan tien: %.0lf\n", tickets[index].price);
                    tickets[index].paymentStatus = 0;
                }
                printf("Huy ve thanh cong!\n");
            }
            break;

        case 2: /* Mo lai ve */
            if (tickets[index].status != 2) {
                printf("Ve khong o trang thai bi huy!\n");
            } else {
                if (tripIndex == -1) {
                    printf("Chuyen xe khong con ton tai!\n");
                } else if (trips[tripIndex].bookedSeats >= trips[tripIndex].totalSeats) {
                    printf("Chuyen xe da het ghe, khong the mo lai ve!\n");
                } else {
                    tickets[index].status = 0;
                    trips[tripIndex].bookedSeats++;
                    printf("Mo lai ve thanh cong!\n");
                }
            }
            break;

        case 3: /* Khoa ve */
            if (tickets[index].status == 3) {
                printf("Ve da bi khoa truoc do!\n");
            } else if (tickets[index].status == 2) {
                printf("Khong the khoa ve da bi huy!\n");
            } else {
                tickets[index].status = 3;
                printf("Khoa ve thanh cong!\n");
            }
            break;

        case 4: /* Mo khoa ve */
            if (tickets[index].status != 3) {
                printf("Ve nay khong bi khoa!\n");
            } else {
                tickets[index].status = 0;
                printf("Mo khoa ve thanh cong!\n");
            }
            break;

        case 0:
            printf("Thoat quan ly trang thai ve.\n");
            break;

        default:
            printf("Lua chon khong hop le!\n");
    }
}


void reportRevenue(Ticket tickets[], int nTickets) {
    double totalPaid, totalUnpaid;
    int paidCount, unpaidCount, cancelledCount, i;
    
    totalPaid = 0;
    totalUnpaid = 0;
    paidCount = 0;
    unpaidCount = 0;
    cancelledCount = 0;
    
    for (i = 0; i < nTickets; i++) {
        if (tickets[i].status == 2) {
            cancelledCount++;
        } else if (tickets[i].paymentStatus == 1) {
            paidCount++;
            totalPaid += tickets[i].price;
        } else {
            unpaidCount++;
            totalUnpaid += tickets[i].price;
        }
    }
    
    printf("\n+================================================+\n");
    printf("|              BAO CAO DOANH THU                 |\n");
    printf("+================================================+\n");
    printf("| Tong so ve           : %-23d |\n", nTickets);
    printf("| Ve da thanh toan     : %-3d  | %.0lf VND\n", paidCount, totalPaid);
    printf("| Ve chua thanh toan   : %-3d  | %.0lf VND (du kien)\n", unpaidCount, totalUnpaid);
    printf("| Ve da huy            : %-23d |\n", cancelledCount);
    printf("+------------------------------------------------+\n");
    printf("| TONG DOANH THU THUC  : %.0lf VND\n", totalPaid);
    printf("+================================================+\n");
}

void reportByTrip(Ticket tickets[], int nTickets, Trip trips[], int nTrips) {
    int i, j;
    double rev;
    int soldCount;
    
    if (nTrips == 0) {
        printf("Khong co chuyen xe de bao cao.\n");
        return;
    }
    
    printf("\n+====================================================================================+\n");
    printf("|                          BAO CAO THEO CHUYEN XE                                   |\n");
    printf("+----------+--------------------+--------------------+------------+------+----------+\n");
    printf("| Trip ID  | Diem di            | Diem den           | Ngay       | Ve   | Doanh thu|\n");
    printf("+----------+--------------------+--------------------+------------+------+----------+\n");
    
    for (i = 0; i < nTrips; i++) {
        rev = 0;
        soldCount = 0;
        
        for (j = 0; j < nTickets; j++) {
            if (strcmp(tickets[j].tripId, trips[i].tripId) == 0 && tickets[j].status != 2) {
                soldCount++;
                if (tickets[j].paymentStatus == 1) {
                    rev += tickets[j].price;
                }
            }
        }
        
        printf("| %-8s | %-18s | %-18s | %-10s | %4d | %8.0lf |\n",
               trips[i].tripId,
               trips[i].departure.name,
               trips[i].destination.name,
               trips[i].date,
               soldCount,
               rev);
    }
    
    printf("+====================================================================================+\n");
}

void reportByDate(Ticket tickets[], int nTickets) {
    char dates[500][20];
    double revs[500];
    int counts[500];
    int nDates, i, j, found;
    
    if (nTickets == 0) {
        printf("Khong co du lieu de bao cao.\n");
        return;
    }
    
    nDates = 0;
    
    for (i = 0; i < nTickets; i++) {
        if (tickets[i].status == 2) continue;
        
        found = -1;
        for (j = 0; j < nDates; j++) {
            if (strcmp(dates[j], tickets[i].date) == 0) {
                found = j;
                break;
            }
        }
        
        if (found == -1) {
            strcpy(dates[nDates], tickets[i].date);
            counts[nDates] = 1;
            revs[nDates] = (tickets[i].paymentStatus == 1) ? tickets[i].price : 0;
            nDates++;
        } else {
            counts[found]++;
            if (tickets[i].paymentStatus == 1) {
                revs[found] += tickets[i].price;
            }
        }
    }
    
    printf("\n+=====================================================+\n");
    printf("|           BAO CAO DOANH THU THEO NGAY               |\n");
    printf("+----------------+---------------+--------------------+\n");
    printf("| Ngay           | So ve         | Doanh thu          |\n");
    printf("+----------------+---------------+--------------------+\n");
    
    for (i = 0; i < nDates; i++) {
        printf("| %-14s | %-13d | %18.0lf |\n", dates[i], counts[i], revs[i]);
    }
    
    printf("+=====================================================+\n");
}

void reportMenu(Ticket tickets[], int nTickets, Trip trips[], int nTrips) {
    int type;
    char typeStr[20];
    
    if (nTickets == 0) {
        printf("Khong co du lieu ve de bao cao.\n");
        return;
    }

    printf("\n+========================================+\n");
    printf("|        BAO CAO THONG KE DOANH THU      |\n");
    printf("+========================================+\n");
    printf("| 1. Bao cao tong doanh thu              |\n");
    printf("| 2. Bao cao theo chuyen xe              |\n");
    printf("| 3. Bao cao theo thoi gian (ngay)       |\n");
    printf("| 0. Thoat                               |\n");
    printf("+----------------------------------------+\n");
    printf("Chon loai bao cao: ");
    
    fgets(typeStr, sizeof(typeStr), stdin);
    removeNewline(typeStr);
    type = atoi(typeStr);

    switch (type) {
        case 1:
            reportRevenue(tickets, nTickets);
            break;
        case 2:
            reportByTrip(tickets, nTickets, trips, nTrips);
            break;
        case 3:
            reportByDate(tickets, nTickets);
            break;
        case 0:
            printf("Thoat bao cao.\n");
            break;
        default:
            printf("Lua chon khong hop le!\n");
    }
}
