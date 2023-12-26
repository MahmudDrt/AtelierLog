// mainwindow.h

#include <QMainWindow>
#include <QLineEdit>
#include <QTimer>
#include <QTime>
#include <QTimeEdit>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    QString isLoginExists(const QString &login, const QString &date);
    ~MainWindow();

private slots:
    void openNewWindow();
    void addNewClothing();
    void openNewWindowForComputerNumber();
    void openNewWindowForBooking();
    void writeToDatabaseWithComputerNumber(const QString &date);
    void writeToFile(const QString &data);
    void releaseComputer();
    void removeFromDatabase(const QString &date, const QString &customer);
    void updateTime();
    void update_time();
    void createReport();
    void QCalendarWidget();
    void updateSum(QString name);

private:
    Ui::MainWindow *ui;
    QLineEdit* nameLineEdit;
    QLineEdit* computerNumberLineEdit;
    QTimer *timer;
    QTime *time;
    QTimeEdit *timeEdit;
    QLabel *time_label1;
    QLabel *time_label2;
    QLabel *welcome_label;
    QTime start_time;

    int ordersCount = 0;
};
