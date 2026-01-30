#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowTitle("Calculator");

    m_numbers = new QButtonGroup(this);
    m_operations = new QButtonGroup(this);
    m_display = new QLabel("0");

    m_layout = new QVBoxLayout(this); // INITIALIZE LAYOUT
    m_grid = new QGridLayout();
    m_layout->addWidget(m_display);
    m_layout->addLayout(m_grid);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_grid->setSpacing(10);
    m_grid->setContentsMargins(10, 10, 10, 10);
    this->setLayout(m_layout);

    for (int i = 1; i <= 9; ++i) {
        QString text = QString::number(i);
        QPushButton *btn = new QPushButton(text, this);

        m_numbers->addButton(btn, i);

        int row = 2 - ((i-1) / 3);
        int col = (i - 1) % 3;

        m_grid->addWidget(btn, row, col);
    }

    m_zero = new QPushButton("0", this);
    m_numbers->addButton(m_zero, 0);
    m_grid->addWidget(m_zero, 3, 1);

    m_dot = new QPushButton(".", this);
    m_grid->addWidget(m_dot, 3, 2);

    m_clear = new QPushButton("C", this);
    m_clear->setObjectName("grayButton");
    m_grid->addWidget(m_clear, 3, 0);

    m_del = new QPushButton("Del", this);
    m_del->setObjectName("grayButton");
    m_grid->addWidget(m_del, 0, 4);

    m_negate = new QPushButton("+/-", this);
    m_negate->setObjectName("grayButton");
    m_grid->addWidget(m_negate, 0, 3);

    m_multiply = new QPushButton("*", this);
    m_multiply->setObjectName("orangeButton");
    m_operations->addButton(m_multiply);
    m_grid->addWidget(m_multiply, 1, 3);

    m_divide = new QPushButton("/", this);
    m_divide->setObjectName("orangeButton");
    m_operations->addButton(m_divide);
    m_grid->addWidget(m_divide, 1, 4);

    m_subtract = new QPushButton("-", this);
    m_subtract->setObjectName("orangeButton");
    m_operations->addButton(m_subtract);
    m_grid->addWidget(m_subtract, 2, 3);

    m_add = new QPushButton("+", this);
    m_add->setObjectName("orangeButton");
    m_operations->addButton(m_add);
    m_grid->addWidget(m_add, 3, 3);

    m_calculate = new QPushButton("=", this);
    m_calculate->setObjectName("orangeButton");
    m_calculate->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    m_grid->addWidget(m_calculate, 2, 4, 2, 1);

    connect(m_numbers, &QButtonGroup::idClicked, this, &MainWindow::digitClicked);
    connect(m_clear, &QPushButton::clicked, this, &MainWindow::clearClicked);
    connect(m_del, &QPushButton::clicked, this, &MainWindow::delClicked);
    connect(m_negate, &QPushButton::clicked, this, &MainWindow::negateClicked);
    connect(m_dot, &QPushButton::clicked, this, &MainWindow::dotClicked);
    connect(m_operations, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked), this, &MainWindow::operatorClicked);
    connect(m_calculate, &QPushButton::clicked, this, &MainWindow::calculateClicked);
}

double MainWindow::performCalculation(double n1, double n2, PendingOperation op) {
    if (op == PendingOperation::Add) return n1 + n2;
    if (op == PendingOperation::Subtract) return n1 - n2;
    if (op == PendingOperation::Multiply) return n1 * n2;
    if (op == PendingOperation::Divide) {
        if (n2 == 0.0) return 0.0;
        return n1 / n2;
    }
    return n2;
}

void MainWindow::operatorClicked(QAbstractButton *button)
{
    QString clickedOp = button->text();

    PendingOperation newOp = PendingOperation::None;
    if (clickedOp == "+") newOp = PendingOperation::Add;
    else if (clickedOp == "-") newOp = PendingOperation::Subtract;
    else if (clickedOp == "*") newOp = PendingOperation::Multiply;
    else if (clickedOp == "/") newOp = PendingOperation::Divide;

    if (m_waitingForOperand && m_pendingOp != PendingOperation::None) {
        m_pendingOp = newOp;
        m_display->setText(clickedOp);
        return;
    }

    double currentValue = m_display->text().toDouble();

    if (m_pendingOp != PendingOperation::None) {
        m_accumulatedValue = performCalculation(m_accumulatedValue, currentValue, m_pendingOp);
    } else {
        m_accumulatedValue = currentValue;
    }

    m_display->setText(clickedOp);

    m_pendingOp = newOp;
    m_waitingForOperand = true;
}

void MainWindow::calculateClicked()
{
    if (m_pendingOp == PendingOperation::None) return;

    double currentValue = m_display->text().toDouble();
    double result = performCalculation(m_accumulatedValue, currentValue, m_pendingOp);

    m_display->setText(QString::number(result));

    m_accumulatedValue = 0.0;
    m_pendingOp = PendingOperation::None;
    m_waitingForOperand = true;
}

void MainWindow::digitClicked(int id)
{
    if (m_waitingForOperand) {
        m_display->clear();
        m_waitingForOperand = false;
    }

    if (m_display->text() == "0"){
        m_display->setText(QString::number(id));
    }else{
        m_display->setText(m_display->text() + QString::number(id));
    }
}

void MainWindow::dotClicked()
{
    if (m_display->text().contains(".") || m_waitingForOperand == true){
        return;
    }
    m_display->setText(m_display->text() + ".");
}

void MainWindow::negateClicked()
{
    QString displayText = m_display->text();
    if (displayText == "0" || m_waitingForOperand == true){
        return;
    }
    if (displayText.contains("-")){
        displayText.remove("-");
    }else{
        displayText = ("-" + displayText);
    }
    m_display->setText(displayText);
}

void MainWindow::clearClicked()
{
    m_waitingForOperand = false;
    m_accumulatedValue = 0.0;
    m_pendingOp = PendingOperation::None;
    m_display->setText("0");
}

void MainWindow::delClicked()
{
    QString displayText = m_display->text();
    if (displayText == "0" ||  m_waitingForOperand == true){
        return;
    }
    displayText.chop(1);
    if (displayText.isEmpty()){
        displayText = "0";
    }
    m_display->setText(displayText);
}
