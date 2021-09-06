#include "outputconfig.h"
#include "resolutionslider.h"
#include "utils.h"
//#include "kcm_screen_debug.h"

#include <QStringBuilder>
#include <QFormLayout>
#include <QComboBox>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>

//#include <KLocalizedString>
#include <QComboBox>
#include <QGSettings>

#include <KF5/KScreen/kscreen/output.h>
#include <KF5/KScreen/kscreen/edid.h>

#include "ComboBox/combobox.h"

#define SCALE_SCHEMAS "org.ukui.SettingsDaemon.plugins.xsettings"
#define SCALE_KEY "scaling-factor"


#define AUTO 0
#define FIXED 3

OutputConfig::OutputConfig(QWidget *parent)
    : QWidget(parent)
    , mOutput(nullptr)
{
    m_outputSessionDbus = new QDBusInterface("com.kylin.statusmanager.interface",
                                              "/",
                                              "com.kylin.statusmanager.interface",
                                              QDBusConnection::sessionBus(),this);

}

OutputConfig::OutputConfig(const KScreen::OutputPtr &output, QWidget *parent)
    : QWidget(parent)
{
    setOutput(output);
}

OutputConfig::~OutputConfig()
{
}

void OutputConfig::setTitle(const QString& title)
{
    mTitle->setText(title);
}


void OutputConfig::initUi()
{
    connect(mOutput.data(), &KScreen::Output::isConnectedChanged,
            this, [=]() {
                if (!mOutput->isConnected()) {
                    setVisible(false);
                }
            });

//    connect(mOutput.data(), &KScreen::Output::isEnabledChanged,
//            this, [=]() {
//                mEnabled->setChecked(mOutput->isEnabled());
//            });

    connect(mOutput.data(), &KScreen::Output::rotationChanged,
            this, [=]() {
                if (m_outputSessionDbus->isValid()) {
                    QDBusReply<bool> reply = m_outputSessionDbus->call("get_auto_rotation");
                    if (reply) {
                        return;
                    }
                }
                const int index = mRotation->findData(mOutput->rotation());
                mRotation->blockSignals(true);
                mRotation->setCurrentIndex(index);
                mRotation->blockSignals(false);
            });

//    connect(mOutput.data(), &KScreen::Output::scaleChanged,
//            this, [=]() {
//                const int index = mScale->findData(mOutput->scale());
//                mScale->setCurrentIndex(index);
//            });


    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->setContentsMargins(0,0,0,0);
    vbox->setSpacing(0);

    //注释显示器上方标签

//    mTitle = new QLabel(this);
//    mTitle->setAlignment(Qt::AlignHCenter);
//    vbox->addWidget(mTitle);
  //  setTitle(Utils::outputName(mOutput));

    //QFormLayout *formLayout = new QFormLayout();



//    mEnabled = new QCheckBox(i18n("已启用"), this);
//    //mEnabled->setVisible(false);
//    mEnabled->setChecked(mOutput->isEnabled());
//    qDebug()<<"是否开启---->mEnable"<<mOutput<<endl;
//    connect(mEnabled, &QCheckBox::clicked,
//            this, [=](bool checked) {
//                  //qDebug()<<"显示屏为---->mEnable"<<mOutput<<endl;
//                  mOutput->setEnabled(checked);
//                  qDebug() << "上层勾选---->"<<mOutput.data() << mOutput->name() << mOutput->isEnabled();
//                  Q_EMIT changed();
//            });
//    formLayout->addRow(i18n("显示:"), mEnabled);

//    mMonitor = new QComboBox(this);
//    formLayout->addRow(i18n("主显示器"),mMonitor);

    //分辨率下拉框
    mResolution = new ResolutionSlider(mOutput, this);
//    mResolution->setFont(ft);
    mResolution->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    mResolution->setMinimumSize(402,48);
    mResolution->setMaximumSize(16777215,48);


    QLabel *resLabel = new QLabel(this);
    resLabel->setText(tr("resolution"));
    resLabel->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    resLabel->setMinimumSize(118,30);
    resLabel->setMaximumSize(118,30);

    QHBoxLayout *resLayout = new QHBoxLayout();
    //resLayout->setContentsMargins(0,5,0,5);
    resLayout->addWidget(resLabel);
    resLayout->addWidget(mResolution);
//    resLayout->addStretch();


    QFrame *resFrame = new QFrame(this);
    resFrame->setFrameShape(QFrame::Shape::NoFrame);
    resFrame->setLayout(resLayout);
    //暂时去掉分辨率调节功能
    resFrame->hide();
//    resWidget->setStyleSheet("background-color:#F4F4F4;border-radius:6px");
//    mResolution->setStyleSheet("background-color:#F8F9F9");

    resFrame->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
//    resFrame->setMinimumSize(552,50);
//    resFrame->setMaximumSize(960,50);
    resFrame->setFixedHeight(64);

    vbox->addWidget(resFrame);

//    connect(mResolution, &ResolutionSlider::resolutionChanged,
//            this, &OutputConfig::whetherApplyResolution);

    QFrame *line_1 = new QFrame(this);
    line_1->setFrameShape(QFrame::Shape::HLine);
    line_1->setFixedHeight(1);
    vbox->addWidget(line_1);

    //方向下拉框
    mRotation = new QComboBox();
//    mRotation->setFont(ft);
    mRotation->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    mRotation->setMinimumSize(402,48);
    mRotation->setMaximumSize(16777215,48);
    mRotation->close();

    rotationRadioBtn = new SwitchButton;

//    mRotation->setStyleSheet(qss);
//    mRotation->setItemDelegate(itemDelege);
    rotationLabel = new QLabel();
    rotationLabel->setText(tr("Only tablet mode supports this feature"));
    rotationLabel->setEnabled(false);

    QLabel *rotateLabel = new QLabel();
    rotateLabel->setText(tr("Auto Screen Rotation"));
//    rotateLabel->setFont(ft);
    rotateLabel->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    rotateLabel->setMinimumSize(160,30);
    rotateLabel->setMaximumSize(160,30);

    QHBoxLayout *rotateLayout = new QHBoxLayout();
    rotateLayout->addWidget(rotateLabel);
    rotateLayout->addItem(new QSpacerItem(40,10,QSizePolicy::Fixed));
    rotateLayout->addWidget(rotationLabel);
    rotateLayout->addStretch();
    rotateLayout->addWidget(rotationRadioBtn);
//    rotateLayout->addStretch();

    QFrame *rotateFrame = new QFrame(this);
    rotateFrame->setFrameShape(QFrame::Shape::NoFrame);
    rotateFrame->setLayout(rotateLayout);
//    rotateWidget->setStyleSheet("background-color:#F4F4F4;border-radius:6px");
//    mRotation->setStyleSheet("background-color:#F8F9F9");

    rotateFrame->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
//    rotateFrame->setMinimumSize(550,50);
//    rotateFrame->setMaximumSize(960,50);
    rotateFrame->setFixedHeight(64);
    rotateFrame->hide();
    mRotation->addItem( tr("arrow-up"), KScreen::Output::None);
    mRotation->addItem( tr("90° arrow-right"), KScreen::Output::Right);
    mRotation->addItem( tr("arrow-down"), KScreen::Output::Inverted);
    mRotation->addItem(tr("90° arrow-left"), KScreen::Output::Left);

    QDBusReply<bool> is_tabletmode = m_outputSessionDbus->call("get_current_tabletmode");
    if (is_tabletmode) {
        rotationRadioBtn->setEnabled(true);
        QDBusReply<bool> autoRation = m_outputSessionDbus->call("get_auto_rotation");
        if (autoRation) {
            rotationRadioBtn->setChecked(true);
        } else {
            rotationRadioBtn->setChecked(false);
        }
    } else {
        rotationRadioBtn->setEnabled(false);
    }

    connect(m_outputSessionDbus, SIGNAL(auto_rotation_change_signal(bool)), this, SLOT(rotationDbusSlot(bool)));
    connect(m_outputSessionDbus, SIGNAL(mode_change_signal(bool)), this, SLOT(tabletModeDbusSlot(bool)));
    connect(rotationRadioBtn,&SwitchButton::checkedChanged,this,[=]{
        if (rotationRadioBtn->isChecked()) {
            m_outputSessionDbus->call("set_auto_rotation", true, "ukcc", "set_auto_rotation");
        } else {
            m_outputSessionDbus->call("set_auto_rotation", false, "ukcc", "set_auto_rotation");
        }
    });
    if (m_outputSessionDbus->isValid()) {
        QDBusReply<bool> reply = m_outputSessionDbus->call("get_current_tabletmode");
        if (reply) {
            mRotation->removeItem(mRotation->findData(KScreen::Output::None));
            mRotation->removeItem(mRotation->findData(KScreen::Output::Right));
            mRotation->removeItem(mRotation->findData(KScreen::Output::Inverted));
            mRotation->removeItem(mRotation->findData(KScreen::Output::Left));
            mRotation->addItem(tr("fixed-rotation"), FIXED);
            mRotation->addItem(tr("auto-rotation"), AUTO);
        }
        connect(m_outputSessionDbus, SIGNAL(mode_change_signal(bool)), this, SLOT(mode_rotationDbusSlot(bool)));
    
        connect(m_outputSessionDbus, SIGNAL(auto_rotation_change_signal(bool)), this, SLOT(tabletModeDbusSlot(bool)));
    }
//    connect(mRotation, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated),
//            this, &OutputConfig::whetherApplyRotation);
    connect(mRotation, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &OutputConfig::whetherApplyRotation);

    if (m_outputSessionDbus->isValid()) {
        QDBusReply<bool> is_tabletmode = m_outputSessionDbus->call("get_current_tabletmode");
        QDBusReply<bool> auto_rotation = m_outputSessionDbus->call("get_auto_rotation");
        if (is_tabletmode && auto_rotation) {
            mRotation->blockSignals(true);
            mRotation->setCurrentIndex(mRotation->findData(AUTO));
            mRotation->blockSignals(false);
        } else {
            mRotation->blockSignals(true);
            mRotation->setCurrentIndex(mRotation->findData(mOutput->rotation()));
            mRotation->blockSignals(false);
        }
    } else {
        mRotation->blockSignals(true);
        mRotation->setCurrentIndex(mRotation->findData(mOutput->rotation()));
        mRotation->blockSignals(false);
    }

    //formLayout->addRow(rotateLabel, mRotation);
    vbox->addWidget(rotateFrame);

    QFrame *line_2 = new QFrame(this);
    line_2->setFrameShape(QFrame::Shape::HLine);
    line_2->setFixedHeight(1);
    vbox->addWidget(line_2);

    //缩放暂时用不到
//    if (!mShowScaleOption) {
//        mScale = new QComboBox(this);
//        mScale->addItem(i18nc("Scale multiplier, show everything at 1 times normal scale", "1x"), 1);
//        mScale->addItem(i18nc("Scale multiplier, show everything at 2 times normal scale", "2x"), 2);
//        connect(mScale, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated),
//                this, &OutputConfig::slotScaleChanged);
//        mScale->setCurrentIndex(mScale->findData(mOutput->scale()));
//        formLayout->addRow(i18n("缩放:"), mScale);
//        formLayout->addItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum));
//    }


    //刷新率下拉框
    mRefreshRate = new QComboBox();
//    mRefreshRate->setFont(ft);
//    mRefreshRate->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    mRefreshRate->setMinimumSize(402,48);
    mRefreshRate->setMaximumSize(16777215,48);
//    mRefreshRate->setStyleSheet(qss);
//    mRefreshRate->setItemDelegate(itemDelege);
//    mRefreshRate->setMaxVisibleItems(5);

    QLabel *freshLabel = new QLabel();
    freshLabel->setText(tr("refresh rate"));
//    freshLabel->setFont(ft);
    freshLabel->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    freshLabel->setMinimumSize(118,30);
    freshLabel->setMaximumSize(118,30);

    QHBoxLayout *freshLayout = new QHBoxLayout();
    freshLayout->addWidget(freshLabel);
    freshLayout->addWidget(mRefreshRate);
//    freshLayout->addStretch();
    QFrame *freshFrame = new QFrame(this);
    freshFrame->setFrameShape(QFrame::Shape::NoFrame);
    freshFrame->setLayout(freshLayout);
//    freshWidget->setStyleSheet("background-color:#F4F4F4;border-radius:6px");

    freshFrame->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
//    freshFrame->setMinimumSize(550,50);
//    freshFrame->setMaximumSize(960,50);
    freshFrame->setFixedHeight(64);

    mRefreshRate->addItem(tr("auto"), -1);
    vbox->addWidget(freshFrame);

    QFrame *line_3 = new QFrame(this);
    line_3->setFrameShape(QFrame::Shape::HLine);
    line_3->setFixedHeight(1);
    vbox->addWidget(line_3);

    //暂时隐藏刷新率设置
    freshFrame->setVisible(false);
    line_3->setVisible(false);

    slotResolutionChanged();

    //currentIndexChanged
    connect(mRefreshRate, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &OutputConfig::slotRefreshRateChanged);



    scaleCombox = new QComboBox();
//    mRefreshRate->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    scaleCombox->setMinimumSize(402,48);
    scaleCombox->setMaximumSize(16777215,48);



    int maxReslu = mResolution->getMaxResolution().width();

#if QT_VERSION < QT_VERSION_CHECK(5,7,0)
    if (maxReslu >= 2000) {
        scaleCombox->addItem(tr("200%"));
    } else {
        scaleCombox->addItem(tr("100%"));
    }
#else
    scaleCombox->addItem(tr("100% (Recommended)"));
    if (maxReslu >= 2000 && maxReslu <= 3800) {
        scaleCombox->addItem(tr("200%"));
    } else if (maxReslu >= 3800 || maxReslu >= 4000) {
        scaleCombox->addItem(tr("200%"));
        scaleCombox->addItem(tr("300%"));
    }
#endif

    QLabel *scaleLabel = new QLabel();
    scaleLabel->setText(tr("screen zoom"));
    scaleLabel->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    scaleLabel->setMinimumSize(118,30);
    scaleLabel->setMaximumSize(118,30);

    QHBoxLayout *scaleLayout = new QHBoxLayout();
    scaleLayout->addWidget(scaleLabel);
    scaleLayout->addWidget(scaleCombox);
//    freshLayout->addStretch();

    QFrame *scaleFrame = new QFrame(this);
    scaleFrame->setFrameShape(QFrame::Shape::NoFrame);
    scaleFrame->setLayout(scaleLayout);
//    scaleWidget->setStyleSheet("background-color:#F4F4F4;border-radius:6px");

    scaleFrame->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
//    scaleFrame->setMinimumSize(550,50);
//    scaleFrame->setMaximumSize(960,50);
    scaleFrame->setFixedHeight(64);
    vbox->addWidget(scaleFrame);

    //屏幕缩放暂时去掉
    scaleFrame->close();

    int scale = getScreenScale();
#if QT_VERSION < QT_VERSION_CHECK(5,7,0)

#else
    scaleCombox->blockSignals(true);
    scaleCombox->setCurrentIndex(0);
    if (scale <= scaleCombox->count() && scale > 0) {
//        qDebug()<<"scale is----->"<<scale<<endl;
        scaleCombox->setCurrentIndex(scale - 1);
    }
    scaleCombox->blockSignals(false);
    slotScaleChanged();
#endif

    connect(scaleCombox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &OutputConfig::whetherApplyScale);

    if(mResolution->currentResolution().isValid()) {
        resolutionIndex = mResolution->mComboBox->currentIndex();
    }
    rotationIndex = mRotation->currentIndex();
    scaleIndex = scaleCombox->currentIndex();
}

int OutputConfig::getScreenScale() {
    QGSettings * dpiSettings;
    QByteArray id(SCALE_SCHEMAS);
    int scale = 1;
    if (QGSettings::isSchemaInstalled(SCALE_SCHEMAS)) {
        dpiSettings = new QGSettings(id);
        if (dpiSettings->keys().contains("scalingFactor")) {
            scale = dpiSettings->get(SCALE_KEY).toInt();
        }
    }
    return scale;
}

void OutputConfig::rotationDbusSlot(bool auto_rotation)
{
    QDBusReply<bool> is_tabletmode = m_outputSessionDbus->call("get_current_tabletmode");
    if (is_tabletmode) {
//                QDBusReply<bool> autoRoation = m_outputSessionDbus->call("get_auto_rotation");
        if (auto_rotation) {
            rotationRadioBtn->blockSignals(true);
            rotationRadioBtn->setChecked(true);
            rotationRadioBtn->blockSignals(false);
        } else {
            rotationRadioBtn->blockSignals(true);
            rotationRadioBtn->setChecked(false);
            rotationRadioBtn->blockSignals(false);
        }
    }
}


void OutputConfig::tabletModeDbusSlot(bool tablet_mode)
{
    if (tablet_mode) {
        rotationRadioBtn->setEnabled(true);
    } else {
        rotationRadioBtn->blockSignals(true);
        rotationRadioBtn->setChecked(false);
        rotationRadioBtn->blockSignals(false);
        rotationRadioBtn->setEnabled(false);
    }
}

void OutputConfig::mode_rotationDbusSlot(bool tablet_mode)
{
    mRotation->blockSignals(true);
    if (tablet_mode) {
        mRotation->removeItem(mRotation->findData(KScreen::Output::None));
        mRotation->removeItem(mRotation->findData(KScreen::Output::Right));
        mRotation->removeItem(mRotation->findData(KScreen::Output::Inverted));
        mRotation->removeItem(mRotation->findData(KScreen::Output::Left));
        mRotation->addItem(tr("fixed-rotation"), FIXED);
        mRotation->addItem(tr("auto-rotation"), AUTO);
        QDBusReply<bool> auto_rotation = m_outputSessionDbus->call("get_auto_rotation");
        if (auto_rotation) {
            mRotation->setCurrentIndex(mRotation->findData(AUTO));
        } else {
            mRotation->setCurrentIndex(mRotation->findData(FIXED));
        }
    } else {
        mRotation->removeItem(mRotation->findData(FIXED));
        mRotation->removeItem(mRotation->findData(AUTO));
        mRotation->addItem(tr("arrow-up"), KScreen::Output::None);
        mRotation->addItem(tr("90° arrow-right"), KScreen::Output::Right);
        mRotation->addItem(tr("arrow-down"), KScreen::Output::Inverted);
        mRotation->addItem(tr("90° arrow-left"), KScreen::Output::Left);
        mRotation->setCurrentIndex(mRotation->findData(mOutput->rotation()));
    }
    mRotation->blockSignals(false);
}

void OutputConfig::mrotationDbusSlot(bool auto_rotation)
{
    mRotation->blockSignals(true);
    QDBusReply<bool> is_tabletmode = m_outputSessionDbus->call("get_current_tabletmode");
    if (is_tabletmode) {
        if (auto_rotation) {
            mRotation->setCurrentIndex(mRotation->findData(AUTO));
        } else {
            mRotation->setCurrentIndex(mRotation->findData(FIXED));
        }
    }
    mRotation->blockSignals(false);
}


void OutputConfig::setOutput(const KScreen::OutputPtr &output)
{
    mOutput = output;
    initUi();
}

KScreen::OutputPtr OutputConfig::output() const
{
    return mOutput;
}

void OutputConfig::whetherApplyResolution() {
    ApplyDialog * applydialog = new ApplyDialog(this);
    connect(applydialog, SIGNAL(do_apply()), this, SLOT(slotResolutionChanged()));
    connect(applydialog, SIGNAL(do_not_apply()), this, SLOT(slotResolutionNotChange()));
    applydialog->exec();
}

void OutputConfig::slotResolutionNotChange() {
    mResolution->mComboBox->blockSignals(true);
    mResolution->mComboBox->setCurrentIndex(resolutionIndex);
    mResolution->mComboBox->blockSignals(false);
}

void OutputConfig::whetherApplyRotation() {
    if (m_outputSessionDbus->isValid()) {
        //平板模式下开关自动旋转不触发弹窗
        QDBusReply<bool> is_tabletmode = m_outputSessionDbus->call("get_current_tabletmode");
        if (is_tabletmode) {
            if (mRotation->currentData() == 0) {
                //打开自动旋转
                m_outputSessionDbus->call("set_auto_rotation", true, "ukcc", "set_auto_rotation");
            } else if (mRotation->currentData() == 3) {
                //关闭自动旋转
                m_outputSessionDbus->call("set_auto_rotation", false, "ukcc", "set_auto_rotation");
            }
            return;
        }
    }
    ApplyDialog * applydialog = new ApplyDialog(this);
    connect(applydialog, SIGNAL(do_apply()), this, SLOT(slotRotationChanged()));
    connect(applydialog, SIGNAL(do_not_apply()), this, SLOT(slotRotationNotChange()));
    applydialog->exec();
}

void OutputConfig::slotRotationNotChange() {
    mRotation->blockSignals(true);
    mRotation->setCurrentIndex(rotationIndex);
    mRotation->blockSignals(false);
}

void OutputConfig::whetherApplyScale() {
    ApplyDialog * applydialog = new ApplyDialog(this);
    connect(applydialog, SIGNAL(do_apply()), this, SLOT(slotScaleChanged()));
    connect(applydialog, SIGNAL(do_not_apply()), this, SLOT(slotScaleNotChange()));
    applydialog->exec();
}

void OutputConfig::slotScaleNotChange() {
    scaleCombox->blockSignals(true);
    scaleCombox->setCurrentIndex(scaleIndex);
    scaleCombox->blockSignals(false);
}

void OutputConfig::slotResolutionChanged()
{
    if (!mResolution->currentResolution().isValid()) {
        return;
    }
    QSize size = mResolution->currentResolution();
    resolutionIndex = mResolution->mComboBox->currentIndex();
    // Ignore disconnected outputs
    if (!size.isValid()) {
        return;
    }

    KScreen::ModePtr selectedMode;
    QList<KScreen::ModePtr> modes;
    Q_FOREACH (const KScreen::ModePtr &mode, mOutput->modes()) {
        if (mode->size() == size) {
            modes << mode;
            if (!selectedMode || selectedMode->refreshRate() < mode->refreshRate()) {
                selectedMode = mode;
            }
        }
    }

    Q_ASSERT(selectedMode);
    mOutput->setCurrentModeId(selectedMode->id());

    // Don't remove the first "Auto" item - prevents ugly flicker of the combobox
    // when changing resolution
//    for (int i = mRefreshRate->count(); i >= 2; --i) {
//        mRefreshRate->removeItem(i - 1);
//    }

//    for (int i = 0, total = modes.count(); i < total; ++i) {
//        const KScreen::ModePtr mode = modes.at(i);
//        mRefreshRate->addItem(tr("%1 Hz").arg(QLocale().toString(mode->refreshRate())), mode->id());
//        // If selected refresh rate is other then what we consider the "Auto" value
//        // - that is it's not the highest resolution - then select it, otherwise
//        // we stick with "Auto"
//        if (mode == selectedMode && mRefreshRate->count() > 1) {
//            // i + 1 since 0 is auto
//            mRefreshRate->blockSignals(true);
//            mRefreshRate->setCurrentIndex(i + 1);
//            mRefreshRate->blockSignals(false);
//        }
//    }

    Q_EMIT changed();
}

void OutputConfig::slotRotationChanged()
{
    int index = mRotation->currentIndex();
    if (index == mRotation->findData(AUTO)) {
        //平板模式下选择自动转屏
        m_outputSessionDbus->call("set_auto_rotation", true, "ukcc", "set_auto_rotation");
        Q_EMIT changed();
        return;
    } else {
        //平板模式下设置转屏方向，自动旋转关闭
        QDBusReply<bool> is_tabletmode = m_outputSessionDbus->call("get_current_tabletmode");
        if (!is_tabletmode) {
            m_outputSessionDbus->call("set_auto_rotation", false, "ukcc", "set_auto_rotation");
        }
    }
    rotationIndex = index;
    KScreen::Output::Rotation rotation =
        static_cast<KScreen::Output::Rotation>(mRotation->itemData(index).toInt());
    switch (rotation) {
    case KScreen::Output::Rotation::Inverted:
    {
        mOutput->setRotation(rotation);
        /*!
         * @brief BUG:14278外接显示器方向配置选择“上下颠倒”后，内置显示器黑屏
         * 【临时】通过setPos，对屏幕进行一次刷新 防止黑屏
         */
        QPoint outSP = mOutput->pos();
        mOutput->setPos(QPoint(outSP.x() + 1, outSP.y()));
        break;
    }
    default:
        mOutput->setRotation(rotation);
        break;
    }
    //mOutput->setRotation(rotation);
    Q_EMIT changed();
}

void OutputConfig::slotRefreshRateChanged(int index)
{
    QString modeId;
    if (index == 0) {
        // Item 0 is "Auto" - "Auto" is equal to highest refresh rate (at least
        // that's how I understand it, and since the combobox is sorted in descending
        // order, we just pick the second item from top
        modeId = mRefreshRate->itemData(1).toString();
    } else {
        modeId = mRefreshRate->itemData(index).toString();
    }
    mOutput->setCurrentModeId(modeId);

    Q_EMIT changed();
}

void OutputConfig::slotScaleChanged()
{
//    auto scale = mScale->itemData(index).toInt();
//    mOutput->setScale(scale);
    int index = scaleCombox->currentIndex();
    scaleIndex = index;
    Q_EMIT scaleChanged(index);
}

void OutputConfig::setShowScaleOption(bool showScaleOption)
{
    mShowScaleOption = showScaleOption;
    if (mOutput) {
        initUi();
    }
}

bool OutputConfig::showScaleOption() const
{
    return mShowScaleOption;
}

//拿取配置
void OutputConfig::initConfig(const KScreen::ConfigPtr &config){
    mConfig = config;
}

QStringList OutputConfig::readFile(const QString& filepath) {
    QFile file(filepath);
    if(file.exists()) {
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "ReadFile() failed to open" << filepath;
            return QStringList();
        }
        QTextStream textStream(&file);
        while(!textStream.atEnd()) {
            QString line= textStream.readLine();
            line.remove('\n');
            this->proRes<<line;
        }
        file.close();
        return this->proRes;
    } else {
        qWarning() << filepath << " not found"<<endl;
        return QStringList();
    }
}

