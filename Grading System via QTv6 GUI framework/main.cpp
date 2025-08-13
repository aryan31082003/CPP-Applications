// main.cpp
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); // Create the QApplication object

    MainWindow w; // Create an instance of your MainWindow
    w.show();     // Show the main window

    return a.exec(); // Start the Qt event loop
}
