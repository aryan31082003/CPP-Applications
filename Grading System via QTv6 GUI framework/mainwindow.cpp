// mainwindow.cpp
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScrollArea> // For scrollable content
#include <QIntValidator> // For numeric input validation
#include <QCoreApplication> // For QCoreApplication::quit()

// Helper function to capitalize the first letter of each word in a QString
// This mimics QString::toCapitalized() which was introduced in Qt 5.10
QString capitalizeEachWord(const QString &input) {
    if (input.isEmpty()) {
        return input;
    }
    QString result = input;
    bool newWord = true;
    for (int i = 0; i < result.length(); ++i) {
        // Check for non-alphanumeric characters or spaces as word separators
        if (!result.at(i).isLetterOrNumber()) { // Use isLetterOrNumber for broader definition of word boundary
            newWord = true;
        } else if (newWord) {
            result[i] = result.at(i).toUpper();
            newWord = false;
        } else {
            result[i] = result.at(i).toLower();
        }
    }
    return result;
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(nullptr) // Initialize ui to nullptr since we're not using a .ui file
{
    // Set window title and fixed size for consistency
    setWindowTitle("Student Grading System");
    setFixedSize(600, 700); // Adjust as needed

    // Main stacked widget to switch between different application screens
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget); // Make stackedWidget the central widget

    // Setup individual pages
    setupLoginPage();
    setupMainMenuPage();
    setupInsertPage();
    setupViewPage();
    setupModifyPage();
    setupDeletePage();

    // Add pages to stacked widget
    stackedWidget->addWidget(loginPage);      // Index 0
    stackedWidget->addWidget(mainMenuPage);   // Index 1
    stackedWidget->addWidget(insertPage);     // Index 2
    stackedWidget->addWidget(viewPage);       // Index 3
    stackedWidget->addWidget(modifyPage);     // Index 4
    stackedWidget->addWidget(deletePage);     // Index 5

    // Start with the login page
    stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    // No need to delete ui object if it's nullptr
    // No need to delete other widgets explicitly if they have a parent, Qt handles it.
}

void MainWindow::setupLoginPage()
{
    loginPage = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(loginPage);
    layout->setContentsMargins(50, 100, 50, 100);
    layout->setSpacing(20);

    QLabel *titleLabel = new QLabel("Admin Login", loginPage);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #333;");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    QFormLayout *formLayout = new QFormLayout();
    formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    formLayout->setLabelAlignment(Qt::AlignRight);

    loginEmailLineEdit = new QLineEdit(loginPage);
    loginEmailLineEdit->setPlaceholderText("Enter admin email");
    loginEmailLineEdit->setStyleSheet("padding: 8px; border: 1px solid #ccc; border-radius: 5px;");
    formLayout->addRow("Email:", loginEmailLineEdit);

    loginPasswordLineEdit = new QLineEdit(loginPage);
    loginPasswordLineEdit->setPlaceholderText("Enter password");
    loginPasswordLineEdit->setEchoMode(QLineEdit::Password);
    loginPasswordLineEdit->setStyleSheet("padding: 8px; border: 1px solid #ccc; border-radius: 5px;");
    formLayout->addRow("Password:", loginPasswordLineEdit);

    layout->addLayout(formLayout);

    loginStatusLabel = new QLabel("", loginPage);
    loginStatusLabel->setStyleSheet("color: red; font-weight: bold;");
    loginStatusLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(loginStatusLabel);

    loginButton = new QPushButton("Login", loginPage);
    loginButton->setStyleSheet(
        "QPushButton { background-color: #4CAF50; color: white; padding: 10px 20px; border-radius: 8px; font-size: 16px; }"
        "QPushButton:hover { background-color: #45a049; }"
        );
    layout->addWidget(loginButton, 0, Qt::AlignCenter);

    layout->addStretch(); // Pushes content to the top

    connect(loginButton, &QPushButton::clicked, this, &MainWindow::on_loginButton_clicked);
}

void MainWindow::setupMainMenuPage()
{
    mainMenuPage = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(mainMenuPage);
    layout->setContentsMargins(50, 50, 50, 50);
    layout->setSpacing(20);

    QLabel *titleLabel = new QLabel("Main Menu", mainMenuPage);
    titleLabel->setStyleSheet("font-size: 28px; font-weight: bold; color: #3498DB;");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    insertStudentButton = new QPushButton("1. Insert Student", mainMenuPage);
    viewStudentButton = new QPushButton("2. View Student", mainMenuPage);
    modifyStudentButton = new QPushButton("3. Modify Student", mainMenuPage);
    deleteStudentButton = new QPushButton("4. Delete Student", mainMenuPage);
    exitButton = new QPushButton("5. Exit", mainMenuPage);

    QList<QPushButton*> buttons = {insertStudentButton, viewStudentButton, modifyStudentButton, deleteStudentButton, exitButton};
    for (QPushButton* btn : buttons) {
        btn->setStyleSheet(
            "QPushButton { background-color: #3498DB; color: white; padding: 12px; border-radius: 8px; font-size: 18px; }"
            "QPushButton:hover { background-color: #2980B9; }"
            );
        layout->addWidget(btn);
    }

    connect(insertStudentButton, &QPushButton::clicked, this, &MainWindow::on_insertStudentButton_clicked);
    connect(viewStudentButton, &QPushButton::clicked, this, &MainWindow::on_viewStudentButton_clicked);
    connect(modifyStudentButton, &QPushButton::clicked, this, &MainWindow::on_modifyStudentButton_clicked);
    connect(deleteStudentButton, &QPushButton::clicked, this, &MainWindow::on_deleteStudentButton_clicked);
    connect(exitButton, &QPushButton::clicked, this, &MainWindow::on_exitButton_clicked);
}

void MainWindow::setupInsertPage()
{
    insertPage = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(insertPage);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(10);

    QLabel *titleLabel = new QLabel("Insert New Student", insertPage);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #2C3E50;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    QScrollArea *scrollArea = new QScrollArea(insertPage);
    scrollArea->setWidgetResizable(true);
    QWidget *scrollWidget = new QWidget();
    QFormLayout *formLayout = new QFormLayout(scrollWidget);
    formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    formLayout->setLabelAlignment(Qt::AlignRight);
    scrollArea->setWidget(scrollWidget);

    insertNameLineEdit = new QLineEdit();
    insertNameLineEdit->setPlaceholderText("Full Name");
    insertNameLineEdit->setStyleSheet("padding: 5px; border: 1px solid #ddd; border-radius: 4px;");
    formLayout->addRow("Name:", insertNameLineEdit);

    insertRollLineEdit = new QLineEdit();
    insertRollLineEdit->setPlaceholderText("e.g., 2K20/CO/001");
    insertRollLineEdit->setStyleSheet("padding: 5px; border: 1px solid #ddd; border-radius: 4px;");
    formLayout->addRow("Roll No.:", insertRollLineEdit);

    insertPhoneLineEdit = new QLineEdit();
    insertPhoneLineEdit->setPlaceholderText("10-digit phone number");
    // Removed QIntValidator to avoid overflow warning; validation done in backend
    insertPhoneLineEdit->setStyleSheet("padding: 5px; border: 1px solid #ddd; border-radius: 4px;");
    formLayout->addRow("Phone:", insertPhoneLineEdit);

    insertDOBLineEdit = new QLineEdit();
    insertDOBLineEdit->setPlaceholderText("dd-mm-yyyy");
    insertDOBLineEdit->setStyleSheet("padding: 5px; border: 1px solid #ddd; border-radius: 4px;");
    formLayout->addRow("DOB:", insertDOBLineEdit);

    insertSemesterComboBox = new QComboBox();
    insertBranchComboBox = new QComboBox();
    configureSemesterBranchComboBoxes(insertSemesterComboBox, insertBranchComboBox);

    formLayout->addRow("Semester:", insertSemesterComboBox);
    formLayout->addRow("Branch:", insertBranchComboBox);

    insertSubjectCountSpinBox = new QSpinBox();
    insertSubjectCountSpinBox->setRange(1, 10); // Assume min 1, max 10 subjects
    insertSubjectCountSpinBox->setValue(4); // Default to 4 subjects
    formLayout->addRow("Number of Subjects:", insertSubjectCountSpinBox);

    // Layout for dynamic grade inputs
    insertGradesLayout = new QVBoxLayout();
    formLayout->addRow("Marks:", insertGradesLayout);
    createDynamicGradeInputs(insertSubjectCountSpinBox->value(), insertGradesLayout, insertGradeMarksLineEdits);

    mainLayout->addWidget(scrollArea);

    insertStatusLabel = new QLabel("", insertPage);
    insertStatusLabel->setStyleSheet("color: red; font-weight: bold;");
    insertStatusLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(insertStatusLabel);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    insertSaveButton = new QPushButton("Save Student", insertPage);
    insertSaveButton->setStyleSheet(
        "QPushButton { background-color: #27AE60; color: white; padding: 10px 15px; border-radius: 6px; font-size: 16px; }"
        "QPushButton:hover { background-color: #229954; }"
        );
    insertBackButton = new QPushButton("Back to Menu", insertPage);
    insertBackButton->setStyleSheet(
        "QPushButton { background-color: #95A5A6; color: white; padding: 10px 15px; border-radius: 6px; font-size: 16px; }"
        "QPushButton:hover { background-color: #7F8C8D; }"
        );
    buttonLayout->addWidget(insertSaveButton);
    buttonLayout->addWidget(insertBackButton);
    mainLayout->addLayout(buttonLayout);

    connect(insertSubjectCountSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MainWindow::on_insertForm_subjectCountSpinBox_valueChanged);
    connect(insertSaveButton, &QPushButton::clicked, this, &MainWindow::on_insertForm_saveButton_clicked);
    connect(insertBackButton, &QPushButton::clicked, this, &MainWindow::on_insertForm_backButton_clicked);
}

void MainWindow::setupViewPage()
{
    viewPage = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(viewPage);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(10);

    QLabel *titleLabel = new QLabel("View Student Details", viewPage);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #2C3E50;");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    QFormLayout *formLayout = new QFormLayout();
    formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    formLayout->setLabelAlignment(Qt::AlignRight);

    QComboBox *viewSemesterComboBox = new QComboBox();
    QComboBox *viewBranchComboBox = new QComboBox();
    configureSemesterBranchComboBoxes(viewSemesterComboBox, viewBranchComboBox);

    formLayout->addRow("Semester:", viewSemesterComboBox);
    formLayout->addRow("Branch:", viewBranchComboBox);

    viewRollLineEdit = new QLineEdit();
    viewRollLineEdit->setPlaceholderText("Enter student roll number (e.g., 2K20/CO/001)");
    viewRollLineEdit->setStyleSheet("padding: 5px; border: 1px solid #ddd; border-radius: 4px;");
    formLayout->addRow("Roll No.:", viewRollLineEdit);

    viewSearchButton = new QPushButton("Search", viewPage);
    viewSearchButton->setStyleSheet(
        "QPushButton { background-color: #3498DB; color: white; padding: 8px 15px; border-radius: 5px; font-size: 15px; }"
        "QPushButton:hover { background-color: #2980B9; }"
        );
    formLayout->addRow("", viewSearchButton); // Empty label to align button

    layout->addLayout(formLayout);

    viewStatusLabel = new QLabel("", viewPage);
    viewStatusLabel->setStyleSheet("color: red; font-weight: bold;");
    viewStatusLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(viewStatusLabel);

    viewDisplayLabel = new QLabel("Student details will appear here.", viewPage);
    viewDisplayLabel->setStyleSheet("background-color: #ECF0F1; padding: 15px; border-radius: 5px; min-height: 150px;");
    viewDisplayLabel->setWordWrap(true);
    viewDisplayLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    layout->addWidget(viewDisplayLabel);

    viewBackButton = new QPushButton("Back to Menu", viewPage);
    viewBackButton->setStyleSheet(
        "QPushButton { background-color: #95A5A6; color: white; padding: 10px 15px; border-radius: 6px; font-size: 16px; }"
        "QPushButton:hover { background-color: #7F8C8D; }"
        );
    layout->addWidget(viewBackButton, 0, Qt::AlignCenter);

    connect(viewSearchButton, &QPushButton::clicked, this, &MainWindow::on_viewForm_searchButton_clicked);
    connect(viewBackButton, &QPushButton::clicked, this, &MainWindow::on_viewForm_backButton_clicked);
    // Connect combo boxes to trigger semester/branch update on selection change
    connect(viewSemesterComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index){
        Q_UNUSED(index); // Suppress unused parameter warning
        this->validateAndSetSemesterBranch("view"); // Pass an identifier for the calling context
    });
    connect(viewBranchComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index){
        Q_UNUSED(index); // Suppress unused parameter warning
        this->validateAndSetSemesterBranch("view"); // Pass an identifier for the calling context
    });
}

void MainWindow::setupModifyPage()
{
    modifyPage = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(modifyPage);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(10);

    QLabel *titleLabel = new QLabel("Modify Student Data", modifyPage);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #2C3E50;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    QScrollArea *scrollArea = new QScrollArea(modifyPage);
    scrollArea->setWidgetResizable(true);
    QWidget *scrollWidget = new QWidget();
    QFormLayout *formLayout = new QFormLayout(scrollWidget);
    formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    formLayout->setLabelAlignment(Qt::AlignRight);
    scrollArea->setWidget(scrollWidget);

    // Search section
    QHBoxLayout *searchLayout = new QHBoxLayout();
    QComboBox *modifySemesterComboBoxSearch = new QComboBox();
    QComboBox *modifyBranchComboBoxSearch = new QComboBox();
    configureSemesterBranchComboBoxes(modifySemesterComboBoxSearch, modifyBranchComboBoxSearch);
    searchLayout->addWidget(new QLabel("Semester:"));
    searchLayout->addWidget(modifySemesterComboBoxSearch);
    searchLayout->addWidget(new QLabel("Branch:"));
    searchLayout->addWidget(modifyBranchComboBoxSearch);

    modifySearchRollLineEdit = new QLineEdit();
    modifySearchRollLineEdit->setPlaceholderText("Enter roll number to modify (e.g., 2K20/CO/001)");
    modifySearchRollLineEdit->setStyleSheet("padding: 5px; border: 1px solid #ddd; border-radius: 4px;");
    searchLayout->addWidget(modifySearchRollLineEdit);

    modifySearchButton = new QPushButton("Search", modifyPage);
    modifySearchButton->setStyleSheet(
        "QPushButton { background-color: #3498DB; color: white; padding: 8px 15px; border-radius: 5px; font-size: 15px; }"
        "QPushButton:hover { background-color: #2980B9; }"
        );
    searchLayout->addWidget(modifySearchButton);
    mainLayout->addLayout(searchLayout);

    // Connect combo boxes for search to trigger semester/branch update on selection change
    connect(modifySemesterComboBoxSearch, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index){
        Q_UNUSED(index); // Suppress unused parameter warning
        this->validateAndSetSemesterBranch("modify"); // Pass an identifier for the calling context
    });
    connect(modifyBranchComboBoxSearch, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index){
        Q_UNUSED(index); // Suppress unused parameter warning
        this->validateAndSetSemesterBranch("modify"); // Pass an identifier for the calling context
    });

    // Modification fields (initially disabled until a student is found)
    modifyNameLineEdit = new QLineEdit();
    modifyRollLineEdit = new QLineEdit();
    modifyPhoneLineEdit = new QLineEdit();
    modifyDOBLineEdit = new QLineEdit();
    modifySemesterComboBox = new QComboBox(); // This will display the student's actual semester/branch
    modifyBranchComboBox = new QComboBox(); // Not for user input, just display
    configureSemesterBranchComboBoxes(modifySemesterComboBox, modifyBranchComboBox);

    modifySubjectCountSpinBox = new QSpinBox();
    modifySubjectCountSpinBox->setRange(1, 10);

    QList<QLineEdit*> editFields = {modifyNameLineEdit, modifyRollLineEdit, modifyPhoneLineEdit, modifyDOBLineEdit};
    for(QLineEdit* field : editFields) {
        field->setStyleSheet("padding: 5px; border: 1px solid #ddd; border-radius: 4px;");
        field->setEnabled(false); // Disable initially
    }
    modifySemesterComboBox->setEnabled(false);
    modifyBranchComboBox->setEnabled(false);
    modifySubjectCountSpinBox->setEnabled(false);

    formLayout->addRow("Name:", modifyNameLineEdit);
    formLayout->addRow("New Roll No.:", modifyRollLineEdit); // User can change roll
    formLayout->addRow("Phone:", modifyPhoneLineEdit);
    formLayout->addRow("DOB:", modifyDOBLineEdit);
    formLayout->addRow("Semester (Fixed):", modifySemesterComboBox); // Display only
    formLayout->addRow("Branch (Fixed):", modifyBranchComboBox);     // Display only
    formLayout->addRow("Number of Subjects:", modifySubjectCountSpinBox);

    modifyGradesLayout = new QVBoxLayout();
    formLayout->addRow("Marks:", modifyGradesLayout);

    mainLayout->addWidget(scrollArea);

    modifyStatusLabel = new QLabel("", modifyPage);
    modifyStatusLabel->setStyleSheet("color: red; font-weight: bold;");
    modifyStatusLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(modifyStatusLabel);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    modifySaveButton = new QPushButton("Save Changes", modifyPage);
    modifySaveButton->setStyleSheet(
        "QPushButton { background-color: #27AE60; color: white; padding: 10px 15px; border-radius: 6px; font-size: 16px; }"
        "QPushButton:hover { background-color: #229954; }"
        );
    modifySaveButton->setEnabled(false); // Disable initially

    modifyBackButton = new QPushButton("Back to Menu", modifyPage);
    modifyBackButton->setStyleSheet(
        "QPushButton { background-color: #95A5A6; color: white; padding: 10px 15px; border-radius: 6px; font-size: 16px; }"
        "QPushButton:hover { background-color: #7F8C8D; }"
        );
    buttonLayout->addWidget(modifySaveButton);
    buttonLayout->addWidget(modifyBackButton);
    mainLayout->addLayout(buttonLayout);

    connect(modifySearchButton, &QPushButton::clicked, this, &MainWindow::on_modifyForm_searchButton_clicked);
    connect(modifySaveButton, &QPushButton::clicked, this, &MainWindow::on_modifyForm_saveButton_clicked);
    connect(modifyBackButton, &QPushButton::clicked, this, &MainWindow::on_modifyForm_backButton_clicked);
    connect(modifySubjectCountSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MainWindow::on_modifyForm_subjectCountSpinBox_valueChanged);
}

void MainWindow::setupDeletePage()
{
    deletePage = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(deletePage);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(10);

    QLabel *titleLabel = new QLabel("Delete Student Record", deletePage);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #2C3E50;");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    QFormLayout *formLayout = new QFormLayout();
    formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    formLayout->setLabelAlignment(Qt::AlignRight);

    QComboBox *deleteSemesterComboBox = new QComboBox();
    QComboBox *deleteBranchComboBox = new QComboBox();
    configureSemesterBranchComboBoxes(deleteSemesterComboBox, deleteBranchComboBox);

    formLayout->addRow("Semester:", deleteSemesterComboBox);
    formLayout->addRow("Branch:", deleteBranchComboBox);

    deleteRollLineEdit = new QLineEdit();
    deleteRollLineEdit->setPlaceholderText("Enter student roll number to delete");
    deleteRollLineEdit->setStyleSheet("padding: 5px; border: 1px solid #ddd; border-radius: 4px;");
    formLayout->addRow("Roll No.:", deleteRollLineEdit);

    layout->addLayout(formLayout);

    deleteStatusLabel = new QLabel("", deletePage);
    deleteStatusLabel->setStyleSheet("color: red; font-weight: bold;");
    deleteStatusLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(deleteStatusLabel);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    deleteButton = new QPushButton("Delete Student", deletePage);
    deleteButton->setStyleSheet(
        "QPushButton { background-color: #E74C3C; color: white; padding: 10px 15px; border-radius: 6px; font-size: 16px; }"
        "QPushButton:hover { background-color: #C0392B; }"
        );
    deleteBackButton = new QPushButton("Back to Menu", deletePage);
    deleteBackButton->setStyleSheet(
        "QPushButton { background-color: #95A5A6; color: white; padding: 10px 15px; border-radius: 6px; font-size: 16px; }"
        "QPushButton:hover { background-color: #7F8C8D; }"
        );
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(deleteBackButton);
    layout->addLayout(buttonLayout);

    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::on_deleteForm_deleteButton_clicked);
    connect(deleteBackButton, &QPushButton::clicked, this, &MainWindow::on_deleteForm_backButton_clicked);
    // Connect combo boxes to trigger semester/branch update on selection change
    connect(deleteSemesterComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index){
        Q_UNUSED(index); // Suppress unused parameter warning
        this->validateAndSetSemesterBranch("delete"); // Pass an identifier for the calling context
    });
    connect(deleteBranchComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index){
        Q_UNUSED(index); // Suppress unused parameter warning
        this->validateAndSetSemesterBranch("delete"); // Pass an identifier for the calling context
    });
}

void MainWindow::configureSemesterBranchComboBoxes(QComboBox *semesterCombo, QComboBox *branchCombo) {
    for (int i = 1; i <= 8; ++i) {
        semesterCombo->addItem(QString::number(i));
    }
    QStringList branches = {"computer", "electrical", "mechanical", "chemical", "civil", "management"};
    for (const QString &branch : branches) {
        branchCombo->addItem(capitalizeEachWord(branch)); // Use the new helper function
    }
}

void MainWindow::clearDynamicGradeInputs(QVBoxLayout *layout, QVector<QLineEdit*>& lineEdits) {
    while (!lineEdits.isEmpty()) {
        QLineEdit *lineEdit = lineEdits.takeLast();
        layout->removeWidget(lineEdit);
        delete lineEdit; // Delete the widget from memory
    }
}

void MainWindow::createDynamicGradeInputs(int count, QVBoxLayout *layout, QVector<QLineEdit*>& lineEdits) {
    clearDynamicGradeInputs(layout, lineEdits); // Clear existing first
    for (int i = 0; i < count; ++i) {
        QLineEdit *gradeLineEdit = new QLineEdit();
        gradeLineEdit->setPlaceholderText(QString("Marks for Subject %1 (0-100)").arg(i + 1));
        gradeLineEdit->setValidator(new QIntValidator(0, 100, this)); // Marks from 0 to 100
        gradeLineEdit->setStyleSheet("padding: 5px; border: 1px solid #ddd; border-radius: 4px;");
        layout->addWidget(gradeLineEdit);
        lineEdits.append(gradeLineEdit);
    }
}

void MainWindow::on_loginButton_clicked()
{
    QString email = loginEmailLineEdit->text();
    QString password = loginPasswordLineEdit->text();

    if (gradingSystem.login(email.toStdString(), password.toStdString()))
    {
        loginStatusLabel->setText("<span style='color: green;'>Login Successful!</span>");
        stackedWidget->setCurrentIndex(1); // Go to main menu
        // Clear login fields after successful login
        loginEmailLineEdit->clear();
        loginPasswordLineEdit->clear();
    }
    else
    {
        loginStatusLabel->setText("<span style='color: red;'>Invalid Email or Password.</span>");
    }
}

void MainWindow::on_insertStudentButton_clicked()
{
    // Clear previous inputs
    insertNameLineEdit->clear();
    insertRollLineEdit->clear();
    insertPhoneLineEdit->clear();
    insertDOBLineEdit->clear();
    insertStatusLabel->clear();
    insertSubjectCountSpinBox->setValue(4); // Reset to default
    clearDynamicGradeInputs(insertGradesLayout, insertGradeMarksLineEdits);
    createDynamicGradeInputs(insertSubjectCountSpinBox->value(), insertGradesLayout, insertGradeMarksLineEdits);

    // Pre-select the current semester/branch in the combo boxes if set from previous operation
    if (!gradingSystem.getSelectedSemester().empty()) {
        insertSemesterComboBox->setCurrentText(QString::fromStdString(gradingSystem.getSelectedSemester()));
    }
    if (!gradingSystem.getSelectedBranch().empty()) {
        insertBranchComboBox->setCurrentText(capitalizeEachWord(QString::fromStdString(gradingSystem.getSelectedBranch())));
    }

    // Set semester and branch for the grading system based on selected values
    // This will load the correct CSV file for the insert operation.
    validateAndSetSemesterBranch("insert");

    stackedWidget->setCurrentIndex(2); // Go to insert page
}

void MainWindow::on_viewStudentButton_clicked()
{
    viewRollLineEdit->clear();
    viewStatusLabel->clear();
    viewDisplayLabel->setText("Student details will appear here.");

    // Pre-select the current semester/branch in the combo boxes if set from previous operation
    // Note: Finding combo boxes like this can be fragile if UI structure changes.
    // Better to store pointers to them if they are not direct members.
    QComboBox *semCombo = viewPage->findChild<QComboBox*>();
    QComboBox *branchCombo = nullptr;
    if (semCombo) { // Find the branch combo after the semester combo
        QList<QComboBox*> combos = viewPage->findChildren<QComboBox*>();
        if (combos.size() >= 2) {
            branchCombo = combos.at(1);
        }
    }

    if (semCombo && !gradingSystem.getSelectedSemester().empty()) {
        semCombo->setCurrentText(QString::fromStdString(gradingSystem.getSelectedSemester()));
    }
    if (branchCombo && !gradingSystem.getSelectedBranch().empty()) {
        branchCombo->setCurrentText(capitalizeEachWord(QString::fromStdString(gradingSystem.getSelectedBranch())));
    }
    validateAndSetSemesterBranch("view"); // Set initial semester/branch for load

    stackedWidget->setCurrentIndex(3); // Go to view page
}

void MainWindow::on_modifyStudentButton_clicked()
{
    // Clear all fields and disable them
    modifySearchRollLineEdit->clear();
    modifyNameLineEdit->clear();
    modifyRollLineEdit->clear();
    modifyPhoneLineEdit->clear();
    modifyDOBLineEdit->clear();
    modifyStatusLabel->clear();
    modifySubjectCountSpinBox->setValue(1); // Reset spin box
    clearDynamicGradeInputs(modifyGradesLayout, modifyGradeMarksLineEdits);
    modifySaveButton->setEnabled(false);

    QList<QLineEdit*> editFields = {modifyNameLineEdit, modifyRollLineEdit, modifyPhoneLineEdit, modifyDOBLineEdit};
    for(QLineEdit* field : editFields) {
        field->setEnabled(false);
    }
    modifySemesterComboBox->setEnabled(false);
    modifyBranchComboBox->setEnabled(false);
    modifySubjectCountSpinBox->setEnabled(false);

    // Pre-select the current semester/branch in the search combo boxes
    // Note: Finding combo boxes like this can be fragile if UI structure changes.
    // Better to store pointers to them if they are not direct members.
    QComboBox *semComboSearch = nullptr;
    QComboBox *branchComboSearch = nullptr;
    // Assuming searchLayout is the first QHBoxLayout in modifyPage
    QHBoxLayout *searchLayout = modifyPage->findChild<QHBoxLayout*>();
    if (searchLayout) {
        QList<QComboBox*> combos = searchLayout->findChildren<QComboBox*>();
        if (combos.size() >= 2) {
            semComboSearch = combos.at(0);
            branchComboSearch = combos.at(1);
        }
    }

    if (semComboSearch && !gradingSystem.getSelectedSemester().empty()) {
        semComboSearch->setCurrentText(QString::fromStdString(gradingSystem.getSelectedSemester()));
    }
    if (branchComboSearch && !gradingSystem.getSelectedBranch().empty()) {
        branchComboSearch->setCurrentText(capitalizeEachWord(QString::fromStdString(gradingSystem.getSelectedBranch())));
    }
    validateAndSetSemesterBranch("modify"); // Set initial semester/branch for load

    stackedWidget->setCurrentIndex(4); // Go to modify page
}

void MainWindow::on_deleteStudentButton_clicked()
{
    deleteRollLineEdit->clear();
    deleteStatusLabel->clear();

    // Pre-select the current semester/branch in the combo boxes
    // Note: Finding combo boxes like this can be fragile if UI structure changes.
    QComboBox *semCombo = deletePage->findChild<QComboBox*>();
    QComboBox *branchCombo = nullptr;
    if (semCombo) { // Find the branch combo after the semester combo
        QList<QComboBox*> combos = deletePage->findChildren<QComboBox*>();
        if (combos.size() >= 2) {
            branchCombo = combos.at(1);
        }
    }

    if (semCombo && !gradingSystem.getSelectedSemester().empty()) {
        semCombo->setCurrentText(QString::fromStdString(gradingSystem.getSelectedSemester()));
    }
    if (branchCombo && !gradingSystem.getSelectedBranch().empty()) {
        branchCombo->setCurrentText(capitalizeEachWord(QString::fromStdString(gradingSystem.getSelectedBranch())));
    }
    validateAndSetSemesterBranch("delete"); // Set initial semester/branch for load

    stackedWidget->setCurrentIndex(5); // Go to delete page
}

void MainWindow::on_exitButton_clicked()
{
    QMessageBox::information(this, "Exit", "Exiting Application. Goodbye!");
    QCoreApplication::quit(); // Properly quit the application
}

void MainWindow::on_insertForm_subjectCountSpinBox_valueChanged(int count)
{
    // Dynamically create/remove QLineEdit widgets for marks based on spin box value
    createDynamicGradeInputs(count, insertGradesLayout, insertGradeMarksLineEdits);
}

void MainWindow::on_insertForm_saveButton_clicked()
{
    // First, ensure semester and branch are set for the current operation
    // The validateAndSetSemesterBranch function now returns bool to indicate if setting was successful
    if (!validateAndSetSemesterBranch("insert")) {
        // Error message already shown by validateAndSetSemesterBranch
        return;
    }

    Student s;
    s.name = insertNameLineEdit->text().toStdString();
    s.roll = insertRollLineEdit->text().toUpper().toStdString(); // Store roll in uppercase
    s.phone = insertPhoneLineEdit->text().toStdString();
    s.dob = insertDOBLineEdit->text().toStdString();
    s.semester = insertSemesterComboBox->currentText().toStdString();
    s.branch = insertBranchComboBox->currentText().toLower().toStdString();

    // Basic validation
    if (!isValidName(s.name)) {
        insertStatusLabel->setText("<span style='color: red;'>Invalid Name. Only alphabets and spaces allowed.</span>"); return;
    }
    if (!gradingSystem.isValidRollForBranch(s.roll, s.branch)) {
        insertStatusLabel->setText(QString("<span style='color: red;'>Invalid Roll Number for %1 branch. Format: 2KXX/BR/XX.</span>").arg(capitalizeEachWord(QString::fromStdString(s.branch)))); return;
    }
    if (!isValidPhone(s.phone)) {
        insertStatusLabel->setText("<span style='color: red;'>Invalid Phone Number. Must be exactly 10 digits.</span>"); return;
    }
    if (!isValidDOB(s.dob)) {
        insertStatusLabel->setText("<span style='color: red;'>Invalid DOB. Must be valid and in dd-mm-yyyy format.</span>"); return;
    }

    s.grades.clear();
    for (QLineEdit *markEdit : insertGradeMarksLineEdits) {
        bool ok;
        int mark = markEdit->text().toInt(&ok);
        if (!ok || mark < 0 || mark > 100) {
            insertStatusLabel->setText("<span style='color: red;'>Invalid marks entered. Marks must be 0-100.</span>");
            return;
        }
        s.grades.push_back(s.getGrade(mark));
    }

    std::pair<bool, std::string> result = gradingSystem.insertStudent(s);
    if (result.first) {
        insertStatusLabel->setText(QString("<span style='color: green;'>%1</span>").arg(QString::fromStdString(result.second)));
        // Clear fields after successful insertion
        insertNameLineEdit->clear();
        insertRollLineEdit->clear();
        insertPhoneLineEdit->clear();
        insertDOBLineEdit->clear();
        insertSubjectCountSpinBox->setValue(4); // Reset to default
        clearDynamicGradeInputs(insertGradesLayout, insertGradeMarksLineEdits);
        createDynamicGradeInputs(4, insertGradesLayout, insertGradeMarksLineEdits);
    } else {
        insertStatusLabel->setText(QString("<span style='color: red;'>%1</span>").arg(QString::fromStdString(result.second)));
    }
}

void MainWindow::on_insertForm_backButton_clicked()
{
    stackedWidget->setCurrentIndex(1); // Go back to main menu
}

void MainWindow::on_viewForm_searchButton_clicked()
{
    if (!validateAndSetSemesterBranch("view")) {
        return;
    }

    std::string roll = viewRollLineEdit->text().toUpper().toStdString(); // Convert to upper for search consistency
    Student s;
    if (gradingSystem.viewStudent(roll, s)) {
        QString details = "<b>Name:</b> " + QString::fromStdString(s.name) + "<br>";
        details += "<b>Roll:</b> " + QString::fromStdString(s.roll) + "<br>";
        details += "<b>Phone:</b> " + QString::fromStdString(s.phone) + "<br>";
        details += "<b>DOB:</b> " + QString::fromStdString(s.dob) + "<br>";
        details += "<b>Semester:</b> " + QString::fromStdString(s.semester) + "<br>";
        details += "<b>Branch:</b> " + capitalizeEachWord(QString::fromStdString(s.branch)) + "<br>"; // Use helper
        details += "<b>Grades:</b><br>";
        for (size_t i = 0; i < s.grades.size(); ++i) {
            details += QString("Subject %1: %2<br>").arg(i + 1).arg(QString::fromStdString(s.grades[i]));
        }
        viewDisplayLabel->setText(details);
        viewStatusLabel->setText("<span style='color: green;'>Student found.</span>");
    } else {
        viewDisplayLabel->setText("Student details will appear here.");
        viewStatusLabel->setText("<span style='color: red;'>Student not found.</span>");
    }
}

void MainWindow::on_viewForm_backButton_clicked()
{
    stackedWidget->setCurrentIndex(1); // Go back to main menu
}

void MainWindow::on_modifyForm_searchButton_clicked()
{
    if (!validateAndSetSemesterBranch("modify")) {
        return;
    }

    std::string roll = modifySearchRollLineEdit->text().toUpper().toStdString();
    Student s;
    if (gradingSystem.viewStudent(roll, s)) {
        currentModifyingRoll = QString::fromStdString(s.roll); // Store the original roll for modification
        modifyNameLineEdit->setText(QString::fromStdString(s.name));
        modifyRollLineEdit->setText(QString::fromStdString(s.roll));
        modifyPhoneLineEdit->setText(QString::fromStdString(s.phone));
        modifyDOBLineEdit->setText(QString::fromStdString(s.dob));

        // Set the combo box values to the student's actual semester/branch (display only)
        modifySemesterComboBox->setCurrentText(QString::fromStdString(s.semester));
        modifyBranchComboBox->setCurrentText(capitalizeEachWord(QString::fromStdString(s.branch))); // Use helper

        modifySubjectCountSpinBox->setValue(s.grades.size());
        createDynamicGradeInputs(s.grades.size(), modifyGradesLayout, modifyGradeMarksLineEdits);
        // Pre-fill grades (assuming direct mark entry for modification)
        // Note: Your C++ code `getGrade` converts mark to grade. Here we need marks.
        // For simplicity, let's assume direct grade input for modification or handle mark conversion on UI side.
        // Since original code only stores grades, we can't reverse grades back to marks directly for editing.
        // User needs to re-enter marks for grades.
        for (QLineEdit* gradeEdit : modifyGradeMarksLineEdits) {
            gradeEdit->clear(); // Clear, user must re-enter marks
            gradeEdit->setPlaceholderText("Re-enter marks (0-100)");
        }


        // Enable modification fields
        QList<QLineEdit*> editFields = {modifyNameLineEdit, modifyRollLineEdit, modifyPhoneLineEdit, modifyDOBLineEdit};
        for(QLineEdit* field : editFields) {
            field->setEnabled(true);
        }
        // modifySemesterComboBox->setEnabled(true); // Don't allow changing student's semester/branch this way
        // modifyBranchComboBox->setEnabled(true);   // unless explicitly needed.
        modifySubjectCountSpinBox->setEnabled(true);
        modifySaveButton->setEnabled(true);

        modifyStatusLabel->setText("<span style='color: green;'>Student found. Ready to modify.</span>");
    } else {
        modifyStatusLabel->setText("<span style='color: red;'>Student not found.</span>");
        modifySaveButton->setEnabled(false);
        // Disable fields again
        QList<QLineEdit*> editFields = {modifyNameLineEdit, modifyRollLineEdit, modifyPhoneLineEdit, modifyDOBLineEdit};
        for(QLineEdit* field : editFields) {
            field->setEnabled(false);
        }
        modifySemesterComboBox->setEnabled(false);
        modifyBranchComboBox->setEnabled(false);
        modifySubjectCountSpinBox->setEnabled(false);
        clearDynamicGradeInputs(modifyGradesLayout, modifyGradeMarksLineEdits);
    }
}

void MainWindow::on_modifyForm_saveButton_clicked()
{
    // Ensure semester and branch are set for the current operation
    if (!validateAndSetSemesterBranch("modify")) {
        return;
    }

    Student s;
    s.name = modifyNameLineEdit->text().toStdString();
    s.roll = modifyRollLineEdit->text().toUpper().toStdString(); // New roll number
    s.phone = modifyPhoneLineEdit->text().toStdString();
    s.dob = modifyDOBLineEdit->text().toStdString();
    s.semester = gradingSystem.getSelectedSemester(); // Keep original student's semester/branch
    s.branch = gradingSystem.getSelectedBranch();

    // Basic validation
    if (!isValidName(s.name)) {
        modifyStatusLabel->setText("<span style='color: red;'>Invalid Name. Only alphabets and spaces allowed.</span>"); return;
    }
    // Only validate roll if it's actually for the selected branch code.
    // If the branch of the student being modified is 'CO', then the new roll should adhere to 'CO' format.
    if (!gradingSystem.isValidRollForBranch(s.roll, s.branch)) {
        modifyStatusLabel->setText(QString("<span style='color: red;'>Invalid Roll Number for %1 branch. Format: 2KXX/BR/XX.</span>").arg(capitalizeEachWord(QString::fromStdString(s.branch)))); return;
    }
    if (!isValidPhone(s.phone)) {
        modifyStatusLabel->setText("<span style='color: red;'>Invalid Phone Number. Must be exactly 10 digits.</span>"); return;
    }
    if (!isValidDOB(s.dob)) {
        modifyStatusLabel->setText("<span style='color: red;'>Invalid DOB. Must be valid and in dd-mm-yyyy format.</span>"); return;
    }

    s.grades.clear();
    for (QLineEdit *markEdit : modifyGradeMarksLineEdits) {
        bool ok;
        int mark = markEdit->text().toInt(&ok);
        if (!ok || mark < 0 || mark > 100) {
            modifyStatusLabel->setText("<span style='color: red;'>Invalid marks entered. Marks must be 0-100.</span>");
            return;
        }
        s.grades.push_back(s.getGrade(mark));
    }

    std::pair<bool, std::string> result = gradingSystem.modifyStudent(currentModifyingRoll.toStdString(), s);
    if (result.first) {
        modifyStatusLabel->setText(QString("<span style='color: green;'>%1</span>").arg(QString::fromStdString(result.second)));
        // Clear fields and disable after successful modification
        modifySearchRollLineEdit->clear();
        modifyNameLineEdit->clear();
        modifyRollLineEdit->clear();
        modifyPhoneLineEdit->clear();
        modifyDOBLineEdit->clear();
        modifySubjectCountSpinBox->setValue(1);
        clearDynamicGradeInputs(modifyGradesLayout, modifyGradeMarksLineEdits);
        modifySaveButton->setEnabled(false);

        QList<QLineEdit*> editFields = {modifyNameLineEdit, modifyRollLineEdit, modifyPhoneLineEdit, modifyDOBLineEdit};
        for(QLineEdit* field : editFields) {
            field->setEnabled(false);
        }
        modifySemesterComboBox->setEnabled(false);
        modifyBranchComboBox->setEnabled(false);
        modifySubjectCountSpinBox->setEnabled(false);

    } else {
        modifyStatusLabel->setText(QString("<span style='color: red;'>%1</span>").arg(QString::fromStdString(result.second)));
    }
}

void MainWindow::on_modifyForm_backButton_clicked()
{
    stackedWidget->setCurrentIndex(1); // Go back to main menu
}

void MainWindow::on_modifyForm_subjectCountSpinBox_valueChanged(int count)
{
    createDynamicGradeInputs(count, modifyGradesLayout, modifyGradeMarksLineEdits);
}

void MainWindow::on_deleteForm_deleteButton_clicked()
{
    if (!validateAndSetSemesterBranch("delete")) {
        return;
    }

    std::string roll = deleteRollLineEdit->text().toUpper().toStdString();
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm Deletion",
                                  "Are you sure you want to delete student with Roll Number: " + QString::fromStdString(roll) + "?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) {
        deleteStatusLabel->setText("<span style='color: orange;'>Deletion cancelled.</span>");
        return;
    }

    std::pair<bool, std::string> result = gradingSystem.deleteStudent(roll);
    if (result.first) {
        deleteStatusLabel->setText(QString("<span style='color: green;'>%1</span>").arg(QString::fromStdString(result.second)));
        deleteRollLineEdit->clear();
    } else {
        deleteStatusLabel->setText(QString("<span style='color: red;'>%1</span>").arg(QString::fromStdString(result.second)));
    }
}

void MainWindow::on_deleteForm_backButton_clicked()
{
    stackedWidget->setCurrentIndex(1); // Go back to main menu
}

// Helper to set semester and branch based on the action context
bool MainWindow::validateAndSetSemesterBranch(const QString &action) {
    QComboBox *semesterCombo = nullptr;
    QComboBox *branchCombo = nullptr;
    QLabel *statusLabel = nullptr;

    // Dynamically find the correct combo boxes and status label based on the current page/action
    if (action == "insert") {
        semesterCombo = insertSemesterComboBox;
        branchCombo = insertBranchComboBox;
        statusLabel = insertStatusLabel;
    } else if (action == "view") {
        // Find the combo boxes within the viewPage's layout
        QList<QComboBox*> combos = viewPage->findChildren<QComboBox*>();
        if (combos.size() >= 2) {
            semesterCombo = combos.at(0);
            branchCombo = combos.at(1);
        }
        statusLabel = viewStatusLabel;
    } else if (action == "modify") {
        // Combo boxes for search in modify page are in the QHBoxLayout
        QHBoxLayout *searchLayout = modifyPage->findChild<QHBoxLayout*>();
        if (searchLayout) {
            QList<QComboBox*> combos = searchLayout->findChildren<QComboBox*>();
            if (combos.size() >= 2) {
                semesterCombo = combos.at(0);
                branchCombo = combos.at(1);
            }
        }
        statusLabel = modifyStatusLabel;
    } else if (action == "delete") {
        // Combo boxes for delete page are in the QFormLayout
        QList<QComboBox*> combos = deletePage->findChildren<QComboBox*>();
        if (combos.size() >= 2) {
            semesterCombo = combos.at(0);
            branchCombo = combos.at(1);
        }
        statusLabel = deleteStatusLabel;
    }

    if (!semesterCombo || !branchCombo || !statusLabel) {
        // This indicates a structural issue in finding the widgets, should not happen if setup is correct
        QMessageBox::critical(this, "UI Error", "Could not find semester/branch combo boxes for the current operation.");
        return false;
    }

    std::string selectedSem = semesterCombo->currentText().toStdString();
    // Convert branch to lowercase for consistency with backend logic
    std::string selectedBr = branchCombo->currentText().toLower().toStdString();

    try {
        int semNum = std::stoi(selectedSem);
        if (semNum < 1 || semNum > 8) {
            statusLabel->setText("<span style='color: red;'>Invalid semester range (1-8).</span>");
            return false;
        }
    } catch (const std::invalid_argument& e) {
        statusLabel->setText("<span style='color: red;'>Invalid semester format.</span>");
        return false;
    } catch (const std::out_of_range& e) {
        statusLabel->setText("<span style='color: red;'>Semester out of range.</span>");
        return false;
    }


    std::vector<std::string> validBranches = {
                                              "computer", "electrical", "mechanical", "chemical", "civil", "management"};
    if (std::find(validBranches.begin(), validBranches.end(), selectedBr) == validBranches.end()) {
        statusLabel->setText("<span style='color: red;'>Invalid branch selected. Allowed: computer, electrical, mechanical, chemical, civil, management.</span>");
        return false;
    }

    gradingSystem.setCurrentSemesterAndBranch(selectedSem, selectedBr);
    statusLabel->setText(QString("<span style='color: blue;'>Data loaded for Semester %1, Branch %2.</span>")
                             .arg(QString::fromStdString(selectedSem))
                             .arg(capitalizeEachWord(QString::fromStdString(selectedBr)))); // Use helper
    return true; // Indicate success
}
