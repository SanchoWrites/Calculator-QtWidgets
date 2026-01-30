#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QApplication>
#include <QButtonGroup>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QDebug>

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
private slots:
    void operatorClicked(QAbstractButton *button);
    void calculateClicked();
    void digitClicked(int id);
    void dotClicked();
    void negateClicked();
    void clearClicked();
    void delClicked();
private:
    QVBoxLayout *m_layout;
    QGridLayout *m_grid;
    QLabel *m_display;
    QButtonGroup *m_numbers;
    QPushButton *m_zero;
    QPushButton *m_negate;
    QPushButton *m_del;
    QPushButton *m_multiply;
    QPushButton *m_divide;
    QPushButton *m_subtract;
    QPushButton *m_add;
    QButtonGroup *m_operations;
    QPushButton *m_dot;
    QPushButton *m_clear;
    QPushButton *m_calculate;
    enum class PendingOperation { None, Add, Subtract, Multiply, Divide };
    double performCalculation(double n1, double n2, PendingOperation op);
    double m_accumulatedValue = 0.0;
    PendingOperation m_pendingOp = PendingOperation::None;
    bool m_waitingForOperand = true;
};
#endif // MAINWINDOW_H
