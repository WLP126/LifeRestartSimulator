#include "LifeWindow.h"
#include "ui_LifeWindow.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QGraphicsView>
#include <QDebug>
#include <QFontDatabase>

LifeWindow::LifeWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LifeWindow),
    m_engine(new LifeEngine(this))
{
    ui->setupUi(this);

    // 1. 创建图形场景
    m_scene = new QGraphicsScene(this);
    m_bgView = new QGraphicsView(m_scene, this);

    // 2. 设置视图属性
    m_bgView->setFrameShape(QFrame::NoFrame);          // 无边框
    m_bgView->setAlignment(Qt::AlignLeft | Qt::AlignTop); // 左上对齐
    m_bgView->setStyleSheet("background: transparent;");  // 透明背景

    // 3. 加载背景图片
    QPixmap bgImage("C:/Users/w1357/Desktop/Qt/LifeRestartSimulator/resources/images/bg_main.jpg");
    if(bgImage.isNull()) {
        qDebug() << "背景图片加载失败！";
    } else {
        QGraphicsPixmapItem *bgItem = m_scene->addPixmap(bgImage);
        bgItem->setPos(0, 0);

        // 自适应窗口大小
        m_scene->setSceneRect(0, 0, bgImage.width(), bgImage.height());
        m_bgView->fitInView(bgItem, Qt::KeepAspectRatioByExpanding);
    }

    // 设置主窗口背景
    setCentralWidget(m_bgView);

    // 5. 将原有UI控件置于背景之上
    ui->centralWidget->setParent(m_bgView->viewport());
    ui->centralWidget->setStyleSheet("background: transparent;"); // 透明背景
    ui->centralWidget->setAttribute(Qt::WA_TranslucentBackground); // 启用透明

    int fontId = QFontDatabase::addApplicationFont(":/fonts/HappyFont.ttf");
    if(fontId == -1) {
        qDebug() << "字体加载失败！";
    } else {
        QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
        if (!fontFamilies.isEmpty()) {
             QFont appFont(fontFamilies.at(0), 12);
             QApplication::setFont(appFont);
             qDebug() << "已加载字体：" << fontFamilies.at(0);
        } else {
             qDebug() << "字体文件中未找到有效字体族！";
        }
    }

    //淡入动画
    setWindowOpacity(0);
        QPropertyAnimation *fadeIn = new QPropertyAnimation(this, "windowOpacity");
        fadeIn->setDuration(1000);
        fadeIn->setStartValue(0);
        fadeIn->setEndValue(1);
        fadeIn->start(QAbstractAnimation::DeleteWhenStopped);

    // === 初始化打字机效果定时器 ===
    connect(m_typeTimer, &QTimer::timeout, [=](){
        if(m_typePos < m_currentDisplayText.length()) {
            ui->eventLabel1->setText(m_currentDisplayText.left(++m_typePos));
        } else {
            m_typeTimer->stop();
        }
    });

    // 初始化按钮数组
    m_choiceButtons[0] = ui->choiceButton0;
    m_choiceButtons[1] = ui->choiceButton1;
    m_choiceButtons[2] = ui->choiceButton2;
    m_choiceButtons[3] = ui->choiceButton3;

    // 设置堆叠小部件和页面
    m_stackedWidget = new QStackedWidget(this);
    m_startPage = new QWidget(this);
    m_gamePage = new QWidget(this);
    m_summaryPage = new QWidget(this);

    // 设置开始页面
    QVBoxLayout *startLayout = new QVBoxLayout(m_startPage);
    QPushButton *startButton = new QPushButton("开始人生", m_startPage);
    startLayout->addWidget(startButton);
    connect(startButton, &QPushButton::clicked, this, &LifeWindow::onStartClicked);

    // 设置游戏页面
    QVBoxLayout *gameLayout = new QVBoxLayout(m_gamePage);
    gameLayout->addWidget(ui->ageLabel);
    gameLayout->addWidget(ui->intelLabel);
    gameLayout->addWidget(ui->wealthLabel);
    gameLayout->addWidget(ui->healthLabel);
    gameLayout->addWidget(ui->happyLabel);
    gameLayout->addWidget(ui->appearLabel);
    gameLayout->addWidget(ui->eventLabel1);
    for (int i = 0; i < 4; ++i) {
        gameLayout->addWidget(m_choiceButtons[i]);
    }

    // 设置总结页面
    m_summaryTextEdit = new QTextEdit(m_summaryPage);
    QVBoxLayout *summaryLayout = new QVBoxLayout(m_summaryPage);
    summaryLayout->addWidget(m_summaryTextEdit);

    m_stackedWidget->addWidget(m_startPage);
    m_stackedWidget->addWidget(m_gamePage);
    m_stackedWidget->addWidget(m_summaryPage);
    setCentralWidget(m_stackedWidget);

    // 连接信号和槽
    connect(ui->startButton, &QPushButton::clicked, this, &LifeWindow::onStartClicked);
    connect(m_engine, &LifeEngine::gameUpdated, this, &LifeWindow::onGameUpdated);
    connect(m_engine, &LifeEngine::gameOver, this, &LifeWindow::onGameOver);

    // 连接选择按钮
    for (int i = 0; i < 4; ++i) {
        connect(m_choiceButtons[i], &QPushButton::clicked, [this, i]() {
            onChoiceMade(i);
        });
    }

    // 显示开始页面
    showStartPage();
}

LifeWindow::~LifeWindow() {
    delete ui;
}

// 点击“开始人生”按钮
void LifeWindow::onStartClicked() {
    m_engine->startNewLife();
    showGamePage();
}

// 用户做出选择
void LifeWindow::onChoiceMade(int index) {
    m_engine->makeChoice(index);

    if(index >=0 && index <4) {
            animateButton(m_choiceButtons[index]);
        }
}

// 游戏更新后刷新界面
void LifeWindow::onGameUpdated() {
    updateUI();
}

// 游戏结束时显示总结信息
void LifeWindow::onGameOver(QString summary) {
    showSummaryPage();
    m_summaryTextEdit->setPlainText(summary); // 直接使用参数
}

// 更新所有 UI 显示
void LifeWindow::updateUI() {
    Attribute attr = m_engine->currentAttributes();

    ui->ageLabel->setText(QString("年龄: %1").arg(m_engine->currentAge()));
    ui->intelLabel->setText(QString("智力: %1").arg(attr.intelligence));
    ui->wealthLabel->setText(QString("财富: %1").arg(attr.wealth));
    ui->healthLabel->setText(QString("健康: %1").arg(attr.health));
    ui->happyLabel->setText(QString("幸福: %1").arg(attr.happiness));
    ui->appearLabel->setText(QString("颜值: %1").arg(attr.appearance));

    // 显示当前事件的描述或年龄状态
    QString eventText;
    if (!m_engine->currentEventDescription().isEmpty()) {
            typeWriterEffect(m_engine->currentEventDescription()); // 使用动画显示
        } else {
            typeWriterEffect(QString("年龄 %1：这一年平安无事").arg(m_engine->currentAge()));
        }
    ui->eventLabel1->setText(eventText);

    // 更新选择按钮文本与显示状态
    QVector<QString> choices = m_engine->currentChoices();
    for (int i = 0; i < 4; ++i) {
        if (i < choices.size()) {
            m_choiceButtons[i]->setText(choices[i]);
            m_choiceButtons[i]->setVisible(true);
        } else {
            m_choiceButtons[i]->setVisible(false);
        }
    }

}

void LifeWindow::showStartPage() {
    m_stackedWidget->setCurrentIndex(0);
}

void LifeWindow::showGamePage() {
    m_stackedWidget->setCurrentIndex(1);
}

void LifeWindow::showSummaryPage() {
    m_stackedWidget->setCurrentIndex(2);
    m_summaryTextEdit->setPlainText(m_engine->lifeSummary());
}

void LifeWindow::animateButton(QPushButton* button) {
    QPropertyAnimation *anim = new QPropertyAnimation(button, "geometry");
    anim->setDuration(200);
    anim->setKeyValueAt(0, button->geometry());
    anim->setKeyValueAt(0.5, button->geometry().adjusted(-5, -5, 5, 5));
    anim->setKeyValueAt(1, button->geometry());
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void LifeWindow::typeWriterEffect(const QString &text)
{
    m_currentDisplayText = text;
    m_typePos = 0;
    ui->eventLabel1->clear();
    m_typeTimer->start(50); // 每50ms显示一个字符
}

void LifeWindow::resizeEvent(QResizeEvent *event) {
    // 先调用基类处理
    QMainWindow::resizeEvent(event);

    // 确保背景视图和场景已初始化
    if (m_bgView && m_scene) {
        // 保持宽高比并填充整个窗口
        m_bgView->fitInView(
            m_scene->sceneRect(),
            Qt::KeepAspectRatioByExpanding
        );
    }

    qDebug() << "窗口新尺寸：" << event->size();
    qDebug() << "场景尺寸：" << m_scene->sceneRect();
}
