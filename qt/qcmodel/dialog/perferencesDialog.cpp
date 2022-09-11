#include "perferencesDialog.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

PerferencesDialog::PerferencesDialog(int parperSize, QWidget *parent) : QDialog(parent)
{
    label = new QLabel(tr("parper size"));
    parperComboBox = new QComboBox();
    parperComboBox->addItem(tr("A2"));
    parperComboBox->addItem(tr("A3"));
    parperComboBox->addItem(tr("A4"));
    parperComboBox->setCurrentIndex(parperSize);

    OKButton = new QPushButton(tr("OK"));
    connect(OKButton, SIGNAL(clicked()), this, SLOT(OK()));

    closeButton = new QPushButton(tr("Close"));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *topleftLayout = new QHBoxLayout;
    topleftLayout->addWidget(label);
    topleftLayout->addWidget(parperComboBox);

    QHBoxLayout *rightLayout = new QHBoxLayout;
    rightLayout->addWidget(OKButton);
    rightLayout->addWidget(closeButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topleftLayout);
    mainLayout->addLayout(rightLayout);

    setLayout(mainLayout);
}

PerferencesDialog::~PerferencesDialog()
{
}

void PerferencesDialog::OK()
{
    accept();
}
