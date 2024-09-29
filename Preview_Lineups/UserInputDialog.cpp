#include "UserInputDialog.h"

UserInputDialog::UserInputDialog(QWidget* parent) : QDialog(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);

    QFormLayout* formLayout = new QFormLayout();
    usernameEdit = new QLineEdit(this);
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);
    countryEdit = new QLineEdit(this);
    postalCodeEdit = new QLineEdit(this);

    formLayout->addRow("Username:", usernameEdit);
    formLayout->addRow("Password:", passwordEdit);
    formLayout->addRow("Country:", countryEdit);
    formLayout->addRow("Postal Code:", postalCodeEdit);

    layout->addLayout(formLayout);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    layout->addWidget(buttonBox);
}

QString UserInputDialog::getUsername() const { return usernameEdit->text(); }
QString UserInputDialog::getPassword() const { return passwordEdit->text(); }
QString UserInputDialog::getCountry() const { return countryEdit->text(); }
QString UserInputDialog::getPostalCode() const { return postalCodeEdit->text(); }
