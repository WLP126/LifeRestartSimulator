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
    // 初始化随机属性(5-20)，确保初始属性不至于过低
    m_attributes = Attribute(
        QRandomGenerator::global()->bounded(5, 21),
        QRandomGenerator::global()->bounded(5, 21),
        QRandomGenerator::global()->bounded(5, 21),
        QRandomGenerator::global()->bounded(5, 21),
        QRandomGenerator::global()->bounded(5, 21)
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

    // 开始游戏循环
    nextYear();
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
    m_attributes.wealth += effect.wealth;
    m_attributes.health += effect.health;
    m_attributes.happiness += effect.happiness;
    m_attributes.appearance += effect.appearance;

    // 确保属性在合理范围内(0-100)
    m_attributes.intelligence = qBound(0, m_attributes.intelligence, 100);
    m_attributes.wealth = qBound(0, m_attributes.wealth, 100);
    m_attributes.health = qBound(0, m_attributes.health, 100);
    m_attributes.happiness = qBound(0, m_attributes.happiness, 100);
    m_attributes.appearance = qBound(0, m_attributes.appearance, 100);

    // 检查游戏结束条件
    if (m_attributes.health <= 0) {
        m_gameOver = true;
        m_lifeRecord << QString("你的健康降到了0，在%1岁时不幸离世...").arg(m_age);
        emit gameOver(lifeSummary());
    } else if (m_age >= 100) {
        m_gameOver = true;
        m_lifeRecord << "你活到了100岁，寿终正寝！";
        emit gameOver(lifeSummary());
    } else {
        // 进入下一年
        nextYear();
    }
}

void LifeEngine::nextYear()
{
    m_age++;
    m_lifeRecord << QString("---- 年龄 %1 ----").arg(m_age);

    // 每年自然变化
    m_attributes.health -= QRandomGenerator::global()->bounded(0, 1);  // 减少健康减少量
    m_attributes.happiness += QRandomGenerator::global()->bounded(-1, 2); // 幸福随机变化

    // 年龄越大健康下降越快
    if (m_age > 60) {
        m_attributes.health -= 1;
    }

    // 确保健康不低于0
    m_attributes.health = qMax(0, m_attributes.health);

    // 尝试触发随机事件
    triggerRandomEvent();

    // 检查游戏结束条件
    if (m_attributes.health <= 0) {
        m_gameOver = true;
        m_lifeRecord << QString("你的健康降到了0，在%1岁时不幸离世...").arg(m_age);
        emit gameOver(lifeSummary());
    } else if (m_age >= 100) {
        m_gameOver = true;
        m_lifeRecord << "你活到了100岁，寿终正寝！";
        emit gameOver(lifeSummary());
    } else {
        // 如果没有事件发生，继续下一年
        if (m_currentEvent.description.isEmpty()) {
            m_lifeRecord << QString("年龄 %1：这一年平安无事").arg(m_age);
            qDebug() << QString("年龄 %1：这一年平安无事").arg(m_age);
            emit gameUpdated();
            QMetaObject::invokeMethod(this, "nextYear", Qt::QueuedConnection);
        }
    }
}

void LifeEngine::loadEvents()
{
    // 清空原有事件
    m_allEvents.clear();

    // ========== 童年事件 (0-12岁) ==========
    m_allEvents.append(LifeEvent(
        "幼儿园小朋友抢你的玩具",
        3, 6,
        {"哭闹找老师", "抢回来", "分享玩具"},
        {
            Attribute(0, 0, -1, -1, 0),  // 选项1效果
            Attribute(0, 0, 0, 1, 0),    // 选项2效果
            Attribute(1, 0, 0, 2, 1)     // 选项3效果
        }
    ));

    m_allEvents.append(LifeEvent(
        "在公园学会骑自行车",
        5, 10,
        {"成功学会", "摔倒受伤", "放弃"},
        {
            Attribute(0, 0, 1, 2, 0),  // 选项1效果：健康+1，幸福+2
            Attribute(0, 0, -2, -1, 0), // 选项2效果：健康-2，幸福-1
            Attribute(0, 0, 0, 0, 0)    // 选项3效果：无变化
        }
    ));

    m_allEvents.append(LifeEvent(
        "发现了一只受伤的小动物",
        4, 8,
        {"带回家照顾", "呼叫动物救援", "置之不理"},
        {
            Attribute(0, 0, 1, 3, 0),  // 选项1效果：幸福+3
            Attribute(0, 0, 0, 2, 0),  // 选项2效果：幸福+2
            Attribute(0, 0, 0, 0, 0)   // 选项3效果：无变化
        }
    ));

    // ========== 学生时代 (13-22岁) ==========
    m_allEvents.append(LifeEvent(
        "高考来临",
        18, 18,
        {"熬夜复习冲刺", "正常作息备考", "放弃考试去打工"},
        {
            Attribute(3, 0, -3, -2, 0),
            Attribute(2, 0, 0, 0, 0),
            Attribute(-2, 2, 1, 1, 0)
        }
    ));

    m_allEvents.append(LifeEvent(
        "第一次暗恋某人",
        13, 17,
        {"表白", "默默关注", "转学逃避"},
        {
            Attribute(0, 0, -1, -2, 0), // 选项1效果：健康-1，幸福-2
            Attribute(0, 0, 0, 1, 0),   // 选项2效果：幸福+1
            Attribute(0, 0, 0, -3, 0)   // 选项3效果：幸福-3
        }
    ));

    m_allEvents.append(LifeEvent(
        "参加学校社团活动",
        14, 18,
        {"积极参加", "偶尔参与", "完全不参与"},
        {
            Attribute(2, 0, 0, 3, 0),  // 选项1效果：智力+2，幸福+3
            Attribute(1, 0, 0, 1, 0),  // 选项2效果：智力+1，幸福+1
            Attribute(0, 0, 0, 0, 0)   // 选项3效果：无变化
        }
    ));

    // ========== 成年阶段 (23-60岁) ==========
    m_allEvents.append(LifeEvent(
        "被公司裁员",
        25, 50,
        {"创业", "找工作", "躺平休息半年"},
        {
            Attribute(2, -5, -2, -1, 0),
            Attribute(0, 0, 0, -1, 0),
            Attribute(-1, -1, 2, 1, 0)
        }
    ));

    m_allEvents.append(LifeEvent(
        "获得晋升机会",
        22, 35,
        {"努力工作争取", "保持现状", "考虑换工作"},
        {
            Attribute(3, 5, -1, 0, 0),  // 选项1效果：智力+3，财富+5，健康-1
            Attribute(0, 0, 0, 0, 0),   // 选项2效果：无变化
            Attribute(0, 2, 1, 0, 0)    // 选项3效果：财富+2，健康+1
        }
    ));

    m_allEvents.append(LifeEvent(
        "认识了新的朋友",
        19, 30,
        {"经常聚会", "偶尔联系", "逐渐疏远"},
        {
            Attribute(0, 0, 0, 3, 0),  // 选项1效果：幸福+3
            Attribute(0, 0, 0, 1, 0),  // 选项2效果：幸福+1
            Attribute(0, 0, 0, -1, 0)  // 选项3效果：幸福-1
        }
    ));

    // ========== 中年阶段 (36-59岁) ==========
    m_allEvents.append(LifeEvent(
        "投资房产",
        35, 55,
        {"大胆投资", "谨慎投资", "不投资"},
        {
            Attribute(0, 10, -2, 0, 0), // 选项1效果：财富+10，健康-2
            Attribute(0, 5, 0, 0, 0),   // 选项2效果：财富+5
            Attribute(0, 0, 0, 0, 0)    // 选项3效果：无变化
        }
    ));

    m_allEvents.append(LifeEvent(
        "子女考上大学",
        45, 60,
        {"庆祝", "节省开支", "无感"},
        {
            Attribute(0, -3, 0, 5, 0),  // 选项1效果：财富-3，幸福+5
            Attribute(0, -1, 0, 2, 0),  // 选项2效果：财富-1，幸福+2
            Attribute(0, 0, 0, 0, 0)    // 选项3效果：无变化
        }
    ));

    // ========== 老年阶段 (60+) ==========
    m_allEvents.append(LifeEvent(
        "子女要搬去外地",
        60, 80,
        {"支持他们追求理想", "要求他们留在身边", "自己也跟着搬过去"},
        {
            Attribute(0, 0, 0, -1, 0),
            Attribute(0, 0, -1, 1, 0),
            Attribute(0, -1, 1, 2, 0)
        }
    ));

    m_allEvents.append(LifeEvent(
        "退休生活",
        60, 80,
        {"积极兴趣爱好", "安静居家", "继续工作"},
        {
            Attribute(0, 0, 1, 4, 0),  // 选项1效果：健康+1，幸福+4
            Attribute(0, 0, 0, 2, 0),  // 选项2效果：幸福+2
            Attribute(0, 1, -1, 0, 0)  // 选项3效果：财富+1，健康-1
        }
    ));

    m_allEvents.append(LifeEvent(
        "孙辈出生",
        60, 80,
        {"积极照顾", "偶尔帮忙", "享受老年生活"},
        {
            Attribute(0, 0, -1, 5, 0), // 选项1效果：健康-1，幸福+5
            Attribute(0, 0, 0, 3, 0),  // 选项2效果：幸福+3
            Attribute(0, 0, 0, 1, 0)   // 选项3效果：幸福+1
        }
    ));

    qDebug() << "已加载" << m_allEvents.size() << "个事件";
}

void LifeEngine::triggerRandomEvent()
{
    QVector<LifeEvent> possibleEvents;

    // 筛选符合年龄的事件
    for (const LifeEvent &event : m_allEvents) {
        if (m_age >= event.minAge && m_age <= event.maxAge) {
            possibleEvents.append(event);
        }
    }

    if (!possibleEvents.isEmpty()) {
        int index = QRandomGenerator::global()->bounded(0, possibleEvents.size());
        m_currentEvent = possibleEvents[index];
        m_lifeRecord << "遇到事件：" << m_currentEvent.description;
        qDebug() << "触发事件：" << m_currentEvent.description;
        emit gameUpdated();  // 确保更新 UI
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

