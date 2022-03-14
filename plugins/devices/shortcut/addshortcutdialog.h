/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2019 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */
#ifndef ADDSHORTCUTDIALOG_H
#define ADDSHORTCUTDIALOG_H

#include <QFileDialog>
#include <QPainter>
#include <QPainterPath>
#include <QRegExpValidator>
#include <KActionCollection>
#include <KGlobalAccel>
#include <QDebug>
#include "keyentry.h"
#include "shortcutline.h"
#include <QLabel>

namespace Ui {
class addShortcutDialog;
}

class addShortcutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addShortcutDialog(QList<KeyEntry *> generalEntries, QList<KeyEntry *> customEntries,
                               QWidget *parent = nullptr);
    ~addShortcutDialog();

public:
    void initSetup();
    void slotsSetup();
    void setTitleText(QString text);
    void openProgramFileDialog();

    void setUpdateEnv(QString path, QString name, QString exec);

    void refreshCertainChecked(int triggerFlag);   //1:输入了程序，2:输入了名字， 3:输入了快捷键
    void limitInput();
    bool conflictWithStandardShortcuts(const QKeySequence &seq);
    bool conflictWithGlobalShortcuts(const QKeySequence &seq);
    bool conflictWithSystemShortcuts(const QKeySequence &seq);
    bool conflictWithCustomShortcuts(const QKeySequence &seq);
    bool isKeyAvailable(const QKeySequence &seq);
    QString keyToLib(QString key);

    void setExecText(const QString &text);
    void setNameText(const QString &text);
    void setKeyText(const QString  &text);
    void setSourceEnable(bool enabled);
    void setKeyIsAvailable(const int key);
    void setIcon(const QString &iconname);

protected:
    bool eventFilter(QObject *o, QEvent *e);

private:
    Ui::addShortcutDialog *ui;

private:
    QString gsPath;
    QString selectedfile;
    QList<KeyEntry *> systemEntry;
    QList<KeyEntry *> customEntry;
    ShortcutLine *shortcutLine = nullptr;
    QString      editName;
    QKeySequence editSeq;
    int keyIsAvailable;         //快捷键有冲突/不可用/正常三种情况，1：冲突，2：不可用，3：正常
    bool nameIsAvailable;
    bool execIsAvailable;
    QLabel *execIcon = nullptr;
    QString mExec = "";

Q_SIGNALS:
    void shortcutInfoSignal(QString path, QString name, QString exec, QString key);
};

#endif // ADDSHORTCUTDIALOG_H
