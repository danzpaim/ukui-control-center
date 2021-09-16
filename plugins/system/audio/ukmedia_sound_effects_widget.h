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
#ifndef UKMEDIASOUNDEFFECTSWIDGET_H
#define UKMEDIASOUNDEFFECTSWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QStyledItemDelegate>
#include "ukui_custom_style.h"
#include "titlelabel.h"
#include "switchbutton.h"

class UkuiMessageBox : public QMessageBox
{
public:
    explicit UkuiMessageBox();
};

class UkmediaSoundEffectsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UkmediaSoundEffectsWidget(QWidget *parent = nullptr);
    ~UkmediaSoundEffectsWidget();
    friend class UkmediaMainWidget;
Q_SIGNALS:

public Q_SLOTS:

private:
    QFrame *m_pLagoutWidget;//注销提示音窗口
    QFrame *m_pWakeupMusicWidget;//唤醒音乐窗口
    QFrame *m_pStartupMusicWidget;//设置开机音乐窗口
    QFrame *m_pPoweroffMusicWidget;//关机提示音
    QFrame *m_pDnsNoiseReductionWidget;

    QFrame *m_pAlertSoundSwitchWidget;//提示音开关窗口
    QFrame *m_pThemeWidget;//提示音下的系统音效主题
    QFrame *m_pAlertSoundWidget;//提示音下的通知提示
    QFrame *m_pVolumeChangeWidget;//提示音下的音量调节


    QString qss;
    QStyledItemDelegate *itemDelegate;
    QLabel *m_pSoundEffectLabel;
    QLabel *m_pSoundThemeLabel;
    QLabel *m_pShutdownlabel;
    QLabel *m_pLagoutLabel;
    QLabel *m_pAlertSoundSwitchLabel;
//    QLabel *m_pAlertSoundLabel;
//    QLabel *m_pAlertVolumeLabel;
    QLabel *m_pWakeupMusicLabel;
    QLabel *m_pVolumeChangeLabel;
    QLabel *m_pPoweroffMusicLabel;
    QLabel *m_pStartupMusicLabel;
    QLabel *m_pDnsNoiseReductionLabel;

    QComboBox *m_pSoundThemeCombobox;
    QComboBox *m_pAlertSoundCombobox;
    QComboBox *m_pLagoutCombobox;
    QComboBox *m_pVolumeChangeCombobox;
    QVBoxLayout *m_pSoundLayout;
    SwitchButton *m_pStartupButton;
    SwitchButton *m_pPoweroffButton;
    SwitchButton *m_pLogoutButton;
    SwitchButton *m_pAlertSoundSwitchButton;
    SwitchButton *m_pWakeupMusicButton;
    SwitchButton *m_pDnsNoiseReductionButton;
//    UkmediaVolumeSlider *m_pAlertSlider;
//    UkuiButtonDrawSvg *m_pAlertIconBtn;
};

#endif // UKMEDIASOUNDEFFECTSWIDGET_H
