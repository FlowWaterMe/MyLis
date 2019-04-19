/********************************************************************************
** Form generated from reading UI file 'LISCommunication.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LISCOMMUNICATION_H
#define UI_LISCOMMUNICATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LISCommunicationClass
{
public:
    QGridLayout *gridLayout;
    QWidget *widget;
    QGridLayout *gridLayout_2;
    QWidget *widget_2;
    QGridLayout *gridLayout_3;
    QTableWidget *tabPatientInfo;
    QLabel *label;
    QWidget *widget_4;
    QGridLayout *gridLayout_4;
    QTableWidget *tabTestInfo;
    QLabel *label_2;
    QTableWidget *tabSampleInfo;
    QLabel *label_13;
    QSpacerItem *horizontalSpacer;
    QTextEdit *TEdtRecord;
    QLabel *labPrompt;
    QLabel *labStatus;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout_6;
    QWidget *widget_5;
    QGridLayout *gridLayout_7;
    QComboBox *cbxSerialNum;
    QLabel *label_7;
    QLabel *label_6;
    QComboBox *cbxStopBit;
    QComboBox *cbxParity;
    QComboBox *cbxBaudrate;
    QComboBox *cbxDataBit;
    QLabel *label_8;
    QLabel *label_10;
    QLabel *label_9;
    QLabel *label_5;
    QComboBox *cbxHandShake;
    QLabel *label_11;
    QLineEdit *edtXON;
    QLabel *label_12;
    QLineEdit *edtXOFF;
    QWidget *tab_2;
    QGridLayout *gridLayout_5;
    QWidget *widget_3;
    QGridLayout *gridLayout_8;
    QLabel *label_3;
    QLineEdit *edtIP;
    QLabel *label_4;
    QLineEdit *edtPort;
    QPushButton *btnClearText;
    QPushButton *btnCon;
    QPushButton *btnDisCon;

    void setupUi(QDialog *LISCommunicationClass)
    {
        if (LISCommunicationClass->objectName().isEmpty())
            LISCommunicationClass->setObjectName(QStringLiteral("LISCommunicationClass"));
        LISCommunicationClass->resize(1177, 679);
        gridLayout = new QGridLayout(LISCommunicationClass);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        widget = new QWidget(LISCommunicationClass);
        widget->setObjectName(QStringLiteral("widget"));
        gridLayout_2 = new QGridLayout(widget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        widget_2 = new QWidget(widget);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        gridLayout_3 = new QGridLayout(widget_2);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        tabPatientInfo = new QTableWidget(widget_2);
        tabPatientInfo->setObjectName(QStringLiteral("tabPatientInfo"));

        gridLayout_3->addWidget(tabPatientInfo, 1, 0, 1, 3);

        label = new QLabel(widget_2);
        label->setObjectName(QStringLiteral("label"));

        gridLayout_3->addWidget(label, 0, 0, 1, 3);

        widget_4 = new QWidget(widget_2);
        widget_4->setObjectName(QStringLiteral("widget_4"));
        gridLayout_4 = new QGridLayout(widget_4);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        tabTestInfo = new QTableWidget(widget_4);
        tabTestInfo->setObjectName(QStringLiteral("tabTestInfo"));

        gridLayout_4->addWidget(tabTestInfo, 1, 1, 1, 1);

        label_2 = new QLabel(widget_4);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout_4->addWidget(label_2, 0, 1, 1, 1);

        tabSampleInfo = new QTableWidget(widget_4);
        tabSampleInfo->setObjectName(QStringLiteral("tabSampleInfo"));

        gridLayout_4->addWidget(tabSampleInfo, 1, 0, 1, 1);

        label_13 = new QLabel(widget_4);
        label_13->setObjectName(QStringLiteral("label_13"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_13->sizePolicy().hasHeightForWidth());
        label_13->setSizePolicy(sizePolicy);

        gridLayout_4->addWidget(label_13, 0, 0, 1, 1);


        gridLayout_3->addWidget(widget_4, 2, 0, 1, 3);


        gridLayout_2->addWidget(widget_2, 2, 0, 1, 6);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 1, 0, 1, 1);

        TEdtRecord = new QTextEdit(widget);
        TEdtRecord->setObjectName(QStringLiteral("TEdtRecord"));
        TEdtRecord->setMinimumSize(QSize(350, 0));
        TEdtRecord->setMaximumSize(QSize(400, 16777215));
        TEdtRecord->setReadOnly(true);

        gridLayout_2->addWidget(TEdtRecord, 0, 8, 4, 1);

        labPrompt = new QLabel(widget);
        labPrompt->setObjectName(QStringLiteral("labPrompt"));

        gridLayout_2->addWidget(labPrompt, 4, 3, 1, 6);

        labStatus = new QLabel(widget);
        labStatus->setObjectName(QStringLiteral("labStatus"));
        labStatus->setMaximumSize(QSize(150, 16777215));

        gridLayout_2->addWidget(labStatus, 4, 0, 1, 1);

        tabWidget = new QTabWidget(widget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy1);
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        gridLayout_6 = new QGridLayout(tab);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        gridLayout_6->setContentsMargins(0, 0, 0, 0);
        widget_5 = new QWidget(tab);
        widget_5->setObjectName(QStringLiteral("widget_5"));
        gridLayout_7 = new QGridLayout(widget_5);
        gridLayout_7->setSpacing(6);
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        cbxSerialNum = new QComboBox(widget_5);
        cbxSerialNum->setObjectName(QStringLiteral("cbxSerialNum"));
        sizePolicy1.setHeightForWidth(cbxSerialNum->sizePolicy().hasHeightForWidth());
        cbxSerialNum->setSizePolicy(sizePolicy1);

        gridLayout_7->addWidget(cbxSerialNum, 0, 1, 1, 1);

        label_7 = new QLabel(widget_5);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout_7->addWidget(label_7, 0, 2, 1, 1);

        label_6 = new QLabel(widget_5);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout_7->addWidget(label_6, 0, 0, 1, 1);

        cbxStopBit = new QComboBox(widget_5);
        cbxStopBit->setObjectName(QStringLiteral("cbxStopBit"));
        sizePolicy1.setHeightForWidth(cbxStopBit->sizePolicy().hasHeightForWidth());
        cbxStopBit->setSizePolicy(sizePolicy1);

        gridLayout_7->addWidget(cbxStopBit, 0, 9, 1, 1);

        cbxParity = new QComboBox(widget_5);
        cbxParity->setObjectName(QStringLiteral("cbxParity"));
        sizePolicy1.setHeightForWidth(cbxParity->sizePolicy().hasHeightForWidth());
        cbxParity->setSizePolicy(sizePolicy1);

        gridLayout_7->addWidget(cbxParity, 0, 7, 1, 1);

        cbxBaudrate = new QComboBox(widget_5);
        cbxBaudrate->setObjectName(QStringLiteral("cbxBaudrate"));

        gridLayout_7->addWidget(cbxBaudrate, 0, 3, 1, 1);

        cbxDataBit = new QComboBox(widget_5);
        cbxDataBit->setObjectName(QStringLiteral("cbxDataBit"));

        gridLayout_7->addWidget(cbxDataBit, 0, 5, 1, 1);

        label_8 = new QLabel(widget_5);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout_7->addWidget(label_8, 0, 4, 1, 1);

        label_10 = new QLabel(widget_5);
        label_10->setObjectName(QStringLiteral("label_10"));

        gridLayout_7->addWidget(label_10, 0, 8, 1, 1);

        label_9 = new QLabel(widget_5);
        label_9->setObjectName(QStringLiteral("label_9"));

        gridLayout_7->addWidget(label_9, 0, 6, 1, 1);

        label_5 = new QLabel(widget_5);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout_7->addWidget(label_5, 1, 0, 1, 1);

        cbxHandShake = new QComboBox(widget_5);
        cbxHandShake->setObjectName(QStringLiteral("cbxHandShake"));

        gridLayout_7->addWidget(cbxHandShake, 1, 1, 1, 1);

        label_11 = new QLabel(widget_5);
        label_11->setObjectName(QStringLiteral("label_11"));

        gridLayout_7->addWidget(label_11, 1, 2, 1, 1);

        edtXON = new QLineEdit(widget_5);
        edtXON->setObjectName(QStringLiteral("edtXON"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(edtXON->sizePolicy().hasHeightForWidth());
        edtXON->setSizePolicy(sizePolicy2);

        gridLayout_7->addWidget(edtXON, 1, 3, 1, 1);

        label_12 = new QLabel(widget_5);
        label_12->setObjectName(QStringLiteral("label_12"));

        gridLayout_7->addWidget(label_12, 1, 4, 1, 1);

        edtXOFF = new QLineEdit(widget_5);
        edtXOFF->setObjectName(QStringLiteral("edtXOFF"));
        sizePolicy2.setHeightForWidth(edtXOFF->sizePolicy().hasHeightForWidth());
        edtXOFF->setSizePolicy(sizePolicy2);

        gridLayout_7->addWidget(edtXOFF, 1, 5, 1, 1);


        gridLayout_6->addWidget(widget_5, 0, 0, 1, 1);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        gridLayout_5 = new QGridLayout(tab_2);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        gridLayout_5->setContentsMargins(0, 0, 0, 0);
        widget_3 = new QWidget(tab_2);
        widget_3->setObjectName(QStringLiteral("widget_3"));
        gridLayout_8 = new QGridLayout(widget_3);
        gridLayout_8->setSpacing(6);
        gridLayout_8->setContentsMargins(11, 11, 11, 11);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        label_3 = new QLabel(widget_3);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout_8->addWidget(label_3, 0, 0, 1, 1);

        edtIP = new QLineEdit(widget_3);
        edtIP->setObjectName(QStringLiteral("edtIP"));

        gridLayout_8->addWidget(edtIP, 0, 1, 1, 1);

        label_4 = new QLabel(widget_3);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout_8->addWidget(label_4, 0, 2, 1, 1);

        edtPort = new QLineEdit(widget_3);
        edtPort->setObjectName(QStringLiteral("edtPort"));

        gridLayout_8->addWidget(edtPort, 0, 3, 1, 1);


        gridLayout_5->addWidget(widget_3, 0, 0, 1, 1);

        tabWidget->addTab(tab_2, QString());

        gridLayout_2->addWidget(tabWidget, 0, 0, 1, 6);

        btnClearText = new QPushButton(widget);
        btnClearText->setObjectName(QStringLiteral("btnClearText"));

        gridLayout_2->addWidget(btnClearText, 1, 3, 1, 1);

        btnCon = new QPushButton(widget);
        btnCon->setObjectName(QStringLiteral("btnCon"));

        gridLayout_2->addWidget(btnCon, 1, 1, 1, 1);

        btnDisCon = new QPushButton(widget);
        btnDisCon->setObjectName(QStringLiteral("btnDisCon"));

        gridLayout_2->addWidget(btnDisCon, 1, 2, 1, 1);


        gridLayout->addWidget(widget, 0, 0, 1, 1);


        retranslateUi(LISCommunicationClass);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(LISCommunicationClass);
    } // setupUi

    void retranslateUi(QDialog *LISCommunicationClass)
    {
        LISCommunicationClass->setWindowTitle(QApplication::translate("LISCommunicationClass", "LISCommunication", Q_NULLPTR));
        label->setText(QApplication::translate("LISCommunicationClass", "\347\227\205\344\272\272\344\277\241\346\201\257", Q_NULLPTR));
        label_2->setText(QApplication::translate("LISCommunicationClass", "\346\265\213\350\257\225\344\277\241\346\201\257", Q_NULLPTR));
        label_13->setText(QApplication::translate("LISCommunicationClass", "\346\240\267\346\234\254\344\277\241\346\201\257", Q_NULLPTR));
        labPrompt->setText(QApplication::translate("LISCommunicationClass", "\346\217\220\347\244\272:", Q_NULLPTR));
        labStatus->setText(QApplication::translate("LISCommunicationClass", "\347\212\266\346\200\201\357\274\232", Q_NULLPTR));
        label_7->setText(QApplication::translate("LISCommunicationClass", "\346\263\242\347\211\271\347\216\207\357\274\232", Q_NULLPTR));
        label_6->setText(QApplication::translate("LISCommunicationClass", "\347\253\257\345\217\243\357\274\232", Q_NULLPTR));
        label_8->setText(QApplication::translate("LISCommunicationClass", "\346\225\260\346\215\256\344\275\215:", Q_NULLPTR));
        label_10->setText(QApplication::translate("LISCommunicationClass", "\345\201\234\346\255\242\344\275\215:", Q_NULLPTR));
        label_9->setText(QApplication::translate("LISCommunicationClass", "\346\243\200\351\252\214\344\275\215:", Q_NULLPTR));
        label_5->setText(QApplication::translate("LISCommunicationClass", "\346\265\201\346\216\247\345\210\266\357\274\232", Q_NULLPTR));
        label_11->setText(QApplication::translate("LISCommunicationClass", "XON:", Q_NULLPTR));
        label_12->setText(QApplication::translate("LISCommunicationClass", "XOFF:", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("LISCommunicationClass", "\344\270\262\345\217\243\350\277\236\346\216\245", Q_NULLPTR));
        label_3->setText(QApplication::translate("LISCommunicationClass", "IP\345\234\260\345\235\200\357\274\232", Q_NULLPTR));
        label_4->setText(QApplication::translate("LISCommunicationClass", "\347\253\257\345\217\243\345\217\267\357\274\232", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("LISCommunicationClass", "\347\275\221\345\217\243\350\277\236\346\216\245", Q_NULLPTR));
        btnClearText->setText(QApplication::translate("LISCommunicationClass", "\346\270\205\351\231\244", Q_NULLPTR));
        btnCon->setText(QApplication::translate("LISCommunicationClass", "\350\277\236\346\216\245", Q_NULLPTR));
        btnDisCon->setText(QApplication::translate("LISCommunicationClass", "\346\226\255\345\274\200", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class LISCommunicationClass: public Ui_LISCommunicationClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LISCOMMUNICATION_H
