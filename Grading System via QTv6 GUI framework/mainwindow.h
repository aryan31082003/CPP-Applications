// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget> // To manage different views
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QFormLayout>
#include <QSpinBox>
#include <QVector> // Qt's dynamic array
#include <QMessageBox> // For pop-up messages

#include "gradingsystem.h" // Include our grading system logic

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Forward declaration of the helper function
// This is needed because it's used in mainwindow.cpp but not a member of MainWindow
QString capitalizeEachWord(const QString &input);

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Login Screen Slots
    void on_loginButton_clicked();

    // Main Menu Slots
    void on_insertStudentButton_clicked();
    void on_viewStudentButton_clicked();
    void on_modifyStudentButton_clicked();
    void on_deleteStudentButton_clicked();
    void on_exitButton_clicked();

    // Insert Student Slots
    void on_insertForm_subjectCountSpinBox_valueChanged(int count);
    void on_insertForm_saveButton_clicked();
    void on_insertForm_backButton_clicked();

    // View Student Slots
    void on_viewForm_searchButton_clicked();
    void on_viewForm_backButton_clicked();

    // Modify Student Slots
    void on_modifyForm_searchButton_clicked();
    void on_modifyForm_saveButton_clicked();
    void on_modifyForm_backButton_clicked();
    void on_modifyForm_subjectCountSpinBox_valueChanged(int count);


    // Delete Student Slots
    void on_deleteForm_deleteButton_clicked();
    void on_deleteForm_backButton_clicked();

    // Helper slot to set semester and branch (called before CRUD ops)
    // IMPORTANT: Changed return type from void to bool here!
    bool validateAndSetSemesterBranch(const QString &action);

private:
    Ui::MainWindow *ui;
    GradingSystem gradingSystem; // Instance of our grading system logic

    QStackedWidget *stackedWidget; // Manages different pages/views

    // --- Widgets for Login Screen ---
    QWidget *loginPage;
    QLineEdit *loginEmailLineEdit;
    QLineEdit *loginPasswordLineEdit;
    QLabel *loginStatusLabel;
    QPushButton *loginButton;

    // --- Widgets for Main Menu Screen ---
    QWidget *mainMenuPage;
    QPushButton *insertStudentButton;
    QPushButton *viewStudentButton;
    QPushButton *modifyStudentButton;
    QPushButton *deleteStudentButton;
    QPushButton *exitButton;

    // --- Widgets for Insert Student Form ---
    QWidget *insertPage;
    QLineEdit *insertNameLineEdit;
    QLineEdit *insertRollLineEdit;
    QLineEdit *insertPhoneLineEdit;
    QLineEdit *insertDOBLineEdit;
    QComboBox *insertSemesterComboBox;
    QComboBox *insertBranchComboBox;
    QSpinBox *insertSubjectCountSpinBox;
    QVBoxLayout *insertGradesLayout; // Layout to hold dynamic grade inputs
    QVector<QLineEdit*> insertGradeMarksLineEdits; // Stores pointers to dynamically created QLineEdits for marks
    QLabel *insertStatusLabel;
    QPushButton *insertSaveButton;
    QPushButton *insertBackButton;

    // --- Widgets for View Student Form ---
    QWidget *viewPage;
    QLineEdit *viewRollLineEdit;
    QLabel *viewStatusLabel;
    QLabel *viewDisplayLabel; // To display student details
    QPushButton *viewSearchButton;
    QPushButton *viewBackButton;

    // --- Widgets for Modify Student Form ---
    QWidget *modifyPage;
    QLineEdit *modifySearchRollLineEdit;
    QLineEdit *modifyNameLineEdit;
    QLineEdit *modifyRollLineEdit; // New roll number if changed
    QLineEdit *modifyPhoneLineEdit;
    QLineEdit *modifyDOBLineEdit;
    QComboBox *modifySemesterComboBox; // Should match current student's semester/branch
    QComboBox *modifyBranchComboBox;
    QSpinBox *modifySubjectCountSpinBox;
    QVBoxLayout *modifyGradesLayout;
    QVector<QLineEdit*> modifyGradeMarksLineEdits;
    QLabel *modifyStatusLabel;
    QPushButton *modifySearchButton;
    QPushButton *modifySaveButton;
    QPushButton *modifyBackButton;
    QString currentModifyingRoll; // Stores the roll of the student being modified


    // --- Widgets for Delete Student Form ---
    QWidget *deletePage;
    QLineEdit *deleteRollLineEdit;
    QLabel *deleteStatusLabel;
    QPushButton *deleteButton;
    QPushButton *deleteBackButton;

    // Helper functions for UI setup
    void setupLoginPage();
    void setupMainMenuPage();
    void setupInsertPage();
    void setupViewPage();
    void setupModifyPage();
    void setupDeletePage();

    // Helper for clearing dynamic grade inputs
    void clearDynamicGradeInputs(QVBoxLayout *layout, QVector<QLineEdit*>& lineEdits);
    void createDynamicGradeInputs(int count, QVBoxLayout *layout, QVector<QLineEdit*>& lineEdits);

    // Common function to configure semester/branch combo boxes
    void configureSemesterBranchComboBoxes(QComboBox *semesterCombo, QComboBox *branchCombo);
};
#endif // MAINWINDOW_H
