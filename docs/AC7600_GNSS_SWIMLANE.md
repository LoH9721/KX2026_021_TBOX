# AC7600 GNSS、UTC校时与MSD触发时间泳道图

## 当前策略

- MCU与AC7600共用UART4主AT通道，不把原始NMEA混入TCP和语音通道。
- `AT+CGPSINFO`正常查询周期为1 s；当前点新鲜度为5 s，只有新有效定位才推进 `T-1…T-9` 历史。
- UTC来源优先级为新鲜GNSS时间、网络NITZ/CCLK时间、无效值0。
- 上电配置一次 `AT+CTZU=1`；随后立即查询 `AT+CCLK?`。时间无效时最多进行6次10 s快速重试，成功或快速重试耗尽后每30 min校时。
- `+CCLK`时区以15 min为单位，例如重庆/北京时间 `+32` 表示UTC+8；存入MSD前统一换算为UTC秒。
- eCall记录触发源到达时的系统tick；触发时已有UTC就立即冻结，否则在构建MSD时用“当前UTC－已过去时间”回算。GNSS位置无效不再强制触发时间无效。

## 1. GNSS后台采集和9点历史

```mermaid
sequenceDiagram
    participant MT as ModemTask（100 ms）
    participant MS as ModemService
    participant MG as ModemGnss
    participant Port as ModemPort/UART4
    participant AC as AC7600
    participant GT as GnssTask（100 ms）
    participant GS as GnssService

    MT->>MS: 周期调度
    MS->>MS: 确认无注册/MSD/语音/时间AT事务
    MS->>MG: 允许后台GNSS使用AT口
    MG->>Port: AT+CGNSSPWR?
    AC-->>Port: +CGNSSPWR: 0/1
    opt GNSS未开启
        MG->>Port: AT+CGNSSPWR=1,0,1
        AC-->>Port: READY/OK
    end
    loop 每1 s且AT口可用
        MG->>Port: AT+CGPSINFO
        AC-->>Port: 纬度,N/S,经度,E/W,日期,UTC,...
        alt 字段完整且UTC/坐标有效
            MG->>MG: 转换为signed e7和Unix UTC
            MG-->>GS: 发布新样本
            GT->>GS: 消费样本
            GS->>GS: 旧当前点→T-1，历史依次移动到T-9
        else 空定位或格式无效
            MG->>MG: 保持上一个缓存，不推进历史
        end
    end
```

## 2. GNSS被紧急业务抢占

```mermaid
sequenceDiagram
    participant ET as EcallTask
    participant MS as ModemService
    participant MG as ModemGnss
    participant Port as ModemPort

    ET->>MS: 新的紧急请求
    alt GNSS AT事务正在执行
        MS->>MG: AbortForEmergency
        MG->>Port: 中止当前GNSS命令
        alt 中止的是GNSS上电等待
            MG->>MG: 保留已生效的上电状态，回到查询等待
        else 中止的是GNSS下电等待
            MG->>MG: 标记GNSS未就绪
        else 中止的是位置查询
            MG->>MG: 回到查询等待，不重复电源循环
        end
        MG-->>MS: AT口让给紧急数据/语音
    else GNSS空闲
        MS->>MS: 直接执行紧急流程
    end
```

紧急抢占只中止当前后台GNSS事务，不把已经成功开启的GNSS误判为关闭，避免每次SOS后重新等待较长的GNSS上电过程。

## 3. NITZ/CCLK低频校时

```mermaid
sequenceDiagram
    participant MS as ModemService
    participant MT as ModemTime
    participant Port as ModemPort
    participant AC as AC7600
    participant TS as TimeService

    MS->>MT: 网络可用且AT口空闲
    MT->>Port: AT+CTZU?
    AC-->>Port: +CTZU: 0/1
    alt CTZU未开启
        MT->>Port: AT+CTZU=1
        AC-->>Port: OK
    end
    MT->>Port: 立即AT+CCLK?
    AC-->>Port: +CCLK: yy/MM/dd,hh:mm:ss±zz
    alt 时间有效
        MT->>MT: 本地时间减去时区，归一化为UTC
        MT-->>TS: 发布网络UTC样本
        MT->>MT: 下一次查询延后30 min
    else 时间无效或查询失败
        loop 最多6次快速获取
            MT->>Port: 每10 s AT+CCLK?
            AC-->>Port: 时间或失败
        end
        MT->>MT: 快速重试耗尽后改为30 min周期
    end
```

网络UTC缓存保存“UTC秒＋采样tick”；两次校时之间用系统tick续算，不需要每10 s占用AT通道。模组复位或MCU重新初始化后会重新执行CTZU/CCLK流程。

## 4. UTC仲裁与对外输出

```mermaid
flowchart TD
    A[TimeService_GetUtcSeconds] --> B{5 s内有新鲜GNSS UTC?}
    B -->|是| C[选择GNSS UTC]
    B -->|否| D{本次上电已取得有效CCLK?}
    D -->|是| E[选择网络UTC]
    D -->|否| F[返回INVALID和0]
    C --> G[缓存UTC秒 + 当前tick差]
    E --> G
    G --> H[返回当前Unix UTC秒]
    H --> I[MSD触发时间]
    H --> J[CAN时间帧]
    H --> K[以太网时间帧]
```

## 5. eCall触发时间冻结与后补

```mermaid
sequenceDiagram
    participant SRC as SOS/CAN/自动触发
    participant ET as EcallTask
    participant TS as TimeService
    participant GS as GnssService
    participant EC as Ecall

    SRC->>ET: RequestTrigger
    ET->>ET: 保存最早触发tick
    ET->>TS: GetUtcSeconds
    alt UTC有效
        TS-->>ET: 当前UTC
        ET->>ET: 当前UTC-排队经过时间，冻结触发UTC
    else UTC无效
        TS-->>ET: 0/INVALID
        ET->>ET: 暂时只保留触发tick
    end
    Note over ET: 手动SOS可经历10 s取消窗口
    ET->>GS: 最多等待3 s取得新鲜位置
    GS-->>ET: 有效位置/历史，或无效坐标
    ET->>TS: 若触发UTC尚未冻结，再查询当前UTC
    alt 后续取得GNSS或网络UTC
        TS-->>ET: 当前UTC
        ET->>ET: 触发UTC=当前UTC-(当前tick-触发tick)
    else 仍无可信时间
        ET->>ET: 触发UTC=0
    end
    ET->>EC: 冻结MSD快照
```

这保证MSD字段表示事件触发时刻，而不是取消窗口结束、GNSS等待结束、TCP连接或上传完成的时刻。同一事件重发继续使用第一次冻结的触发时间。

## 6. MSD位置与时间字段

| 标准区偏移 | 长度 | 内容 | 当前来源与无效值 |
|---:|---:|---|---|
| 23 | 4 | 触发时间 | Unix UTC秒，大端；无可信时间为0。 |
| 27 | 4 | 当前经度 | e7转换为度`×3,600,000`；无效为标准无效值。 |
| 31 | 4 | 当前纬度 | e7转换为度`×3,600,000`；无效为标准无效值。 |
| 35–106 | 72 | 9个历史点 | 每点先经度后纬度，顺序T-1至T-9。 |
| 107–110 | 4 | 事故字段 | 纵向Delta-V、横向Delta-V、乘员数、翻滚。 |

坐标换算采用整数四舍五入：`AECS raw = round(e7 × 9 / 25)`。位置是否有效与时间是否有效分别判断：没有新鲜坐标时，UTC仍可来自网络时间；没有网络时间时，新鲜GNSS UTC仍可用于触发时间。

## 7. 验证要求

1. 上电后确认CTZU只配置一次，首次CCLK立即查询；有效后日志中不再每10 s出现 `AT+CCLK?`。
2. 断开GNSS但保持蜂窝网络，触发MSD，确认触发时间来自CCLK且经纬度为无效值。
3. 触发时UTC无效，在10 s取消窗口或3 s GNSS等待期间取得时间，确认上传值回算到原触发时刻。
4. 同一事件发生15 s ACK超时并重传，确认 `ser`、MSD二进制和 `triggerTimeUtc` 均不改变。
5. 连续采集定位，核对当前点与T-1至T-9不重复、方向符号和坐标单位正确。
