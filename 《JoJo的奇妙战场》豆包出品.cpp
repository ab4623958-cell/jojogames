#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

struct Skill {
    string name;
    int baseDamage;
    int effect; // 1=暴击(50%)，2=稳定，3=回血(15-25)，4=降防(+20%)，5=3段连击，6=群体伤害
};

class StandUser {
private:
    string name;
    string stand;
    int hp;
    int maxHP;
    Skill skills[4];
    int skillCount;
    bool defenseDown;

public:
    StandUser(string n, string s, int h, Skill s1, Skill s2, Skill s3, Skill s4 = {"",0,0}) 
        : name(n), stand(s), hp(h), maxHP(h), skillCount(3), defenseDown(false) {
        skills[0] = s1;
        skills[1] = s2;
        skills[2] = s3;
        if (s4.name != "") {
            skills[3] = s4;
            skillCount = 4;
        }
    }

    int getHP() { return hp; }
    string getName() { return name; }
    string getStand() { return stand; }
    int getSkillCount() { return skillCount; }
    Skill getSkill(int idx) { return skills[idx]; }
    bool isDefenseDown() { return defenseDown; }
    void setDefenseDown(bool state) { defenseDown = state; }
    void resetHP() { hp = maxHP; }

    int useSkill(Skill skill, StandUser& target) {
        int totalDamage = 0;
        if (skill.effect == 1) { // 高暴击
            totalDamage = skill.baseDamage + (rand() % 12 - 3);
            if (rand() % 2 == 0) {
                totalDamage *= 2;
                cout << "【超暴击！】";
            }
        } else if (skill.effect == 2) { // 稳定
            totalDamage = skill.baseDamage + (rand() % 6 + 2);
        } else if (skill.effect == 3) { // 回血
            totalDamage = skill.baseDamage + (rand() % 10 - 4);
// 在effect == 3的分支中修改回血数值
            int heal = rand() % 11 + 18;  // 从15+10→18+10，上限28
            hp = min(hp + heal, maxHP);
            cout << "【" << name << "大量回血！】";
        } else if (skill.effect == 4) { // 降防
            totalDamage = skill.baseDamage + (rand() % 9 - 3);
            target.setDefenseDown(true);
            cout << "【" << target.getName() << "防御大幅下降！】";
        } else if (skill.effect == 5) { // 3段连击
            int hit1 = skill.baseDamage / 3 + (rand() % 7 - 2);
            int hit2 = skill.baseDamage / 3 + (rand() % 7 - 2);
            int hit3 = skill.baseDamage / 3 + (rand() % 7 - 2);
            totalDamage = hit1 + hit2 + hit3;
            cout << "【3段连击！】";
        } else if (skill.effect == 6) { // 群体伤害
            totalDamage = skill.baseDamage + (rand() % 8 - 2);
            cout << "【范围攻击！】";
        }

        if (target.isDefenseDown()) {
            totalDamage = (int)(totalDamage * 1.2);
            target.setDefenseDown(false);
        }
        return totalDamage;
    }

    void takeDamage(int damage) {
        if (damage < 0) {
            hp = min(hp - damage, maxHP); // 负伤害视为回血
            cout << name << "回复了" << -damage << "点HP！" << endl;
            return;
        }
        hp -= damage;
        if (hp < 0) hp = 0;
        cout << name << "受到" << damage << "点伤害！" << endl;
    }
};

// 敌人构造函数
StandUser dio(string n, string s, int h, Skill s1, Skill s2, Skill s3, Skill s4 = {"",0,0}) {
    return StandUser(n, s, h, s1, s2, s3, s4);
}
StandUser kira(string n, string s, int h, Skill s1, Skill s2, Skill s3, Skill s4 = {"",0,0}) {
    return StandUser(n, s, h, s1, s2, s3, s4);
}
StandUser diavolo(string n, string s, int h, Skill s1, Skill s2, Skill s3, Skill s4 = {"",0,0}) {
    return StandUser(n, s, h, s1, s2, s3, s4);
}
StandUser toru(string n, string s, int h, Skill s1, Skill s2, Skill s3, Skill s4 = {"",0,0}) {
    return StandUser(n, s, h, s1, s2, s3, s4);
}
StandUser pucci(string n, string s, int h, Skill s1, Skill s2, Skill s3, Skill s4 = {"",0,0}) {
    return StandUser(n, s, h, s1, s2, s3, s4);
}

// 训练场模式
void trainingMode(StandUser& player) {
    cout << endl << "===== 训练场模式 =====" << endl;
    cout << "在这里可以无限制练习技能，敌人不会攻击你！" << endl;
    StandUser dummy("练习假人", "无替身", 9999, 
        {"被动承受", 0, 2}, {"静止不动", 0, 2}, {"毫无反抗", 0, 2});

    while (true) {
        cout << endl << "【你的回合】HP：" << player.getHP() << endl;
        cout << "选择技能（输入0退出训练场）：" << endl;
        for (int j = 0; j < player.getSkillCount(); j++) {
            Skill s = player.getSkill(j);
            cout << j+1 << ". " << s.name << " (基础伤害：" << s.baseDamage << ") ";
            if (s.effect == 1) cout << "[暴击率50%]";
            else if (s.effect == 3) cout << "[回血18-28]";
            else if (s.effect == 4) cout << "[大幅降防]";
            else if (s.effect == 5) cout << "[3段连击]";
            else cout << "[稳定伤害]";
            cout << endl;
        }
        int choice;
        cin >> choice;
        if (choice == 0) break;
        if (choice < 1 || choice > player.getSkillCount()) choice = 1;
        
        Skill pSkill = player.getSkill(choice-1);
        cout << player.getName() << "使用了" << pSkill.name << "！";
        int pDmg = player.useSkill(pSkill, dummy);
        dummy.takeDamage(pDmg);
        cout << "假人剩余HP：" << dummy.getHP() << endl;
    }
    cout << "退出训练场模式" << endl;
}

bool bossMode(StandUser& player) {
    cout << endl << "===== 柱男BOSS模式开启！ =====" << endl;
    cout << "你需要依次击败四位柱男：桑塔纳 → 瓦姆乌 → 艾斯迪斯 → 卡兹！" << endl;
    cout << "输入任意数字开始挑战：";
    int start;
    cin >> start;

    // 新增桑塔纳BOSS
    StandUser santana_boss(
        "原始柱男·桑塔纳", "生物吞噬", 160,
        { "细胞分解", 25, 5 }, { "肉体融合", 20, 3 }, 
        { "高速突袭", 30, 1 }
    );

    StandUser wamuu_boss(
        "风之柱男·瓦姆乌", "风之骑士", 200,
        { "风之绞刑", 35, 5 }, { "神圣砂岚·改", 40, 6 }, 
        { "风之屏障", 20, 3 }, { "风之预判", 30, 4 }
    );

    StandUser esidisi_boss(
        "炎之柱男·艾斯迪斯", "火焰使者", 220,
        { "火焰喷射", 38, 2 }, { "热血奔流", 30, 3 }, 
        { "炎之拳", 42, 1 }, { "火焰风暴", 25, 6 }
    );

    StandUser kars_boss(
        "终极柱男·卡兹", "石鬼面进化", 200,
        { "光流乱舞", 35, 5 }, { "艾哲红石·完美吸收", 20, 3 }, 
        { "全能力爆发", 40, 1 }, { "世界重构", 30, 4 }
    );

    // BOSS数组更新为4个
    StandUser bosses[] = {santana_boss, wamuu_boss, esidisi_boss, kars_boss};
    string bossNames[] = {"原始柱男·桑塔纳", "风之柱男·瓦姆乌", "炎之柱男·艾斯迪斯", "终极柱男·卡兹"};

    // 循环次数从3次改为4次（i < 4）
    for (int i = 0; i < 4; i++) {
        StandUser currentBoss = bosses[i];
        cout << endl << "===== 挑战" << bossNames[i] << " =====" << endl;
        cout << currentBoss.getName() << "出现了！HP：" << currentBoss.getHP() << endl;

        // 完整战斗循环（补全省略的代码）
        while (player.getHP() > 0 && currentBoss.getHP() > 0) {
            // 玩家回合
            cout << endl << "【你的回合】HP：" << player.getHP() << endl;
            cout << "选择技能：" << endl;
            for (int j = 0; j < player.getSkillCount(); j++) {
                Skill s = player.getSkill(j);
                cout << j+1 << ". " << s.name << " (基础伤害：" << s.baseDamage << ") ";
                if (s.effect == 1) cout << "[暴击率50%]";
                else if (s.effect == 3) cout << "[回血18-28]";
                else if (s.effect == 4) cout << "[大幅降防]";
                else if (s.effect == 5) cout << "[3段连击]";
                else cout << "[稳定伤害]";
                cout << endl;
            }
            int choice;
            cin >> choice;
            if (choice < 1 || choice > player.getSkillCount()) choice = 1;
            Skill pSkill = player.getSkill(choice-1);
            cout << player.getName() << "使用了" << pSkill.name << "！";
            int pDmg = player.useSkill(pSkill, currentBoss);
            currentBoss.takeDamage(pDmg);
            if (currentBoss.getHP() == 0) break;

            // BOSS回合
            cout << endl << "【" << currentBoss.getName() << "的回合】HP：" << currentBoss.getHP() << endl;
            int eSkillIdx = rand() % currentBoss.getSkillCount();
            Skill eSkill = currentBoss.getSkill(eSkillIdx);
            cout << currentBoss.getName() << "使用了" << eSkill.name << "！";
            int eDmg = currentBoss.useSkill(eSkill, player);
            player.takeDamage(eDmg);
            if (player.getHP() == 0) break;
        }

        // 战斗结果判定
        if (player.getHP() <= 0) {
            cout << endl << "你被" << currentBoss.getName() << "击败了...";
            return false;
        } else {
            cout << endl << "恭喜！你击败了" << currentBoss.getName() << "！" << endl;
            if (i < 3) {  // 前3个BOSS击败后回血（桑塔纳、瓦姆乌、艾斯迪斯）
                int heal = rand() % 50 + 50;
                player.takeDamage(-heal);
                cout << "你获得了休整，回复了" << heal << "点HP！" << endl;
            }
        }
    }
    return true;
}

// 普通模式战斗逻辑
void normalMode(StandUser& player) {
    // 普通模式敌人列表
    StandUser enemies[] = {
        dio("迪奥·布兰度", "世界", 160, { "木大拳", 29, 2 }, { "时间停止·9秒", 36, 1 }, { "吸血鬼吸血", 15, 3 }),
        kira("吉良吉影", "杀手皇后", 130, { "第一炸弹", 27, 1 }, { "第二炸弹·穿心攻击", 22, 5 }, { "第三炸弹·败者食尘", 30, 4 }),
        diavolo("迪亚波罗", "绯红之王", 140, { "时删突袭", 31, 1 }, { "墓志铭预判", 20, 3 }, { "绯红之王连击", 28, 5 }),
        toru("透龙", "Wonder Of U", 150, { "灾厄诱导", 26, 4 }, { "因果律反转", 30, 1 }, { "Calamity", 32, 6 }),
        pucci("恩里克·普奇", "白蛇", 145, { "记忆抽取", 24, 2 }, { "替身夺取", 28, 4 }, { "新月", 33, 5 })
    };
    
    // 显示敌人选择列表
    cout << endl << "【选择对手】" << endl;
    for (int i = 0; i < 5; i++) {
        cout << i+1 << ". " << enemies[i].getName() << " - 替身：" << enemies[i].getStand() 
             << " | 生命值：" << enemies[i].getHP() << endl;
    }
    cout << "输入对手编号(1-5)：";
    int enemyChoice;
    cin >> enemyChoice;
    if (enemyChoice < 1 || enemyChoice > 5) enemyChoice = 1;
    StandUser enemy = enemies[enemyChoice - 1];
    cout << endl << "===== 挑战" << enemy.getName() << "开始！ =====" << endl;
    cout << enemy.getName() << "的替身" << enemy.getStand() << "出现了！" << endl;

    // 战斗循环
    while (player.getHP() > 0 && enemy.getHP() > 0) {
        // 玩家回合
        cout << endl << "【你的回合】HP：" << player.getHP() << endl;
        cout << "选择技能：" << endl;
        for (int j = 0; j < player.getSkillCount(); j++) {
            Skill s = player.getSkill(j);
            cout << j+1 << ". " << s.name << " (基础伤害：" << s.baseDamage << ") ";
            if (s.effect == 1) cout << "[暴击率50%]";
            else if (s.effect == 3) cout << "[回血18-28]";
            else if (s.effect == 4) cout << "[大幅降防]";
            else if (s.effect == 5) cout << "[3段连击]";
            else cout << "[稳定伤害]";
            cout << endl;
        }
        int choice;
        cin >> choice;
        if (choice < 1 || choice > player.getSkillCount()) choice = 1;
        Skill pSkill = player.getSkill(choice - 1);
        cout << player.getName() << "使用了" << pSkill.name << "！";
        int pDmg = player.useSkill(pSkill, enemy);
        enemy.takeDamage(pDmg);
        if (enemy.getHP() == 0) break;

        // 敌人回合
        cout << endl << "【" << enemy.getName() << "的回合】HP：" << enemy.getHP() << endl;
        int eSkillIdx = rand() % enemy.getSkillCount();
        Skill eSkill = enemy.getSkill(eSkillIdx);
        cout << enemy.getName() << "使用了" << eSkill.name << "！";
        int eDmg = enemy.useSkill(eSkill, player);
        player.takeDamage(eDmg);
        if (player.getHP() == 0) break;
    }

    // 战斗结果判定
    if (player.getHP() <= 0) {
        cout << endl << "===== 战斗结束 =====" << endl;
        cout << "很遗憾，你被" << enemy.getName() << "击败了..." << endl;
    } else {
        cout << endl << "===== 战斗胜利 =====" << endl;
        cout << "恭喜！你成功击败了" << enemy.getName() << "！" << endl;
        cout << "你展现了强大的替身能力！" << endl;
    }
}

int main() {
    srand(time(0));    
    
    // 花京院典明角色
    StandUser kakyoin("花京院典明", "绿色法皇", 125,
        { "绿宝石喷射", 25, 5 }, { "法皇结界", 22, 4 }, 
        { "法皇之绿束缚", 18, 2 },    { "绿宝石水花", 30, 1 });

// 新增西撒·齐贝林角色
StandUser cesar("西撒·齐贝林", "波纹气功", 135,
{ "泡沫波纹疾走", 26, 5 }, { "波纹割刀", 24, 4 },
{ "泡沫阵", 20, 2 }, { "终极波纹·青花鱼", 32, 1 });

// 强化后的乔鲁诺·乔巴拿（黄金体验镇魂曲完全体）
StandUser giorno_complete("乔鲁诺·乔巴拿（完全体）", "黄金体验镇魂曲", 180,  // 生命值从160提升至180
    { "镇魂曲·归零打击", 45, 1 },  // 基础伤害从40提升至45（暴击率50%保持）
    { "生命创造·镇魂曲", 40, 3 },  // 基础伤害从35提升至40，回血效果增强
    { "时删无效化", 35, 4 },        // 基础伤害从30提升至35，降防效果持续更稳定
    { "黄金体验·绝对支配", 50, 5 } // 基础伤害从45提升至50（5段连击伤害总和更高）
);

StandUser players[] = {
kakyoin, // 花京院典明
{"乔纳森·乔斯达", "波纹气功", 150, { "波纹直拳", 22, 2 }, { "太阳波纹疾走", 25, 3 }, { "波纹十字路", 18, 4 }},
{"乔瑟夫·乔斯达", "隐者之紫", 130, { "隐者之紫抽打", 20, 5 }, { "波纹泡泡", 18, 4 }, { "汤姆逊波纹弹", 24, 1 }},
{"空条承太郎", "白金之星", 140, { "流星指刺", 28, 1 }, { "欧拉连打", 32, 5 }, { "时间停止·5秒", 35, 1 }},
{"东方仗助", "疯狂钻石", 120, { "修复打击", 23, 3 }, { "疯狂连击", 26, 5 }, { "急救修复", 10, 3 }},
{"乔鲁诺·乔巴拿", "黄金体验", 110, { "生命赋予·拳头", 24, 2 }, { "黄金体验镇魂曲", 38, 1 }, { "植物生长", 15, 3 }},
{"空条徐伦", "石之自由", 105, { "线绳捆扎", 21, 4 }, { "石之自由连击", 27, 5 }, { "线缝修复", 12, 3 }},
cesar, // 西撒·齐贝林
giorno_complete // 乔鲁诺·乔巴拿（完全体）
};

StandUser player = players[0];
int choice;

cout << "===== JoJo的奇妙冒险：柱男BOSS篇 =====" << endl;
cout << "【选择你的角色】" << endl;
for (int i = 0; i < 9; i++) { // 角色数量改为9
cout << i+1 << ". " << players[i].getName() << " - 能力：" << players[i].getStand() << endl;
}
cout << "输入编号(1-9)："; // 选项范围改为1-9
cin >> choice;
if (choice >= 1 && choice <= 9) player = players[choice-1];

// 后续模式选择代码不变...
cout << endl << "【选择模式】" << endl;
cout << "1. 普通模式（挑战单个敌人）" << endl;
cout << "2. BOSS模式（挑战四大柱男）" << endl;
cout << "3. 训练场模式（练习技能）" << endl;
cout << "输入模式编号：";
cin >> choice;

if (choice == 3) {
trainingMode(player);
} else if (choice == 2) {
bool clear = bossMode(player);
if (clear) {
cout << endl << "===== 恭喜通关BOSS模式！ =====" << endl;
cout << "你成功击败了所有柱男，拯救了世界！" << endl;
} else {
cout << "但柱男的威胁仍在，下次一定要赢回来！" << endl;
}
} else {
normalMode(player);
}

return 0;
}
