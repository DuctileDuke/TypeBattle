#include <iostream>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <string>
#include <cstdlib>
#include <ctime>
#include <list>
#include <algorithm>
#include <filesystem>

using namespace std;

int RNG() {
    return rand() % 50 + 1;
}

string trim(string smth) {
    smth.erase(remove(smth.begin(), smth.end(), '\r'), smth.end());
    return smth;
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    // Adding random IDs
    list<int> nums;
    for (int i = 0; i < 5; ++i) {
        nums.push_back(RNG());
    }

    SQLHANDLE envHandle = nullptr;
    SQLHANDLE connHandle = nullptr;
    SQLHANDLE stmtHandle = nullptr;
    SQLRETURN retCode;

    retCode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &envHandle);
    retCode = SQLSetEnvAttr(envHandle, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
    retCode = SQLAllocHandle(SQL_HANDLE_DBC, envHandle, &connHandle);

    // Database connection
    string connStr =
        "Driver={Microsoft Access Driver (*.mdb, *.accdb)};"
        "DBQ=./data/Words.accdb;";

    retCode = SQLDriverConnect(connHandle, NULL,
        (SQLCHAR*)(const unsigned char*)connStr.c_str(),
        SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE);
    if (!SQL_SUCCEEDED(retCode)) {
        cout << "Error - Connection failed." << endl;
        return 1;
    }

    list<string> LoadedWords;

    for (const int& RNumber : nums) {

        // Loading words from the database based on random IDs
        string query = "SELECT * FROM WordSource WHERE Id = " + to_string(RNumber);

        SQLAllocHandle(SQL_HANDLE_STMT, connHandle, &stmtHandle);
        retCode = SQLExecDirect(stmtHandle, (SQLCHAR*)query.c_str(), SQL_NTS);

        SQLCHAR columnData[256];
        if (SQLFetch(stmtHandle) == SQL_SUCCESS) {
            SQLGetData(stmtHandle, 2, SQL_C_CHAR, columnData, sizeof(columnData), NULL);
            string TempWord = reinterpret_cast<char*>(columnData);
            LoadedWords.push_back(TempWord);
        }

        SQLFreeHandle(SQL_HANDLE_STMT, stmtHandle);
    }

    // Close database connection
    SQLDisconnect(connHandle);
    SQLFreeHandle(SQL_HANDLE_DBC, connHandle);
    SQLFreeHandle(SQL_HANDLE_ENV, envHandle);

    // Display loaded words
    for (const string& word : LoadedWords) {
        cout << trim(word) << endl;
    }

    return 0;
}