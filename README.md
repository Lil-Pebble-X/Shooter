# UE5 GAS 第三人称射击 Demo

基于 Unreal Engine 5.7 的 TPS 战斗 Demo，战斗系统全链路使用
**Gameplay Ability System (GAS)** 实现，支持**服务器 / 客户端双端联机**。

🎬 **演示视频**：[B站链接]
📄 **开发日志**：[DEVLOG.md](DEVLOG.md)

---

## 技术栈

| 分类 | 内容 |
|---|---|
| 引擎 / 语言 | Unreal Engine 5.7 / C++（42 个类 / 90 个源文件） |
| Gameplay | GameplayAbilitySystem（ASC / AttributeSet / GA / GE / ExecCalc / GameplayCue） |
| 输入 | Enhanced Input + GameplayTags |
| AI | BehaviorTree + Blackboard + NavMesh + 视线遮挡检测 |
| UI | UMG + WidgetController 分层架构 |
| 网络 | 服务器权威 / 属性复制 / Server·Client·NetMulticast RPC / 自定义 NetSerialize |
| 表现 | Niagara / 伤害飘字 |

---

## 核心实现

- **战斗链路**：Enhanced Input → GameplayAbility → TargetData → 抛射物 / 射线 →
  ExecCalc 伤害计算 → AttributeSet 结算 → UI 反馈
- **伤害系统**：自定义 ExecutionCalculation，捕获攻防双方属性；
  元素伤害按类型分类，叠加暴击、易伤、攻击力的复合公式，全程服务器权威
- **网络同步**：属性复制（RepNotify）+ 三类 RPC；自定义 GameplayEffectContext
  并手写 NetSerialize（位掩码压缩可选字段）
- **双套 ASC 架构**：玩家 ASC 挂载 PlayerState（支持重生持久化），AI 挂载 Character
- **AI 感知**：感知半径 + LineTrace 视线遮挡检测，可见才追击，否则巡逻
- **声明式死亡**：`Die()` 恒定三行，其余系统各自声明对 `Status.Dead` 的反应

---

## 关键设计点

### 为什么要 `IncomingDamage` 这个 Meta 属性
把"伤害计算"和"伤害结算"分离：ExecCalc 只负责算，AttributeSet 的
`PostGameplayEffectExecute` 统一负责分配（先扣盾再扣血、飘字、死亡）。

### 为什么易伤做成属性而不是 Tag
Tag 适合二元状态门控（是/否、不可堆叠），Attribute 适合数值修正
（多少、可堆叠、策划可调）。做成属性后，多个来源的易伤能自然叠加。

### 为什么 AI 用 LineTrace 而不是 AIPerceptionComponent
AIPerception 的 Sight 内部也是同样的射线检测，但多了一套感知系统的注册和
刺激源管理。本项目需求单一（判断候选目标是否被遮挡），直接一次 LineTrace
更直接、状态更可控。

---

## 如何运行

- **引擎版本**：Unreal Engine 5.7
- **需要的插件**：Motion Warping
- **默认关卡**：`Content/Sandbox.umap`

> ⚠️ **本仓库包含第三方素材包**。
> Clone 后代码与蓝图逻辑完整可读，可完整运行，但素材过杂没有分类，可能体积较大。

---

## 素材来源

本项目使用了以下第三方素材（版权归原作者所有，仅用于学习演示）：

| 素材包 | 来源 |
|---|---|
| Paragon Wraith | Epic Games（虚幻商城免费） |
| LowPolyPack | 虚幻商城免费 |
| LP_FPSLite_JC / LP_LiteWeapons_JC | 虚幻商城免费 |
| QuantumCharacter | 虚幻商城免费 |
| Vefects | 虚幻商城免费 |
| Fire_EXP_Vol01_Free | 虚幻商城免费 |

---

## 参考

- 架构参考 **Aura** 的 GAS 教程
- 阅读了 **Lyra** 官方示例，用于理解输入管线与能力归属的设计意图

---

> 开发过程记录见 [DEVLOG.md](DEVLOG.md)
