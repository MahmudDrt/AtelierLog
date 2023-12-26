#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QSqlQuery>
#include <QDebug>
#include <QInputDialog>
#include <QFile>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QComboBox>
#include <QFormLayout>
#include <QSqlError>
#include <QDateEdit>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("dipper.db");

    if (!db.open())
    {
        qDebug() << "Error: connection with database failed";
    }
    else
    {
        qDebug() << "Database: connection ok";
    }

    //главное окно
    ui->setupUi(this);
    this -> setGeometry(650, 250, 300, 200);
    // Установка заголовка окна
    this->setWindowTitle("Учет заказов швейного ателье");
    this->setGeometry(650, 250, 300, 200);

    //кнопка Список товаров
    QPushButton *btnOccupy = new QPushButton("Добавить товар", this);
    btnOccupy->setStyleSheet("QPushButton { background-color: black; color: white; font-size: 16px; width: auto; height: auto; }");
    connect(btnOccupy, &QPushButton::clicked, this, &MainWindow::addNewClothing);

    //кнопка Заказать товар
    QPushButton *btnRegister = new QPushButton("Заказать товар", this);
    btnRegister->setStyleSheet("QPushButton { background-color: black; color: white; font-size: 16px; width: auto; height: auto; }");
    connect(btnRegister, &QPushButton::clicked, this, &MainWindow::openNewWindowForBooking);

    //кнопка Освободить заказ
    QPushButton *btnRelease = new QPushButton("Освободить заказ", this);
    btnRelease->setStyleSheet("QPushButton { background-color: black; color: white; font-size: 16px; width: auto; height: auto; }");
    connect(btnRelease, &QPushButton::clicked, this, &MainWindow::releaseComputer);

    // Создание QLabel для отображения приветствия
    welcome_label = new QLabel(this);
    welcome_label->setText("Добро пожаловать в систему учета заказов швейного ателье!");
    welcome_label->setAlignment(Qt::AlignCenter);
    welcome_label->setStyleSheet("border: 16px solid white; font-size: 20px; color: blue; background-color: white;");
    welcome_label->setGeometry(0, 50, 300, 50);

    // Создание QLabel для отображения времени
    time_label1 = new QLabel(this);
    time_label1->setAlignment(Qt::AlignCenter);
    time_label1->setStyleSheet("font-size: 20px; font-weight: bold;");
    time_label1->setGeometry(0, 0, 300, 50);

    // Создание таймера
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::update_time);
    timer->start(1000);

    // Создание QLabel для отображения времени
    time_label2 = new QLabel(this);
    time_label2->setAlignment(Qt::AlignCenter);
    time_label2->setStyleSheet("font-size: 20px; font-weight: bold;");
    time_label2->setGeometry(0, 0, 300, 50);

    // Создание таймера
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::update_time);
    start_time = QTime::currentTime();
    timer->start(1000);

    //кнопка отчет швейного отелье
    QPushButton *btnReport = new QPushButton("Отчет швейного отелье", this);
    btnReport->setStyleSheet("QPushButton { background-color: black; color: white; font-size: 16px; width: auto; height: auto; }");
    connect(btnReport, &QPushButton::clicked, this, &MainWindow::createReport);

    // Создание вертикального менеджера компановки
    QVBoxLayout *vLayout = new QVBoxLayout;

    // Добавление кнопок в менеджер компановки
    vLayout->addWidget(welcome_label);
    vLayout->addWidget(time_label1);
    vLayout->addWidget(time_label2);
    vLayout->addWidget(btnOccupy);
    vLayout->addWidget(btnRegister);
    vLayout->addWidget(btnRelease);
    vLayout->addWidget(btnReport);

    // Установка менеджера компановки для главного окна
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(vLayout);
    setCentralWidget(centralWidget);
}

void MainWindow::update_time()
{
    QTime current_time = QTime::currentTime();
    QString time_text1 = current_time.toString("hh:mm:ss");
    time_label1->setText(time_text1);
    int elapsed_seconds = start_time.secsTo(current_time);
    QString time_text2 = QTime(0, 0).addSecs(elapsed_seconds).toString("hh:mm:ss");
    time_label2->setText(time_text2);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateTime() {
    *time = time->addSecs(1);
    timeEdit->setTime(*time); // Обновите время в QTimeEdit
}

void MainWindow::addNewClothing()
{
    bool ok;
    QString name = QInputDialog::getText(this, "Добавить товар", "Введите название товара:", QLineEdit::Normal, "", &ok);
    if (ok && !name.isEmpty()) {
        QString sum = QInputDialog::getText(this, "Добавить товар", "Введите сумму товара:", QLineEdit::Normal, "", &ok);
        if (ok && !sum.isEmpty()) {
            QSqlQuery query;
            query.prepare("INSERT INTO tovarbase (name, sum) VALUES (:name, :sum)");
            query.bindValue(":name", name);
            query.bindValue(":sum", sum);

            if(!query.exec())
            {
                qDebug() << "Error inserting into the table.";
            } else {
                qDebug() << "New clothing added successfully!";
            }
        }
    }
}


void MainWindow::openNewWindowForBooking()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Заказать товар");

    QLabel *nameLabel = new QLabel("Фамилия:", dialog);
    QLineEdit *nameLineEdit = new QLineEdit(dialog);

    QLabel *tovarLabel = new QLabel("Товар:", dialog);
    QComboBox *tovarComboBox = new QComboBox(dialog);
    QSqlQuery query;
    query.prepare("SELECT DISTINCT name FROM tovarbase");
    if(!query.exec())
    {
        qDebug() << "Error selecting from the table.";
    } else {
        while (query.next()) {
            QString name = query.value(0).toString();
            tovarComboBox->addItem(name);
        }
    }
    QLabel *sumLabel = new QLabel("Сумма:", dialog);



    //QComboBox *tovarComboBox = new QComboBox(dialog);
    QLineEdit *sumLineEdit = new QLineEdit(dialog);

    //QSqlQuery query;
    //QLineEdit *sumLineEdit = new QLineEdit(dialog);
    query.prepare("SELECT sum FROM tovarbase WHERE name = :name");
    query.bindValue(":name", tovarComboBox->currentText());
    if(!query.exec())
    {
        qDebug() << "Error selecting from the table.";
    } else {
        while (query.next()) {
            QString sum = query.value(0).toString();
            sumLineEdit->setText(sum);
        }
    }

    connect(tovarComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(updateSum(QString)));


    QDateEdit *dateEdit = new QDateEdit(QDate::currentDate(), dialog);
    dateEdit->setCalendarPopup(true);

    QPushButton *okButton = new QPushButton("OK", dialog);
    QPushButton *backButton = new QPushButton("Назад", dialog);
        connect(backButton, &QPushButton::clicked, dialog, &QDialog::close);



    QGridLayout *layout = new QGridLayout(dialog);
    layout->addWidget(nameLabel, 0, 0);
    layout->addWidget(nameLineEdit, 0, 1);
    layout->addWidget(tovarLabel, 1, 0);
    layout->addWidget(tovarComboBox, 1, 1);
    layout->addWidget(sumLabel, 2, 0);
    layout->addWidget(sumLineEdit, 2, 1);
    //layout->addWidget(dateLabel, 3, 0);
    layout->addWidget(dateEdit, 3, 1);
    layout->addWidget(okButton, 4, 0);
    //layout->addWidget(cancelButton, 4, 1);
    layout->addWidget(backButton);

    connect(okButton, &QPushButton::clicked, dialog, &QDialog::accept);
    //connect(cancelButton, &QPushButton::clicked, dialog, &QDialog::reject);

    if (dialog->exec() == QDialog::Accepted) {
        QString customer = nameLineEdit->text();
        QString tovar = tovarComboBox->currentText();
        QString sum = sumLineEdit->text();
        QString date = dateEdit->date().toString("dd.MM.yyyy");

        QSqlQuery query;
        query.prepare("INSERT INTO base (date, customer, tovar, sum) VALUES (:date, :customer, :tovar, :sum)");
        query.bindValue(":date", date);
        query.bindValue(":customer", customer);
        query.bindValue(":tovar", tovar);
        query.bindValue(":sum", sum);

        if (!query.exec()) {
            qDebug() << "Ошибка при вставке записи в таблицу.";
        } else {
            qDebug() << "Новая запись успешно добавлена!";
            writeToFile("Фамилия: " + customer + ", Дата: " + date + ", Товар: " + tovar + ", Сумма: " + sum);
        }
    }
    dialog->setLayout(layout);

}

void MainWindow::releaseComputer()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("dipper.db");

    if (!db.open())
    {
        qDebug() << "Error: connection with database failed";
    }
    else
    {
        qDebug() << "Database: connection ok";
    }
    QSqlQuery query;
    query.exec("SELECT DISTINCT customer FROM base");

    QStringList logins;
    while (query.next()) {
    logins.append(query.value(0).toString());
    }

    if (logins.isEmpty()) {
        QMessageBox::information(this, "Освободить заказ", "Нет доступных фамилий для освобождения заказа.");
    } else {
        bool ok;
        QString selectedLogin = QInputDialog::getItem(this, "Освободить заказ", "Выберите фамилию:", logins, 0, false, &ok);

        if (ok && !selectedLogin.isEmpty()) {
            query.prepare("SELECT DISTINCT date FROM base WHERE customer = :customer");
            query.bindValue(":customer", selectedLogin);

            if(query.exec())
            {
                QStringList dates;
                while (query.next()) {
                    dates.append(query.value(0).toString());
                }

                if (dates.isEmpty()) {
                    QMessageBox::information(this, "Освободить заказ", "Нет доступных дат для освобождения заказа.");
                } else {
                    QString selectedDate = QInputDialog::getItem(this, "Освободить заказ", "Выберите дату:", dates, 0, false, &ok);
                    if (ok && !selectedDate.isEmpty()) {
                        removeFromDatabase(selectedLogin, selectedDate);
                    }
                }
            }
        }
    }
}

void MainWindow::removeFromDatabase(const QString &date, const QString &customer)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("dipper.db");

    if (!db.open())
    {
        qDebug() << "Error: connection with database failed";
    }
    else
    {
        qDebug() << "Database: connection ok";
    }

    QSqlQuery query;
    query.prepare("DELETE FROM base WHERE customer = :customer AND date = :date");
    query.bindValue(":customer", customer);
    query.bindValue(":date", date);

    if (!query.exec()) {
        qDebug() << "Error deleting from the table.";
    } else {
        qDebug() << "Record deleted successfully!";
    }
}

void MainWindow::writeToFile(const QString &data)
{
    QFile file("output.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QTextStream stream(&file);
        stream << data << "\n";
        file.close();
    } else {
        qDebug() << "Error opening the file.";
    }
}

void MainWindow::createReport()
{
    QSqlQuery query;
    query.exec("SELECT DISTINCT date FROM base");

    QStringList dates;
    while (query.next()) {
        dates.append(query.value(0).toString());
    }

    if (dates.isEmpty()) {
        QMessageBox::information(this, "Отчет швейного отелье", "Нет доступных дат для создания отчета.");
    } else {
        bool ok;
        QString selectedDate = QInputDialog::getItem(this, "Отчет швейного отелье", "Выберите дату:", dates, 0, false, &ok);
        query.prepare("SELECT * FROM base WHERE date = :date");
        if (ok && !selectedDate.isEmpty()) {

            query.bindValue(":date", selectedDate);

            if(query.exec())
            {
                QString report;
                while (query.next()) {
                    report += " date: " + query.value(0).toString();
                    report += ", customer: " + query.value(1).toString();
                    report += ", tovar: " + query.value(2).toString();
                    report += ", sum: " + query.value(3).toString() + "\n";
                }
                if (report.isEmpty()) {
                    QMessageBox::information(this, "Отчет швейного отелье", "Нет записей для выбранной даты.");
                } else {
                    QMessageBox::information(this, "Отчет швейного отелье", report);
                }
            }
        }
    }
}


