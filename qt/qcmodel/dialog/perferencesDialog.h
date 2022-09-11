#ifndef _PREFERENCESDIALOG_H
#define _PREFERENCESDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>

class PerferencesDialog : public QDialog
{
    Q_OBJECT
public:
    PerferencesDialog(int parperSize, QWidget *parent = 0);
    ~PerferencesDialog();

    int parperSize() const { return parperComboBox->currentIndex(); }

private:
    QLabel *label;
    QComboBox *parperComboBox;
    QPushButton *OKButton;
    QPushButton *closeButton;

private slots:
    void OK();
};

#endif
