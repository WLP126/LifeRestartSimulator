#ifndef LIFEENGINE_H
#define LIFEENGINE_H

#include <QObject>
#include <QList>
#include <QString>
#include <QVector>

// 属性结构体
struct Attribute {
    int intelligence;  // 智力 (0-100)
    int wealth;        // 财富 (0-100)
    int health;        // 健康 (0-100)
    int happiness;     // 幸福 (0-100)
    int appearance;    // 颜值 (0-100)

    // 构造函数方便初始化
    Attribute(int intel = 0, int w = 0, int h = 0, int happy = 0, int app = 0)
        : intelligence(intel), wealth(w), health(h), happiness(happy), appearance(app) {}
};

// 人生事件结构体
struct LifeEvent {
    QString description;       // 事件描述
    int minAge;               // 最小触发年龄
    int maxAge;               // 最大触发年龄
    QVector<QString> choices; // 可选操作
    QVector<Attribute> effects; // 每个操作对应的属性影响

    // 构造函数方便初始化
    LifeEvent(const QString& desc = "",
             int min = 0, int max = 100,
             const QVector<QString>& ch = QVector<QString>(),
             const QVector<Attribute>& eff = QVector<Attribute>())
        : description(desc), minAge(min), maxAge(max), choices(ch), effects(eff) {}
};

class LifeEngine : public QObject {
    Q_OBJECT

public:
    explicit LifeEngine(QObject *parent = nullptr);

    // 游戏控制
    void startNewLife();      // 开始新人生
    void makeChoice(int choiceIndex); // 做出选择
    void nextYear();          // 进入下一年

    // 获取当前状态
    Attribute currentAttributes() const { return m_attributes; }
    int currentAge() const { return m_age; }
    QString currentEventDescription() const { return m_currentEvent.description; }
    QVector<QString> currentChoices() const { return m_currentEvent.choices; }
    bool isGameOver() const { return m_gameOver; }
    QString lifeSummary() const; // 人生总结

signals:
    void gameUpdated();       // 游戏状态更新信号
    void gameOver(QString summary); // 游戏结束信号
public:
    void loadEvents();        // 加载所有事件（硬编码）
    void triggerRandomEvent(); // 触发随机事件

    // 游戏状态
    Attribute m_attributes;   // 当前属性
    int m_age;                // 当前年龄
    LifeEvent m_currentEvent; // 当前遭遇的事件
    QVector<LifeEvent> m_allEvents; // 所有可能的事件
    bool m_gameOver;          // 游戏是否结束
    QStringList m_lifeRecord; // 人生经历记录
};

#endif // LIFEENGINE_H
