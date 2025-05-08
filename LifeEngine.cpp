#include "LifeEngine.h"
#include <QRandomGenerator>
#include <QDebug>

LifeEngine::LifeEngine(QObject *parent)
    : QObject(parent),
      m_age(0),
      m_gameOver(false)
{
    loadEvents();  // 初始化时加载所有事件
}

void LifeEngine::startNewLife()
{
    // 初始化随机属性(1-10)
    m_attributes = Attribute(
        QRandomGenerator::global()->bounded(1, 11),
        QRandomGenerator::global()->bounded(1, 11),
        QRandomGenerator::global()->bounded(1, 11),
        QRandomGenerator::global()->bounded(1, 11),
        QRandomGenerator::global()->bounded(1, 11)
    );

    m_age = 0;
    m_gameOver = false;
    m_lifeRecord.clear();

    // 记录初始状态
    m_lifeRecord << "你出生了！初始属性："
                << QString("智力:%1 财富:%2 健康:%3 幸福:%4 颜值:%5")
                   .arg(m_attributes.intelligence)
                   .arg(m_attributes.wealth)
                   .arg(m_attributes.health)
                   .arg(m_attributes.happiness)
                   .arg(m_attributes.appearance);

    // 触发第一个事件
    triggerRandomEvent();

    emit gameUpdated();
}

void LifeEngine::loadEvents()
{
    // 清空原有事件
    m_allEvents.clear();

    // ========== 童年事件 (0-12岁) ==========
    m_allEvents << LifeEvent(
        "幼儿园小朋友抢你的玩具",
        3, 6,
        {"哭闹找老师", "抢回来", "分享玩具"},
        {
            Attribute(0, 0, -1, -1, 0),  // 选项1效果
            Attribute(0, 0, 0, 1, 0),     // 选项2效果
            Attribute(1, 0, 0, 2, 1)      // 选项3效果
        }
    );

    // ========== 学生时代 (13-22岁) ==========
    m_allEvents << LifeEvent(
        "高考来临",
        18, 18,
        {"熬夜复习冲刺", "正常作息备考", "放弃考试去打工"},
        {
            Attribute(3, 0, -3, -2, 0),
            Attribute(2, 0, 0, 0, 0),
            Attribute(-2, 2, 1, 1, 0)
        }
    );

    // ========== 成年阶段 (23-60岁) ==========
    m_allEvents << LifeEvent(
        "被公司裁员",
        25, 50,
        {"创业", "找工作", "躺平休息半年"},
        {
            Attribute(2, -5, -2, -1, 0),
            Attribute(0, 0, 0, -1, 0),
            Attribute(-1, -1, 2, 1, 0)
        }
    );

    // ========== 老年阶段 (60+) ==========
    m_allEvents << LifeEvent(
        "子女要搬去外地",
        60, 80,
        {"支持他们追求理想", "要求他们留在身边", "自己也跟着搬过去"},
        {
            Attribute(0, 0, 0, -1, 0),
            Attribute(0, 0, -1, 1, 0),
            Attribute(0, -1, 1, 2, 0)
        }
    );

    qDebug() << "已加载" << m_allEvents.size() << "个事件";
}

void LifeEngine::makeChoice(int choiceIndex)
{
    if (m_gameOver || choiceIndex < 0 || choiceIndex >= m_currentEvent.choices.size()) {
        return;
    }

    // 记录选择
    m_lifeRecord << QString("年龄%1：%2 → 选择[%3]")
                   .arg(m_age)
                   .arg(m_currentEvent.description)
                   .arg(m_currentEvent.choices[choiceIndex]);

    // 应用效果
    Attribute effect = m_currentEvent.effects[choiceIndex];
    m_attributes.intelligence += effect.intelligence;
    m_attributes.wealth       += effect.wealth;
    m_attributes.health       += effect.health;
    m_attributes.happiness    += effect.happiness;
    m_attributes.appearance   += effect.appearance;

    // 确保属性在合理范围内(0-100)
    m_attributes.intelligence = qBound(0, m_attributes.intelligence, 100);
    m_attributes.wealth       = qBound(0, m_attributes.wealth, 100);
    m_attributes.health       = qBound(0, m_attributes.health, 100);
    m_attributes.happiness    = qBound(0, m_attributes.happiness, 100);
    m_attributes.appearance   = qBound(0, m_attributes.appearance, 100);

    // 检查游戏结束条件
    if (m_attributes.health <= 0) {
        m_gameOver = true;
        m_lifeRecord << QString("你的健康降到了0，在%1岁时不幸离世...").arg(m_age);
        emit gameOver(lifeSummary());
        return;
    }

    if (m_age >= 100) {
        m_gameOver = true;
        m_lifeRecord << "你活到了100岁，寿终正寝！";
        emit gameOver(lifeSummary());
        return;
    }

    // 进入下一年
    nextYear();
}

void LifeEngine::nextYear()
{
    m_age++;
    m_lifeRecord << QString("---- 年龄 %1 ----").arg(m_age);

    // 每年自然变化
    m_attributes.health    -= QRandomGenerator::global()->bounded(0, 2);  // 健康自然下降
    m_attributes.happiness += QRandomGenerator::global()->bounded(-1, 2); // 幸福随机变化

    // 年龄越大健康下降越快
    if (m_age > 60) {
        m_attributes.health -= 1;
    }

    // 确保健康不低于0
    m_attributes.health = qMax(0, m_attributes.health);

    // 60%概率触发随机事件
    if (QRandomGenerator::global()->bounded(0, 100) < 60) {
        triggerRandomEvent();
    } else {
        m_currentEvent = LifeEvent(); // 清空当前事件
        m_lifeRecord << "这一年平安无事";
    }

    emit gameUpdated();
}

void LifeEngine::triggerRandomEvent()
{
    QVector<LifeEvent> possibleEvents;

    // 筛选符合年龄的事件
    for (const LifeEvent &event : m_allEvents) {
        if (m_age >= event.minAge && m_age <= event.maxAge) {
            possibleEvents << event;
        }
    }

    if (!possibleEvents.isEmpty()) {
        int index = QRandomGenerator::global()->bounded(0, possibleEvents.size());
        m_currentEvent = possibleEvents[index];
        m_lifeRecord << "遇到事件：" << m_currentEvent.description;
    } else {
        m_currentEvent = LifeEvent(); // 没有合适事件时清空
    }
}

QString LifeEngine::lifeSummary() const
{
    QString summary = "=== 人生总结 ===\n";

    // 所有人生记录
    for (const QString &record : m_lifeRecord) {
        summary += record + "\n";
    }

    // 计算人生评分
    int score = (m_attributes.intelligence +
                m_attributes.wealth * 2 +
                m_attributes.health * 3 +
                m_attributes.happiness * 2 +
                m_attributes.appearance) / 9;

    summary += "\n=== 最终评价 ===\n";
    if (score >= 90) {
        summary += "✨ 传奇人生！你活出了精彩的一生！";
    } else if (score >= 70) {
        summary += "👍 优秀人生！你的选择很明智！";
    } else if (score >= 50) {
        summary += "💼 平凡人生。大多数人都是这样过的。";
    } else if (score >= 30) {
        summary += "😟 艰难人生。下次会更好的...";
    } else {
        summary += "💀 悲惨人生。建议重开...";
    }

    summary += QString("\n\n最终属性：\n智力:%1 财富:%2 健康:%3 幸福:%4 颜值:%5\n年龄:%6")
              .arg(m_attributes.intelligence)
              .arg(m_attributes.wealth)
              .arg(m_attributes.health)
              .arg(m_attributes.happiness)
              .arg(m_attributes.appearance)
              .arg(m_age);

    return summary;
}
