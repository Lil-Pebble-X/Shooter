# 学习记录

### 第 1 次 - 2026-08-26

**本次主题**：
- UE GAS（Gameplay Ability System）即时命中武器的正规网络架构（参照 Lyra 官方示例）
- C++ 创建的 AbilityTask 必须手动调用 ReadyForActivation()（蓝图节点是自动激活）——踩坑两次：TargetDataUnderCrosshair、PlayMontageAndWait
- LocalPredicted 技能下客户端/服务端生命周期解耦：客户端随蒙太奇结束，服务端随结算完成结束
- “客户端提供证据、服务端校验结算”的权威模型
- UE 5.7 API 差异：C++ 的 HasAuthority 需传 &CurrentActivationInfo；PlayMontageAndWait 的 C++ 真名是 CreatePlayMontageAndWaitProxy

**已解决**：
- 步枪间歇性无伤害：根因是蒙太奇通知帧驱动瞄准造成的时序竞态 → 重构为激活同帧发起瞄准
- 右键无反应：ActivateAbility 里漏调 StartTargeting()，且蒙太奇任务漏调 ReadyForActivation()

**练习的技能**：
- 读引擎源码确认 API 签名（UE_5.7 安装目录）
- 用 UE_LOG 分段定位网络复制问题

**备注**：
- 用户目标：做 demo 找工作，要求最正规实现并能讲清原理；非代码部分一律中文
- 待办：表现层升级为 GameplayCue（目前 Niagara 仅本地可见）；Launcher 用同思路加固

---

**已掌握**：
- （暂无）

**需复习**：
- AbilityTask 激活规则（C++ 手动 / 蓝图自动）
- Lyra 武器链路与本工程实现的映射表

**目标**：
- 跑通正规 hitscan 链路（每发稳定伤害） - 状态：进行中
- 能口头复述 Lyra 式武器架构三原则 - 状态：进行中

### 第 2 次 - 2026-08-26

**本次主题**：
- __debugbreak 的含义：引擎 ensure 软断言的暂停形式，F5 可继续，需看调用栈定位
- 射击节奏（Delay）与 GAS 正规冷却（CooldownGameplayEffectClass + CommitAbility）的区别
- 最终分工定案：蓝图播蒙太奇 + 等开火通知 + Delay 控节奏 + EndAbility；C++ 只负责激活同帧瞄准（StartTargeting）与服务端校验结算

**已解决**：
- 蒙太奇不播放：C++ 蒙太奇任务漏 ReadyForActivation → 改回蓝图 PlayMontageAndWait 节点（自动激活）
- 无射击节奏：重构时删了蓝图 Delay → 恢复 Delay 0.2 → EndAbility
- C++ 蒙太奇任务未激活被回收触发的 ensure 断点 → 删除 C++ 蒙太奇代码

**需复习**：
- “C++ 任务手动激活 / 蓝图节点自动激活”这条规则已踩坑三次，务必牢记

**目标**：
- 跑通正规 hitscan 链路（每发稳定伤害） - 状态：进行中
- Launcher 加固 + 正规冷却机制 - 状态：未开始

### 第 3 次 - 2026-08-26

**本次主题**：
- 预测技能生命周期规则：客户端拥有生命周期，客户端 EndAbility 自动镜像到服务端（ServerEndAbility），服务端不应在结算后立即自杀式 EndAbility
- 输入层行为：AbilityInputTagHeld 每帧对未激活技能 TryActivateAbility——技能一旦瞬间结束，按住就会每帧重开火
- GAS 正规射速机制：CooldownGameplayEffectClass + CommitAbility（CheckCooldown 在激活检查中拒绝冷却中的再激活）

**已解决**：
- Delay 不生效/通知处连发：根因是 OnTargetDataReady 服务端立刻 EndAbility，宿主玩家实例每帧死亡重激活 → 删掉该行，生命周期交还蓝图 Delay→End
- 补上 CommitAbility + GE_RifleCooldown 作为正规 CD

**需复习**：
- “谁拥有生命周期”：预测技能=客户端；服务端镜像结束
- 射速两件套：蓝图 Delay 管按住连发间隔，冷却 GE 管点按与服务器门控

### 第 4 次 - 2026-08-26

**本次主题**：
- 全链路总结复盘：输入→激活→同帧瞄准→目标数据复制→服务端校验结算→伤害公式→属性扣减
- 整理 12 条复习知识点与 6 道自测题（见当次回复）

**已解决**：
- 总结会话，无新代码改动；射速修复（删服务端即时 EndAbility + CommitAbility 冷却）等待用户测试反馈

**目标**：
- 跑通正规 hitscan 链路（每发稳定伤害） - 状态：等待测试
- Launcher 加固 + 正规冷却机制 - 状态：未开始
- 表现层升级 GameplayCue - 状态：未开始

### 第 5 次 - 2026-08-26

**本次主题**：
- 旧架构时序竞态原理：逻辑挂在服务端不可靠的动画通知时钟上，追赶客户端 0.2 秒生命周期窄窗口 → 概率性丢数据
- 重构清单：触发点前移到激活帧、蓝图退化为表现层、UFUNCTION/ReadyForActivation 修复、服务端校验、生命周期归客户端、CommitAbility 冷却
- 新架构确定性来源：双端激活帧对齐 + 引擎缓存补发兜底 + 服务端瞬时注册

**已掌握**：
- 能复述“旧架构为何间歇失败、新架构为何确定”的完整因果链 - 2026-08-26
