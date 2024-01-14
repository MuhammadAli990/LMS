#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
using namespace std;

string date;
int totalstudents = 11;
int totalbooks = 15;
struct student{
    string name;
    string regno;
    float cgpa;
    int booksissued;
    int bookstoreturn;
    int finepaid;
    int finetopay;
    string date;
};
student students[11];
string books[15];


void checkrecord(){
        // checking students record:
        bool found=false;
        string registrationno;
        cout<<"Enter the registration number of the student: ";
        cin>>registrationno;
        cout<<endl;
        // checking registration number exists or not:
        int temp =0;
        for(int i=0;i<totalstudents;i++){
            if(registrationno==students[i].regno){
                found=true;
                temp = i;
                break;
            }
        }
        // displaying record:
        if(found){
            cout<<"Name : "<<students[temp].name<<endl;
            cout<<"Registration no : "<<students[temp].regno<<endl;
            cout<<"CGPA : "<<students[temp].cgpa<<endl;
            cout<<"Total books ever issued (which are returned) : "<<students[temp].booksissued<<endl;
            cout<<"Total books to return : "<<students[temp].bookstoreturn<<endl;
            cout<<"Total fine ever paid : "<<students[temp].finepaid<<endl;
            cout<<"Fine pending : "<<students[temp].finetopay<<endl<<endl;
        }
        // student not found:
        else if(!found){
            cout<<"Student of "<<registrationno<<" not found."<<endl;
        }
}

void payfine(){
    bool found = false;
    string registrationno;
    cout<<"Enter the Reg. no of the student: ";
    cin>>registrationno;
    // converting to uppercase:
    for(int i=0;i<registrationno.length();i++){
        registrationno[i]=toupper(registrationno[i]);
    }
    
    // finding student exists or not
    int temp;
    for(int i=0;i<totalstudents;i++){
        if(registrationno==students[i].regno){
            found = true;
            temp = i;
            break;
        }
    }
    if(found){
        cout<<"The pending fine is: ";
        cout<<students[temp].finetopay<<endl;
        if(students[temp].finetopay==0){
            // program exits........
        }
        else if(students[temp].finetopay>0){
            int choice;
            cout<<"Press 1 to pay the fine: ";
            cin>>choice;
            if(choice==1){
                int fine = 0;
                cout<<"Enter the fine amount: ";
                cin>>fine;
                if(fine==students[temp].finetopay){
                    ifstream inFile("students.txt");
                    ofstream outFile("tempfile.txt");
                    string line1;
                    while (getline(inFile, line1)) {
                        istringstream iss(line1);
                        string a,reg,c,d,e,f,g,h;
                        iss >> a>>reg>>c>>d>>e>>f>>g>>h;
                        if (reg == registrationno) {
                            g="0";
                            int ff = fine+stoi(f);
                            f = to_string(ff);
                            outFile <<a<< " " <<reg<< " " <<c<<" "<<d<<" "<<e<<" "<<ff<<" "<<g<<" "<<h<<endl;
                        } 
                        else {
                        outFile << line1 << endl;
                    }}

                    inFile.close();
                    outFile.close();
                    remove("students.txt");
                    rename("tempfile.txt", "students.txt");
                    cout<<"The fine has been paid."<<endl;
            }
            else {
                cout<<"Insufficent amount or invalid input."<<endl;
            }
        }
        }
    }
    else if(!found){
        cout<<"Student not found.";
    }

}

void depositbook(){
    int prompt;
    bool found = false;
    cout<<"Enter 1 to deposit a new book.\n";
    cout<<"Enter 2 to deposit a returned book.\n";
    cin>>prompt;
    cin.ignore();
    // Depositing a New book in the database:
    if(prompt==1){
    string bookname;
    cout<<"Enter the name of the book you want to deposit: ";
    getline(cin,bookname);
    // converting bookname into lowercase:
    for(int i=0;i<bookname.length();i++){
        bookname[i]=tolower(bookname[i]);
    }
    // saving book in the data base:
    ofstream booksFile("books.txt",ios::app);
    if (booksFile.is_open()) { 
        booksFile << bookname<<"\n";
        booksFile.close(); 
    }
    cout<<"Book has been deposited."<<endl;
    }
    // Depositing a returned book in the database:
    else if(prompt==2){
        string bookname;
        cout<<"Enter the name of the book you want to deposit: ";
        getline(cin,bookname);
        // converting bookname into lowercase:
        for(int i=0;i<bookname.length();i++){
            bookname[i]=tolower(bookname[i]);
        }
        // checking it is a returned book or not:
        ifstream issuedFile("issuedbooks.txt");
        string line;
        string issuedate;
        if (issuedFile.is_open()) {
            while (getline(issuedFile, line)) {
                if (line == bookname) {
                    if (getline(issuedFile, line)) {
                        //  accessing the issue date of the book:
                        issuedate=line;
                        found = true;
                        break;
                    }
                    found = true;
                    break;
                }
            }
        }
        issuedFile.close();
        if(found){
            bool studentavailable=false;
            string registrationno;
            cout<<"Enter the registration number of the student (who is returning the book): ";
            getline(cin,registrationno);
            // cin.ignore();
            // converting regno to upper case:
            for(int i=0;i<registrationno.length();i++){
                registrationno[i]=toupper(registrationno[i]);
            }
            // checking student exists or not:
            for(int i=0;i<totalstudents;i++){
                if(registrationno==students[i].regno){
                    studentavailable=true;
                    break;
                }
            }

            if(studentavailable){
            // saving book in the database(books library):
            ofstream booksFile("books.txt",ios::app);
            if (booksFile.is_open()) { 
                booksFile << bookname<<"\n";
                booksFile.close(); 
            }
            // deleting book from the issued books db (also its date):
            ifstream inputFile("issuedbooks.txt");
            ofstream tempFile("tempfile.txt");
            string line;
            bool skipline = false;
            if (inputFile.is_open() && tempFile.is_open()) {
            while (getline(inputFile, line)) {
                if (skipline) {
                    skipline = false; 
                }
                else if (line.find(bookname) != string::npos) {
                    skipline = true;
                } 
                else {
                tempFile << line << "\n"; 
                }
        }
            inputFile.close();
            tempFile.close();
            remove("issuedbooks.txt");
            rename("tempfile.txt", "issuedbooks.txt");
            // adding the record of student that the book has been returned:
            ifstream inFile("students.txt");
            ofstream outFile("tempfile.txt");
            string line1;
            while (getline(inFile, line1)) {
                istringstream iss(line1);
                string a,reg,c,d,e,f,g,h;
                iss >> a>>reg>>c>>d>>e>>f>>g>>h;
                if (reg == registrationno) {
                    int decrementedValue = stoi(e) - 1;
                    h="0";
                    int nn=stoi(d)+1;
                    outFile << a << " " << reg << " " <<c<<" "<< nn <<" "<<decrementedValue<<" "<<f<<" "<<g<<" "<<h<<endl;
                } else {
                    outFile << line1 << endl;
                }}
            
            inFile.close();
            outFile.close();
            remove("students.txt");
            rename("tempfile.txt", "students.txt");

            // telling fine on the returned book (if returned late):

            string currentdate=date;

            int month = stoi(issuedate.substr(0, 2));
            int day = stoi(issuedate.substr(2, 2));

            int currentmonth = stoi(currentdate.substr(0, 2));
            int currentday = stoi(currentdate.substr(2, 2));

            int duetotaldays = month * 30 + day;
            int currenttotaldays = currentmonth * 30 + currentday;

            if (currenttotaldays > duetotaldays) {
                int delaydays = currenttotaldays - duetotaldays - 1; 
                if (delaydays > 7) {
                    int fineAmount = ((delaydays -7) * 10)+10;

                    cout << "Book is delayed by more than 7 days." << endl;
                    cout << "Fine amount on this book: " << fineAmount << " rupees." << endl;
                } 
                else {
                    cout << "Book is delayed, but less than 7 days." << endl;
                }
            } 
            else {
                cout << "Book is not delayed." << endl;
            }
            cout<<"Book has been deposited."<<endl;

            }}
            else{
                cout<<"Student not found."<<endl;
            }
            
        }}
        // no book found in issuebook.txt
        if(!found){
            cout<<"This book is not a returned book (no record found).\nPlease deposit it as a new book."<<endl;
        }
}

void issuebook(){
    string bookname,registrationno;
    bool bookavailable=false;
    bool studentavailable=false;
    cout<<"Enter the name of the book here: ";
    cin.ignore();
    getline(cin,bookname);
    // converting to lowercase:
    for(int i=0;i<bookname.length();i++){
        bookname[i]=tolower(bookname[i]);
    }
    int temp=0;
    // checking book bookavailable or not:
    for(int i=0;i<totalbooks;i++){
        if(bookname==books[i]){
            cout<<"Book is available."<<endl;
            bookavailable = true;
            break;
        }
    }
    // book bookavailable:
    if(bookavailable){
        cout<<"Enter the registration no of the student: ";
        cin>>registrationno;
        // converting regno to upper case:
        for(int i=0;i<registrationno.length();i++){
        registrationno[i]=toupper(registrationno[i]);
        }
        // checking student exists or not:
        int temp;
        for(int i=0;i<totalstudents;i++){
            if(registrationno==students[i].regno){
                studentavailable=true;
                temp = i;
                break;
            }
        }
        // STUDENT EXISTS:
        if(studentavailable){
            if(students[temp].bookstoreturn!=0){
                cout<<"One book has already been issued to the student. Return that book to issue a new book."<<endl;
            }
            if(students[temp].bookstoreturn==0){

            // code to delete the book from db:
            ifstream inputFile("books.txt");
            ofstream tempFile("tempfile.txt");
            string line;
            while (getline(inputFile, line)) {
            if (line.find(bookname) == string::npos) {
                tempFile << line << endl;
                }
            }   
            inputFile.close();
            tempFile.close();
            remove("books.txt");
            rename("tempfile.txt", "books.txt");
            // code to add book in the students record
            ifstream inFile("students.txt");
            ofstream outFile("tempfile.txt");
            string line1;
            while (getline(inFile, line1)) {
                istringstream iss(line1);
                string a,reg,c,d,e,f,g,h;
                iss >> a>>reg>>c>>d>>e>>f>>g>>h;
                if (reg == registrationno) {
                    int incrementedValue = stoi(e) + 1;
                    outFile << a << " " << reg << " " <<c<<" "<< d <<" "<<incrementedValue<<" "<<f<<" "<<g<<" "<<h<< endl;
                } else {
                    outFile << line1 << endl;
                }
            }
            inFile.close();
            outFile.close();
            remove("students.txt");
            rename("tempfile.txt", "students.txt");
            // storing the issued book and date in the issued books database:
            string issuedate;
            cout<<"Enter today's date (in MMDD form): ";
            cin>>issuedate;
            // storing issue date:
            ofstream issuedFile("issuedbooks.txt",ios::app);
            if (issuedFile.is_open()) { 
            issuedFile << bookname<<"\n";
            issuedFile << issuedate<<"\n";
            issuedFile.close(); 
            // storing issuedate in students file
            ifstream inFile("students.txt");
            ofstream outFile("tempfile.txt");
            string line1;
            while (getline(inFile, line1)) {
                istringstream iss(line1);
                string a,reg,c,d,e,f,g,h;
                iss >> a>>reg>>c>>d>>e>>f>>g>>h;
                if (reg == registrationno) {
                    h=issuedate;
                    outFile << a << " " << reg << " " <<c<<" "<< d <<" "<<e<<" "<<f<<" "<<g<<" "<<h<< endl;
                } else {
                    outFile << line1 << endl;
                }
            }
            inFile.close();
            outFile.close();
            remove("students.txt");
            rename("tempfile.txt", "students.txt");
            cout<<"Book has been issued."<<endl;
            }}
        // students not exists:
        else if(!studentavailable){
            cout<<"Student of reg no: "<<registrationno<<" does not exists."<<endl;
        }
    }
    // book not bookavailable:
    if(!bookavailable){
        cout<<"The Book is not available."<<endl;
    }
    }
}

int mainscreen(){
    int prompt;
    cout<<"1  Issue Book"<<endl;
    cout<<"2  Deposit Book"<<endl;
    cout<<"3  Check/Pay Fine"<<endl;
    cout<<"4  Check Record"<<endl;
    cout<<"5  Exit"<<endl;
    cin>>prompt;
    cout<<endl;

    if(prompt==1){
        issuebook();
    }
    else if(prompt==2){
        depositbook();
    }
    else if(prompt==3){
        payfine();
    }
    else if(prompt==4){
        checkrecord();    
    }
    else if(prompt==5){
        return 0;
    }
}

int main(){
    char restart;
    bool exit;
    do{
    //getting date from the system:
    time_t currentTime = time(nullptr);
    tm *currentDate = localtime(&currentTime);
    int month = currentDate->tm_mon + 1; 
    int day = currentDate->tm_mday;
    stringstream formattedDate;
    formattedDate << setw(2) << setfill('0') << month << setw(2) << setfill('0') << day;
    date = formattedDate.str();  
    // loading data at start of program from database to our program:
    ifstream studentFile("students.txt");
    string line;
    int i=0;
    getline(studentFile, line);
    while (i < totalstudents && studentFile >> students[i].name >> students[i].regno >> students[i].cgpa >> students[i].booksissued >> students[i].bookstoreturn >> students[i].finepaid >> students[i].finetopay >> students[i].date) {
        i++;
    }
    studentFile.close();

    // updating all fines:
    for(int i=0;i<totalstudents;i++){
        if(students[i].date!="0"){
            string issuedate = students[i].date;
            string currentdate=date;
            int fineamount;
            int oldfine=students[i].finetopay;
            int month = stoi(issuedate.substr(0, 2));
            int day = stoi(issuedate.substr(2, 2));
            int currentmonth = stoi(currentdate.substr(0, 2));
            int currentday = stoi(currentdate.substr(2, 2));
            int duetotaldays = month * 30 + day;
            int currenttotaldays = currentmonth * 30 + currentday;
            if (currenttotaldays > duetotaldays) {
                int delaydays = currenttotaldays - duetotaldays - 1; 
                if (delaydays > 7) {
                    fineamount = oldfine+((delaydays -7) * 10)+10;
                } 
                else {
                    fineamount=oldfine;
                }
            } 
            else {
                    fineamount=oldfine;
            }
            students[i].finetopay=fineamount;
            // storing fine in db:
            ifstream inFile("students.txt");
            ofstream outFile("tempfile.txt");
            string line1;
            while (getline(inFile, line1)) {
                istringstream iss(line1);
                string a,reg,c,d,e,f,g,h;
                iss >> a>>reg>>c>>d>>e>>f>>g>>h;
                if (reg == students[i].regno) {
                    
                    outFile << a << " " << reg << " " <<c<<" "<< d <<" "<<e<<" "<<f<<" "<<fineamount<<" "<<h<< endl;
                } else {
                    outFile << line1 << endl;
                }
            }
            inFile.close();
            outFile.close();
            remove("students.txt");
            rename("tempfile.txt", "students.txt");
        }
    }
    


    // ///////////////////////
    i=0;
    ifstream bookFile("books.txt");
    for (int i = 0; i < totalbooks; ++i) {
        if (!getline(bookFile, books[i])) {
            // cout << "Error reading ."<<endl;
            break; 
        }}
    bookFile.close();
    
    //////////////////////////////////////////////////////////////////
    exit=mainscreen();
    // exit called from mainscreen():
    if(!exit){
        return 0;
    }
    // if user wants to run the program again:
    cout<<"Press Y if you want to run this program again: ";
    cin>>restart;
    }while(restart == 'Y' || restart == 'y');
    return 0;
}