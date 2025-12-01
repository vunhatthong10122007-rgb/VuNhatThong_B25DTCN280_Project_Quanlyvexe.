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
    int paymentStatus;  
    int status;         
    char date[20];    
} Ticket;


void removeNewline(char *str);
int hasNoSpace(char str[]);
int isEmpty(char str[], int size);
int isValidDate(const char *date);
int findTripIndex(Trip trips[], int nTrips, const char *tripId);
int findTicketIndex(Ticket tickets[], int nTickets, const char *ticketId);


void addTrip(Trip trips[], int *tripCount);
void updateTrip(Trip trips[], int nTrips);
void buyTicket(Trip trips[], int nTrips, Ticket tickets[], int *nTickets);
void checkTicket(Ticket tickets[], int nTickets);
void listTrips(Trip trips[], int nTrips);
void payTicket(Ticket tickets[], int nTickets);
void manageTicketStatus(Ticket tickets[], int nTickets, Trip trips[], int nTrips);


void reportMenu(Ticket tickets[], int nTickets, Trip trips[], int nTrips);
void reportRevenue(Ticket tickets[], int nTickets);
void reportByTrip(Ticket tickets[], int nTickets, Trip trips[], int nTrips);
void reportByDate(Ticket tickets[], int nTickets);



int main() {
    char choose[50];
    int choice;
    Trip trips[100];        /* Mang luu cac chuyen xe */
    Ticket tickets[500];    /* Mang luu cac ve xe */
    int nTrips = 0, nTickets = 0;
    int i, isDigit;
    
    do {
        /* Hien thi menu chinh */
        printf("\n+------------------------------------------+\n");
        printf("|           TICKET MANAGEMENT              |\n");
        printf("+------------------------------------------+\n");
        printf("|1. Add Trip                               |\n");
        printf("|2. Update Trip                            |\n");
        printf("|3. Book Ticket                            |\n");
        printf("|4. Check Booked Tickets                   |\n");
        printf("|5. List Trips                             |\n");
        printf("|6. Pay Ticket                             |\n");
        printf("|7. Manage Ticket Status                   |\n");
        printf("|8. Reports & Revenue                      |\n");
        printf("|9. Exit                                   |\n");
        printf("+------------------------------------------+\n");
        printf("Enter choice (1-9): ");
       
        fgets(choose, sizeof(choose), stdin);
        removeNewline(choose);
        
        /* Kiem tra dau vao co phai la khoang trang khong */
        if (isEmpty(choose, 50)) {
            printf("You entered nothing!\n");
            continue;
        }
        
        /* Kiem tra dau vao co phai la so khong */
        isDigit = 1;
        for (i = 0; i < (int)strlen(choose); i++) {
            if (choose[i] < '0' || choose[i] > '9') {
                isDigit = 0;
                break;
            }
        }
        if (!isDigit) {
            printf("Please enter a number from 1 to 9!\n");
            continue;
        }
        
        choice = atoi(choose);
        if (choice < 1 || choice > 9) {
            printf("Only enter 1 to 9!\n");
            continue;
        }
        
        /* Bat buoc phai co chuyen xe truoc khi thuc hien cac chuc nang khac */
        if (choice != 1 && choice != 9 && nTrips == 0) {
            printf("No data available! Please Add Trip first (Option 1).\n");
            continue;
        }
       
        /* Dieu huong den cac chuc nang tuong ung */
        switch (choice) {
            case 1: 
                addTrip(trips, &nTrips); 
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
                printf("Thank you for using the program!\n"); 
                break;
        }
    } while (choice != 9);
    
    return 0;
}


/* Xoa ky tu xuong dong o cuoi chuoi do fgets tao ra */
void removeNewline(char *str) {
    str[strcspn(str, "\n")] = 0;
}

/* Kiem tra chuoi co chua khoang trang hay khong (dung cho ID) */
int hasNoSpace(char str[]) {
    int len = strlen(str);
    int i;
    for (i = 0; i < len; i++) {
        if (isspace((unsigned char)str[i])) return 0;
    }
    return 1;
}

/* Kiem tra xem chuoi co bi bo trong hay chi toan khoang trang */
int isEmpty(char str[], int size) {
    int i;
    if (strlen(str) == 0) return 1;
    for (i = 0; i < (int)strlen(str); i++) {
        if (!isspace((unsigned char)str[i])) {
            return 0; /* Co ky tu khac khoang trang */
        }
    }
    return 1;
}

/* Tim vi tri (index) cua chuyen xe trong mang */
int findTripIndex(Trip trips[], int nTrips, const char *tripId) {
    int i;
    for (i = 0; i < nTrips; i++) {
        if (strcmp(trips[i].tripId, tripId) == 0) {
            return i;
        }
    }
    return -1;
}

/* Tim vi tri (index) cua ve trong mang */
int findTicketIndex(Ticket tickets[], int nTickets, const char *ticketId) {
    int i;
    for (i = 0; i < nTickets; i++) {
        if (strcmp(tickets[i].ticketId, ticketId) == 0) {
            return i;
        }
    }
    return -1;
}

/* Kiem tra tinh hop le cua ngay thang (dd/mm/yyyy) */
int isValidDate(const char *date) {
    if (strlen(date) != 10){ 
		return 0;
	}
    if (date[2] != '/' || date[5] != '/'){ 
		return 0;
	}

    int dd = (date[0]-'0')*10 + (date[1]-'0');
    int mm = (date[3]-'0')*10 + (date[4]-'0');
    int yyyy = (date[6]-'0')*1000 + (date[7]-'0')*100 + (date[8]-'0')*10 + (date[9]-'0');

    if (dd < 1 || dd > 31) return 0;
    if (mm < 1 || mm > 12) return 0;
    if (yyyy < 1900) return 0;

    /* So ngay toi da trong tung thang */
    int maxDays[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    /* Kiem tra nam nhuan */
    if ((yyyy%4==0 && yyyy%100!=0) || (yyyy%400==0)) maxDays[2]=29;

    if (dd > maxDays[mm]) return 0;

    return 1;
}


/* Them mot chuyen xe moi vao he thong */
void addTrip(Trip trips[], int *tripCount) {
    Trip c1; /* Bien tam de luu thong tin chuyen xe dang nhap */
    int i, isDuplicate, hopLe;
    char buf[20]; /* Bo dem de xu ly nhap so an toan */

    /* Can kiem tra ky: khong de trong, khong co khoang trang, khong trung ID cu */
    while (1) {
        printf("Enter Trip ID: ");
        fgets(c1.tripId, sizeof(c1.tripId), stdin);
        removeNewline(c1.tripId); /* Xoa ky tu \n do fgets de lai */
        
        /* Kiem tra 1: ID khong duoc rong */
        if (strlen(c1.tripId) == 0) {
            printf("ID cannot be empty!\n");
            continue;
        }
        
        /* Kiem tra 2: ID khong duoc chua khoang trang (dung cho viec tim kiem sau nay) */
        if (!hasNoSpace(c1.tripId)) {
            printf("ID cannot contain spaces!\n");
            continue;
        }
        
        /* Kiem tra 3: Quet toan bo mang de dam bao ID la duy nhat */
        isDuplicate = 0;
        for (i = 0; i < *tripCount; i++) {
            if (strcmp(trips[i].tripId, c1.tripId) == 0) {
                printf("ID already exists!\n");
                isDuplicate = 1;
                break;
            }
        }
        /* Neu khong trung lap thi thoat vong lap nhap ID */
        if (!isDuplicate) break;
    }

    /* Su dung fgets de doc ca dong, bao gom khoang trang o ten dia diem */
    
    /* Nhap ten diem di */
    while (1) {
        printf("Enter Departure Name: ");
        fgets(c1.departure.name, 50, stdin);
        removeNewline(c1.departure.name);
        /* Ham isEmpty giup kiem tra nguoi dung chi nhap toan dau cach */
        if (!isEmpty(c1.departure.name, 50)) break;
        else printf("Invalid departure name.\n");
    }
   
    /* Nhap dia chi diem di */
    while (1) {
        printf("Enter Departure Address: ");
        fgets(c1.departure.address, 100, stdin);
        removeNewline(c1.departure.address);
        if (!isEmpty(c1.departure.address, 100)) break;
        else printf("Invalid departure address.\n");
    }
   
    /* Nhap ten diem den */
    while (1) {
        printf("Enter Destination Name: ");
        fgets(c1.destination.name, 50, stdin);
        removeNewline(c1.destination.name);
        if (!isEmpty(c1.destination.name, 50)) break;
        else printf("Invalid destination name.\n");
    }
   
    /* Nhap dia chi diem den */
    while (1) {
        printf("Enter Destination Address: ");
        fgets(c1.destination.address, 100, stdin);
        removeNewline(c1.destination.address);
        if (!isEmpty(c1.destination.address, 100)) break;
        else printf("Invalid destination address.\n");
    }
  
    /* Bat buoc phai dung dinh dang de thuan tien cho viec bao cao sau nay */
    while (1) {
	    printf("Enter Date (dd/mm/yyyy): ");
	    fgets(c1.date, 20, stdin);
	    removeNewline(c1.date);
	    /* Goi ham isValidDate de kiem tra logic ngay thang nam va nam nhuan */
	    if (isValidDate(c1.date)) break;
	    printf("Invalid date format. Please try again!\n");
	}

    /* Khong dung scanf truc tiep de tranh bi troi lenh hoac loi khi nhap chu */
    while (1) {
        printf("Enter Total Seats: ");
        fgets(buf, sizeof(buf), stdin); /* Doc vao chuoi truoc */
        removeNewline(buf);
        if (strlen(buf) == 0) continue;
        
        /* Kiem tra tung ky tu xem co phai la so khong */
        hopLe = 1;
        for (i = 0; i < (int)strlen(buf); i++) {
            if (buf[i] < '0' || buf[i] > '9') {
                hopLe = 0;
                break;
            }
        }
        if (!hopLe) {
            printf("Please enter a positive number!\n");
            continue;
        }
        
        /* Chuyen doi tu chuoi sang so nguyen */
        c1.totalSeats = atoi(buf);
        
        /* Logic nghiep vu: Xe phai co it nhat 1 ghe */
        if (c1.totalSeats <= 0) {
            printf("Seats must be > 0!\n");
            continue;
		}
     
    c1.bookedSeats = 0; /* Khoi tao so ghe da dat la 0 */
    
    /* Luu vao mang tai vi tri hien tai (*tripCount) */
    trips[*tripCount] = c1;
    
    /* Tang bien dem tong so chuyen xe len 1 (thay doi gia tri cua bien trong main) */
    (*tripCount)++;
    
    printf("Add Trip Successful!\n");
}

/* Cap nhat thong tin chuyen xe da co */
void updateTrip(Trip trips[], int nTrips) {
    char id[20];
    int index = -1; /* Luu vi tri cua chuyen xe tim thay trong mang */
    int choice, newSeats;
    char s[150]; /* Bien tam de nhap du lieu moi */
    char choiceStr[20];
    
    /* --- KHOI 1: TIM KIEM CHUYEN XE CAN SU --- */
    /* Bat buoc nguoi dung phai nhap dung ID thi moi cho phep sua */
    while (1) {
        printf("Enter Trip ID to update: ");
        fgets(id, sizeof(id), stdin);
        removeNewline(id);
        
        /* Kiem tra khong duoc de trong ID */
        if (isEmpty(id, 20)) {
            printf("ID cannot be empty!\n");
            continue;
        }
        
        /* Tim ID trong mang, neu thay tra ve index, khong thay tra ve -1 */
        index = findTripIndex(trips, nTrips, id);
        if (index != -1) break; /* Tim thay thi thoat vong lap */
        
        printf("Trip ID not found!\n");
    }
    
    /* --- KHOI 2: MENU CAP NHAT CHI TIET --- */
    /* Dung vong lap de cho phep sua nhieu thong tin lien tuc ma khong can nhap lai ID */
    while (1) {
        printf("\n+------ UPDATE TRIP %s --------+\n", trips[index].tripId);
        printf("|1. Departure Name              |\n");
        printf("|2. Departure Address           |\n");
        printf("|3. Destination Name            |\n");
        printf("|4. Destination Address         |\n");
        printf("|5. Date                        |\n");
        printf("|6. Total Seats                 |\n");
        printf("|0. Exit Update                 |\n");
        printf("+-------------------------------+\n");
        printf("Enter choice (0-6): ");
        
        /* Nhap lua chon an toan bang fgets */
        fgets(choiceStr, sizeof(choiceStr), stdin);
        removeNewline(choiceStr);
        if (isEmpty(choiceStr, sizeof(choiceStr))) continue;
        choice = atoi(choiceStr);
        
        /* Chon 0 de thoat khoi menu cap nhat */
        if (choice == 0) break;
        
        switch (choice) {
            /* Chi cap nhat neu nguoi dung nhap noi dung moi. 
               Neu de trong (enter luon) thi giu nguyen thong tin cu. */
            case 1:
                printf("Enter new Departure Name: ");
                fgets(s, sizeof(s), stdin);
                removeNewline(s);
                if (!isEmpty(s, 50)) strcpy(trips[index].departure.name, s);
                break;
            case 2:
                printf("Enter new Departure Address: ");
                fgets(s, sizeof(s), stdin);
                removeNewline(s);
                if (!isEmpty(s, 100)) strcpy(trips[index].departure.address, s);
                break;
            case 3:
                printf("Enter new Destination Name: ");
                fgets(s, sizeof(s), stdin);
                removeNewline(s);
                if (!isEmpty(s, 50)) strcpy(trips[index].destination.name, s);
                break;
            case 4:
                printf("Enter new Destination Address: ");
                fgets(s, sizeof(s), stdin);
                removeNewline(s);
                if (!isEmpty(s, 100)) strcpy(trips[index].destination.address, s);
                break;
                
           case 5: {
                /* Can tao block {} vi khai bao bien tmpDate ben trong switch case */
			    char tmpDate[20];
			    printf("Enter new Date (dd/mm/yyyy): ");
			    fgets(tmpDate, sizeof(tmpDate), stdin);
			    removeNewline(tmpDate);
			    
			    /* Vong lap kiem tra tinh hop le cua ngay thang (dd/mm/yyyy) */
			    while (!isValidDate(tmpDate)) {
			        printf("Invalid date! Format dd/mm/yyyy: ");
			        fgets(tmpDate, sizeof(tmpDate), stdin);
			        removeNewline(tmpDate);
			    }
			    strcpy(trips[index].date, tmpDate);
			    break;
			}

            case 6:
                printf("Enter new Total Seats: ");
                fgets(s, sizeof(s), stdin);
                removeNewline(s);
                newSeats = atoi(s);
                
               
                if (newSeats >= trips[index].bookedSeats) {
                    trips[index].totalSeats = newSeats;
                } else {
                    printf("Cannot be less than booked seats (%d)!\n", trips[index].bookedSeats);
                }
                break;
                
            default:
                printf("Invalid choice!\n");
                continue;
        }
        printf("Update successful!\n");
    }
}

/* Dat ve xe moi */
void buyTicket(Trip trips[], int nTrips, Ticket tickets[], int *nTickets) {
    char id[20];               // Khai bao bien luu ID chuyen xe
    int index = -1;            // Bien luu vi tri cua chuyen xe tim thay
    Ticket tk;                 // Khai bao bien cau truc ve de luu thong tin ve moi
    char payStr[20];           // Bien tam de nhap lua chon thanh toan
    int payChoice;             // Bien luu lua chon thanh toan (so nguyen)
    
    if (nTrips == 0) {         // Kiem tra neu chua co chuyen xe nao trong he thong
        printf("No trips available!\n"); // Thong bao khong co chuyen xe
        return;                // Ket thuc ham
    }
    
    /* Chon chuyen xe va kiem tra cho trong */
    while (1) {                // Vong lap de nhap ID chuyen xe cho den khi hop le
        printf("Enter Trip ID: ");       // Yeu cau nhap ID chuyen xe
        fgets(id, 20, stdin);            // Nhap chuoi ID tu ban phim
        removeNewline(id);               // Xoa ky tu xuong dong o cuoi chuoi
        if (isEmpty(id, 20)) continue;   // Neu chuoi rong thi yeu cau nhap lai

        index = findTripIndex(trips, nTrips, id); // Tim vi tri chuyen xe trong mang
        if (index == -1) {               // Neu khong tim thay chuyen xe (index la -1)
            printf("Trip not found!\n"); // Thong bao khong tim thay
            continue;                    // Quay lai dau vong lap
        }
        // Kiem tra so ghe da dat co vuot qua tong so ghe khong
        if (trips[index].bookedSeats >= trips[index].totalSeats) {
            printf("Trip is full! Cannot book ticket.\n"); // Thong bao xe da day
            return;                      // Ket thuc ham, khong dat duoc ve
        }
        break;                           // ID hop le va con cho, thoat vong lap
    }
    
    strcpy(tk.tripId, id);     // Copy ID chuyen xe vua nhap vao thong tin ve
    
    /* Nhap ma ve va kiem tra trung lap */
    while (1) {                // Vong lap nhap ID ve
        printf("Enter Ticket ID: ");     // Yeu cau nhap ID ve
        fgets(tk.ticketId, 20, stdin);   // Nhap ID ve tu ban phim
        removeNewline(tk.ticketId);      // Xoa ky tu xuong dong
        if (strlen(tk.ticketId) == 0) {  // Kiem tra neu ID ve bi bo trong
            printf("Ticket ID cannot be empty!\n"); // Thong bao khong duoc de trong
            continue;                    // Nhap lai
        }
        // Kiem tra xem ID ve da ton tai trong danh sach tickets chua
        if (findTicketIndex(tickets, *nTickets, tk.ticketId) != -1) {
            printf("Ticket ID already exists! Try another.\n"); // Thong bao ID da ton tai
            continue;                    // Nhap lai
        }
        break;                           // ID hop le, thoat vong lap
    }
    
    /* Nhap thong tin khach hang */
    while (1) {                // Vong lap nhap ten khach hang
        printf("Enter Passenger Name: ");      // Yeu cau nhap ten
        fgets(tk.passenger.name, 50, stdin);   // Nhap ten vao bien cau truc
        removeNewline(tk.passenger.name);      // Xoa xuong dong
        if (!isEmpty(tk.passenger.name, 50)) break; // Neu ten khong rong thi thoat lap
        printf("Invalid name!\n");             // Thong bao ten khong hop le
    }
    
    while (1) {                // Vong lap nhap so dien thoai
        printf("Enter Passenger Phone: ");     // Yeu cau nhap SDT
        fgets(tk.passenger.phone, 15, stdin);  // Nhap SDT
        removeNewline(tk.passenger.phone);     // Xoa xuong dong
        if (!isEmpty(tk.passenger.phone, 15)) break; // Neu SDT khong rong thi thoat lap
        printf("Invalid phone!\n");            // Thong bao SDT khong hop le
    }
    
    strcpy(tk.date, trips[index].date);        // Lay ngay di tu thong tin chuyen xe gan vao ve
    tk.seatNumber = trips[index].bookedSeats + 1; // Tu dong gan so ghe tiep theo
    tk.price = 100000;         // Gan gia ve co dinh la 100000 (vi du)
    tk.status = 0;             // Dat trang thai ve la 0 (Hoat dong)
    
    /* Chon trang thai thanh toan */
    while (1) {                // Vong lap chon phuong thuc thanh toan
        printf("Payment (1. Now, 2. Later): "); // Hoi thanh toan ngay hay sau
        fgets(payStr, sizeof(payStr), stdin);   // Nhap lua chon duoi dang chuoi
        removeNewline(payStr);                  // Xoa xuong dong
        payChoice = atoi(payStr);               // Chuyen chuoi thanh so nguyen
        if (payChoice == 1 || payChoice == 2) break; // Neu chon 1 hoac 2 thi hop le
        printf("Please enter 1 or 2!\n");       // Yeu cau nhap lai neu sai
    }
    
    tk.paymentStatus = (payChoice == 1) ? 1 : 0; // Gan trang thai thanh toan (1: Da tra, 0: Chua)
        
    tickets[*nTickets] = tk;   // Luu thong tin ve vua tao vao mang tickets
    (*nTickets)++;             // Tang tong so luong ve len 1
    trips[index].bookedSeats++; // Tang so luong ghe da dat cua chuyen xe len 1
    
    // In thong bao dat ve thanh cong kem so ghe va gia tien
    printf("Booking Successful! Seat %d. Price: %.0lf\n", tk.seatNumber, tk.price);
}

/* Hien thi danh sach cac ve da dat */
void checkTicket(Ticket tickets[], int nTickets) {
    int i;                     // Bien chay vong lap
    const char *statusStr;     // Con tro chuoi de luu ten trang thai
    
    if (nTickets == 0) {       // Kiem tra neu danh sach ve rong
        printf("No tickets found!\n"); // Thong bao khong co ve
        return;                // Ket thuc ham
    }
    
    for (i = 0; i < nTickets; i++) { // Duyet qua tat ca cac ve trong mang
        /* Chuyen doi trang thai so sang chuoi */
        switch (tickets[i].status) { // Kiem tra gia tri status (int)
            case 0: statusStr = "Active"; break;    // 0 la Hoat dong
            case 2: statusStr = "Cancelled"; break; // 2 la Da huy
            case 3: statusStr = "Locked"; break;    // 3 la Bi khoa
            default: statusStr = "Unknown";         // Cac gia tri khac la Khong xac dinh
        }
        
        // In thong tin chi tiet cua tung ve
        printf("\n===== TICKET %d =====\n", i + 1); // In tieu de so thu tu ve
        printf("Ticket ID : %s\n", tickets[i].ticketId);      // Ma ve
        printf("Trip ID   : %s\n", tickets[i].tripId);        // Ma chuyen xe
        printf("Customer  : %s\n", tickets[i].passenger.name);// Ten khach
        printf("Phone     : %s\n", tickets[i].passenger.phone);// SDT khach
        printf("Seat No   : %d\n", tickets[i].seatNumber);    // So ghe
        printf("Date      : %s\n", tickets[i].date);          // Ngay di
        printf("Price     : %.0lf\n", tickets[i].price);      // Gia tien
        // In trang thai thanh toan: 1 la Paid (Da tra), 0 la Unpaid (Chua tra)
        printf("Payment   : %s\n", tickets[i].paymentStatus ? "Paid" : "Unpaid");
        printf("Status    : %s\n", statusStr);                // Trang thai ve (chuoi da xu ly o tren)
    }
}

/* Liet ke cac chuyen xe (co phan trang) */
void listTrips(Trip trips[], int nTrips) {
    int pageSize, totalPages, pageNumber, start, end, i; // Khai bao cac bien phan trang
    char cmd[10];              // Bien luu lenh nguoi dung nhap (chuyen trang)
    
    if (nTrips == 0) {         // Kiem tra neu khong co chuyen xe nao
        printf("No trips to display.\n"); // Thong bao danh sach rong
        return;                // Ket thuc ham
    }
    
    pageSize = 10;             // Quy dinh so chuyen xe tren 1 trang la 10
    // Tinh tong so trang: cong thuc (n + size - 1) / size de lam tron len
    totalPages = (nTrips + pageSize - 1) / pageSize;
    pageNumber = 1;            // Bat dau tu trang 1
    
    while (1) {                // Vong lap hien thi trang cho den khi nguoi dung thoat
        start = (pageNumber - 1) * pageSize; // Tinh chi so bat dau cua trang hien tai
        end = start + pageSize;              // Tinh chi so ket thuc (ly thuyet)
        if (end > nTrips) end = nTrips;      // Neu vuot qua tong so, gan lai bang so luong thuc te
   
        // In phan dau bang va tieu de trang
        printf("+--------------------------------------------------------------------------------------+\n");
        printf("|                        TRIP LIST (Page %d / %d)                                      |\n", pageNumber, totalPages);
        printf("+----------+-------------------------+-------------------------+------------+----------+\n");
        printf("| Trip ID  | Departure               | Destination             | Date       | Seats(Bk)|\n");
        printf("+----------+-------------------------+-------------------------+------------+----------+\n");
        
        // Duyet va in cac chuyen xe thuoc trang hien tai (tu start den end)
        for (i = start; i < end; i++) {
            // In thong tin chuyen xe theo cot duoc can chinh (format string)
            printf("| %-8s | %-23s | %-23s | %-10s | %3d/%-3d  |\n",
                   trips[i].tripId,             // ID chuyen
                   trips[i].departure.name,     // Diem di
                   trips[i].destination.name,   // Diem den
                   trips[i].date,               // Ngay di
                   trips[i].bookedSeats, trips[i].totalSeats); // Ghe da dat / Tong ghe
        }
        printf("+--------------------------------------------------------------------------------------+\n");
        
        // Huong dan nguoi dung nhap lenh chuyen trang
        printf("Enter 1 (Next Page), 0 (Back to Menu): ");
        fgets(cmd, sizeof(cmd), stdin);    // Nhap lenh tu ban phim
        removeNewline(cmd);                // Xoa ky tu xuong dong
        
        // Xu ly logic chuyen trang
        if (cmd[0] == '1' && pageNumber < totalPages) pageNumber++; // Neu chon 1 va chua phai trang cuoi -> Tang trang
        else if (cmd[0] == '0') break;     // Neu chon 0 -> Thoat khoi vong lap (ve menu)
        else if (cmd[0] == '1') printf("This is the last page!\n"); // Thong bao neu dang o trang cuoi ma van chon Next
    }
}

/* Thanh toan cho mot ve */
void payTicket(Ticket tickets[], int nTickets) {
    char id[20];               // Khai bao bien luu ID ve nhap vao
    int found, i;              // Bien co danh dau va bien chay vong lap
    
    printf("Enter Ticket ID to pay: "); // Yeu cau nhap ID ve can thanh toan
    fgets(id, 20, stdin);      // Nhap chuoi ID tu ban phim
    removeNewline(id);         // Xoa ky tu xuong dong thua
    
    found = 0;                 // Khoi tao co tim thay la 0 (chua tim thay)
    for (i = 0; i < nTickets; i++) { // Duyet qua danh sach cac ve
        if (strcmp(tickets[i].ticketId, id) == 0) { // So sanh ID trong mang voi ID nhap vao
            found = 1;         // Danh dau da tim thay ve
            /* Kiem tra cac dieu kien chan thanh toan */
            if (tickets[i].status == 2) { // Neu trang thai la 2 (Da huy)
                printf("Cannot pay for Cancelled ticket!\n"); // Thong bao khong the thanh toan
            } else if (tickets[i].status == 3) { // Neu trang thai la 3 (Bi khoa)
                printf("Cannot pay for Locked ticket!\n");    // Thong bao khong the thanh toan
            } else if (tickets[i].paymentStatus == 1) { // Neu trang thai thanh toan la 1 (Da tra)
                printf("This ticket is already paid!\n");     // Thong bao da thanh toan roi
            } else {           // Neu ve hop le va chua thanh toan
                tickets[i].paymentStatus = 1; // Cap nhat trang thai thanh toan thanh 1 (Da tra)
                // In thong bao thanh cong va so tien
                printf("Payment successful for Ticket %s. Amount: %.0lf\n", id, tickets[i].price);
            }
            break;             // Tim thay va xu ly xong thi thoat vong lap
        }
    }
    if (!found) printf("Ticket ID not found!\n"); // Neu duyet het ma khong thay thi bao loi
}

/* Quan ly trang thai ve (Huy, Khoa, Mo khoa) */
void manageTicketStatus(Ticket tickets[], int nTickets, Trip trips[], int nTrips) {
    char id[20];               // Bien luu ID ve
    int index, tripIndex, choice; // Bien luu vi tri ve, vi tri chuyen xe, va lua chon menu
    char choiceStr[20];        // Bien tam de nhap lua chon
    
    if (nTickets == 0) {       // Kiem tra neu danh sach ve rong
        printf("No tickets in system!\n"); // Thong bao khong co ve
        return;                // Ket thuc ham
    }

    printf("Enter Ticket ID to manage: "); // Yeu cau nhap ID ve can quan ly
    fgets(id, sizeof(id), stdin); // Nhap ID
    removeNewline(id);         // Xoa xuong dong

    index = findTicketIndex(tickets, nTickets, id); // Tim vi tri cua ve trong mang tickets
    if (index == -1) {         // Neu khong tim thay
        printf("Ticket not found!\n"); // Thong bao loi
        return;                // Ket thuc ham
    }

    // Tim vi tri chuyen xe tuong ung voi ve de cap nhat so ghe
    tripIndex = findTripIndex(trips, nTrips, tickets[index].tripId);

    // Hien thi menu quan ly cho ve vua tim thay
    printf("\n+================================+\n");
    printf("|   MANAGE TICKET STATUS         |\n");
    printf("+================================+\n");
    printf("| Ticket ID : %-20s|\n", tickets[index].ticketId); // Hien thi ID ve
    printf("| Current Status: ");  // Hien thi trang thai hien tai
    switch (tickets[index].status) {
        case 0: printf("Active        |\n"); break; // 0: Hoat dong
        case 2: printf("Cancelled     |\n"); break; // 2: Da huy
        case 3: printf("Locked        |\n"); break; // 3: Bi khoa
        default: printf("Unknown       |\n");       // Khac: Khong xac dinh
    }
    printf("+--------------------------------+\n");
    printf("| 1. Cancel Ticket               |\n"); // Lua chon 1: Huy ve
    printf("| 2. Re-open Ticket (if Cancel)  |\n"); // Lua chon 2: Mo lai ve da huy
    printf("| 3. Lock Ticket                 |\n"); // Lua chon 3: Khoa ve
    printf("| 4. Unlock Ticket               |\n"); // Lua chon 4: Mo khoa ve
    printf("| 0. Exit                        |\n"); // Lua chon 0: Thoat
    printf("+--------------------------------+\n");
    printf("Select: ");        // Nhap lua chon
    
    fgets(choiceStr, sizeof(choiceStr), stdin); // Nhap chuoi lua chon
    removeNewline(choiceStr);  // Xoa xuong dong
    choice = atoi(choiceStr);  // Chuyen thanh so nguyen

    switch (choice) {
        case 1: /* Huy ve */
            if (tickets[index].status == 2) { // Neu ve da huy roi
                printf("Ticket is already Cancelled!\n");
            } else if (tickets[index].status == 3) { // Neu ve dang bi khoa
                printf("Ticket is Locked, cannot cancel. Unlock first!\n"); // Phai mo khoa truoc
            } else { // Hop le de huy
                tickets[index].status = 2; // Dat trang thai ve la 2 (Huy)
                // Neu chuyen xe con ton tai va so ghe da dat > 0
                if (tripIndex != -1 && trips[tripIndex].bookedSeats > 0) {
                    trips[tripIndex].bookedSeats--; /* Giam so ghe da dat cua chuyen xe di 1 */
                }
                /* Hoan tien neu da thanh toan */
                if (tickets[index].paymentStatus == 1) { // Neu khach da tra tien
                    printf("Ticket paid. Refund amount: %.0lf\n", tickets[index].price); // Thong bao hoan tien
                    tickets[index].paymentStatus = 0; // Dat lai trang thai la chua thanh toan
                }
                printf("Ticket Cancelled successfully!\n"); // Thong bao huy thanh cong
            }
            break;

        case 2: /* Mo lai ve da huy */
            if (tickets[index].status != 2) { // Neu ve khong phai trang thai Huy
                printf("Ticket is not in Cancelled status!\n");
            } else {
                if (tripIndex == -1) { // Neu chuyen xe khong con ton tai
                    printf("Associated Trip no longer exists!\n");
                } else if (trips[tripIndex].bookedSeats >= trips[tripIndex].totalSeats) { // Neu xe da day
                    printf("Trip is full, cannot re-open ticket!\n"); // Khong the mo lai
                } else { // Hop le de mo lai
                    tickets[index].status = 0; // Dat lai trang thai ve la 0 (Hoat dong)
                    trips[tripIndex].bookedSeats++; // Tang lai so ghe da dat
                    printf("Ticket Re-opened successfully!\n"); // Thong bao thanh cong
                }
            }
            break;

        case 3: /* Khoa ve */
            if (tickets[index].status == 3) { // Neu ve da khoa roi
                printf("Ticket is already Locked!\n");
            } else if (tickets[index].status == 2) { // Neu ve da huy
                printf("Cannot lock a Cancelled ticket!\n"); // Khong khoa duoc ve huy
            } else { // Hop le de khoa
                tickets[index].status = 3; // Dat trang thai ve la 3 (Khoa)
                printf("Ticket Locked successfully!\n");
            }
            break;

        case 4: /* Mo khoa ve */
            if (tickets[index].status != 3) { // Neu ve khong bi khoa
                printf("Ticket is not Locked!\n");
            } else { // Hop le de mo khoa
                tickets[index].status = 0; // Dat lai trang thai ve la 0 (Hoat dong)
                printf("Ticket Unlocked successfully!\n");
            }
            break;

        case 0: // Thoat
            printf("Exiting management.\n");
            break;

        default: // Nhap sai
            printf("Invalid choice!\n");
    }
}


/* Bao cao tong doanh thu */
void reportRevenue(Ticket tickets[], int nTickets) {
    double totalPaid, totalUnpaid;     // Tong doanh thu thuc te, tong doanh thu du kien
    int paidCount, unpaidCount, cancelledCount, i; // So luong ve da thanh toan, chua TT, da huy, bien chay
    
    // Khoi tao cac bien dem va tong bang 0
    totalPaid = 0;
    totalUnpaid = 0;
    paidCount = 0;
    unpaidCount = 0;
    cancelledCount = 0;
    
    for (i = 0; i < nTickets; i++) {   // Duyet qua tat ca cac ve
        if (tickets[i].status == 2) {  // Neu ve bi huy (status = 2)
            cancelledCount++;          // Tang so luong ve da huy
        } else if (tickets[i].paymentStatus == 1) { // Neu ve da thanh toan
            paidCount++;               // Tang so luong ve da thanh toan
            totalPaid += tickets[i].price; // Cong don vao tong doanh thu thuc te
        } else {                       // Neu ve chua thanh toan (status = 0 hoac 3)
            unpaidCount++;             // Tang so luong ve chua thanh toan
            totalUnpaid += tickets[i].price; // Cong don vao tong doanh thu du kien
        }
    }
    
    // In thong tin bao cao tong the
    printf("\n+================================================+\n");
    printf("|              REVENUE REPORT                    |\n");
    printf("+================================================+\n");
    printf("| Total Tickets         : %-23d |\n", nTickets);     // Tong so ve
    printf("| Paid Tickets          : %-3d  | %.0lf VND\n", paidCount, totalPaid); // Ve da tra, doanh thu thuc te // Ve chua tra, doanh thu du kien
    printf("| Unpaid Tickets        : %-3d  | %.0lf VND (Expected)\n", unpaidCount, totalUnpaid);
    printf("| Cancelled              : %-23d |\n", cancelledCount); // Ve da huy
    printf("+------------------------------------------------+\n");
    printf("| ACTUAL REVENUE        : %.0lf VND\n", totalPaid); // Tong doanh thu thuc te cuoi cung
    printf("+================================================+\n");
}

/* Bao cao theo tung chuyen xe */
void reportByTrip(Ticket tickets[], int nTickets, Trip trips[], int nTrips) {
    int i, j;                  // Bien chay vong lap (i cho Trip, j cho Ticket)
    double rev;                // Bien luu doanh thu cua chuyen xe hien tai
    int soldCount;             // Bien luu so ve da ban cua chuyen xe hien tai
    
    if (nTrips == 0) {         // Kiem tra neu khong co chuyen xe nao
        printf("No trips to report.\n");
        return;
    }
    
    // In tieu de bang bao cao
    printf("\n+====================================================================================+\n");
    printf("|                                REPORT BY TRIP                                      |\n");
    printf("+----------+--------------------+--------------------+------------+------+----------+\n");
    printf("| Trip ID  | Departure          | Destination        | Date       | Sold | Revenue  |\n");
    printf("+----------+--------------------+--------------------+------------+------+----------+\n");
    
    for (i = 0; i < nTrips; i++) { // Vong lap ngoai: Duyet qua tung chuyen xe
        rev = 0;                   // Reset tong doanh thu cho chuyen xe moi
        soldCount = 0;             // Reset so luong ve da ban cho chuyen xe moi
        
        for (j = 0; j < nTickets; j++) { // Vong lap trong: Duyet qua tat ca cac ve
            /* Chi tinh ve cua chuyen nay va khong bi huy */
            // So sanh ID chuyen xe va kiem tra ve khong bi huy
            if (strcmp(tickets[j].tripId, trips[i].tripId) == 0 && tickets[j].status != 2) {
                soldCount++;       // Tang so luong ve da ban (ve con active/locked)
                if (tickets[j].paymentStatus == 1) { // Neu ve da thanh toan
                    rev += tickets[j].price; // Cong don vao doanh thu thuc te
                }
            }
        }
        
        // In ket qua bao cao cho chuyen xe hien tai
        printf("| %-8s | %-18s | %-18s | %-10s | %4d | %8.0lf |\n",
               trips[i].tripId,             // Ma chuyen xe
               trips[i].departure.name,     // Diem di
               trips[i].destination.name,   // Diem den
               trips[i].date,               // Ngay
               soldCount,                   // So ve da ban (active/locked)
               rev);                        // Tong doanh thu thuc te
    }
    
    printf("+====================================================================================+\n");
}

/* Bao cao theo ngay */
void reportByDate(Ticket tickets[], int nTickets) {
    char dates[500][20];       // Mang de luu danh sach cac ngay duy nhat
    double revs[500];          // Mang luu tong doanh thu tuong ung voi tung ngay
    int counts[500];           // Mang luu tong so ve ban duoc tuong ung voi tung ngay
    int nDates, i, j, found;   // Bien dem so ngay duy nhat va cac bien chay
    
    if (nTickets == 0) {       // Kiem tra neu khong co du lieu ve
        printf("No data to report.\n");
        return;
    }
    
    nDates = 0;                // Khoi tao so luong ngay duy nhat bang 0
    
    for (i = 0; i < nTickets; i++) { // Duyet qua tat ca cac ve
        if (tickets[i].status == 2) continue; /* Bo qua ve da huy */
        
        found = -1;                // Khoi tao chi so tim thay
        for (j = 0; j < nDates; j++) { // Vong lap tim xem ngay nay da ton tai trong mang dates chua
            if (strcmp(dates[j], tickets[i].date) == 0) { // Neu ngay da ton tai
                found = j;             // Luu lai vi tri cua ngay do
                break;                 // Thoat vong lap tim kiem
            }
        }
        
        if (found == -1) {         
            strcpy(dates[nDates], tickets[i].date); // Them ngay moi vao danh sach
            counts[nDates] = 1;        // Bat dau dem so ve cho ngay moi la 1
            // Neu ve da thanh toan, cong gia ve, neu khong thi la 0
            revs[nDates] = (tickets[i].paymentStatus == 1) ? tickets[i].price : 0;
            nDates++;                  // Tang so luong ngay duy nhat
        } else {                   /
            counts[found]++;           // Tang so luong ve cho ngay do
            if (tickets[i].paymentStatus == 1) { // Neu ve nay da thanh toan
                revs[found] += tickets[i].price; // Cong don doanh thu thuc te
            }
        }
    }
    
    // In bao cao theo ngay
    printf("\n+=====================================================+\n");
    printf("|             DAILY REVENUE REPORT                    |\n");
    printf("+----------------+---------------+--------------------+\n");
    printf("| Date           | Tickets       | Revenue            |\n");
    printf("+----------------+---------------+--------------------+\n");
    
    for (i = 0; i < nDates; i++) { // Duyet qua danh sach cac ngay duy nhat da tong hop
        printf("| %-14s | %-13d | %18.0lf |\n", dates[i], counts[i], revs[i]); 
    }
    
    printf("+=====================================================+\n");
}

/* Menu bao cao */
void reportMenu(Ticket tickets[], int nTickets, Trip trips[], int nTrips) {
    int type;                  // Bien luu lua chon loai bao cao
    char typeStr[20];          // Bien tam de nhap lua chon
    
    if (nTickets == 0) {       
        printf("No ticket data available for report.\n");
        return;
    }

    // Hien thi menu bao cao
    printf("\n+========================================+\n");
    printf("|          STATISTICS & REPORTS          |\n");
    printf("+========================================+\n");
    printf("| 1. Total Revenue Report                |\n"); // Bao cao tong the
    printf("| 2. Report by Trip                      |\n"); // Bao cao theo chuyen xe
    printf("| 3. Report by Date                      |\n"); // Bao cao theo ngay
    printf("| 0. Exit Report                         |\n"); // Thoat
    printf("+----------------------------------------+\n");
    printf("Select report type: ");
    
    fgets(typeStr, sizeof(typeStr), stdin); // Nhap lua chon duoi dang chuoi
    removeNewline(typeStr);    
    type = atoi(typeStr);     

    switch (type) {            
        case 1:
            reportRevenue(tickets, nTickets); // Goi bao cao tong doanh thu
            break;
        case 2:
            reportByTrip(tickets, nTickets, trips, nTrips); // Goi bao cao theo chuyen xe
            break;
        case 3:
            reportByDate(tickets, nTickets); // Goi bao cao theo ngay
            break;
        case 0:
            printf("Exiting reports.\n"); 
            break;
        default:
            printf("Invalid choice!\n");
    }
}

