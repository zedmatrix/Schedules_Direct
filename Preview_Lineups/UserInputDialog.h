#ifndef USERINPUTDIALOG_H
#define USERINPUTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QVBoxLayout>

class UserInputDialog : public QDialog {
    Q_OBJECT

public:
    explicit UserInputDialog(QWidget* parent = nullptr);

    QString getUsername() const;
    QString getPassword() const;
    QString getCountry() const;
    QString getPostalCode() const;

private:
    QLineEdit* usernameEdit;
    QLineEdit* passwordEdit;
    QLineEdit* countryEdit;
    QLineEdit* postalCodeEdit;
};

#endif // USERINPUTDIALOG_H
