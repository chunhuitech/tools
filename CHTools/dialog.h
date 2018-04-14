#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_pushButtonSelFolder_clicked();

    void on_pushButtonModTime_clicked();


    void on_uiPushButtonGenSql_clicked();

private:
    QString getNewFileNameApp(QString newfileName, int appIndex);
private:
    QString selDirPath;
private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
