#ifndef UTILSFORUSERINFO_H
#define UTILSFORUSERINFO_H

#include <QObject>
#include <QMap>
#include <QVariant>

class QFrame;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QLabel;

class UtilsForUserinfo : public QObject
{
    Q_OBJECT
public:
    explicit UtilsForUserinfo(QObject *parent = nullptr);

public:
    QFrame * buildItemForUsers();

    void refreshUserLogo(QString logo);
    void refreshUserNickname(QString name);
    void refreshUserType(QString type);
    void setObjectPathData(QString op);

public:
    QFrame * mainItemFrame;

    QPushButton * logoBtn;
    QPushButton * changeTypeBtn;
    QPushButton * changePwdBtn;
    QPushButton * delUserBtn;

    QLabel * nickNameLabel;
    QLabel * typeLabel;

    QVBoxLayout * mainUserVerLayout;
    QVBoxLayout * subUserInfoVerLayout;
    QHBoxLayout * mUserInfoHorLayout;

protected:
    QFrame * createHLine(QFrame * f, int len = 0);
    QFrame * createVLine(QFrame * f, int len = 0);
    bool setUtilsTextDynamic(QLabel * label, QString string);

public slots:
    void userPropertyChangedSlot(QString, QMap<QString, QVariant>, QStringList);

signals:
    void changePwdBtnPressed();
    void changeLogoBtnPressed();
    void changeTypeBtnPressed();
    void deleteUserBtnPressed();

};

#endif // UTILSFORUSERINFO_H
